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


extern void PromisingSelectBest();

extern UINT32 iVarAgeHistoryLen;
extern UBIGINT VarAge(UINT32 iVar, UINT32 iIndex);
extern UINT32 *aNextClauseLit;

void PickJack();
void FlipJack();
void InitJackConfig();
void PreRunJack();
void RunCalculationsJack();

FLOAT *aJackGreedy;
FLOAT *aJackDiv;
UINT32 *aJackCandidates;

void CreateJackScores() {
  aJackGreedy = (FLOAT *) AllocateRAM(iMaxClauseLen*sizeof(FLOAT));
  aJackDiv = (FLOAT *) AllocateRAM(iMaxClauseLen*sizeof(FLOAT));
  aJackCandidates = (UINT32 *) AllocateRAM(iMaxClauseLen*sizeof(UINT32));
}

UINT32 *aClauseLastFlipVar;

void CreateClauseLastFlipVar();
void InitClauseLastFlipVar();

void CreateClauseLastFlipVar() {
  aClauseLastFlipVar = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitClauseLastFlipVar() {
  memset(aClauseLastFlipVar,0,iNumClauses*sizeof(UINT32));
}


void CreateJackFlopCount();
void InitJackFlopCount();

void InitAdaptJackDiv();
void AdaptJackDivAdjust();
PROBABILITY iJackDiv;

extern UINT32 *aGNovPromVarsList;
extern UINT32 *aVarIsGNovProm;
extern UINT32 iNumGNovPromVars;

UBIGINT *aLitJackFlopCount;
UBIGINT *aLitJackResetFlopCount;
FLOAT *aLitJackNormFlopCount;

BOOL bJackUsePromising;

FLOAT fScoreNeg;
FLOAT fScorePos;

UINT32 iScoreRatioType;

UINT32 iAgeExp;
FLOAT fAgeDiv;
UINT32 iAgeGap;

#define JACK_NUMPROP 20
#define JACK_NUM_GREEDY_PROP 9
#define JACK_NUM_DIV_PROP 17

#define EPSILON_RELBREAK 0.001

REPORT *pRepJackTrace;
BOOL bJackHeader;

FLOAT aPropGWeights[JACK_NUMPROP];
FLOAT fPropGWeightsSum;
FLOAT aPropDWeights[JACK_NUMPROP];
FLOAT fPropDWeightsSum;

#define NUMCLAUSERANGE 4

FLOAT aGreedyWeights[NUMCLAUSERANGE];
FLOAT aMixedWeights[NUMCLAUSERANGE];
FLOAT aDivWeights[NUMCLAUSERANGE];
FLOAT aSumGreedyMixedDivWeights[NUMCLAUSERANGE];

PROBABILITY aUseGreedyBest[NUMCLAUSERANGE];
PROBABILITY aUseMixedBest[NUMCLAUSERANGE];
PROBABILITY aUseDivBest[NUMCLAUSERANGE];

UBIGINT iNumPromFlips;
UINT32 iSumSelClauseLen;
FLOAT fMeanSelClauseLen;

UINT32 iVarAgeHistoryLen;
UBIGINT *aVarAgeHistoryData;
UBIGINT **pVarAgeHistory;
void CreateAgeHistory();
void InitAgeHistory();
void UpdateAgeHistory();
UBIGINT VarAge(UINT32 iVar, UINT32 iIndex);


void AddJack() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("jack","",0,
    "Captain Jack",
    "Tompkins, Balint, Hoos [SAT 11]",
    "PickJack",
    "DefaultProcedures,Flip+DecPromVars+FCL,DecPromVars,FalseClauseList,VarLastChange,CreateJackScores,MakeBreak,FlipCounts,JackFlopCount,AdaptJackDivAdjust,PreRunJack,RunCalculationsJack,InitJackConfig,AgeHistory,CreateNextClauseLit,InitNextClauseLit,CreateClauseLastFlipVar,InitClauseLastFlipVar", //ClauseLitFlipCount,PromFlipCount
    "default","default");
  
  AddParmBool(&pCurAlg->parmList,"-prom","use promising variables [default %s]","","",&bJackUsePromising,1);

  AddParmFloat(&pCurAlg->parmList,"-selgreedy2","weight of greedy-only [default %s]","","",&aGreedyWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-selmixed2","weight of mixed [default %s]","","",&aMixedWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-seldiv2","weight of div-only [default %s]","","",&aDivWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-selgreedy3","weight of greedy-only [default %s]","","",&aGreedyWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-selmixed3","weight of mixed [default %s]","","",&aMixedWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-seldiv3","weight of div-only [default %s]","","",&aDivWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-selgreedy4","weight of greedy-only [default %s]","","",&aGreedyWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-selmixed4","weight of mixed [default %s]","","",&aMixedWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-seldiv4","weight of div-only [default %s]","","",&aDivWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-selgreedy5","weight of greedy-only [default %s]","","",&aGreedyWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-selmixed5","weight of mixed [default %s]","","",&aMixedWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-seldiv5","weight of div-only [default %s]","","",&aDivWeights[3],1);

  AddParmProbability(&pCurAlg->parmList,"-bestgreedy2","greedy distribution % [default %s]","","",&aUseGreedyBest[0],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestmixed2","mixed distribution % [default %s]","","",&aUseMixedBest[0],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestdiv2","div distribution % [default %s]","","",&aUseDivBest[0],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestgreedy3","greedy distribution % [default %s]","","",&aUseGreedyBest[1],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestmixed3","mixed distribution % [default %s]","","",&aUseMixedBest[1],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestdiv3","div distribution % [default %s]","","",&aUseDivBest[1],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestgreedy4","greedy distribution % [default %s]","","",&aUseGreedyBest[2],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestmixed4","mixed distribution % [default %s]","","",&aUseMixedBest[2],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestdiv4","div distribution % [default %s]","","",&aUseDivBest[2],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestgreedy5","greedy distribution % [default %s]","","",&aUseGreedyBest[3],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestmixed5","mixed distribution % [default %s]","","",&aUseMixedBest[3],0.5);
  AddParmProbability(&pCurAlg->parmList,"-bestdiv5","div distribution % [default %s]","","",&aUseDivBest[3],0.5);

  AddParmFloat(&pCurAlg->parmList,"-gscore","score [default %s]","","",&aPropGWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-gmake","make [default %s]","","",&aPropGWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-gbreak","break [default %s]","","",&aPropGWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-grelscore","relScore [default %s]","","",&aPropGWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-grelmake","relMake [default %s]","","",&aPropGWeights[4],1);
  AddParmFloat(&pCurAlg->parmList,"-grelbreak","relBreak [default %s]","","",&aPropGWeights[5],1);
  AddParmFloat(&pCurAlg->parmList,"-gsscore","sparrow score [default %s]","","",&aPropGWeights[6],1);
  AddParmFloat(&pCurAlg->parmList,"-gscorerat","score ratio [default %s]","","",&aPropGWeights[7],1);
  AddParmFloat(&pCurAlg->parmList,"-grelscorerat","score ratio [default %s]","","",&aPropGWeights[8],1);

  if (JACK_NUM_GREEDY_PROP != 9) {
    ReportPrint(pRepErr,"Unexpected Error: JACK_NUM_GREEDY_PROP\n");
    AbnormalExit();
    exit(1);
  }

  AddParmFloat(&pCurAlg->parmList,"-dnone","none/uniform [default %s]","","",&aPropDWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-drand","random [default %s]","","",&aPropDWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-dage","age [default %s]","","",&aPropDWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-dagegap","floor(age/gap) [default %s]","","",&aPropDWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-dtabu","tabu [default %s]","","",&aPropDWeights[4],1);
  AddParmFloat(&pCurAlg->parmList,"-dsage","sparrow age [default %s]","","",&aPropDWeights[5],1);
  AddParmFloat(&pCurAlg->parmList,"-dage1","age [default %s]","","",&aPropDWeights[6],1);
  AddParmFloat(&pCurAlg->parmList,"-dage5","age [default %s]","","",&aPropDWeights[7],1);
  AddParmFloat(&pCurAlg->parmList,"-dflip","flip [default %s]","","",&aPropDWeights[8],1);
  AddParmFloat(&pCurAlg->parmList,"-drelflip","relFlip [default %s]","","",&aPropDWeights[9],1);
  AddParmFloat(&pCurAlg->parmList,"-dflop","flopCount [default %s]","","",&aPropDWeights[10],1);
  AddParmFloat(&pCurAlg->parmList,"-dnormflop","normFlopCount [default %s]","","",&aPropDWeights[11],1);
  AddParmFloat(&pCurAlg->parmList,"-drelflop","flopCount [default %s]","","",&aPropDWeights[12],1);
  AddParmFloat(&pCurAlg->parmList,"-drelnormflop","normFlopCount [default %s]","","",&aPropDWeights[13],1);
  AddParmFloat(&pCurAlg->parmList,"-dresetflop","resetFlopCount [default %s]","","",&aPropDWeights[14],1);
  AddParmFloat(&pCurAlg->parmList,"-dfair","fair selection [default %s]","","",&aPropDWeights[15],1);
  AddParmFloat(&pCurAlg->parmList,"-dlast","last flip var [default %s]","","",&aPropDWeights[16],1);

  if (JACK_NUM_DIV_PROP != 17) {
    ReportPrint(pRepErr,"Unexpected Error: JACK_NUM_DIV_PROP\n");
    AbnormalExit();
    exit(1);
  }

  AddParmFloat(&pCurAlg->parmList,"-xssneg","sparrow Negative score adjustment parameter [default %s]","","",&fScoreNeg,2.0);
  AddParmFloat(&pCurAlg->parmList,"-xsspos","sparrow Positive score adjustment parameter [default %s]","","",&fScorePos,1.0);
  AddParmUInt(&pCurAlg->parmList,"-xscorerattype","score ratio type [default %s]","","",&iScoreRatioType,1);

  AddParmUInt(&pCurAlg->parmList,"-xageexp","INT sparrow age polynomial parameter [default %s]","adjusts the influence of the age","",&iAgeExp,4);
  AddParmFloat(&pCurAlg->parmList,"-xagediv","sparrow age threshold parameter [default %s]","threshold for age calculation","",&fAgeDiv,100000.0);
  AddParmUInt(&pCurAlg->parmList,"-xagegap","floor(age/agegap) for -dagegap [default %s]","","",&iAgeGap,1);
  AddParmUInt(&pCurAlg->parmList,"-xtabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,0);

  CreateTrigger("PickJack",ChooseCandidate,PickJack,"","");
  CreateTrigger("InitJackConfig",PostParameters,InitJackConfig,"","");

  CreateTrigger("CreateJackFlopCount",CreateStateInfo,CreateJackFlopCount,"","");
  CreateTrigger("InitJackFlopCount",InitStateInfo,InitJackFlopCount,"","");
  CreateContainerTrigger("JackFlopCount","CreateJackFlopCount,InitJackFlopCount");

  CreateTrigger("InitAdaptJackDiv",PostInit,InitAdaptJackDiv,"","");
  CreateTrigger("AdaptJackDivAdjust",PostFlip,AdaptJackDivAdjust,"InitAdaptJackDiv","");

  pRepJackTrace = CreateReport("Jacktrace","Jacktrace","Prints data for every var every step","stdout","");

  CreateTrigger("CreateJackScores",CreateStateInfo,CreateJackScores,"","");

  CreateTrigger("PreRunJack",PreRun,PreRunJack,"","");
  CreateTrigger("RunCalculationsJack",RunCalculations,RunCalculationsJack,"","");

  CreateTrigger("CreateClauseLastFlipVar",CreateStateInfo,CreateClauseLastFlipVar,"","");
  CreateTrigger("InitClauseLastFlipVar",InitStateInfo,InitClauseLastFlipVar,"","");
	
  AddColumnUBigInt("percentprom","Percent of Promising Flips",
    "Percent",
    "   Prom",
    "  Flips",
    "%7.1f",
    &iNumPromFlips,"",ColTypeFinalDivStep100);

  AddColumnFloat("meanclauselen","Average Selected Clause Length",
    "Mean",
    "Cls.",
    " Len",
    "%4.1f",
    &fMeanSelClauseLen,"",ColTypeFinal);

  AddStatCol("percentprom","PercentProm","mean",0);

  CreateTrigger("CreateAgeHistory",CreateStateInfo,CreateAgeHistory,"VarLastChange,FlipCounts","");
  CreateTrigger("InitAgeHistory",InitStateInfo,InitAgeHistory,"","");
  CreateTrigger("UpdateAgeHistory",UpdateStateInfo,UpdateAgeHistory,"","UpdateVarLastChange,UpdateFlipCounts");
  CreateContainerTrigger("AgeHistory","CreateAgeHistory,InitAgeHistory,UpdateAgeHistory");
}


void PreRunJack() {

  iNumPromFlips = 0;
  iSumSelClauseLen = 0;

}

void RunCalculationsJack() {
  fMeanSelClauseLen = ((FLOAT) (iSumSelClauseLen) / (FLOAT) (iStep - iNumPromFlips));
}


void InitJackConfig() {
  UINT32 j;

  fPropGWeightsSum = FLOATZERO;
  for (j=0; j < JACK_NUM_GREEDY_PROP; j++) {
    fPropGWeightsSum += aPropGWeights[j];
  }
  if (fPropGWeightsSum == FLOATZERO) {
    ReportPrint(pRepErr,"Warning! set Greedy Property weights \n");
    fPropGWeightsSum = 1.0;
    aPropGWeights[0] = 1.0;
  }

  fPropDWeightsSum = FLOATZERO;
  for (j=0; j < JACK_NUM_DIV_PROP; j++) {
    fPropDWeightsSum += aPropDWeights[j];
  }
  if (fPropDWeightsSum == FLOATZERO) {
    ReportPrint(pRepErr,"Warning! set Div Property weights \n");
    fPropDWeightsSum = 1.0;
    aPropDWeights[0] = 1.0;
  }

  for (j=0; j < NUMCLAUSERANGE; j++) {
    aSumGreedyMixedDivWeights[j] = aGreedyWeights[j] + aMixedWeights[j] + aDivWeights[j];
    if (aSumGreedyMixedDivWeights[j] == FLOATZERO) {
      ReportPrint(pRepErr,"Warning! set Greedy/Mixed/Div weights \n");
      aSumGreedyMixedDivWeights[j] = 1.0;
      aGreedyWeights[j] = 1.0;
    }
  }

  iVarAgeHistoryLen = 6;
}

void PickJack() {

  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 j;
  UINT32 k;
 
  FLOAT fDiv;

  FLOAT fSelectSum;
  FLOAT fSelectPos;

  FLOAT fMaxPropValue;
  FLOAT fMinPropValue;
  FLOAT fPropValue;
  //FLOAT fPropValueSum;
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

  fDiv = ProbToFloat(iJackDiv);
  
  iFlipCandidate = 0;

  if (iNumFalse==0) {
    return;
  }

  ////// PROM VARS

  if (bJackUsePromising) {
    if (iNumDecPromVars > 0) {
      PromisingSelectBest();
    }
    if (iFlipCandidate != 0) {
      //aLitPromFlipCount[GetFalseLit(iFlipCandidate)]++;
      iNumPromFlips++;
      return;
    }
  }

  // On to Jack

  iClause = aFalseList[RandomInt(iNumFalse)];
  iClauseLen = aClauseLen[iClause];
  iSumSelClauseLen += iClauseLen;

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
          fRelBreak = ((FLOAT) aBreakCount[iVar]) / (EPSILON_RELBREAK + (FLOAT) aNumLitOcc[GetNegatedLit(*pLit)]);
          fPropValue = fRelMake - fRelBreak;
          break;
        case 4:
          fRelMake = ((FLOAT) aMakeCount[iVar]) / ((FLOAT) aNumLitOcc[*pLit]);
          fPropValue = fRelMake;
          break;
        case 5:
          fRelBreak = ((FLOAT) aBreakCount[iVar]) / (EPSILON_RELBREAK + (FLOAT) aNumLitOcc[GetNegatedLit(*pLit)]);
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
          fRelBreak = ((FLOAT) aBreakCount[iVar]) / (EPSILON_RELBREAK + (FLOAT) aNumLitOcc[GetNegatedLit(*pLit)]);
          if (iScoreRatioType) {
            fPropValue = fRelMake / (fRelMake + fRelBreak);
          } else {
            fPropValue = fRelMake / (fRelMake + fRelBreak);
            fPropValue = fRelMake / (fRelBreak + EPSILON_RELBREAK);
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
        fPropertyAddOffset = EPSILON_RELBREAK - fMinPropValue;
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

void CreateJackFlopCount() {
  aLitJackFlopCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT));
  aLitJackResetFlopCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT));
  aLitJackNormFlopCount = (FLOAT *) AllocateRAM((iNumVars+1)*2*sizeof(FLOAT));
}

void InitJackFlopCount() {
  memset(aLitJackFlopCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
  memset(aLitJackResetFlopCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
  memset(aLitJackNormFlopCount,0,(iNumVars+1)*2*sizeof(FLOAT));
}

void InitAdaptJackDiv() {
  iLastAdaptStep=iStep;
  iLastAdaptNumFalse=iNumFalse;
  iJackDiv = 0;
}

void AdaptJackDivAdjust() {

  /* this varaint allows for different values of Phi & Theta */

  if (iStep-iLastAdaptStep > iNumClauses*fAdaptTheta) {
    iJackDiv += (PROBABILITY) ((UINT32MAX - iJackDiv)*fAdaptPhi);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  } else if (iNumFalse < iLastAdaptNumFalse) {
    iJackDiv -= (PROBABILITY) (iJackDiv * fAdaptPhi / 2);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  }
}


void CreateAgeHistory() {
  UINT32 j;
  aVarAgeHistoryData = (UBIGINT *) AllocateRAM((iNumVars+1)*iVarAgeHistoryLen*sizeof(UBIGINT));
  pVarAgeHistory = (UBIGINT **) AllocateRAM((iNumVars+1)*sizeof(UBIGINT *));
  for (j=0;j<=iNumVars;j++) {
    pVarAgeHistory[j] = &aVarAgeHistoryData[j*iVarAgeHistoryLen];
  }
}

void InitAgeHistory() {
  memset(aVarAgeHistoryData,0,(iNumVars+1)*iVarAgeHistoryLen*sizeof(UBIGINT));
}

void UpdateAgeHistory() {
  // replaces UpdateVarLastChange() & UpdateFlipCounts()

  // SCREWS UP IF THERE IS A RESET

  pVarAgeHistory[iFlipCandidate][aFlipCounts[iFlipCandidate] % iVarAgeHistoryLen] = iStep - aVarLastChange[iFlipCandidate];
  aFlipCounts[iFlipCandidate]++;
  aVarLastChange[iFlipCandidate] = iStep;

}

UBIGINT VarAge(UINT32 iVar, UINT32 iIndex) {
  UINT32 j;
  UINT32 iPos;
  UBIGINT iAge;

  iAge = iStep - aVarLastChange[iVar];
  iPos = (aFlipCounts[iVar] + iVarAgeHistoryLen - iIndex) % iVarAgeHistoryLen;

  for (j=1; j <= iIndex; j++) {
    iPos = (aFlipCounts[iVar] + iVarAgeHistoryLen - j) % iVarAgeHistoryLen;
    iAge += pVarAgeHistory[iVar][iPos];
  }

  return iAge;

}




#ifdef __cplusplus
}
#endif
