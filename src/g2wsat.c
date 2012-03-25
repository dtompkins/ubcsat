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

void PickG2WSatGeneral();
void ConfigureG2WSatGeneral();

void InitAdaptG2WSatNoise();
void AdaptG2WSatNoise();

FXNPTR fxnG2WsatPromisingSelect;
FXNPTR fxnG2WsatDiversificationSelect;
FXNPTR fxnG2WsatBaseAlgSelect;

UINT32 iG2WsatPromisingSelectID;
UINT32 iG2WsatDiversificationSelectID;
UINT32 iG2WsatBaseAlgSelectID;

void AddG2WSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("g2wsat","",0,
    "G2WSAT: Gradient-based Greedy WalkSAT (generalized)",
    "Li, Huang  [SAT 05] and Li, Wei, Zhang [SAT 07]",
    "PickG2WSatGeneral,ConfigureG2WSatGeneral",
    "DefaultProcedures,Flip+DecPromVars+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmProbability(&pCurAlg->parmList,"-wpdp","walk / diversification probability [default %s]","with probability PR, perform diversification~according to -subalg setting","",&iNovWpDp,0.05);

  AddParmUInt(&pCurAlg->parmList,"-prom","G2WSAT Decreasing Promising Variable ID [default %s]","0: best~1: oldest variable~2: longest promising","",&iG2WsatPromisingSelectID,0);
  AddParmUInt(&pCurAlg->parmList,"-div","G2WSAT Diversification ID [default %s]","0: random (Novelty+)~1: oldest (Novelty++)~2: random[n-1] excl. youngest (Novelty-1)~3: random[n-2] excl. best 2  (Novelty-2)","",&iG2WsatDiversificationSelectID,1);
  AddParmUInt(&pCurAlg->parmList,"-base","G2WSAT Base Algorithm ID [default %s]","0: Novelty~1: Novelty+p~2: Top2","",&iG2WsatBaseAlgSelectID,0);

  CreateTrigger("PickG2WSatGeneral",ChooseCandidate,PickG2WSatGeneral,"","");
  CreateTrigger("ConfigureG2WSatGeneral",PostParameters,ConfigureG2WSatGeneral,"","");

  pCurAlg = CreateAlgorithm("adaptg2wsat","",0,
    "Adaptive G2WSat (generalized)",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatGeneral,ConfigureG2WSatGeneral,InitAdaptG2WSatNoise,AdaptG2WSatNoise",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");

  AddParmUInt(&pCurAlg->parmList,"-prom","G2WSAT Decreasing Promising Variable ID [default %s]","0: best~1: oldest variable~2: longest promising","",&iG2WsatPromisingSelectID,0);
  AddParmUInt(&pCurAlg->parmList,"-div","G2WSAT Diversification ID [default %s]","0: random (Novelty+)~1: oldest (Novelty++)~2: random[n-1] excl. youngest (Novelty-1)~3: random[n-2] excl. best 2  (Novelty-2)","",&iG2WsatDiversificationSelectID,0);
  AddParmUInt(&pCurAlg->parmList,"-base","G2WSAT Base Algorithm ID [default %s]","0: Novelty~1: Novelty+p~2: Top2","",&iG2WsatBaseAlgSelectID,0);

  AddParmFloat(&pCurAlg->parmList,"-phi","adjustment parameter phi [default %s]","phi determines the rate of change of noise","",&fAdaptPhi,0.1);
  AddParmFloat(&pCurAlg->parmList,"-theta","adjustment parameter theta [default %s]","theta determines the stagnation detection","",&fAdaptTheta,0.2f);

  CreateTrigger("InitAdaptG2WSatNoise",PostInit,InitAdaptG2WSatNoise,"","");
  CreateTrigger("AdaptG2WSatNoise",PostFlip,AdaptG2WSatNoise,"InitAdaptG2WSatNoise","");

  //todo... add theta & phi
}

void PromisingSelectBest() {
  UINT32 j;
  UINT32 iVar;
  SINT32 iScore;
  iFlipCandidate = aDecPromVarsList[0];
  iBestScore = aVarScore[iFlipCandidate];        
  for (j=1;j<iNumDecPromVars;j++) {
    iVar = aDecPromVarsList[j];
    iScore = aVarScore[iVar];
    if (iScore < iBestScore) {
      iFlipCandidate = iVar;
      iBestScore = iScore;
    } else {
      /* If there is a tie, prefer the 'oldest' variable */
      if (iScore == iBestScore) {
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
      }
    }
  }
}

void PromisingSelectOldest() {
  UINT32 j;
  UINT32 iVar;
  iFlipCandidate = aDecPromVarsList[0];
  for (j=1;j<iNumDecPromVars;j++) {
    iVar = aDecPromVarsList[j];
    if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
      iFlipCandidate = iVar;
    }
  }
}

void PromisingSelectLongest() {
}

void DiversificationRandom() {
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  iClause = aFalseList[RandomInt(iNumFalse)];
  iClauseLen = aClauseLen[iClause];
  litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
  iFlipCandidate = GetVarFromLit(litPick);
}

void DiversificationOldest() {
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

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
}

void DiversificationMinusOne() {
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  UBIGINT iYoungestAge;
  UINT32 iYoungestVarIndex;
  UINT32 iPickIndex;
  LITTYPE *pLit;
  LITTYPE litPick;

  iClause = aFalseList[RandomInt(iNumFalse)];
  iClauseLen = aClauseLen[iClause];

  if (iClauseLen <= 2) {
    DiversificationRandom();
  }

  pLit = pClauseLits[iClause];
  iVar = GetVarFromLit(*pLit);
  iYoungestAge = aVarLastChange[iVar];
  iYoungestVarIndex = 0;
  pLit++;
  for (j=1;j<iClauseLen;j++) {
    iVar = GetVarFromLit(*pLit);
    if (aVarLastChange[iVar] > iYoungestAge) {
      iYoungestAge = aVarLastChange[iVar];
      iYoungestVarIndex = j;
    }
    pLit++;
  }
  iPickIndex = RandomInt(iClauseLen-1);
  if (iPickIndex >= iYoungestVarIndex) {
    iPickIndex++;
  }
  litPick = pClauseLits[iClause][iPickIndex];
  iFlipCandidate = GetVarFromLit(litPick);
}

void DiversificationMinusTwo() {
}

void ConfigureG2WSatGeneral() {
  switch (iG2WsatPromisingSelectID) {
    case 0: // Best
      fxnG2WsatPromisingSelect = PromisingSelectBest;
      break;
    case 1: // Oldest
      fxnG2WsatPromisingSelect = PromisingSelectOldest;
      break;
    case 2: // Longest
      // fxnG2WsatPromisingSelect = PromisingSelectOldest;
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -prom %"P32"\n",iG2WsatPromisingSelectID);
      AbnormalExit();
      exit(1);
  }
  switch (iG2WsatDiversificationSelectID) {
    case 0: // Random
      fxnG2WsatDiversificationSelect = DiversificationRandom;
      break;
    case 1: // Oldest
      fxnG2WsatDiversificationSelect = DiversificationOldest;
      break;
    case 2: // MinusOne
      fxnG2WsatDiversificationSelect = DiversificationMinusOne;
      break;
    case 3: // MinusTwo
      // fxnG2WsatDiversificationSelect = DiversificationMinusTwo;
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -div %"P32"\n",iG2WsatDiversificationSelectID);
      AbnormalExit();
      exit(1);
  }
  switch (iG2WsatBaseAlgSelectID) {
    case 0: // Novelty
      fxnG2WsatBaseAlgSelect = PickNoveltyVarScore;
      break;
    case 1: // Novelty+p
      ActivateTriggers("LookAhead");
      fxnG2WsatBaseAlgSelect = PickNoveltyP;
      break;
    case 2: // Top2
      //fxnG2WsatBaseAlgSelect = Top2;
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -base %"P32"\n",iG2WsatBaseAlgSelectID);
      AbnormalExit();
      exit(1);
  }
}

void PickG2WSatGeneral() {
  iFlipCandidate = 0;
  if (iNumFalse) {
    if (iNumDecPromVars > 0 ) {
      fxnG2WsatPromisingSelect();
    } else {
      if (RandomProb(iNovWpDp)) {
        fxnG2WsatDiversificationSelect();
      } else {
        fxnG2WsatBaseAlgSelect();
      }
    }
  }
}

void InitAdaptG2WSatNoise() {
  InitAdaptNoveltyNoise();
  iNovWpDp = 0;
}

void AdaptG2WSatNoise() {
  AdaptNoveltyNoiseAdjust();
  iNovWpDp = iNovNoise/10;
}

#ifdef __cplusplus
}
#endif

