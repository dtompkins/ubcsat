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

extern UINT32 *aGNovPromVarsList;
extern UINT32 *aVarIsGNovProm;
extern UINT32 iNumGNovPromVars;

FLOAT *aSparrowScores;
void CreateSparrowScores() {
  aSparrowScores = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
}

FLOAT fSparrowC1;
UINT32 iSparrowC2;
FLOAT fSparrowC3;

void AddSparrow() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("sparrow","",0,
    "gNovelty+",
    "Balint, Froehlich [SAT 10]",
    "PickSparrow",
    "DefaultProcedures,Flip+GNovPromVars+FCL,GNovPromVars,FalseClauseList,VarLastChange,PenClauseList,AdaptNoveltyNoise,VarsShareClauses,CreateSparrowScores",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","after a scaling step, ~smooth penalties with probability PR","",&iPs,0.347);
  
  AddParmFloat(&pCurAlg->parmList,"-c1","sparrow score adjustment parameter [default %s]","adjusts the importance of the score","",&fSparrowC1,2.0);
  AddParmUInt(&pCurAlg->parmList,"-c2","sparrow age polynomial parameter [default %s]","adjusts the influence of the age","",&iSparrowC2,4);
  AddParmFloat(&pCurAlg->parmList,"-c3","sparrow age threshold parameter [default %s]","threshold for age calculation","",&fSparrowC3,100000.0);
  
  CreateTrigger("PickSparrow",ChooseCandidate,PickSparrow,"","");
  CreateTrigger("CreateSparrowScores",CreateStateInfo,CreateSparrowScores,"","");

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
    
    fScoreProb = pow(fSparrowC1,iScore);
    fBaseAgeProb = (iStep - aVarLastChange[iVar]) / fSparrowC3;
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


#ifdef __cplusplus
}
#endif
