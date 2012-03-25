/*

      ##  ##  #####    #####   $$$$$   $$$$   $$$$$$    
      ##  ##  ##  ##  ##      $$      $$  $$    $$      
      ##  ##  #####   ##       $$$$   $$$$$$    $$      
      ##  ##  ##  ##  ##          $$  $$  $$    $$      
       ####   #####    #####  $$$$$   $$  $$    $$      
  ======================================================
  SLS SAT Solver from The University of British Columbia
  ======================================================
  ...... Developed & Maintained by Dave Tompkins .......
  ------------------------------------------------------
  ...... consult legal.txt for legal information .......
  ------------------------------------------------------
  .... project website: http://ubcsat.dtompkins.com ....
  ------------------------------------------------------
  source repository: https://github.com/dtompkins/ubcsat
  ------------------------------------------------------
  ..... contact us at ubcsat [@] googlegroups.com ......
  ------------------------------------------------------

*/

#include "ubcsat.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif

PROBABILITY iNovNoise;
PROBABILITY iNovWpDp;

void PickNovelty();
void PickNoveltyPlus();
void PickNoveltyPlusPlus();

void PickNoveltyVarScore();
void PickNoveltyPlusVarScore();
void PickNoveltyPlusPlusVarScore();

void AddNovelty() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty","",0,
    "Novelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNovelty",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  CreateTrigger("PickNovelty",ChooseCandidate,PickNovelty,"","");

}

void AddNoveltyPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+","",0,
    "Novelty+: Novelty with random walk",
    "Hoos [AAAI 99]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",0);

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.01);

  CreateTrigger("PickNoveltyPlus",ChooseCandidate,PickNoveltyPlus,"","");


  pCurAlg = CreateAlgorithm("novelty+","cache",0,
    "Novelty+: Novelty with random walk [cached score]",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusVarScore",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange,VarScore",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",0);

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.01);

  CreateTrigger("PickNoveltyPlusVarScore",ChooseCandidate,PickNoveltyPlusVarScore,"","");


}

void AddNoveltyPlusPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++","",0,
    "Novelty++: Novelty+ with a modified diversification mechanism",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the least recently flipped~variable from a randomly selected unsat clause","",&iNovWpDp,0.05);

  CreateTrigger("PickNoveltyPlusPlus",ChooseCandidate,PickNoveltyPlusPlus,"","");
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

  iBestScore = (SINT32) iNumClauses;
  iSecondBestScore = (SINT32) iNumClauses;

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

  /* if the best is NOT the youngest, select it */

  if (iFlipCandidate != iYoungestVar) {
    return;
  }

  /* otherwise, choose the second best with probability (novnoise) */

  if (RandomProb(iNovNoise)) {
    iFlipCandidate = iSecondBestVar;
  }
}

void PickNoveltyPlus() {
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;

  /* with probability (iNovWpDp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iNovWpDp)) {
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

void PickNoveltyPlusPlus() {
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iNovWpDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */

  if (RandomProb(iNovWpDp)) {
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

  iBestScore = (SINT32) iNumClauses;
  iSecondBestScore = (SINT32) iNumClauses;

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

  if (RandomProb(iNovWpDp)) {
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

  if (RandomProb(iNovWpDp)) {
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

#ifdef __cplusplus

}
#endif
