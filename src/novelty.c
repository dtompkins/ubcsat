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
void PickNoveltyW();
void PickNoveltyTabu();
void PickNoveltyTabuW();


void PickNoveltyPlus();
void PickNoveltyPlusW();
void PickNoveltyPlusTabu();
void PickNoveltyPlusTabuW();

void PickNoveltyPlusPlus();
void PickNoveltyPlusPlusW();
void PickNoveltyPlusPlusTabu();
void PickNoveltyPlusPlusTabuW();

void PickNoveltyPlusPlusPrime();
void PickNoveltyPlusPlusPrimeW();
void PickNoveltyPlusPlusPrimeTabu();
void PickNoveltyPlusPlusPrimeTabuW();

void PickNoveltyPromising();
void PickNoveltyPromisingW();
void PickNoveltyPromisingTabu();
void PickNoveltyPromisingTabuW();

void PickNoveltyPlusPromising();
void PickNoveltyPlusPromisingW();
void PickNoveltyPlusPromisingTabu();
void PickNoveltyPlusPromisingTabuW();


void PickNoveltyPlusPlusPromising();
void PickNoveltyPlusPlusPromisingW();
void PickNoveltyPlusPlusPromisingTabu();
void PickNoveltyPlusPlusPromisingTabuW();

void PickNoveltyPlusPlusPrimePromising();
void PickNoveltyPlusPlusPrimePromisingW();
void PickNoveltyPlusPlusPromisingTabu();

void PickNoveltyPlusFC();
void PickNoveltyPlusPromisingFC();

extern UINT32 SelectClause();



void InitSimulatedFlip();
void CreateSimulatedFlip();
void InitLookAhead();
void CreateLookAhead();



UINT32 PromisingScore(UINT32);
UINT32 iSelectClause;
UINT32 *aSimulatedBreakCount;
UINT32 *aSimulatedMakeCount;
UINT32 *aSimulatedCritSat;
SINT32 BestLookAheadScore(UINT32);
SINT32 BestLookAheadPenScore(UINT32);
UINT32 *aIsLookAhead;
UINT32 *aLookAheadList;
SINT32 *aLookAheadScoreChange;

extern UINT32 iNumDecPromVars;
extern UINT32 *aDecPromVarsList;
#define UpdateLookAhead(var,diff) {if(aIsLookAhead[var]==FALSE) {aIsLookAhead[var]=TRUE; aLookAheadList[iNumLookAhead++] = var; aLookAheadScoreChange[var] = (diff);} else {aLookAheadScoreChange[var] += (diff);}};

void AddNovelty() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty","",FALSE,
    "Novelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNovelty",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmUInt(&pCurAlg->parmList,"-selectclause","select clause [default %s]","specifies choice to select clause","",&iSelectClause,1);

  CreateTrigger("PickNovelty",ChooseCandidate,PickNovelty,"","");


  pCurAlg = CreateAlgorithm("novelty","",TRUE,
    "Novelty: (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

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
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);
  
  CreateTrigger("PickNoveltyPlus",ChooseCandidate,PickNoveltyPlus,"","");

  

  pCurAlg = CreateAlgorithm("novelty+","",TRUE,
    "Novelty+: Novelty with random walk (weighted)",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE,0);

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
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the least recently flipped~variable from a randomly selected unsat clause","",&iDp,0.05);

  CreateTrigger("PickNoveltyPlusPlus",ChooseCandidate,PickNoveltyPlusPlus,"","");


  pCurAlg = CreateAlgorithm("novelty++","",TRUE,
    "Novelty++: Novelty+ with a modified diversification mechanism (weighted)",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty++","",FALSE,0);

  //This is a bug need to report Dave
  CreateTrigger("PickNoveltyPlusPlusW",ChooseCandidate,PickNoveltyPlusPlusW,"","");


}


void AddNoveltyPromising() {
  
 ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("noveltyp","",FALSE,
    "Novelty+p: Novelty with random walk and look-ahead",
    "Li, Wei, and Zhang [SAT 07]",
    "PickNoveltyPromising",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  CreateTrigger("PickNoveltyPromising",ChooseCandidate,PickNoveltyPromising,"","");


  CreateTrigger("CreateLookAhead",CreateStateInfo,CreateLookAhead,"","");
  CreateTrigger("InitLookAhead",InitStateInfo,InitLookAhead,"","");
  CreateContainerTrigger("LookAhead","CreateLookAhead,InitLookAhead");

  pCurAlg = CreateAlgorithm("noveltyp","",TRUE,
    "Novelty+p: Novelty with random walk and look-ahead",
    "Li, Wei, and Zhang [SAT 07]",
    "PickNoveltyPromisingW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVars,FalseClauseList,VarLastChange,LookAhead,VarScoreW",
    "default_w","default");

  CopyParameters(pCurAlg,"noveltyp","",FALSE,0);

  CreateTrigger("PickNoveltyPromisingW",ChooseCandidate,PickNoveltyPromisingW,"","");

 
}

void AddNoveltyPlusPromising() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+p","",FALSE,
    "Novelty+p: Novelty+ with an additional look-ahead step",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusPromising",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE,0);

  //AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);

  CreateTrigger("PickNoveltyPlusPromising",ChooseCandidate,PickNoveltyPlusPromising,"","");


  pCurAlg = CreateAlgorithm("novelty+p","",TRUE,
    "Novelty+p: Novelty+ with an additional look-ahead step",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusPromisingW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVars,FalseClauseList,VarLastChange,LookAhead,VarScoreW",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE,0);

  //AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);

  CreateTrigger("PickNoveltyPlusPromisingW",ChooseCandidate,PickNoveltyPlusPromisingW,"","");
 //TODO weighted version 

 
}

void AddNoveltyPlusPlusPromising() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++p","",FALSE,
    "Novelty++p: Novelty++ with a look-ahead approach",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusPromising",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the least recently flipped~variable from a randomly selected unsat clause","",&iDp,0.05);

  CreateTrigger("PickNoveltyPlusPlusPromising",ChooseCandidate,PickNoveltyPlusPlusPromising,"","");

  pCurAlg = CreateAlgorithm("novelty++p","",TRUE,
    "Novelty++p: Novelty++ with a look-ahead approach",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusPromisingW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVars,FalseClauseList,VarLastChange,LookAhead,VarScoreW",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty++p","",FALSE,0);

  

  CreateTrigger("PickNoveltyPlusPlusPromisingW",ChooseCandidate,PickNoveltyPlusPlusPromisingW,"","");

}

void AddNoveltyPlusPlusPrime() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++'","",FALSE,
    "Novelty++'",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrime",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  

  CopyParameters(pCurAlg,"novelty++","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPlusPrime",ChooseCandidate,PickNoveltyPlusPlusPrime,"","");

  pCurAlg = CreateAlgorithm("novelty++'","",TRUE,
    "Novelty++'(weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrime",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,FalseClauseList,VarLastChange,VarScoreW",
    "default_w","default");
  
  //AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  

  CopyParameters(pCurAlg,"novelty++","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPlusPrimeW",ChooseCandidate,PickNoveltyPlusPlusPrimeW,"","");

}

void AddNoveltyPlusPlusPrimePromising() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++'p","",FALSE,
    "Novelty++'p",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrimePromising",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  //AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  

  CopyParameters(pCurAlg,"novelty++","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPlusPrimePromising",ChooseCandidate,PickNoveltyPlusPlusPrimePromising,"","");

  
  pCurAlg = CreateAlgorithm("novelty++'p","",TRUE,
    "Novelty++'p",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrimePromisingW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVars,FalseClauseList,VarLastChange,LookAhead,VarScoreW",
    "default_w","default");
  
  //AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  

  CopyParameters(pCurAlg,"novelty++'","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPlusPrimePromisingW",ChooseCandidate,PickNoveltyPlusPlusPrimePromisingW,"","");



}


void PickNovelty() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  SINT32 iBestScoreWithoutPen;
  SINT32 iScoreWithoutPen;
  

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  
 
  iBestScore = bPen ? iTotalWeight : iNumClauses;
  iSecondBestScore = bPen ? iTotalWeight : iNumClauses;

  /* select an unsatisfied clause uniformly at random */
  
  
  if (iNumFalse) {
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iScore = 0;
    iScoreWithoutPen = 0;
    iVar = GetVarFromLit(*pLit);
    /*The computation of score is necessary only when 
      the varscore trigger is not activated. Also,
      we need to check whether clause penalty is
      activated. If it is activated then, we need 
      to take account of the clause weight*/
    if(!bPromisingList && !bPen){
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
        iScoreWithoutPen --;
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
        iScoreWithoutPen++;
      }
      pClause++;
    }
    }else if(!bPromisingList && bPen){
    //Promising List is not used but Clause Penalty is activated
        iNumOcc = aNumLitOcc[*pLit];
        pClause = pLitClause[*pLit];
        for (i=0;i<iNumOcc;i++) {
         if (aNumTrueLit[*pClause]==0) {
          iScore -= aClausePen[*pClause];
          iScoreWithoutPen--;
         } 
        pClause++;
      } 

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
        iScoreWithoutPen++;
      }
      pClause++;
    }


   } else {

   iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
   iScoreWithoutPen = aVarScore[iVar];
  }

  
    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
      iBestScoreWithoutPen = iScoreWithoutPen;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }

    pLit++;
  }
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */
  
  //printf("\n %d The best score is ", aVarScore[iBestVar]);
  //printf("\n %d The second best score is ", aVarScore[iSecondBestVar]);

  //printf("%f ", ProbToFloat(iNovNoise));
  
  if((bPerformNoveltyAlternate && iBestScoreWithoutPen ==0)|| (RandomProb(iNovNoise)))
    iFlipCandidate = iSecondBestVar;
  

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
      iClause = SelectClause();
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

  /* for general comments, review the unweighted version */
 
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
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
    //iClause = PickClauseWCS();
    iClause = SelectClause();
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

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

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

  if (iFlipCandidate != iYoungestVar)
    return;

  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;
}

void PickNoveltyPlusW() {

   /* for comments, review the unweighted version */
 
  UINT32 iClause;
  UINT32 iClauseLen;

  LITTYPE litPick;

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

    /* otherwise, use regular novelty */

    PickNovelty();
  }
}

void PickNoveltyPlusPlusW() {
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */

  if (RandomProb(iDp)) {
    if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

    /* otherwise, use regular novelty */

    PickNoveltyW();
  }
}


void PickNoveltyPromising()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  SINT32 iScore;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  SINT32 iBestScoreWithoutPen;
  SINT32 iScoreWithoutPen;
 
  LITTYPE litPick;

  SINT32 iSecondBestLookAheadScore;
  SINT32 iBestLookAheadScore;

  if (iNumFalse) {

    /* select random unsatisfied clause */

    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];

    if (FALSE) {


    } else {
      iBestScore = bPen ? iTotalWeight : iNumClauses;
      iSecondBestScore = bPen ? iTotalWeight : iNumClauses;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);

      /* for each literal in the clause */

      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        /* use the cached score value for that literal */
        iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
        iScoreWithoutPen = aVarScore[iVar];
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
          iBestScoreWithoutPen = iScoreWithoutPen;
        } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
          iSecondBestVar = iVar;
          iSecondBestScore = iScore;
        }
        pLit++;
      }

      /* choose the 'best' variable by default */

      iFlipCandidate = iBestVar;

      /* If the best is the youngest, with probability (iNovNoise) select the 2nd best */

      if (iFlipCandidate == iYoungestVar) {
      if((bPerformNoveltyAlternate && (iBestScoreWithoutPen ==0))|| (RandomProb(iNovNoise))){
       iFlipCandidate = iSecondBestVar;
          return;
        }
      } else {

        /* If the best is older than then 2nd best, just choose the best */

        if (aVarLastChange[iSecondBestVar] >= aVarLastChange[iFlipCandidate]) {
          return;
        }
      }

      /* otherwise, determine the 'look ahead' score for the 2nd best variable */

      iSecondBestLookAheadScore = bPen ? aVarPenScore[iSecondBestVar] + BestLookAheadPenScore(iSecondBestVar): aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);

      if (iSecondBestLookAheadScore > iBestScore) {
        iBestLookAheadScore = iBestScore;
      } else {

        /* if the 'look ahead' score for the 2nd variable is better than the regular score
           for the best variable, calculate the look ahead score for the best variable */

        iBestLookAheadScore =bPen ?  aVarPenScore[iFlipCandidate] + BestLookAheadPenScore(iFlipCandidate) : aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
      }

      /* choose the variable with the best look ahead score */

      /* Note that this BREAKS TIES by selecting the 2nd best variable -- as in the paper and in the author's code */

      if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
        iFlipCandidate = iSecondBestVar;
      }
    }
  } else {
    iFlipCandidate = 0;
  }






}

void PickNoveltyPlusPromising()
{
 
  UINT32 iClause;
  UINT32 iClauseLen;

  LITTYPE litPick;

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyPromising();
  }
}

void PickNoveltyPlusPlusPromising() {
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */

  
  if (RandomProb(iDp)) {
    if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

    /* otherwise, use regular novelty */

    PickNoveltyPromising();
  }
}

void CreateSimulatedFlip() {
  aSimulatedBreakCount = AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aSimulatedMakeCount = AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aSimulatedCritSat = AllocateRAM(iNumClauses*sizeof(UINT32));
 
}

void InitSimulatedFlip() {
  memset(aSimulatedBreakCount,0,(iNumVars+1)*sizeof(UINT32));
  memset(aSimulatedMakeCount,0,(iNumVars+1)*sizeof(UINT32));
  memset(aSimulatedCritSat,0,iNumClauses*sizeof(UINT32));

}


UINT32 PromisingScore(UINT32 FlipVariable)
{
	//copy the scores first 
    //simulates flip
	//iterate over all the variables to figure out the best score
	//return the best score 
  UINT32 j;
  UINT32 iBestPromisingScore =0;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

   for(j = 0; j <iNumVars; j ++)
   {
	   aSimulatedBreakCount[j]= aBreakCount[j];
	   aSimulatedMakeCount[j] = aMakeCount[j];
       //TODO How to copy critsat?? need to know
   }

   for(j = 0; j <iNumClauses; j ++)
   {
	   aSimulatedCritSat[j]= aCritSat[j];
       //TODO How to copy critsat?? need to know
   }

  litWasTrue = GetFalseLit(FlipVariable);
  litWasFalse = GetTrueLit(FlipVariable);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      aSimulatedBreakCount[FlipVariable]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aSimulatedMakeCount[GetVarFromLit(*pLit)]++;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aSimulatedCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aSimulatedMakeCount[iVar]--;
        pLit++;
      }
      aSimulatedBreakCount[FlipVariable]++;
      aSimulatedCritSat[*pClause] = FlipVariable;
    }
    if (aNumTrueLit[*pClause]==2) {
      aSimulatedBreakCount[aSimulatedCritSat[*pClause]]--;
    }
    pClause++;
  }

   for(j = 0; j <iNumVars; j ++)
   {
       //TODO very bad logic, need to change this one
	   //Need to talk to Dave whether this is correct 

	   if((aSimulatedMakeCount[j] > aSimulatedBreakCount[j]) && (aSimulatedMakeCount[j] < iNumClauses)){
	   if((aSimulatedMakeCount[j] - aSimulatedBreakCount[j]) > iBestPromisingScore){
		iBestPromisingScore = aSimulatedMakeCount[j] - aSimulatedBreakCount[j];
	    
		


	   }
	   }
   }

   return iBestPromisingScore;
}





void PickNoveltyPlusPlusPrime() {
  UINT32 i,index;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  LITTYPE litPick;
  SINT32 iBestScoreWithoutPen;
  SINT32 iScoreWithoutPen;

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = bPen ? iTotalWeight : iNumClauses;
  iSecondBestScore = bPen ? iTotalWeight : iNumClauses;

  
  /* When we pick randomly a variable from the clause we need to
     create a pool of variables excluding the best and second best*/

  iNumCandidates = 0;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause(); 
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iScore = 0;
    iScoreWithoutPen = 0;
    iVar = GetVarFromLit(*pLit);

        /* Complex scoring mechanism aimed to make things efficient */
        if(!bPromisingList && !bPen){
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
        iScoreWithoutPen--;
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
        iScoreWithoutPen++;
      }
      pClause++;
    }
    }else if(!bPromisingList && bPen){
    //Promising List is not used but Clause Penalty is activated
        iNumOcc = aNumLitOcc[*pLit];
        pClause = pLitClause[*pLit];
        for (i=0;i<iNumOcc;i++) {
         if (aNumTrueLit[*pClause]==0) {
          iScore -= aClausePen[*pClause];
          iScoreWithoutPen--;
         }
        pClause++;
      }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
        iScoreWithoutPen++;
      }
      pClause++;
    }


   }
   else{
  iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
  iScoreWithoutPen = aVarScore[iVar];
  }


    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

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
  
/* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
   if(random()%100 <intDp){
  //if (RandomProb(iDp)) { 
  pLit = pClauseLits[iClause];
  iNumCandidates = 0;
  
  if(iClauseLen <= 2){
   if(iClauseLen == 1){
     litPick = (pClauseLits[iClause][0]);
     iFlipCandidate = GetVarFromLit(litPick);
     return;
   }
   else{
     litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
     iFlipCandidate = GetVarFromLit(litPick);
     return;
   }

  } 
  if(iClauseLen == 3)
   index = 0;
  else
   index = RandomInt(iClauseLen - 2);
   i = 0;
  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */
    
    iVar = GetVarFromLit(*pLit);

	if((iVar != iBestVar) && (iVar != iSecondBestVar)) {
	    if(i == index){
             iFlipCandidate = iVar;	
             return;
            }
           else{
            i++;
           }
	}

	pLit++;

  }
 /* if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
  return;
*/

}
  
 /* Perform Novelty */ 
  
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */
    if(random()%100 < intNovNoise)
 // if((bPerformNoveltyAlternate && iBestScoreWithoutPen ==0)|| (RandomProb(iNovNoise)))
    iFlipCandidate = iSecondBestVar;

}



void PickNoveltyPlusPlusPrimeW() {
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  FLOAT fSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  fBestScore = fTotalWeight;
  fSecondBestScore = fTotalWeight;

  
  /* When we pick randomly a variable from the clause we need to
     create a pool of variables excluding the best and second best*/

  iNumCandidates = 0;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    fScore = FLOATZERO;

    iVar = GetVarFromLit(*pLit);

    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause++]==0) {
        fScore -= aClauseWeight[*pClause];
      }
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause++]==1) {
        fScore += aClauseWeight[*pClause];
      }
    }

    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

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
  
/* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
  
  if (RandomProb(iDp)) { 
  pLit = pClauseLits[iClause];
  iNumCandidates = 0;

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iVar = GetVarFromLit(*pLit);

	if((iVar != iBestVar) && (iVar != iSecondBestVar)) {
		aCandidateList[iNumCandidates++] = iVar;
	}

	pLit++;

  }
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
  return;


}
  
 /* Perform Novelty */ 
  
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */

  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;

}


void CreateLookAhead() {
  aIsLookAhead = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aLookAheadList = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aLookAheadScoreChange = AllocateRAM((iNumVars+1) * sizeof(SINT32));
}

void InitLookAhead() {
  memset(aIsLookAhead,0,(iNumVars+1) * sizeof(UINT32));
}

SINT32 BestLookAheadScore(UINT32 iLookVar) {
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWillbeTrue;
  LITTYPE litWillbeFalse;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;

  LITTYPE *pLit;
  UINT32 iNumLookAhead;
  SINT32 iScore;

  SINT32 iBestLookAheadScore;

  if (iLookVar == 0) {
    return(0);
  }

  iNumLookAhead = 0;

  for (j=0;j<iNumDecPromVars;j++) {
    UpdateLookAhead(aDecPromVarsList[j],0);
  }

/*
  litWillbeTrue = GetFalseLit(iLookVar);
  litWillbeFalse = GetTrueLit(iLookVar);

  pClause = pLitClause[litWillbeTrue];
  for (j=0;j<aNumLitOcc[litWillbeTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,+1);
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      UpdateLookAhead(iVar,-1);
    }
    pClause++;
  }

  pClause = pLitClause[litWillbeFalse];
  for (j=0;j<aNumLitOcc[litWillbeFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,-1);
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==2) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          if (iVar != iLookVar) {
            UpdateLookAhead(iVar,+1);
            break;
          }
        }
        pLit++;
      }
    }
    pClause++;
  }
*/

  litWasTrue = GetTrueLit(iLookVar);
  litWasFalse = GetFalseLit(iLookVar);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==1) { 
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,-1);
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==2) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          if (iVar != iLookVar) {
            UpdateLookAhead(iVar,+1);
            break;
          }
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==0) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,+1);
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      UpdateLookAhead(iVar,-1);
    }
    pClause++;
  }

  iBestLookAheadScore = iNumClauses;

  for (j=0;j<iNumLookAhead;j++) {
    iVar = aLookAheadList[j];
    if (iVar != iLookVar) {
      iScore = aVarScore[iVar] + aLookAheadScoreChange[iVar];
      
      // This is to mimic g2wsat implementation:
      if ((j < iNumDecPromVars)||(aVarScore[iVar]>=0)) {
        if (iScore < iBestLookAheadScore) {
          iBestLookAheadScore = iScore;
        }
      }

    }
    aIsLookAhead[iVar] = FALSE;
  }
  if (iBestLookAheadScore < 0) {
    return(iBestLookAheadScore);
  } else {
    return(0);
  }
}



void PickNoveltyPlusPlusPrimePromising()
{
   UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  SINT32 iScore;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  SINT32 iBestScoreWithoutPen;
  SINT32 iScoreWithoutPen;

  LITTYPE litPick;

  SINT32 iSecondBestLookAheadScore;
  SINT32 iBestLookAheadScore;

  if (iNumFalse) {

    /* select random unsatisfied clause */

    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
    if(iClauseLen ==0){
    printf(" This is surely wrong");
    iFlipCandidate = 0;
    return;
   }
  

    if (FALSE) {


    } else {
      iBestScore = bPen ? iTotalWeight : iNumClauses;
      iSecondBestScore = bPen ? iTotalWeight : iNumClauses;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);

      /* for each literal in the clause */

      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        /* use the cached score value for that literal */
        iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
        iScoreWithoutPen = aVarScore[iVar];
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
          iBestScoreWithoutPen = iScoreWithoutPen;
        } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
          iSecondBestVar = iVar;
          iSecondBestScore = iScore;
        }
        pLit++;
      }

         if (RandomProb(iDp)) {
         

}




      /* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
      /* choose the 'best' variable by default */
      /* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
      /* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
  if (RandomProb(iDp)) {
  pLit = pClauseLits[iClause];
  iNumCandidates = 0;

  if(iClauseLen <= 2){
   if(iClauseLen == 1){
     litPick = (pClauseLits[iClause][0]);
     iFlipCandidate = GetVarFromLit(litPick);
     return;
   }
   else{
     litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
     iFlipCandidate = GetVarFromLit(litPick);
     return;
   }

  }

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iVar = GetVarFromLit(*pLit);

        if((iVar != iBestVar) && (iVar != iSecondBestVar)) {
                aCandidateList[iNumCandidates++] = iVar;
        }

        pLit++;

  }
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
  return;


}


      iFlipCandidate = iBestVar;

      /* If the best is the youngest, with probability (iNovNoise) select the 2nd best */

      if (iFlipCandidate == iYoungestVar) {
      if((bPerformNoveltyAlternate && iBestScoreWithoutPen ==0)|| (RandomProb(iNovNoise))){
       iFlipCandidate = iSecondBestVar;
          return;
        }
      } else {

        /* If the best is older than then 2nd best, just choose the best */

        if (aVarLastChange[iSecondBestVar] >= aVarLastChange[iFlipCandidate]) {
          return;
        }
      }

      /* otherwise, determine the 'look ahead' score for the 2nd best variable */

      iSecondBestLookAheadScore = bPen ? aVarPenScore[iSecondBestVar] + BestLookAheadPenScore(iSecondBestVar): aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);

      if (iSecondBestLookAheadScore > iBestScore) {
        iBestLookAheadScore = iBestScore;
      } else {

        /* if the 'look ahead' score for the 2nd variable is better than the regular score
           for the best variable, calculate the look ahead score for the best variable */

        iBestLookAheadScore =bPen ?  aVarPenScore[iFlipCandidate] + BestLookAheadPenScore(iFlipCandidate) : aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
      }

      /* choose the variable with the best look ahead score */

      /* Note that this BREAKS TIES by selecting the 2nd best variable -- as in the paper and in the author's code */

      if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
        iFlipCandidate = iSecondBestVar;
      }
    }
  } else {
    iFlipCandidate = 0;
  }


 

}



UINT32 SelectClause() {
    
    UINT32 selectedClause;
    int j;
    int biggestLen = 0;
    UINT32 biggestClause =0;
    UINT32 biggestWeightedClause =0;
    UINT32 smallestFlipCountClause;
    FLOAT biggestClauseWeight = FLOATZERO;
    FLOAT lowestClauseVarFlipCount = iStep;
    FLOAT fRandClause;
    FLOAT fClauseSum;
    UINT32 iClause = 0;

    //printf("\n %u", iNumFalse); 
    if(!bWeighted){
    switch(iSelectClause){
      case 1: return aFalseList[RandomInt(iNumFalse)];
              break;

      case 2: for(j = 0; j<iNumFalse; j++){
              if(aClauseLen[aFalseList[j]] > biggestLen){
                 biggestLen = aClauseLen[aFalseList[j]];
                 biggestClause = aFalseList[j];
               } 
             }
             //printf("\n %d",biggestLen);
             return biggestClause;
             break;
    
     case 3:  smallestFlipCountClause = aFalseList[0];
              for(j = 0; j<iNumFalse; j++){
              if(fClauseVarFlipCounts[aFalseList[j]] <=  lowestClauseVarFlipCount){
                 lowestClauseVarFlipCount = fClauseVarFlipCounts[aFalseList[j]];
                 smallestFlipCountClause = aFalseList[j];
               }
             }
            // printf("\n %f ",lowestClauseVarFlipCount);
             return smallestFlipCountClause;
             break;
        case 4:smallestFlipCountClause = aFalseList[0]; 
               lowestClauseVarFlipCount = FLOATZERO;
               for(j = 0; j<iNumFalse; j++){
              if(fClauseVarFlipCounts[aFalseList[j]] >=  lowestClauseVarFlipCount){
                 lowestClauseVarFlipCount = fClauseVarFlipCounts[aFalseList[j]];
                 smallestFlipCountClause = aFalseList[j];
               }
             }
            // printf("\n %f ",lowestClauseVarFlipCount);
             return smallestFlipCountClause;
             break;

        case 5: 

                fRandClause = RandomFloat() * fSumClauseVarFlipCount;

                fClauseSum = FLOATZERO;

                for (j=0;j<iNumFalse;j++) {
                  iClause = aFalseList[j];
                  fClauseSum += fClauseVarFlipCounts[iClause];
                  if (fRandClause < fClauseSum) {
                  break;
                 }
                }
               return(iClause);
               break;
         
        case 6:

                fRandClause = -(RandomFloat() * fSumClauseVarFlipCount);

                fClauseSum = FLOATZERO;

                for (j=0;j<iNumFalse;j++) {
                  iClause = aFalseList[j];
                  fClauseSum -= fClauseVarFlipCounts[iClause];
                  if (fRandClause > fClauseSum) {
                  break;
                 }
                }
               return(iClause);
               break;

        case 7: if(bPen)
                return PickClauseWCSPen();
                break; 
     }
    }
    else{
      switch(iSelectClause){
      case 1: return PickClauseWCS();
              break;

      case 2: for(j = 0; j<iNumFalse; j++){
              if(aClauseLen[aFalseList[j]] > biggestLen){
                 biggestLen = aClauseLen[aFalseList[j]];
                 biggestClause = aFalseList[j];
               } 
             }
             return biggestClause;
             break;

      case 3: for(j = 0; j<iNumFalse; j++){
              if(aClauseWeight[aFalseList[j]] > biggestClauseWeight){
                 biggestClauseWeight = aClauseWeight[aFalseList[j]];
                 biggestWeightedClause = aFalseList[j];
               } 
             }
             return biggestWeightedClause;
             break;

       
     }


    }
      

    return 0;
}




void AddNoveltyPlusFC() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+fc","",FALSE,
    "Novelty+: Novelty with diversification mechanism based on flipcounts",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusFC",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange,FlipCounts",
    "default","default");

  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the variable that has been flipped for least number of times  from randomly selected unsat clause","",&iDp,0.01);

  CreateTrigger("PickNoveltyPlusFC",ChooseCandidate,PickNoveltyPlusFC,"","");

  pCurAlg = CreateAlgorithm("novelty+pfc","",FALSE,
    "Novelty+p: Noveltyp with diversification mechanism based on flipcount",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusPromisingFC",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");

  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the variable that has been flipped for least number of times  from randomly selected unsat clause","",&iDp,0.01);

  CreateTrigger("PickNoveltyPlusPromisingFC",ChooseCandidate,PickNoveltyPlusPromisingFC,"","");


}

void PickNoveltyPlusFC() {

  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iDp)  choose an unsatisfied clause,
     and then select the variable that has been flipped least number of times */


      if (RandomProb(iDp)) {
      if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];

      pLit = pClauseLits[iClause];

      iFlipCandidate = GetVarFromLit(*pLit);

      pLit++;

      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        if (aFlipCounts[iVar] < aFlipCounts[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }

        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */
    if(!bTabu)
    PickNovelty();
    else
    PickNoveltyTabu();
  }
}



void PickNoveltyPlusPromisingFC() {

  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iDp)  choose an unsatisfied clause,
     and then select the variable that has been flipped least number of times */


      if (RandomProb(iDp)) {
      if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];

      pLit = pClauseLits[iClause];

      iFlipCandidate = GetVarFromLit(*pLit);

      pLit++;

      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        if (aFlipCounts[iVar] < aFlipCounts[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }

        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */
    if(!bTabu)
    PickNoveltyPromising();
    else
    PickNoveltyPromisingTabu();
  }
}



void AddNoveltyTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("noveltytabu","",FALSE,
    "Novelty (tabu)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyTabu",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange,VarScore",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmUInt(&pCurAlg->parmList,"-selectclause","select clause [default %s]","specifies choice to select clause","",&iSelectClause,1);
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);
  CreateTrigger("PickNoveltyTabu",ChooseCandidate,PickNoveltyTabu,"","");


   pCurAlg = CreateAlgorithm("noveltytabu","",TRUE,
    "Novelty (tabu)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyTabuW",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange,VarScore",
    "default","default");
  
  CopyParameters(pCurAlg,"noveltytabu","",FALSE,0);

  CreateTrigger("PickNoveltyTabuW",ChooseCandidate,PickNoveltyTabuW,"","");

  

}

void PickNoveltyTabu()
{
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  LITTYPE litPick;
  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  UINT32 iTabuCutoff;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;
  
   
   
   /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }
  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause();
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

   
    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    

    iVar = GetVarFromLit(*pLit);   
    

    iScore = 0;
    /* Complex scoring mechanism aimed to make things efficient */
        if(!bPromisingList && !bPen){
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }
    }else if(!bPromisingList && bPen){
    //Promising List is not used but Clause Penalty is activated
        iNumOcc = aNumLitOcc[*pLit];
        pClause = pLitClause[*pLit];
        for (i=0;i<iNumOcc;i++) {
         if (aNumTrueLit[*pClause]==0) {
          iScore -= aClausePen[*pClause];
         }
        pClause++;
      }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
      }
      pClause++;
    }


   } 
   else{
  iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];

  }
  


 
    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

   if((aVarLastChange[iVar] < iTabuCutoff)||(iScore ==0))
    {

    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }

    
  }
    pLit++;
  }
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */
  
  //printf("\n %d The best score is ", aVarScore[iBestVar]);
  //printf("\n %d The second best score is ", aVarScore[iSecondBestVar]);

  //printf("%f ", ProbToFloat(iNovNoise));
  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;
}

void PickNoveltyTabuW()
{
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  LITTYPE litPick;
  UINT32 iYoungestVar;

  FLOAT fSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  UINT32 iTabuCutoff;

  fBestScore = fTotalWeight;
  fSecondBestScore = fTotalWeight;
  
   
   
   /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }
  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause();
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

   
    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    

    iVar = GetVarFromLit(*pLit);   
    

    if(aVarLastChange[iVar] < iTabuCutoff)
    {
    fScore = FLOATZERO;
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
    
    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((fScore < fBestScore) || ((fScore == fBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      fSecondBestScore = fBestScore;
      fBestScore = fScore;
    } else if ((fScore < fSecondBestScore) || ((fScore == fSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      fSecondBestScore = fScore;
    }

    
  }
    pLit++;
  }
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */
  
  //printf("\n %d The best score is ", aVarScore[iBestVar]);
  //printf("\n %d The second best score is ", aVarScore[iSecondBestVar]);

  //printf("%f ", ProbToFloat(iNovNoise));
  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;
}


void AddNoveltyPlusTabu() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+tabu","",FALSE,
    "Novelty+: Novelty with random walk",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusTabu",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"noveltytabu","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);
  
  CreateTrigger("PickNoveltyPlusTabu",ChooseCandidate,PickNoveltyPlusTabu,"","");

  pCurAlg = CreateAlgorithm("novelty+tabu","",TRUE,
    "Novelty+: Novelty with random walk",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusTabuW",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+tabu","",FALSE,0);

  
  CreateTrigger("PickNoveltyPlusTabuW",ChooseCandidate,PickNoveltyPlusTabuW,"","");

  

 
}
void PickNoveltyPlusTabu()
{
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 j;
  LITTYPE litPick;
  LITTYPE *pLit;
  UINT32 iVar;
  UINT32 iTabuCutoff;

  
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iNumCandidates = 0;
      for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);  
      if(aVarLastChange[iVar] < iTabuCutoff){
       aCandidateList[iNumCandidates++] = iVar;
       }
       pLit++;
      }
       if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
      else
    iFlipCandidate = aCandidateList[0];
      
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyTabu();
  }

}


void PickNoveltyPlusTabuW()
{
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 j;
  LITTYPE litPick;
  LITTYPE *pLit;
  UINT32 iVar;
  UINT32 iTabuCutoff;

  
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iNumCandidates = 0;
      for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);  
      if(aVarLastChange[iVar] < iTabuCutoff){
       aCandidateList[iNumCandidates++] = iVar;
       }
       pLit++;
      }
       if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
      else
    iFlipCandidate = aCandidateList[0];
      
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyTabuW();
  }

}


void AddNoveltyPlusPlusTabu() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++tabu","",FALSE,
    "Novelty++: Novelty+ with a modified diversification mechanism",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusTabu",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the least recently flipped~variable from a randomly selected unsat clause","",&iDp,0.05);
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);
  CreateTrigger("PickNoveltyPlusPlusTabu",ChooseCandidate,PickNoveltyPlusPlusTabu,"","");

  
  pCurAlg = CreateAlgorithm("novelty++tabu","",TRUE,
    "Novelty++: Novelty+ with a modified diversification mechanism and tabu",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusTabuW",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty++tabu","",FALSE,0);

  
  CreateTrigger("PickNoveltyPlusPlusTabuW",ChooseCandidate,PickNoveltyPlusPlusTabuW,"","");


}


void PickNoveltyPlusPlusTabu()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 iTabuCutoff;

   if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  
  
  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */
    
  
      if (RandomProb(iDp)) {
      if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

      if(aVarLastChange[iFlipCandidate] < iTabuCutoff){
        
      }
      else{
         iFlipCandidate = 0;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyTabu();
  }
}

void PickNoveltyPlusPlusTabuW()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 iTabuCutoff;

   if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  
  
  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */
    
  
      if (RandomProb(iDp)) {
      if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

      if(aVarLastChange[iFlipCandidate] < iTabuCutoff){
        
      }
      else{
         iFlipCandidate = 0;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyTabuW();
  }
}

void AddNoveltyPromisingTabu() {
  
 ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("noveltyptabu","",FALSE,
    "Novelty+p: Novelty with random walk and look-ahead",
    "Li, Wei, and Zhang [SAT 07]",
    "PickNoveltyPromisingTabu",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);

  CreateTrigger("PickNoveltyPromisingTabu",ChooseCandidate,PickNoveltyPromisingTabu,"","");

  pCurAlg = CreateAlgorithm("noveltyptabu","",TRUE,
    "Novelty+p: Novelty with random walk and look-ahead",
    "Li, Wei, and Zhang [SAT 07]",
    "PickNoveltyPromisingTabuW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVars,FalseClauseList,VarLastChange,LookAhead,VarScoreW",
    "default_w","default");

  
  CopyParameters(pCurAlg,"noveltyptabu","",FALSE,0);

  CreateTrigger("PickNoveltyPromisingTabuW",ChooseCandidate,PickNoveltyPromisingTabuW,"","");
  
  
}

void PickNoveltyPromisingTabu()
{

  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  SINT32 iScore;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  UINT32 iTabuCutoff;

  LITTYPE litPick;

  SINT32 iSecondBestLookAheadScore;
  SINT32 iBestLookAheadScore;

  /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
	
      iBestScore = iNumClauses;
      iSecondBestScore = iNumClauses;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        iScore = aVarScore[iVar];
        if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
          iYoungestVar = iVar;
        }
         if((aVarLastChange[iVar] < iTabuCutoff)||(iScore ==0))
        {
        if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
          iSecondBestVar = iBestVar;
          iBestVar = iVar;
          iSecondBestScore = iBestScore;
          iBestScore = iScore;
        } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
          iSecondBestVar = iVar;
          iSecondBestScore = iScore;
        }
        }
        pLit++;
      }
      
      iFlipCandidate = iBestVar;
   
      if (iFlipCandidate == iYoungestVar) {
        
        if (RandomProb(iNovNoise)) {
          iFlipCandidate = iSecondBestVar;
          return;
        } else {
         
          iSecondBestLookAheadScore = aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);
         
          if (iSecondBestLookAheadScore > iBestScore) {
            iBestLookAheadScore = iBestScore;
          } else {
            iBestLookAheadScore = aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
          }
          
          if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
          }
        }
      } else {
        
        if (aVarLastChange[iFlipCandidate] > aVarLastChange[iSecondBestVar]) {
          
          iSecondBestLookAheadScore = aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);
         
          if (iSecondBestLookAheadScore > iBestScore) {
            iBestLookAheadScore = iBestScore;
          } else {
            iBestLookAheadScore = aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
          }
          
          if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
           
          }
        } else {
          
        }
      }
    
  } else {
    iFlipCandidate = 0;
  }
    
}

void AddNoveltyPlusPromisingTabu() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+ptabu","",FALSE,
    "Novelty+p: Novelty+ with an additional look-ahead step",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusPromisingTabu",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+tabu","",FALSE,0);

  

  CreateTrigger("PickNoveltyPlusPromisingTabu",ChooseCandidate,PickNoveltyPlusPromisingTabu,"","");

 pCurAlg = CreateAlgorithm("novelty+ptabu","",TRUE,
    "Novelty+p: Novelty+ with an additional look-ahead step",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusPromisingTabuW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVars,FalseClauseList,VarLastChange,LookAhead,VarScoreW",
    "default_w","default");

  
  CopyParameters(pCurAlg,"novelty+tabu","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPromisingTabuW",ChooseCandidate,PickNoveltyPlusPromisingTabuW,"","");

 
}



void PickNoveltyPlusPromisingTabu()
{
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 j;
  LITTYPE litPick;
  LITTYPE *pLit;
  UINT32 iVar;
  UINT32 iTabuCutoff;

  
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iNumCandidates = 0;
      for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);  
      if(aVarLastChange[iVar] < iTabuCutoff){
       aCandidateList[iNumCandidates++] = iVar;
       }
       pLit++;
      }
       if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
      else
    iFlipCandidate = aCandidateList[0];
      
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyPromisingTabu();
  }

}


void AddNoveltyPlusPlusPromisingTabu() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++ptabu","",FALSE,
    "Novelty++p: Novelty++ with a look-ahead approach",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusPromisingTabu",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability PR, select the least recently flipped~variable from a randomly selected unsat clause","",&iDp,0.05);
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);
  CreateTrigger("PickNoveltyPlusPlusPromisingTabu",ChooseCandidate,PickNoveltyPlusPlusPromisingTabu,"","");



//TODO Weighted version

}





void PickNoveltyPlusPlusPromisingTabu()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 iTabuCutoff;

   if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  
  
  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */
    
  
      if (RandomProb(iDp)) {
      if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

      if(aVarLastChange[iFlipCandidate] < iTabuCutoff){
        
      }
      else{
         iFlipCandidate = 0;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyPromisingTabu();
  }
}


void AddNoveltyPlusPlusPrimeTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++'tabu","",FALSE,
    "Novelty++'tabu",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrimeTabu",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  //AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  

  CopyParameters(pCurAlg,"novelty++","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPlusPrimeTabu",ChooseCandidate,PickNoveltyPlusPlusPrimeTabu,"","");

  pCurAlg = CreateAlgorithm("novelty++'tabu","",TRUE,
    "Novelty++'(weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrimeTabuW",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  //AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  

  CopyParameters(pCurAlg,"novelty++'","",FALSE,0);

 
  CreateTrigger("PickNoveltyPlusPlusPrimeTabuW",ChooseCandidate,PickNoveltyPlusPlusPrimeTabuW,"","");

}


void AddNoveltyPlusPlusPrimePromisingTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty++'primetabu","",FALSE,
    "Novelty++'tabu",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyPlusPlusPrimePromisingTabu",
    "DefaultProcedures,Flip+Trackchanges+FCL,VarLastChange,LookAhead,DecPromVars",
    "default","default");

  
  CopyParameters(pCurAlg,"novelty++tabu","",FALSE,0);


  CreateTrigger("PickNoveltyPlusPlusPrimePromisingTabu",ChooseCandidate,PickNoveltyPlusPlusPrimePromisingTabu,"","");


}

void PickNoveltyPlusPlusPrimeTabu()
{
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  UINT32 iTabuCutoff;

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;

  
   /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  
  /* When we pick randomly a variable from the clause we need to
     create a pool of variables excluding the best and second best*/

  iNumCandidates = 0;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

   
    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    

    iVar = GetVarFromLit(*pLit);   
    

    iScore = 0;
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }
    
    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */
   if((aVarLastChange[iVar] < iTabuCutoff)||(iScore ==0)){
    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }

    
  }
    pLit++;
  }
  
/* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
  
  if (RandomProb(iDp)) { 
  pLit = pClauseLits[iClause];
  iNumCandidates = 0;

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iVar = GetVarFromLit(*pLit);

	if((iVar != iBestVar) && (iVar != iSecondBestVar)&&(aVarLastChange[iVar] < iTabuCutoff)) {
		aCandidateList[iNumCandidates++] = iVar;
	}

	pLit++;

  }
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
  return;


}
  
 /* Perform Novelty */ 
  
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */

  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;

}



void PickNoveltyPlusPlusPrimeTabuW()
{
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  UINT32 iTabuCutoff;

  UINT32 iYoungestVar;

  FLOAT fSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  fBestScore = fTotalWeight;
  fSecondBestScore = fTotalWeight;

  
   /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }



  
  /* When we pick randomly a variable from the clause we need to
     create a pool of variables excluding the best and second best*/

  iNumCandidates = 0;

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

  for (j=0;j<iClauseLen;j++) {

   
    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    

    iVar = GetVarFromLit(*pLit);   
    

    if(aVarLastChange[iVar] < iTabuCutoff)
    {
    fScore = 0;
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
    
    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((fScore < fBestScore) || ((fScore == fBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      fSecondBestScore = fBestScore;
      fBestScore = fScore;
    } else if ((fScore < fSecondBestScore) || ((fScore == fSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      fSecondBestScore = fScore;
    }

    
  }
    pLit++;
  }
  
/* For diversification probability dp a variable is selected at random after excluding the best and second best
   variables */
  
  if (RandomProb(iDp)) { 
  pLit = pClauseLits[iClause];
  iNumCandidates = 0;

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iVar = GetVarFromLit(*pLit);

	if((iVar != iBestVar) && (iVar != iSecondBestVar)&&(aVarLastChange[iVar] < iTabuCutoff)) {
		aCandidateList[iNumCandidates++] = iVar;
	}

	pLit++;

  }
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
  return;


}
  
 /* Perform Novelty */ 
  
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */

  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;

}


void PickNoveltyPromisingW()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  FLOAT  fScore;
  UINT32 iYoungestVar;
  FLOAT  fSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  LITTYPE litPick;

  FLOAT fSecondBestLookAheadScore;
  FLOAT fBestLookAheadScore;

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
	
      fBestScore = fTotalWeight;
      fSecondBestScore = fTotalWeight;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        fScore = aVarScoreW[iVar];
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
      
      if (iFlipCandidate == iYoungestVar) {
        
        if (RandomProb(iNovNoise)) {
          iFlipCandidate = iSecondBestVar;
          return;
        } else {
          
          fSecondBestLookAheadScore = aVarScoreW[iSecondBestVar] + (FLOAT) BestLookAheadScore(iSecondBestVar);
          
          if (fSecondBestLookAheadScore > fBestScore) {
            fBestLookAheadScore = fBestScore;
          } else {
            fBestLookAheadScore = aVarScoreW[iFlipCandidate] + (FLOAT) BestLookAheadScore(iFlipCandidate);
          }
          
          if (fBestLookAheadScore >= fSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
          }
        }
      } else {
        
        if (aVarLastChange[iFlipCandidate] > aVarLastChange[iSecondBestVar]) {
          
          fSecondBestLookAheadScore = aVarScoreW[iSecondBestVar] + (FLOAT) BestLookAheadScore(iSecondBestVar);
          
          if (fSecondBestLookAheadScore > fBestScore) {
            fBestLookAheadScore = fBestScore;
          } else {
            fBestLookAheadScore = aVarScoreW[iFlipCandidate] + (FLOAT) BestLookAheadScore(iFlipCandidate);
          }
          
          if (fBestLookAheadScore >= fSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
            
          }
        } else {
          
        }
      }
    
  } else {
    iFlipCandidate = 0;
  }

    
}



void PickNoveltyPromisingTabuW()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  FLOAT  fScore;
  UINT32 iYoungestVar;
  FLOAT  fSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  UINT32 iTabuCutoff;

  LITTYPE litPick;

  FLOAT fSecondBestLookAheadScore;
  FLOAT fBestLookAheadScore;

  /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
	
      fBestScore = fTotalWeight;
      fSecondBestScore = fTotalWeight;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        if(aVarLastChange[iVar] < iTabuCutoff)
        {
        fScore = aVarScoreW[iVar];
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
        }
        pLit++;
      }
      
      iFlipCandidate = iBestVar;
     
      if (iFlipCandidate == iYoungestVar) {
       
        if (RandomProb(iNovNoise)) {
          iFlipCandidate = iSecondBestVar;
          return;
        } else {
          
          fSecondBestLookAheadScore = aVarScoreW[iSecondBestVar] + (FLOAT) BestLookAheadScore(iSecondBestVar);
          
          if (fSecondBestLookAheadScore > fBestScore) {
            fBestLookAheadScore = fBestScore;
          } else {
            fBestLookAheadScore = aVarScoreW[iFlipCandidate] + (FLOAT) BestLookAheadScore(iFlipCandidate);
          }
          
          if (fBestLookAheadScore >= fSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
          }
        }
      } else {
        
        if (aVarLastChange[iFlipCandidate] > aVarLastChange[iSecondBestVar]) {
          
          fSecondBestLookAheadScore = aVarScore[iSecondBestVar] + (FLOAT) BestLookAheadScore(iSecondBestVar);
          
          if (fSecondBestLookAheadScore > fBestScore) {
            fBestLookAheadScore = fBestScore;
          } else {
            fBestLookAheadScore = aVarScoreW[iFlipCandidate] + (FLOAT) BestLookAheadScore(iFlipCandidate);
          }
          
          if (fBestLookAheadScore >= fSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
            
          }
        } else {
          
        }
      }
    
  } else {
    iFlipCandidate = 0;
  }


}



void PickNoveltyPlusPromisingW()
{
 
  UINT32 iClause;
  UINT32 iClauseLen;

  LITTYPE litPick;

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyPromisingW();
  }
}


void PickNoveltyPlusPromisingTabuW()
{
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 j;
  LITTYPE litPick;
  LITTYPE *pLit;
  UINT32 iVar;
  UINT32 iTabuCutoff;

  
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iNumCandidates = 0;
      for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);  
      if(aVarLastChange[iVar] < iTabuCutoff){
       aCandidateList[iNumCandidates++] = iVar;
       }
       pLit++;
      }
       if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
      else
    iFlipCandidate = aCandidateList[0];
      
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyPromisingTabuW();
  }

}


void PickNoveltyPlusPlusPromisingW() {
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */

  
  if (RandomProb(iDp)) {
    if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

    /* otherwise, use regular novelty */

    PickNoveltyPromisingW();
  }
}


void PickNoveltyPlusPlusPromisingTabuW()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 iTabuCutoff;

   if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  
  
  /* with probability (iDp) uniformly choose an unsatisfied clause,
     and then select the "oldest" literal from that clause */
    
  
      if (RandomProb(iDp)) {
      if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
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

      if(aVarLastChange[iFlipCandidate] < iTabuCutoff){
        
      }
      else{
         iFlipCandidate = 0;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNoveltyPromisingTabuW();
  }
}



void PickNoveltyPlusPlusPrimePromisingTabu()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  SINT32 iScore;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  UINT32 iTabuCutoff;
  
  LITTYPE litPick;
  SINT32 iSecondBestLookAheadScore;
  SINT32 iBestLookAheadScore;
  
  
   /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }


  if (iNumFalse) {
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];

      iBestScore = iNumClauses;
      iSecondBestScore = iNumClauses;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        iScore = aVarScore[iVar];
        if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
          iYoungestVar = iVar;
        }
         if((aVarLastChange[iVar] < iTabuCutoff)||(iScore ==0)){
          if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
          iSecondBestVar = iBestVar;
          iBestVar = iVar;
          iSecondBestScore = iBestScore;
          iBestScore = iScore;
        } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
          iSecondBestVar = iVar;
          iSecondBestScore = iScore;
        }
      }
        pLit++;
      }

      iFlipCandidate = iBestVar;

      if (RandomProb(iDp)) {
        pLit = pClauseLits[iClause];
        iNumCandidates = 0;

        for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

        iVar = GetVarFromLit(*pLit);

            if((iVar != iBestVar) && (iVar != iSecondBestVar)) {
                    aCandidateList[iNumCandidates++] = iVar;
            }

            pLit++;

    }
      if (iNumCandidates > 1)
        iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
      else
        iFlipCandidate = aCandidateList[0];
      return;
  }
  //perform Noveltyp Step

  iFlipCandidate = iBestVar;
      if (iFlipCandidate == iYoungestVar) {
        if (RandomProb(iNovNoise)) {
          iFlipCandidate = iSecondBestVar;
          return;
        } else {
          iSecondBestLookAheadScore = aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);
          if (iSecondBestLookAheadScore > iBestScore) {
            iBestLookAheadScore = iBestScore;
          } else {
            iBestLookAheadScore = aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
          }
          if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
          }
        }
      } else {
        if (aVarLastChange[iFlipCandidate] > aVarLastChange[iSecondBestVar]) {
          iSecondBestLookAheadScore = aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);
          if (iSecondBestLookAheadScore > iBestScore) {
            iBestLookAheadScore = iBestScore;
          } else {
            iBestLookAheadScore = aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
          }
          if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
          }
        } else {
        }
      }



  }
  else {
    iFlipCandidate = 0;
  }


      
 } 


void PickNoveltyPlusPlusPrimePromisingW()
{
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  FLOAT  fScore;
  UINT32 iYoungestVar;
  FLOAT  fSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  LITTYPE litPick;

  FLOAT fSecondBestLookAheadScore;
  FLOAT fBestLookAheadScore;
  printf("I am here");
  if (iNumFalse) {
      //iClause = aFalseList[RandomInt(iNumFalse)];
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
	
      fBestScore = fTotalWeight;
      fSecondBestScore = fTotalWeight;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        fScore = aVarScoreW[iVar];
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

      if (RandomProb(iDp)) { 
        pLit = pClauseLits[iClause];
        iNumCandidates = 0;

        for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

        iVar = GetVarFromLit(*pLit);

	    if((iVar != iBestVar) && (iVar != iSecondBestVar)) {
		    aCandidateList[iNumCandidates++] = iVar;
	    }

	    pLit++;

    }
      if (iNumCandidates > 1)
        iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
      else
        iFlipCandidate = aCandidateList[0];
      return;
  }
  //perform Noveltyp Step

  iFlipCandidate = iBestVar;

      
      if (iFlipCandidate == iYoungestVar) {
        
        if (RandomProb(iNovNoise)) {
          iFlipCandidate = iSecondBestVar;
          return;
        } else {
          
          fSecondBestLookAheadScore = aVarScoreW[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);
          
          if (fSecondBestLookAheadScore > fBestScore) {
            fBestLookAheadScore = fBestScore;
          } else {
            fBestLookAheadScore = aVarScoreW[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
          }
         
          if (fBestLookAheadScore >= fSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
          }
        }
      } else {
        
        if (aVarLastChange[iFlipCandidate] > aVarLastChange[iSecondBestVar]) {
         
          fSecondBestLookAheadScore = aVarScoreW[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);
         
          if (fSecondBestLookAheadScore > fBestScore) {
            fBestLookAheadScore = fBestScore;
          } else {
            fBestLookAheadScore = aVarScoreW[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
          }
          
          if (fBestLookAheadScore >= fSecondBestLookAheadScore) {
            iFlipCandidate = iSecondBestVar;
            
          }
        } else {
         
        }
      }

      
  
  }
  else {
    iFlipCandidate = 0;
  }

}









SINT32 BestLookAheadPenScore(UINT32 iLookVar) {
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWillbeTrue;
  LITTYPE litWillbeFalse;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;

  LITTYPE *pLit;
  UINT32 iNumLookAhead;
  SINT32 iScore;

  SINT32 iBestLookAheadScore;

  if (iLookVar == 0) {
    return(0);
  }

  iNumLookAhead = 0;

  for (j=0;j<iNumDecPromVars;j++) {
    UpdateLookAhead(aDecPromVarsList[j],0);
  }


  litWasTrue = GetTrueLit(iLookVar);
  litWasFalse = GetFalseLit(iLookVar);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,-aClausePen[*pClause]);
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==2) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          if (iVar != iLookVar) {
            UpdateLookAhead(iVar,+aClausePen[*pClause]);
            break;
          }
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==0) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,+aClausePen[*pClause]);
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      UpdateLookAhead(iVar,-aClausePen[*pClause]);
    }
    pClause++;
  }

  iBestLookAheadScore = iNumClauses;

  for (j=0;j<iNumLookAhead;j++) {
    iVar = aLookAheadList[j];
    if (iVar != iLookVar) {
      iScore = aVarScore[iVar] + aLookAheadScoreChange[iVar];

      // This is to mimic g2wsat implementation:
      if ((j < iNumDecPromVars)||(aVarPenScore[iVar]>=0)) {
        if (iScore < iBestLookAheadScore) {
          iBestLookAheadScore = iScore;
        }
      }

    }
    aIsLookAhead[iVar] = FALSE;
  }
  if (iBestLookAheadScore < 0) {
    return(iBestLookAheadScore);
  } else {
    return(0);
  }
}

