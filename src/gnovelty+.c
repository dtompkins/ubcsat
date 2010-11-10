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

#ifdef __cplusplus 
namespace ubcsat {
#endif

void PickGNovletyPlus();
void PickGNovletyPlusVersion1();
void ScalePawsWithDecPromVars();
void SmoothPawsWithDecPromVars();

void AddGNoveltyPlus() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gnovelty+","",0,
    "gNovelty+",
    "Pham, Thornton, Gretton, Sattar [AI 07]",
    "PickGNovletyPlusVersion1",
    "DefaultProcedures,Flip+PenaltyDecPromVars+FCL,PenaltyDecPromVars,FalseClauseList,VarLastChange,PenClauseList,AdaptNoveltyNoise",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.01);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","after a scaling step, ~smooth penalties with probability PR","",&iPs,0.4);

  CreateTrigger("PickGNovletyPlusVersion1",ChooseCandidate,PickGNovletyPlusVersion1,"","");

  //CreateTrigger("PickGNovletyPlus",ChooseCandidate,PickGNovletyPlus,"","");
  

}

void PickGNovletyPlus() {

  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  UINT32 iVar;
  SINT32 iScore;
  LITTYPE *pLit;
  UINT32 j;
  
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  if (iNumFalse) {

    // with probability iNovWpDp, do a WalkSAT random walk

    if (RandomProb(iNovWpDp)) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);

    } else if (iNumPenaltyDecPromVars > 0 ) { // if (weighted) Decreasing Promising Variables Exist, select one

      iFlipCandidate = aPenaltyDecPromVarsList[0];
      iBestScore = (SINT32) aBreakPenaltyINT[iFlipCandidate] - (SINT32) aMakePenaltyINT[iFlipCandidate];
      for (j=1;j<iNumPenaltyDecPromVars;j++) {
        iVar = aPenaltyDecPromVarsList[j];
        iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
        if (iScore < iBestScore) {
          iFlipCandidate = iVar;
          iBestScore = iScore;
        } else {
          // If there is a tie, prefer the 'oldest' variable
          if (iScore == iBestScore) {
            if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
              iFlipCandidate = iVar;
            }
          }
        }
      }
    } else {

      // Note: as per original author's gNovelty+ code, perform Clause penalty update before 'Novelty-like' step

      ScalePawsWithDecPromVars();

      /* with probability, perform smoothing */

      if (RandomProb(iPs)) {
        SmoothPawsWithDecPromVars();
      }

      /* now pick according to (Modified) Adaptive Novelty+ */

      iBestScore = (SINT32) iTotalPenaltyINT;
      iSecondBestScore = (SINT32) iTotalPenaltyINT;

      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
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
      // This is where the gNovelty+ is different than traditional Novelty...
      if (iBestScore == 0) { // NOTE: gNovelty+ 1.0 uses the UNweighted score
        iFlipCandidate = iSecondBestVar;
      } else {
        if (RandomProb(iNovNoise)) { 
          iFlipCandidate = iSecondBestVar;
        }
      }
    }
  }
}

void PickGNovletyPlusVersion1() {

  // See regular version...
  // the difference here is to check if the UNweighted score is == 0

  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  UINT32 iVar;
  SINT32 iScore;
  LITTYPE *pLit;
  UINT32 i;
  UINT32 j;
  UINT32 *pClause;
  UINT32 iNumOcc;


  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  if (iNumFalse) {

    // with probability iNovWpDp, do a WalkSAT random walk

    if (RandomProb(iNovWpDp)) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);

    } else if (iNumPenaltyDecPromVars > 0 ) { // if (weighted) Decreasing Promising Variables Exist, select one

      iFlipCandidate = aPenaltyDecPromVarsList[0];
      iBestScore = (SINT32) aBreakPenaltyINT[iFlipCandidate] - (SINT32) aMakePenaltyINT[iFlipCandidate];
      for (j=1;j<iNumPenaltyDecPromVars;j++) {
        iVar = aPenaltyDecPromVarsList[j];
        iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
        if (iScore < iBestScore) {
          iFlipCandidate = iVar;
          iBestScore = iScore;
        } else {
          // If there is a tie, prefer the 'oldest' variable
          if (iScore == iBestScore) {
            if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
              iFlipCandidate = iVar;
            }
          }
        }
      }
    } else {

      // Note: as per original author's gNovelty+ code, perform Clause penalty update before 'Novelty-like' step

      ScalePawsWithDecPromVars();

      /* with probability, perform smoothing */

      if (RandomProb(iPs)) {
        SmoothPawsWithDecPromVars();
      }

      /* now pick according to (Modified) Adaptive Novelty+ */

      iBestScore = (SINT32) iTotalPenaltyINT;
      iSecondBestScore = (SINT32) iTotalPenaltyINT;

      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
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
      // This is where the gNovelty+ is different than traditional Novelty...
      
      // NOTE: gNovelty+ 1.0 uses the UNweighted score

      // Calculate UNweighted score of BestVar
      {
        iScore = 0;
        litPick = GetFalseLit(iBestVar);
        iNumOcc = aNumLitOcc[litPick];
        pClause = pLitClause[litPick];
        for (i=0;i<iNumOcc;i++) {
          if (aNumTrueLit[*pClause]==0) {
            iScore--;
          }
          pClause++;
        }
        iNumOcc = aNumLitOcc[GetNegatedLit(litPick)];
        pClause = pLitClause[GetNegatedLit(litPick)];
        for (i=0;i<iNumOcc;i++) {
          if (aNumTrueLit[*pClause]==1) {
            iScore++;
          }
          pClause++;
        }
      }
      if (iScore == 0) { 
        iFlipCandidate = iSecondBestVar;
      } else {
        if (RandomProb(iNovNoise)) { 
          iFlipCandidate = iSecondBestVar;
        }
      }
    }
  }
}

void ScalePawsWithDecPromVars() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  UINT32 iClause;
  LITTYPE *pLit;

  for(j=0;j<iNumFalse;j++) {
    iClause = aFalseList[j];
    aClausePenaltyINT[iClause]++;
    if (aClausePenaltyINT[iClause]==2) {
      aPenClauseList[iNumPenClauseList] = iClause;
      aPenClauseListPos[iClause] = iNumPenClauseList++;
    }
    pLit = pClauseLits[iClause];
    for (k=0;k<aClauseLen[iClause];k++) {
      iVar = GetVarFromLit(*pLit);
      aMakePenaltyINT[iVar]++;
      if ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1) { // todo step-1 accurate here?
        aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
      }
      pLit++;
    }
  }
}

void SmoothPawsWithDecPromVars() {
  
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  UINT32 iClause;
  UINT32 iLoopMax;
  LITTYPE *pLit;

  iLoopMax = iNumPenClauseList;
  iTotalPenaltyINT -= iNumPenClauseList;
  for (j=0;j<iLoopMax;j++) {
    iClause = aPenClauseList[j];
    aClausePenaltyINT[iClause]--;
    if (aClausePenaltyINT[iClause]==1) {
      aPenClauseList[aPenClauseListPos[iClause]] = aPenClauseList[--iNumPenClauseList]; // can be better... fix in PAWS too
      aPenClauseListPos[aPenClauseList[iNumPenClauseList]] = aPenClauseListPos[iClause];
    }
    if (aNumTrueLit[iClause]==1) {
      iVar = aCritSat[iClause];
      aBreakPenaltyINT[iVar]--;
      if ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1) { // todo (see above) -- also, orig. authors don't do this
        aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
      }
    }
    if (aNumTrueLit[iClause]==0) {
      pLit = pClauseLits[iClause];
      for (k=0;k<aClauseLen[iClause];k++) {
        aMakePenaltyINT[GetVarFromLit(*pLit)]--;
        pLit++;
      }
    }
  }
}




#ifdef __cplusplus
}
#endif

