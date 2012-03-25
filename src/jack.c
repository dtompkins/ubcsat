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

#define JACK_NUM_GREEDY_PROP 9
#define JACK_NUM_DIV_PROP 17
#define JACK_NUM_CLAUSE_GROUPS 4
#define JACK_EPSILON 0.001

void PickJack();
void FlipJack();
void InitJackConfig();

void CreateJackScores();
FLOAT *aJackGreedy;
FLOAT *aJackDiv;
UINT32 *aJackCandidates;

void CreateClauseLastFlipVar();
void InitClauseLastFlipVar();
UINT32 *aClauseLastFlipVar;

void CreateJackFlopCount();
void InitJackFlopCount();
UBIGINT *aLitJackFlopCount;
UBIGINT *aLitJackResetFlopCount;
FLOAT *aLitJackNormFlopCount;

UINT32 iVarAgeHistoryBufferLen;
UBIGINT *aVarAgeHistoryData;
UBIGINT **pVarAgeHistory;
void CreateAgeHistory();
void InitAgeHistory();
void UpdateAgeHistory();
UBIGINT VarAge(UINT32 iVar, UINT32 iIndex);

BOOL bJackUsePromising;

FLOAT fScoreNeg;
FLOAT fScorePos;

UINT32 iScoreRatioType;

UINT32 iAgeExp;
FLOAT fAgeDiv;
UINT32 iAgeGap;

FLOAT aPropGWeights[JACK_NUM_GREEDY_PROP];
FLOAT fPropGWeightsSum;
FLOAT aPropDWeights[JACK_NUM_DIV_PROP];
FLOAT fPropDWeightsSum;

FLOAT aGreedyWeights[JACK_NUM_CLAUSE_GROUPS];
FLOAT aMixedWeights[JACK_NUM_CLAUSE_GROUPS];
FLOAT aDivWeights[JACK_NUM_CLAUSE_GROUPS];
FLOAT aSumGreedyMixedDivWeights[JACK_NUM_CLAUSE_GROUPS];

PROBABILITY aUseGreedyBest[JACK_NUM_CLAUSE_GROUPS];
PROBABILITY aUseMixedBest[JACK_NUM_CLAUSE_GROUPS];
PROBABILITY aUseDivBest[JACK_NUM_CLAUSE_GROUPS];

void AddJack() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("jack","",0,
    "Captain Jack",
    "Tompkins, Balint, Hoos [SAT 11]",
    "PickJack",
    "DefaultProcedures,Flip+DecPromVars+FCL,DecPromVars,FalseClauseList,VarLastChange,CreateJackScores,MakeBreak,FlipCounts,JackFlopCount,InitJackConfig,AgeHistory,CreateNextClauseLit,InitNextClauseLit,ClauseLastFlipVar",
    "default","default");

  AddParmBool(&pCurAlg->parmList,"-prom","use promising variables [default %s]","(see paper for all parameter descriptions)","",&bJackUsePromising,1);

  AddParmFloat(&pCurAlg->parmList,"-wg2","weight of greedy-only steps (clen <=2) [default %s]","","",&aGreedyWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-wm2","weight of mixed steps (clen <=2) [default %s]","","",&aMixedWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-wd2","weight of div-only steps (clen <=2) [default %s]","","",&aDivWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-wg3","weight of greedy-only steps (clen =3) [default %s]","","",&aGreedyWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-wm3","weight of mixed steps (clen =3) [default %s]","","",&aMixedWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-wd3","weight of div-only steps (clen =3) [default %s]","","",&aDivWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-wg4","weight of greedy-only steps (clen 4..9) [default %s]","","",&aGreedyWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-wm4","weight of mixed steps (clen 4..9) [default %s]","","",&aMixedWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-wd4","weight of div-only steps (clen 4..9) [default %s]","","",&aDivWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-wg10","weight of greedy-only steps (clen >=10) [default %s]","","",&aGreedyWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-wm10","weight of mixed steps (clen >=10) [default %s]","","",&aMixedWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-wd10","weight of div-only steps (clen >=10) [default %s]","","",&aDivWeights[3],1);

  AddParmProbability(&pCurAlg->parmList,"-maxg2","greedy: prob. of max VSM (clen <= 2) [default %s]","","",&aUseGreedyBest[0],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxm2","mixed: prob. of max VSM (clen <= 2) [default %s]","","",&aUseMixedBest[0],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxd2","div: prob. of max VSM (clen <= 2) [default %s]","","",&aUseDivBest[0],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxg3","greedy: prob. of max VSM (clen =3) [default %s]","","",&aUseGreedyBest[1],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxm3","mixed: prob. of max VSM (clen =3) [default %s]","","",&aUseMixedBest[1],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxd3","div: prob. of max VSM (clen =3) [default %s]","","",&aUseDivBest[1],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxg4","greedy: prob. of max VSM (clen 4..9) [default %s]","","",&aUseGreedyBest[2],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxm4","mixed: prob. of max VSM (clen 4..9) [default %s]","","",&aUseMixedBest[2],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxd4","div: prob. of max VSM (clen 4..9) [default %s]","","",&aUseDivBest[2],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxg10","greedy: prob. of max VSM (clen >=10) [default %s]","","",&aUseGreedyBest[3],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxm10","mixed: prob. of max VSM (clen >=10) [default %s]","","",&aUseMixedBest[3],0.5);
  AddParmProbability(&pCurAlg->parmList,"-maxd10","div: prob. of max VSM (clen >=10) [default %s]","","",&aUseDivBest[3],0.5);

  AddParmFloat(&pCurAlg->parmList,"-score","greedy: weight of score [default %s]","","",&aPropGWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-make","greedy: weight of make [default %s]","","",&aPropGWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-break","greedy: weight of break [default %s]","","",&aPropGWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-relscore","greedy: weight of relScore [default %s]","","",&aPropGWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-relmake","greedy: weight of relMake [default %s]","","",&aPropGWeights[4],1);
  AddParmFloat(&pCurAlg->parmList,"-relbreak","greedy: weight of relBreak [default %s]","","",&aPropGWeights[5],1);
  AddParmFloat(&pCurAlg->parmList,"-sparrowscore2","greedy: weight of sparrowScore2 [default %s]","","",&aPropGWeights[6],1);
  AddParmFloat(&pCurAlg->parmList,"-scoreratio","greedy: weight of scoreRatio [default %s]","","",&aPropGWeights[7],1);
  AddParmFloat(&pCurAlg->parmList,"-relscoreratio","greedy: weight of relScoreRatio [default %s]","","",&aPropGWeights[8],1);

  AddParmFloat(&pCurAlg->parmList,"-flat","div: weight of flat [default %s]","","",&aPropDWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-rand","div: weight of random [default %s]","","",&aPropDWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-age","div: weight of age [default %s]","","",&aPropDWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-agerange","div: weight of ageRange [default %s]","","",&aPropDWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-tabu","div: weight of tabu [default %s]","","",&aPropDWeights[4],1);
  AddParmFloat(&pCurAlg->parmList,"-sparrowage","div: weight of sparrowAge [default %s]","","",&aPropDWeights[5],1);
  AddParmFloat(&pCurAlg->parmList,"-age1","div: weight of age1 [default %s]","","",&aPropDWeights[6],1);
  AddParmFloat(&pCurAlg->parmList,"-age5","div: weight of age5 [default %s]","","",&aPropDWeights[7],1);
  AddParmFloat(&pCurAlg->parmList,"-flips","div: weight of flips [default %s]","","",&aPropDWeights[8],1);
  AddParmFloat(&pCurAlg->parmList,"-relflips","div: weight of relFlips [default %s]","","",&aPropDWeights[9],1);
  AddParmFloat(&pCurAlg->parmList,"-flops","div: weight of flops [default %s]","","",&aPropDWeights[10],1);
  AddParmFloat(&pCurAlg->parmList,"-normflops","div: weight of normFlops [default %s]","","",&aPropDWeights[11],1);
  AddParmFloat(&pCurAlg->parmList,"-relflops","div: weight of relFlops [default %s]","","",&aPropDWeights[12],1);
  AddParmFloat(&pCurAlg->parmList,"-relnormflops","div: weight of relNormFlops [default %s]","","",&aPropDWeights[13],1);
  AddParmFloat(&pCurAlg->parmList,"-resetflops","div: weight of resetFlops [default %s]","","",&aPropDWeights[14],1);
  AddParmFloat(&pCurAlg->parmList,"-fair","div: weight of fair [default %s]","","",&aPropDWeights[15],1);
  AddParmFloat(&pCurAlg->parmList,"-last","div: weight of last [default %s]","","",&aPropDWeights[16],1);

  AddParmFloat(&pCurAlg->parmList,"-cb","cb: base of sparrowscore2 for negative scores [default %s]","","",&fScoreNeg,2.0);
  AddParmFloat(&pCurAlg->parmList,"-cs","cs: sparrowscore2 value for positive scores [default %s]","","",&fScorePos,1.0);
  AddParmUInt(&pCurAlg->parmList,"-bs","bs: use make in denominator for scoreratio [default %s]","","",&iScoreRatioType,1);

  AddParmUInt(&pCurAlg->parmList,"-ce","ce: (INT) exponent for sparrowAge [default %s]","","",&iAgeExp,4);
  AddParmFloat(&pCurAlg->parmList,"-cd","cd: denominator for sparrowAge [default %s]","","",&fAgeDiv,100000.0);
  AddParmUInt(&pCurAlg->parmList,"-cr","cr: denominator for ageRange [default %s]","","",&iAgeGap,1);
  AddParmUInt(&pCurAlg->parmList,"-ct","ct: tabu tenure for -tabu [default %s]","","",&iTabuTenure,0);

  CreateTrigger("PickJack",ChooseCandidate,PickJack,"","");
  CreateTrigger("InitJackConfig",PostParameters,InitJackConfig,"","");

  CreateTrigger("CreateJackFlopCount",CreateStateInfo,CreateJackFlopCount,"","");
  CreateTrigger("InitJackFlopCount",InitStateInfo,InitJackFlopCount,"","");
  CreateContainerTrigger("JackFlopCount","CreateJackFlopCount,InitJackFlopCount");

  CreateTrigger("CreateJackScores",CreateStateInfo,CreateJackScores,"","");

  CreateTrigger("CreateClauseLastFlipVar",CreateStateInfo,CreateClauseLastFlipVar,"","");
  CreateTrigger("InitClauseLastFlipVar",InitStateInfo,InitClauseLastFlipVar,"","");
  CreateContainerTrigger("ClauseLastFlipVar","CreateClauseLastFlipVar,InitClauseLastFlipVar");

  CreateTrigger("CreateAgeHistory",CreateStateInfo,CreateAgeHistory,"VarLastChange,FlipCounts","");
  CreateTrigger("InitAgeHistory",InitStateInfo,InitAgeHistory,"","");
  CreateTrigger("UpdateAgeHistory",UpdateStateInfo,UpdateAgeHistory,"","UpdateVarLastChange,UpdateFlipCounts");
  CreateContainerTrigger("AgeHistory","CreateAgeHistory,InitAgeHistory,UpdateAgeHistory");
}


void InitJackConfig() {
  UINT32 j;

  fPropGWeightsSum = FLOATZERO;
  for (j=0; j < JACK_NUM_GREEDY_PROP; j++) {
    fPropGWeightsSum += aPropGWeights[j];
  }
  if (fPropGWeightsSum == FLOATZERO) {
    ReportHdrPrefix(pRepErr);
    ReportHdrPrint(pRepErr,"Warning! set Greedy Property weights \n");
    fPropGWeightsSum = 1.0;
    aPropGWeights[0] = 1.0;
  }

  fPropDWeightsSum = FLOATZERO;
  for (j=0; j < JACK_NUM_DIV_PROP; j++) {
    fPropDWeightsSum += aPropDWeights[j];
  }
  if (fPropDWeightsSum == FLOATZERO) {
    ReportHdrPrefix(pRepErr);
    ReportHdrPrint(pRepErr,"Warning! set Div Property weights \n");
    fPropDWeightsSum = 1.0;
    aPropDWeights[0] = 1.0;
  }

  for (j=0; j < JACK_NUM_CLAUSE_GROUPS; j++) {
    aSumGreedyMixedDivWeights[j] = aGreedyWeights[j] + aMixedWeights[j] + aDivWeights[j];
    if (aSumGreedyMixedDivWeights[j] == FLOATZERO) {
      ReportHdrPrefix(pRepErr);
      ReportHdrPrint(pRepErr,"Warning! set Greedy/Mixed/Div weights \n");
      aSumGreedyMixedDivWeights[j] = 1.0;
      aGreedyWeights[j] = 1.0;
    }
  }

  iVarAgeHistoryBufferLen = 6;
}

void PickJack() {

  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 j;
  UINT32 k;

  FLOAT fSelectSum;
  FLOAT fSelectPos;

  FLOAT fMaxPropValue;
  FLOAT fMinPropValue;
  FLOAT fPropValue;
  FLOAT fPropertyAddOffset;

  BOOL bUseBest;

  UINT32 iSelectProperty;

  UINT32 iClauseIndex;

  FLOAT fRelMake;
  FLOAT fRelBreak;
  FLOAT fTemp;

  BOOL bJustGreedy;
  BOOL bJustDiv;
  BOOL bMixed;

  FLOAT fNormClauseLen;

  UINT32 iNumJackCandidates;

  iFlipCandidate = 0;

  if (iNumFalse==0) {
    return;
  }

  if (bJackUsePromising) {
    if (iNumDecPromVars > 0) {
      PromisingSelectBest();
      return;
    }
  }

  iClause = aFalseList[RandomInt(iNumFalse)];
  iClauseLen = aClauseLen[iClause];

  if (iClauseLen <= 2) {
    iClauseIndex = 0;
  } else {
    if (iClauseLen == 3) {
      iClauseIndex = 1;
    } else {
      if (iClauseLen < 10) {
        iClauseIndex = 2;
      } else {
        iClauseIndex = 3;
      }
    }
  }

  // determine if JustGreedy, JustDiv, or Mixed... and if select Best or Distribution
  bJustGreedy = 0;
  bMixed = 0;
  bJustDiv = 0;

  fSelectPos = RandomFloat()*aSumGreedyMixedDivWeights[iClauseIndex];
  if (fSelectPos <= aGreedyWeights[iClauseIndex] ) {
    bJustGreedy = 1;
    bUseBest = RandomProb(aUseGreedyBest[iClauseIndex]);
  } else {
    if (fSelectPos <= aGreedyWeights[iClauseIndex] + aMixedWeights[iClauseIndex]) {
      bMixed = 1;
      bUseBest = RandomProb(aUseMixedBest[iClauseIndex]);
    } else {
      bJustDiv = 1;
      bUseBest = RandomProb(aUseDivBest[iClauseIndex]);
    }
  }

  iNumJackCandidates = 0;
  fMaxPropValue = -FLOATMAX;
  fMinPropValue = FLOATMAX;

  if ((bJustGreedy)||(bMixed)) {

    // determine which greedy property to use
    fSelectPos = RandomFloat()*fPropGWeightsSum;
    fSelectSum = FLOATZERO;
    iSelectProperty = 0;
    for (j=0;j<JACK_NUM_GREEDY_PROP;j++) {
      fSelectSum += aPropGWeights[j];
      if (fSelectPos <= fSelectSum) {
        iSelectProperty = j;
        break;
      }
    }

    pLit = pClauseLits[iClause];
    for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);

      switch (iSelectProperty) {
        case 0:
          fPropValue = (FLOAT) aMakeCount[iVar] - (FLOAT) aBreakCount[iVar];
          break;
        case 1:
          fPropValue = (FLOAT) aMakeCount[iVar];
          break;
        case 2:
          fPropValue = -(FLOAT) aBreakCount[iVar];
          break;
        case 3:
          fRelMake = ((FLOAT) aMakeCount[iVar]) / ((FLOAT) aNumLitOcc[*pLit]);
          fRelBreak = ((FLOAT) aBreakCount[iVar]) / (JACK_EPSILON + (FLOAT) aNumLitOcc[GetNegatedLit(*pLit)]);
          fPropValue = fRelMake - fRelBreak;
          break;
        case 4:
          fRelMake = ((FLOAT) aMakeCount[iVar]) / ((FLOAT) aNumLitOcc[*pLit]);
          fPropValue = fRelMake;
          break;
        case 5:
          fRelBreak = ((FLOAT) aBreakCount[iVar]) / (JACK_EPSILON + (FLOAT) aNumLitOcc[GetNegatedLit(*pLit)]);
          fPropValue = 1.0 - fRelBreak;
          break;
        case 6:
          fPropValue = (FLOAT) aMakeCount[iVar] - (FLOAT) aBreakCount[iVar];
          if (fPropValue == FLOATZERO) {
            fPropValue = 1.0f;
          } else {
            if (fPropValue > FLOATZERO) {
              fPropValue = fScorePos;
            } else {
              fPropValue = pow(fScoreNeg,fPropValue);
            }
          }
          break;
        case 7:
          if (iScoreRatioType) {
            fPropValue = (FLOAT) aMakeCount[iVar] / ((FLOAT) (aMakeCount[iVar] + aBreakCount[iVar]));
          } else {
            fPropValue = (FLOAT) aMakeCount[iVar] / ((FLOAT) (1 + aBreakCount[iVar]));
          }
          break;
        case 8:
          fRelMake = ((FLOAT) aMakeCount[iVar]) / ((FLOAT) aNumLitOcc[*pLit]);
          fRelBreak = ((FLOAT) aBreakCount[iVar]) / (JACK_EPSILON + (FLOAT) aNumLitOcc[GetNegatedLit(*pLit)]);
          if (iScoreRatioType) {
            fPropValue = fRelMake / (fRelMake + fRelBreak);
          } else {
            fPropValue = fRelMake / (fRelMake + fRelBreak);
            fPropValue = fRelMake / (fRelBreak + JACK_EPSILON);
          }
          break;

        default:
          ReportPrint(pRepErr,"Unexpected Error: invalid iSelectProperty\n");
          AbnormalExit();
          exit(1);
      }

      if ((bJustGreedy)&&(bUseBest)) {
        if ( fPropValue >= fMaxPropValue) {
          if (fPropValue > fMaxPropValue) {
            iNumJackCandidates = 0;
            fMaxPropValue = fPropValue;
          }
          aJackCandidates[iNumJackCandidates++] = iVar;
        }
      } else {
        if ( fPropValue < fMinPropValue) {
          fMinPropValue = fPropValue;
        }
        aJackGreedy[j] = fPropValue;
      }
      pLit++;
    }

    if ((bMixed)||(!bUseBest)) {
      fPropertyAddOffset = FLOATZERO;
      if ((iSelectProperty==0)||(iSelectProperty==2)) {
        fPropertyAddOffset = 1.0f - fMinPropValue;
      }
      if (iSelectProperty==3) {
        fPropertyAddOffset = JACK_EPSILON - fMinPropValue;
      }
      if (fPropertyAddOffset != FLOATZERO) {
        for (j=0;j<iClauseLen;j++) {
          aJackGreedy[j] += fPropertyAddOffset;
        }
      }
    }
  }

  fMinPropValue = FLOATMAX;

  if ((bJustDiv)||(bMixed)) {
    // determine which Div to use

    fSelectPos = RandomFloat()*fPropDWeightsSum;
    fSelectSum = FLOATZERO;
    iSelectProperty = 0;
    for (j=0;j<(JACK_NUM_DIV_PROP);j++) {
      fSelectSum += aPropDWeights[j];
      if (fSelectPos <= fSelectSum) {
        iSelectProperty = j;
        break;
      }
    }

    pLit = pClauseLits[iClause];
    for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);

      switch (iSelectProperty) {
        case 0:
          fPropValue = 1.0;
          break;
        case 1:
          fPropValue =  RandomFloat();
          break;
        case 2:
          fPropValue = (FLOAT) (iStep - aVarLastChange[iVar]);
          break;
        case 3:
          fPropValue = (FLOAT) ((iStep - aVarLastChange[iVar]) / iAgeGap);
          break;
        case 4:
          fPropValue = 1.0f;
          if (iTabuTenure) {
            if ((iStep - aVarLastChange[iVar]) < iTabuTenure) {
              fPropValue = 0.0f;
            }
          }
          break;
        case 5:
          fTemp =  ((FLOAT) (iStep - aVarLastChange[iVar])) / fAgeDiv;
          fPropValue = 1.0;
          for (k=0; k < iAgeExp; k++) {
            fPropValue *= fTemp;
          }
          fPropValue += 1.0;
          break;
        case 6:
          fPropValue = (FLOAT) VarAge(iVar,1);
          break;
        case 7:
          fPropValue = (FLOAT) VarAge(iVar,5);
          break;
        case 8:
          fPropValue = -(FLOAT) aFlipCounts[iVar];
          break;
        case 9:
          fPropValue = - ((FLOAT) aFlipCounts[iVar])  / ((FLOAT) aNumLitOcc[*pLit]);
          break;
        case 10:
          fPropValue = (FLOAT) aLitJackFlopCount[*pLit];
          break;
        case 11:
          fPropValue = (FLOAT) aLitJackNormFlopCount[*pLit];
          break;
        case 12:
          fPropValue = (FLOAT) aLitJackFlopCount[*pLit]  / ((FLOAT) aNumLitOcc[*pLit]);
          break;
        case 13:
          fPropValue = (FLOAT) aLitJackNormFlopCount[*pLit]  / ((FLOAT) aNumLitOcc[*pLit]);
          break;
        case 14:
          fPropValue = (FLOAT) aLitJackResetFlopCount[*pLit];
          break;
        case 15:
          if (aNextClauseLit[iClause] == j) {
            fPropValue = 1.0f;
          } else {
            fPropValue = FLOATZERO;
          }
          break;
        case 16:
          if (aClauseLastFlipVar[iClause] == iVar) {
            fPropValue = FLOATZERO;
          } else {
            fPropValue = 1.0f;
          }
          break;

        default:
          ReportPrint(pRepErr,"Unexpected Error: invalid iSelectProperty\n");
          AbnormalExit();
          exit(1);
      }

      if ((bJustDiv)&&(bUseBest)) {
        if ( fPropValue >= fMaxPropValue) {
          if (fPropValue > fMaxPropValue) {
            iNumJackCandidates = 0;
            fMaxPropValue = fPropValue;
          }
          aJackCandidates[iNumJackCandidates++] = iVar;
        }
      } else {
        if ( fPropValue < fMinPropValue) {
          fMinPropValue = fPropValue;
        }
        aJackDiv[j] = fPropValue;
      }
      pLit++;
    }
    if ((bMixed)||(!bUseBest)) {
      fPropertyAddOffset = FLOATZERO;
      if ((iSelectProperty==8)||(iSelectProperty==9)) {
        fPropertyAddOffset = 1.0f - fMinPropValue;
      }
      if (fPropertyAddOffset != FLOATZERO) {
        for (j=0;j<iClauseLen;j++) {
          aJackDiv[j] += fPropertyAddOffset;
        }
      }
    }
    if (iSelectProperty == 15) {
      aNextClauseLit[iClause]++;
      if (aNextClauseLit[iClause]==iClauseLen) {
        aNextClauseLit[iClause] = 0;
      }
    }

  }

  if ((bJustDiv)&&(!bUseBest)) {
    for (j=0;j<iClauseLen;j++) {
      aJackGreedy[j] = aJackDiv[j];
    }
  }

  if (bMixed) {
    fMaxPropValue = -FLOATMAX;
    for (j=0;j<iClauseLen;j++) {
      aJackGreedy[j] *= aJackDiv[j];
      if (bUseBest) {
        if ( aJackGreedy[j] >= fMaxPropValue) {
          if (aJackGreedy[j] > fMaxPropValue) {
            iNumJackCandidates=0;
            fMaxPropValue = aJackGreedy[j];
          }
          aJackCandidates[iNumJackCandidates++] = GetVar(iClause,j);
        }
      }
    }
  }

  if (bUseBest) {
    if (iNumJackCandidates > 1) {
      iFlipCandidate = aJackCandidates[RandomInt(iNumJackCandidates)];
    } else {
      iFlipCandidate = aJackCandidates[0];
    }
  } else {
    fSelectSum = FLOATZERO;
    for (j=0;j<iClauseLen;j++) {
      if (aJackGreedy[j] < FLOATZERO) {
        aJackGreedy[j] = FLOATZERO;
      }
      fSelectSum += aJackGreedy[j];
    }
    if (fSelectSum == FLOATZERO) {
      j = RandomInt(iClauseLen);
      iFlipCandidate = GetVar(iClause,j);
    } else {
      fSelectPos = RandomFloat()*fSelectSum;
      fSelectSum = FLOATZERO;
      for (j=0;j<iClauseLen;j++) {
        fSelectSum += aJackGreedy[j];
        if (fSelectPos <= fSelectSum) {
          iFlipCandidate = GetVar(iClause,j);
          break;
        }
      }
    }
  }

  fNormClauseLen = 1.0f / (FLOAT) iClauseLen;
  pLit = pClauseLits[iClause];
  for (j=0;j<iClauseLen;j++) {
    if (GetVarFromLit(*pLit) != iFlipCandidate) {
      aLitJackFlopCount[*pLit]++;
      aLitJackResetFlopCount[*pLit]++;
      aLitJackNormFlopCount[*pLit] += fNormClauseLen;
    }
    pLit++;
  }
  aLitJackResetFlopCount[GetFalseLit(iFlipCandidate)] = 0;
  aClauseLastFlipVar[iClause] = iFlipCandidate;
}



void CreateAgeHistory() {
  UINT32 j;
  aVarAgeHistoryData = (UBIGINT *) AllocateRAM((iNumVars+1)*iVarAgeHistoryBufferLen*sizeof(UBIGINT), HeapData);
  pVarAgeHistory = (UBIGINT **) AllocateRAM((iNumVars+1)*sizeof(UBIGINT *), HeapData);
  for (j=0;j<=iNumVars;j++) {
    pVarAgeHistory[j] = &aVarAgeHistoryData[j*iVarAgeHistoryBufferLen];
  }
}

void InitAgeHistory() {
  memset(aVarAgeHistoryData,0,(iNumVars+1)*iVarAgeHistoryBufferLen*sizeof(UBIGINT));
}

void UpdateAgeHistory() {
  // replaces UpdateVarLastChange() & UpdateFlipCounts()
  // SCREWS UP IF THERE IS A RESET
  pVarAgeHistory[iFlipCandidate][aFlipCounts[iFlipCandidate] % iVarAgeHistoryBufferLen] = iStep - aVarLastChange[iFlipCandidate];
  aFlipCounts[iFlipCandidate]++;
  aVarLastChange[iFlipCandidate] = iStep;
}

UBIGINT VarAge(UINT32 iVar, UINT32 iIndex) {
  UINT32 j;
  UINT32 iPos;
  UBIGINT iAge;

  iAge = iStep - aVarLastChange[iVar];
  iPos = (aFlipCounts[iVar] + iVarAgeHistoryBufferLen - iIndex) % iVarAgeHistoryBufferLen;

  for (j=1; j <= iIndex; j++) {
    iPos = (aFlipCounts[iVar] + iVarAgeHistoryBufferLen - j) % iVarAgeHistoryBufferLen;
    iAge += pVarAgeHistory[iVar][iPos];
  }
  return iAge;
}


void CreateJackScores() {
  aJackGreedy = (FLOAT *) AllocateRAM(iMaxClauseLen*sizeof(FLOAT), HeapData);
  aJackDiv = (FLOAT *) AllocateRAM(iMaxClauseLen*sizeof(FLOAT), HeapData);
  aJackCandidates = (UINT32 *) AllocateRAM(iMaxClauseLen*sizeof(UINT32), HeapData);
}


void CreateClauseLastFlipVar() {
  aClauseLastFlipVar = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32), HeapData);
}

void InitClauseLastFlipVar() {
  memset(aClauseLastFlipVar,0,iNumClauses*sizeof(UINT32));
}


void CreateJackFlopCount() {
  aLitJackFlopCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT), HeapData);
  aLitJackResetFlopCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT), HeapData);
  aLitJackNormFlopCount = (FLOAT *) AllocateRAM((iNumVars+1)*2*sizeof(FLOAT), HeapData);
}

void InitJackFlopCount() {
  memset(aLitJackFlopCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
  memset(aLitJackResetFlopCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
  memset(aLitJackNormFlopCount,0,(iNumVars+1)*2*sizeof(FLOAT));
}


#ifdef __cplusplus
}
#endif
