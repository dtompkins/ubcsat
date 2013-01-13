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

/*
    Note: For consistency, we use the same weighted promising variable scheme as in gNovelty+, which has some quirks,
    espeically during smoothing & scaling updates.
*/

void PickSparrow();
void ScaleSparrow();
void SmoothSparrow();
void FlipSparrow();
void InitSparrow();
void SparrowSat2011Settings();

void CreateSparrowPromVars();
void InitSparrowPromVars();
UINT32 *aSparrowPromVars;
BOOL *aVarIsGNovCandVar;
UINT32 iNumSparrowPromVars;

void CreateSparrowScore();
void InitSparrowScore();
SINT32 *aSparrowScore;

void CreateSparrowWeights();
FLOAT *aSparrowWeights;

FLOAT fSparrowC1;
UINT32 iSparrowC2;
FLOAT fSparrowC3;
FLOAT fInvSparrowC3;

FLOAT aSparrowPreCalc[11];


void AddSparrow() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("sparrow","",0,
    "Sparrow",
    "Balint, Froehlich [SAT 10]",
    "PickSparrow",
    "DefaultProcedures,FlipSparrow,SparrowPromVars,FalseClauseList,VarLastChange,PenClauseList,VarsShareClauses,CreateSparrowWeights,InitSparrow",
    "default","default");

  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","after a scaling step, ~smooth penalties with probability PR","",&iPs,0.347);

  AddParmFloat(&pCurAlg->parmList,"-c1","sparrow score adjustment parameter [default %s]","adjusts the importance of the score","",&fSparrowC1,2.0);
  AddParmUInt(&pCurAlg->parmList,"-c2","sparrow age polynomial parameter [default %s]","adjusts the influence of the age","",&iSparrowC2,4);
  AddParmFloat(&pCurAlg->parmList,"-c3","sparrow age threshold parameter [default %s]","threshold for age calculation","",&fSparrowC3,100000.0);

  CreateTrigger("InitSparrow",PostRead,InitSparrow,"","");

  CreateTrigger("PickSparrow",ChooseCandidate,PickSparrow,"","");
  CreateTrigger("FlipSparrow",FlipCandidate,FlipSparrow,"SparrowPromVars,FalseClauseList","DefaultFlip,UpdateFalseClauseList");
  CreateTrigger("CreateSparrowWeights",CreateStateInfo,CreateSparrowWeights,"","");

  CreateTrigger("CreateSparrowPromVars",CreateStateInfo,CreateSparrowPromVars,"CreateSparrowScore","");
  CreateTrigger("InitSparrowPromVars",InitStateInfo,InitSparrowPromVars,"InitSparrowScore","");
  CreateContainerTrigger("SparrowPromVars","CreateSparrowPromVars,InitSparrowPromVars");

  CreateTrigger("CreateSparrowScore",CreateStateInfo,CreateSparrowScore,"CreateClausePenaltyINT","");
  CreateTrigger("InitSparrowScore",InitStateInfo,InitSparrowScore,"InitClausePenaltyINT","");
  CreateContainerTrigger("SparrowScore","CreateSparrowScore,InitSparrowScore");


  pCurAlg = CreateAlgorithm("sparrow","sat11",0,
    "Sparrow [SAT 11 Competition]",
    "Balint, Froehlich [SAT 10]",
    "PickSparrow,SparrowSat2011Settings",
    "DefaultProcedures,FlipSparrow,SparrowPromVars,FalseClauseList,VarLastChange,PenClauseList,VarsShareClauses,CreateSparrowWeights",
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
  SINT32 iScoreAdjust;
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

  if (iNumSparrowPromVars > 0 ) {
    iBestScore = 0;
    j=0;
    k=0;
    while (j < iNumSparrowPromVars) {
      iVar = aSparrowPromVars[k];
      if (aSparrowScore[iVar] >= 0) {
        iNumSparrowPromVars--;
        aVarIsGNovCandVar[iVar] = 0;
      } else {
        if (aSparrowScore[iVar] < iBestScore) {
          iFlipCandidate = iVar;
          iBestScore = aSparrowScore[iVar];
        } else {
          if (aSparrowScore[iVar] == iBestScore) {
            if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
              iFlipCandidate = iVar;
            }
          }
        }
        aSparrowPromVars[j++]=aSparrowPromVars[k];
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

    // note :: unlike in sparrow paper, negative score is "good"

    iScoreAdjust = aSparrowScore[iVar];

    if (iScoreAdjust > 10) {
      iScoreAdjust = 10;
    }
    if (iScoreAdjust < 0) {
      iScoreAdjust = 0;
    }

    fScoreProb = aSparrowPreCalc[iScoreAdjust];
    fBaseAgeProb = (iStep - aVarLastChange[iVar]) * fInvSparrowC3;
    fAgeProb = 1.0;
    for (k=0; k < iSparrowC2; k++) {
      fAgeProb *= fBaseAgeProb;
    }
    fAgeProb += 1.0;

    aSparrowWeights[j] = fScoreProb * fAgeProb;
    fScoreSum += aSparrowWeights[j];

    pLit++;
  }

  fScorePos = RandomFloat()*fScoreSum;
  fScoreSum = 0.0;

  for (j=0;j<iClauseLen;j++) {
    fScoreSum += aSparrowWeights[j];
    if (fScorePos <= fScoreSum) {
      iFlipCandidate = GetVar(iClause,j);
      break;
    }
  }

  if (RandomProb(iPs)) {
    SmoothSparrow();
  } else {
    ScaleSparrow();
  }
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
        aPenClauseList[aPenClauseListPos[iClause]] = aPenClauseList[--iNumPenClauseList];
        aPenClauseListPos[aPenClauseList[iNumPenClauseList]] = aPenClauseListPos[iClause];
      }
      if (aNumTrueLit[iClause]==1) {
        iVar = aCritSat[iClause];
        aSparrowScore[iVar]--;
			  if ((!aVarIsGNovCandVar[iVar]) && (aSparrowScore[iVar] < 0 ) && (aVarLastChange[iVar] < iStep - 1)) {
				  aSparrowPromVars[iNumSparrowPromVars++] = iVar;
				  aVarIsGNovCandVar[iVar] = 1;
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
      aSparrowScore[iVar]--;
			if ((!aVarIsGNovCandVar[iVar]) && (aSparrowScore[iVar] < 0 ) && (aVarLastChange[iVar] < iStep - 1)) {
				aSparrowPromVars[iNumSparrowPromVars++] = iVar;
				aVarIsGNovCandVar[iVar] = 1;
			}
      pLit++;
    }
  }
}

void FlipSparrow() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  SINT32 iPenalty;

  UINT32 *pShareVar;

  if (iFlipCandidate == 0) {
    return;
  }

  pShareVar = pVarsShareClause[iFlipCandidate];
  for (j=0; j < aNumVarsShareClause[iFlipCandidate]; j++) {
    if (aSparrowScore[*pShareVar] < 0) {
      aVarIsGNovCandVar[*pShareVar] = 1;
    } else {
      aVarIsGNovCandVar[*pShareVar] = 0;
    }
    pShareVar++;
  }

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

      aSparrowScore[iFlipCandidate] -= iPenalty;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aSparrowScore[iVar] -= iPenalty;

        pLit++;

      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aSparrowScore[iVar] += iPenalty;
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
        aSparrowScore[iVar] += iPenalty;

        pLit++;

      }
      aSparrowScore[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aSparrowScore[aCritSat[*pClause]] -= iPenalty;
    }
    pClause++;
  }

  pShareVar = pVarsShareClause[iFlipCandidate];
  for (j=0; j < aNumVarsShareClause[iFlipCandidate]; j++) {
    if (aSparrowScore[*pShareVar] < 0) {
      if (!aVarIsGNovCandVar[*pShareVar]) {
        aSparrowPromVars[iNumSparrowPromVars++] = *pShareVar;
        aVarIsGNovCandVar[*pShareVar] = 1;
      }
    }
    pShareVar++;
  }
}

void CreateSparrowPromVars() {
  aSparrowPromVars = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32), HeapData);
  aVarIsGNovCandVar = (BOOL *) AllocateRAM((iNumVars+1) * sizeof(BOOL), HeapData);
}

void InitSparrowPromVars() {

  UINT32 j;

  iNumSparrowPromVars = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aSparrowScore[j] < 0) {
      aSparrowPromVars[iNumSparrowPromVars++] = j;
      aVarIsGNovCandVar[j] = 1;
    } else {
      aVarIsGNovCandVar[j] = 0;
    }
  }
}

void CreateSparrowWeights() {
  aSparrowWeights = (FLOAT *) AllocateRAM((iMaxClauseLen+1)*sizeof(FLOAT), HeapData);
}

void CreateSparrowScore() {
  aSparrowScore = (SINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32), HeapData);
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32), HeapData);
}

void InitSparrowScore() {

  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;

  for (j=1;j<=iNumVars;j++) {
    aSparrowScore[j] = 0;
  }

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aSparrowScore[GetVar(j,k)] -= aClausePenaltyINT[j];
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aSparrowScore[iVar] += aClausePenaltyINT[j];
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}

#ifdef __cplusplus
}
#endif
