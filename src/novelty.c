/*

      ##  ##  #####    #####   $$$$$   $$$$   $$$$$$    
      ##  ##  ##  ##  ##      $$      $$  $$    $$      
      ##  ##  #####   ##       $$$$   $$$$$$    $$      
      ##  ##  ##  ##  ##          $$  $$  $$    $$      
       ####   #####    #####  $$$$$   $$  $$    $$      
  ======================================================
  SLS SAT Solver from The University of British Columbia
  ======================================================
  ...Developed by Dave Tompkins (davet [@] cs.ubc.ca)...
  ------------------------------------------------------
  .......consult legal.txt for legal information........
  ......consult revisions.txt for revision history......
  ------------------------------------------------------
  ... project website: http://www.satlib.org/ubcsat ....
  ------------------------------------------------------
  .....e-mail ubcsat-help [@] cs.ubc.ca for support.....
  ------------------------------------------------------

*/

#include "ubcsat.h"

PROBABILITY iNovNoise;
PROBABILITY iDp;

void PickNovelty();
void PickNoveltyPlus();
void PickNoveltyW();
void PickNoveltyPlusW();

void PickNoveltyPlusPlus();
void PickNoveltyPlusPlusW();

void PickNoveltyVarScore();
void PickNoveltyPlusVarScore();
void PickNoveltyPlusPlusVarScore();

void AddNovelty() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty","",FALSE,
    "Novelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNovelty",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  CreateTrigger("PickNovelty",ChooseCandidate,PickNovelty,"","");


  pCurAlg = CreateAlgorithm("novelty","",TRUE,
    "Novelty: (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  CreateTrigger("PickNoveltyW",ChooseCandidate,PickNoveltyW,"","");

}

void AddNoveltyPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+","",FALSE,
    "Novelty+: Novelty with random walk",
    "Hoos [AAAI 99]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);

  CreateTrigger("PickNoveltyPlus",ChooseCandidate,PickNoveltyPlus,"","");


  pCurAlg = CreateAlgorithm("novelty+","",TRUE,
    "Novelty+: Novelty with random walk (weighted)",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE);

  CreateTrigger("PickNoveltyPlusW",ChooseCandidate,PickNoveltyPlusW,"","");
 
}

void AddNoveltyPlusPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++","",FALSE,
    "Novelty++: Novelty+ with a modified diversification mechanism",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the least recently flipped~variable from a randomly selected unsat clause","",&iDp,0.05);

  CreateTrigger("PickNoveltyPlusPlus",ChooseCandidate,PickNoveltyPlusPlus,"","");


  pCurAlg = CreateAlgorithm("novelty++","",TRUE,
    "Novelty++: Novelty+ with a modified diversification mechanism (weighted)",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty++","",FALSE);

  CreateTrigger("PickNoveltyPlusW",ChooseCandidate,PickNoveltyPlusW,"","");

}


void PickNovelty() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  /* Note: for WalkSAT variants, it's faster to calculate the
     score for each literal than to cache the values */

  /* for each literal in the clause */

  for (j=0;j<iClauseLen;j++) {

    iScore = 0;
    iVar = GetVarFromLit(*pLit);

    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];

    /* for each appearance of the literal in a false clause,
       decrease score by one (increase makecount) */
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
      }
      pClause++;
    }

    /* for each appearance of the negated literal in a critially satisfied clause,
       increase score by one (increase breakcount) */

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }

    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
      iYoungestVar = iVar;
    }

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }

    pLit++;
  }
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar) {
    return;
  }

  /* otherwise, choose the second best with probability (novnoise) */

  if (RandomProb(iNovNoise)) {
    iFlipCandidate = iSecondBestVar;
  }
}

void PickNoveltyPlus()
{
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNovelty();
  }
}

void PickNoveltyW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  UINT32 iYoungestVar;
  FLOAT fSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  fBestScore = fTotalWeight;
  fSecondBestScore = fTotalWeight;

  /* select the clause according to a weighted scheme */

  if (iNumFalse) {
    iClause = PickClauseWCS();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }
  pLit = pClauseLits[iClause];
  iYoungestVar = GetVarFromLit(*pLit);
  for (j=0;j<iClauseLen;j++) {
    fScore = FLOATZERO;
    iVar = GetVarFromLit(*pLit);
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        fScore -= aClauseWeight[*pClause];
      }
      pClause++;
    }
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        fScore += aClauseWeight[*pClause];
      }
      pClause++;
    }
    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
      iYoungestVar = iVar;
    }
    if ((fScore < fBestScore) || ((fScore == fBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      fSecondBestScore = fBestScore;
      fBestScore = fScore;
    } else if ((fScore < fSecondBestScore) || ((fScore == fSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      fSecondBestScore = fScore;
    }
    pLit++;
  }
  iFlipCandidate = iBestVar;
  if (iFlipCandidate != iYoungestVar) {
    return;
  }
  if (RandomProb(iNovNoise)) {
    iFlipCandidate = iSecondBestVar;
  }
}

void PickNoveltyPlusW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  
  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyW();
  }
}

void PickNoveltyPlusPlus() {
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */

  if (RandomProb(iDp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];

      /* set the oldest to be the first literal */

      iFlipCandidate = GetVarFromLit(*pLit);
      pLit++;

      /* for each remaining literal, check to see if it is older */

      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {
    
    /* otherwise, use regular novelty */

    PickNovelty();
  }
}

void PickNoveltyPlusPlusW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  if (RandomProb(iDp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iFlipCandidate = GetVarFromLit(*pLit);
      pLit++;
      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyW();
  }
}

void PickNoveltyVarScore() {

  /* simpler varaint -- uses cached VarScore values instead of calculating score */
 
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 iVar;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }
  pLit = pClauseLits[iClause];
  iYoungestVar = GetVarFromLit(*pLit);
  for (j=0;j<iClauseLen;j++) {
    iVar = GetVarFromLit(*pLit);
    iScore = aVarScore[iVar];
    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
      iYoungestVar = iVar;
    }
    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }
    pLit++;
  }
  iFlipCandidate = iBestVar;
  if (iFlipCandidate != iYoungestVar) {
    return;
  }
  if (RandomProb(iNovNoise)) {
    iFlipCandidate = iSecondBestVar;
  }
}

void PickNoveltyPlusVarScore() {

  /* simpler varaint -- uses cached VarScore values instead of calculating score */
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyVarScore();
  }
}

void PickNoveltyPlusPlusVarScore() {

  /* simpler varaint -- uses cached VarScore values instead of calculating score */
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  if (RandomProb(iDp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iFlipCandidate = GetVarFromLit(*pLit);
      pLit++;
      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyVarScore();
  }
}
