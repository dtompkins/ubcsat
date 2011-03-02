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

void PickSparrow();
void ScaleSparrow();
void InitSparrow();
void SparrowSat2011Settings();

void CreateGNovPromVars();
void InitGNovPromVars();
void UpdateGNovPromVars();
void FlipGNovPromVarsFCL();


UINT32 *aGNovPromVarsList;
UINT32 *aVarIsGNovProm;
UINT32 iNumGNovPromVars;

FLOAT *aSparrowScores;
void CreateSparrowScores() {
  aSparrowScores = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
}

FLOAT fSparrowC1;
UINT32 iSparrowC2;
FLOAT fSparrowC3;
FLOAT fInvSparrowC3;

FLOAT aSparrowPreCalc[11];


void AddSparrow() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("sparrow","",0,
    "gNovelty+",
    "Balint, Froehlich [SAT 10]",
    "PickSparrow",
    "DefaultProcedures,Flip+GNovPromVars+FCL,GNovPromVars,FalseClauseList,VarLastChange,PenClauseList,AdaptNoveltyNoise,VarsShareClauses,CreateSparrowScores,InitSparrow",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","after a scaling step, ~smooth penalties with probability PR","",&iPs,0.347);
  
  AddParmFloat(&pCurAlg->parmList,"-c1","sparrow score adjustment parameter [default %s]","adjusts the importance of the score","",&fSparrowC1,2.0);
  AddParmUInt(&pCurAlg->parmList,"-c2","sparrow age polynomial parameter [default %s]","adjusts the influence of the age","",&iSparrowC2,4);
  AddParmFloat(&pCurAlg->parmList,"-c3","sparrow age threshold parameter [default %s]","threshold for age calculation","",&fSparrowC3,100000.0);
  
  CreateTrigger("PickSparrow",ChooseCandidate,PickSparrow,"","");
  CreateTrigger("CreateSparrowScores",CreateStateInfo,CreateSparrowScores,"","");

  CreateTrigger("CreateGNovPromVars",CreateStateInfo,CreateGNovPromVars,"CreateTrackChanges,CreateMakeBreakPenaltyINT","CreateVarScore");
  CreateTrigger("InitGNovPromVars",InitStateInfo,InitGNovPromVars,"InitTrackChanges,InitMakeBreakPenaltyINT","InitVarScore");
  CreateTrigger("UpdateGNovPromVars",UpdateStateInfo,UpdateGNovPromVars,"UpdateTrackChanges,UpdateMakeBreakPenaltyINT","UpdateVarScore");
  CreateContainerTrigger("GNovPromVars","CreateGNovPromVars,InitGNovPromVars,UpdateGNovPromVars");

  CreateTrigger("Flip+GNovPromVars+FCL",FlipCandidate,FlipGNovPromVarsFCL,"GNovPromVars,FalseClauseList","DefaultFlip,UpdateTrackChanges,UpdateGNovPromVars,UpdateFalseClauseList,CreateTrackChanges,InitTrackChanges,UpdateTrackChanges,UpdateMakeBreakPenaltyINT");

  CreateTrigger("InitSparrow",PostRead,InitSparrow,"","");

  pCurAlg = CreateAlgorithm("sparrow","sat11",0,
    "Sparrow [SAT 11 Competition]",
    "Balint, Froehlich [SAT 10]",
    "PickSparrow,SparrowSat2011Settings",
    "DefaultProcedures,Flip+GNovPromVars+FCL,GNovPromVars,FalseClauseList,VarLastChange,PenClauseList,AdaptNoveltyNoise,VarsShareClauses,CreateSparrowScores",
    "default","default");

  CreateTrigger("SparrowSat2011Settings",PostRead,SparrowSat2011Settings,"","");
}

void SparrowSat2011Settings() {
  // These settings provided by Adrian Balint for the SAT 2011 competition
  if (iMaxClauseLen < 4) {
    fSparrowC1 = 2.15;
    iSparrowC2 = 4;
    fSparrowC3 = 100000.0;
    iPs = FloatToProb(0.347);
  } else {
    if (iMaxClauseLen < 6) {
      fSparrowC1 = 2.85;
      iSparrowC2 = 4;
      fSparrowC3 = 75000.0;
      iPs = FloatToProb(1.0);
    } else {
      fSparrowC1 = 6.5;
      iSparrowC2 = 4;
      fSparrowC3 = 100000.0;
      iPs = FloatToProb(0.83);
    }
  }
  InitSparrow();
}

void InitSparrow() {
  SINT32 j;
  for (j=0;j <= 10; j++) {
    aSparrowPreCalc[j] = pow(fSparrowC1,-j);
  }
  fInvSparrowC3 = 1.0f / fSparrowC3;
}

void PickSparrow() {

  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  SINT32 iScore;
  LITTYPE *pLit;
  UINT32 j;
  UINT32 k;

  FLOAT fScoreProb;
  FLOAT fAgeProb;
  FLOAT fBaseAgeProb;
  FLOAT fScoreSum;
  FLOAT fScorePos;
  
  iFlipCandidate = 0;

  if (iNumFalse==0) {
    return;
  }

  // this part is the same as in gNovelty+
  if (iNumGNovPromVars > 0 ) { 
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

  fScoreSum = 0.0;

  iClause = aFalseList[RandomInt(iNumFalse)];
  iClauseLen = aClauseLen[iClause];

  pLit = pClauseLits[iClause];

  for (j=0;j<iClauseLen;j++) {
    iVar = GetVarFromLit(*pLit);
    // note :: positive iScore is "good"
    iScore = (SINT32) aMakePenaltyINT[iVar] - (SINT32) aBreakPenaltyINT[iVar];
    
    if (iScore < -10) {
      iScore = -10;
    }
    if (iScore > 0) {
      iScore = 0;
    }
    
    fScoreProb = aSparrowPreCalc[-iScore];
    fBaseAgeProb = (iStep - aVarLastChange[iVar]) * fInvSparrowC3;
    fAgeProb = 1.0;
    for (k=0; k < iSparrowC2; k++) {
      fAgeProb *= fBaseAgeProb;
    }
    fAgeProb += 1.0;

    aSparrowScores[j] = fScoreProb * fAgeProb;
    fScoreSum += aSparrowScores[j];

    pLit++;
  }

  fScorePos = RandomFloat()*fScoreSum;
  fScoreSum = 0.0;

  for (j=0;j<iClauseLen;j++) {
    fScoreSum += aSparrowScores[j];
    if (fScorePos <= fScoreSum) {
      iFlipCandidate = GetVar(iClause,j);
      break;
    }
  }
  ScaleSparrow();
}

void SmoothSparrow() {
  
  UINT32 j;
  UINT32 iVar;
  UINT32 iClause;
  UINT32 iLoopMax;

  iLoopMax = iNumPenClauseList;

  for (j=0;j<iLoopMax;j++) {
    iClause = aPenClauseList[j];
    if (aNumTrueLit[iClause] > 0) {
      aClausePenaltyINT[iClause]--;
      iTotalPenaltyINT--;
      if (aClausePenaltyINT[iClause]==1) {
        aPenClauseList[aPenClauseListPos[iClause]] = aPenClauseList[--iNumPenClauseList]; // can be better... fix in PAWS too
        aPenClauseListPos[aPenClauseList[iNumPenClauseList]] = aPenClauseListPos[iClause];
      }
      if (aNumTrueLit[iClause]==1) {
        iVar = aCritSat[iClause];
        aBreakPenaltyINT[iVar]--;
			  if ((aVarIsGNovProm[iVar] == 0) && ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] < 0 ) && (aVarLastChange[iVar] < iStep - 1)) {
				  aGNovPromVarsList[iNumGNovPromVars++] = iVar;
				  aVarIsGNovProm[iVar] = 1;
			  } 
      }
    }
  }
}


void ScaleSparrow() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  UINT32 iClause;
  LITTYPE *pLit;

  if (RandomProb(iPs)) {
    SmoothSparrow();
    return;
  }

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

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

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






#ifdef __cplusplus
}
#endif
