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

extern UINT32 *aNumVarsShareClause; // todo move
extern UINT32 **pVarsShareClause;

void PickGNovletyPlus();
void PickGNovletyPlusVersion1();

void ScaleGNovP();
void SmoothGNovP();

/***** Trigger GNovPromVars *****/

void CreateGNovPromVars();
void InitGNovPromVars();
void UpdateGNovPromVars();

UINT32 *aGNovPromVarsList;
UINT32 *aVarIsGNovProm;
UINT32 iNumGNovPromVars;

void FlipGNovPromVarsFCL();


void AddGNoveltyPlus() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gnovelty+","",0,
    "gNovelty+",
    "Pham, Thornton, Gretton, Sattar [AI 07]",
    "PickGNovletyPlusVersion1",
    "DefaultProcedures,Flip+GNovPromVars+FCL,GNovPromVars,FalseClauseList,VarLastChange,PenClauseList,AdaptNoveltyNoise,VarsShareClauses",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.01);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","after a scaling step, ~smooth penalties with probability PR","",&iPs,0.4);

  CreateTrigger("PickGNovletyPlusVersion1",ChooseCandidate,PickGNovletyPlusVersion1,"","");

  //CreateTrigger("PickGNovletyPlus",ChooseCandidate,PickGNovletyPlus,"","");

  CreateTrigger("CreateGNovPromVars",CreateStateInfo,CreateGNovPromVars,"CreateTrackChanges,CreateMakeBreakPenaltyINT","CreateVarScore");
  CreateTrigger("InitGNovPromVars",InitStateInfo,InitGNovPromVars,"InitTrackChanges,InitMakeBreakPenaltyINT","InitVarScore");
  CreateTrigger("UpdateGNovPromVars",UpdateStateInfo,UpdateGNovPromVars,"UpdateTrackChanges,UpdateMakeBreakPenaltyINT","UpdateVarScore");
  CreateContainerTrigger("GNovPromVars","CreateGNovPromVars,InitGNovPromVars,UpdateGNovPromVars");

  CreateTrigger("Flip+GNovPromVars+FCL",FlipCandidate,FlipGNovPromVarsFCL,"GNovPromVars,FalseClauseList","DefaultFlip,UpdateTrackChanges,UpdateGNovPromVars,UpdateFalseClauseList,CreateTrackChanges,InitTrackChanges,UpdateTrackChanges,UpdateMakeBreakPenaltyINT");

}

void PickGNovletyPlusVersion1() {

  // in Version 1.0, the difference here is to check if the UNweighted score is == 0

  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  UINT32 iVar;
  SINT32 iScore;
  LITTYPE *pLit;
  UINT32 i;
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iNumOcc;

  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;
  
  iFlipCandidate = 0;

  if (iNumFalse==0) {
    return;
  }

  // with probability iNovWpDp, do a WalkSAT random walk

  if (RandomProb(iNovWpDp)) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
    litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
    iFlipCandidate = GetVarFromLit(litPick);
    return;
  } 

  if (iNumGNovPromVars > 0 ) { 
    // note: aGNovPromVarsList is "dirty", so we clean it up as we go
    iBestScore = (SINT32) iTotalPenaltyINT;
    j=0;
    k=0;
    while (j < iNumGNovPromVars) {
      iVar = aGNovPromVarsList[k];
      iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
      if (iScore >= 0) {
        iNumGNovPromVars--;
        aVarIsGNovProm[iVar] = 0;
      } else {
        if (iScore < iBestScore) {
          iFlipCandidate = iVar;
          iBestScore = iScore;
        } else {
          if (iScore == iBestScore) {
            if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
              iFlipCandidate = iVar;
            }
          }
        }
        aGNovPromVarsList[j++]=aGNovPromVarsList[k];
      }
      k++;
    }
  }
  if (iFlipCandidate != 0) {
    return;
  }

  // Note: as per original author's gNovelty+ code, perform Clause penalty update before 'Novelty-like' step

  ScaleGNovP();

  /* with probability, perform smoothing */

  if (RandomProb(iPs)) {
    SmoothGNovP();
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



void ScaleGNovP() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  UINT32 iClause;
  LITTYPE *pLit;

  // Do PAWS scaling and update GNovPromVars according to original author's scheme

  iTotalPenaltyINT += iNumFalse;

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
			if ((aVarIsGNovProm[iVar] == 0) && ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] < 0 ) && (aVarLastChange[iVar] < iStep - 1)) {
				aGNovPromVarsList[iNumGNovPromVars++] = iVar;
				aVarIsGNovProm[iVar] = 1;
			} 
      pLit++;
    }
  }
}

void SmoothGNovP() {
  
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  UINT32 iClause;
  UINT32 iLoopMax;
  LITTYPE *pLit;

  // do NOT update aGNovPromVarsList as per original author's code

  iTotalPenaltyINT -= iNumPenClauseList;

  iLoopMax = iNumPenClauseList;

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

void FlipGNovPromVarsFCL() {

  // update GNovPromVars according to author's scheme

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  SINT32 iPenalty;

  UINT32 *pShareVar;
  SINT32 iShareScore;

  if (iFlipCandidate == 0) {
    return;
  }

  pShareVar = pVarsShareClause[iFlipCandidate];
  for (j=0; j < aNumVarsShareClause[iFlipCandidate]; j++) {
    iShareScore = (SINT32) aBreakPenaltyINT[*pShareVar] - (SINT32) aMakePenaltyINT[*pShareVar];
    if (iShareScore < 0) {
      aVarIsGNovProm[*pShareVar] = 1;
    } else {
      aVarIsGNovProm[*pShareVar] = 0;
    }
    pShareVar++;
  }

  /// REGULAR FLIP ///

  iNumChanges = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      //aBreakCount[iFlipCandidate]--;
      
      aBreakPenaltyINT[iFlipCandidate] -= iPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        //aMakeCount[iVar]++;
        aMakePenaltyINT[iVar] += iPenalty;

        //if (aMakeCount[iVar]==1) {
        //  aVarInFalseList[iNumVarsInFalseList] = iVar;
        //  aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        //}
        
        pLit++;

      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          //aBreakCount[iVar]++;
          aBreakPenaltyINT[iVar] += iPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        //aMakeCount[iVar]--;
        aMakePenaltyINT[iVar] -= iPenalty;

        //if (aMakeCount[iVar]==0) {
        //  aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
        //  aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        //}
        
        pLit++;

      }
      //aBreakCount[iFlipCandidate]++;
      aBreakPenaltyINT[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      //aBreakCount[aCritSat[*pClause]]--;
      aBreakPenaltyINT[aCritSat[*pClause]] -= iPenalty;
    }
    pClause++;
  }

/* PREVIOUS ATTEMPT

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    if (aNumTrueLit[*pClause]==1) {
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
      
      //aVarScore[iFlipCandidate]--;
      aBreakPenaltyINT[iFlipCandidate] -= iPenalty;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        iPrevScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
        aMakePenaltyINT[iVar] += iPenalty;
        //if ((iPrevScore >= 0)&&(iPrevScore < iPenalty)) { // ie: new score now < 0
        //  aGNovPromVarsList[iNumGNovPromVars++] = iVar;
        //}
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      iPrevScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
      //aVarScore[iVar]--;
      aBreakPenaltyINT[iVar] -= iPenalty;
      //if ((iPrevScore >= 0)&&(iPrevScore < iPenalty)) { // see above
      //  aGNovPromVarsList[iNumGNovPromVars++] = iVar;
      //}
    }
    pClause++;
  }

  // 2nd pass

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          //aVarScore[iVar]++;
          aBreakPenaltyINT[iVar] += iPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        //aVarScore[iVar]++;
        aMakePenaltyINT[iVar] -= iPenalty;
        pLit++;
      }
      //aVarScore[iFlipCandidate]++;
      aBreakPenaltyINT[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    pClause++;
  }
  */

  pShareVar = pVarsShareClause[iFlipCandidate];
  for (j=0; j < aNumVarsShareClause[iFlipCandidate]; j++) {
    iShareScore = (SINT32) aBreakPenaltyINT[*pShareVar] - (SINT32) aMakePenaltyINT[*pShareVar];
    if (iShareScore < 0) {
      if (aVarIsGNovProm[*pShareVar] == 0) {
        aGNovPromVarsList[iNumGNovPromVars++] = *pShareVar;
        aVarIsGNovProm[*pShareVar] = 1;
      }
    }
    pShareVar++;
  }
}

void CreateGNovPromVars() {
  aGNovPromVarsList = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aVarIsGNovProm = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
}

void InitGNovPromVars() {

  UINT32 j;

  iNumGNovPromVars = 0;

  for (j=1;j<=iNumVars;j++) {
    if ((SINT32) aBreakPenaltyINT[j] - (SINT32) aMakePenaltyINT[j] < 0) {
      aGNovPromVarsList[iNumGNovPromVars++] = j;
      aVarIsGNovProm[j] = 1;
    } else {
      aVarIsGNovProm[j] = 0;
    }
  }
}

void UpdateGNovPromVars() {

  ReportPrint(pRepErr,"Unexpected Error: TODO: UpdateGNovPromVars\n");
  AbnormalExit();
  exit(1);
}








// TODO -- THE FOLLOWING SHOULDN'T BE USED FOR NOW

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
      //TODO
      //if ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1) { // todo step-1 accurate here?
      if (((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1)&&(aVarLastChange[iVar] < iStep - 1)) { // todo step-1 accurate here?
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
      //TODO
      if ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1) { // todo (see above) -- also, orig. authors don't do this
      //  aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
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

