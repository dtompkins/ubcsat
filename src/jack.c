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

void PickNewJack();
void FlipNewJack();
void InitNewJackConfig();
void PreRunNewJack();
void RunCalculationsNewJack();

FLOAT *aNewJackGreedy;
FLOAT *aNewJackDiv;
UINT32 *aNewJackCandidates;

void CreateNewJackScores() {
  aNewJackGreedy = (FLOAT *) AllocateRAM(iMaxClauseLen*sizeof(FLOAT));
  aNewJackDiv = (FLOAT *) AllocateRAM(iMaxClauseLen*sizeof(FLOAT));
  aNewJackCandidates = (UINT32 *) AllocateRAM(iMaxClauseLen*sizeof(UINT32));
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


void CreateNewJackFlopCount();
void InitNewJackFlopCount();

//void CreatePromFlipCount();
//void InitPromFlipCount();

void InitAdaptNewJackDiv();
void AdaptNewJackDivAdjust();
PROBABILITY iNewJackDiv;

//void CreateClauseLitFlipCount();
//void InitClauseLitFlipCount();
//UBIGINT *pClauseLitFlipCountData;
//UBIGINT **pClauseLitFlipCount;


extern UINT32 *aGNovPromVarsList;
extern UINT32 *aVarIsGNovProm;
extern UINT32 iNumGNovPromVars;

UBIGINT *aLitNewJackFlopCount;
UBIGINT *aLitNewJackResetFlopCount;
FLOAT *aLitNewJackNormFlopCount;

//UBIGINT *aLitPromFlipCount;

BOOL bNewJackUsePromising;

FLOAT fScoreNeg;
FLOAT fScorePos;

UINT32 iScoreRatioType;

UINT32 iAgeExp;
FLOAT fAgeDiv;
UINT32 iAgeGap;

#define NEWJACKNUMPROP 20
#define NUMGREEDYPROP 9
#define NUMDIVPROP 17

#define EPSILON_RELBREAK 0.001

REPORT *pRepNewJackTrace;
BOOL bNewJackHeader;

FLOAT aPropGWeights[NEWJACKNUMPROP];
FLOAT fPropGWeightsSum;
FLOAT aPropDWeights[NEWJACKNUMPROP];
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
    "PickNewJack",
    "DefaultProcedures,Flip+DecPromVars+FCL,DecPromVars,FalseClauseList,VarLastChange,CreateNewJackScores,MakeBreak,FlipCounts,NewJackFlopCount,AdaptNewJackDivAdjust,PreRunNewJack,RunCalculationsNewJack,InitNewJackConfig,AgeHistory,CreateNextClauseLit,InitNextClauseLit,CreateClauseLastFlipVar,InitClauseLastFlipVar", //ClauseLitFlipCount,PromFlipCount
    "default","default");
  
  AddParmBool(&pCurAlg->parmList,"-prom","use promising variables [default %s]","","",&bNewJackUsePromising,1);

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

  if (NUMGREEDYPROP != 9) {
    ReportPrint(pRepErr,"Unexpected Error: NUMGREEDYPROP\n");
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

  if (NUMDIVPROP != 17) {
    ReportPrint(pRepErr,"Unexpected Error: NUMDIVPROP\n");
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

  //AddParmFloat(&pCurAlg->parmList,"-phi","adjustment parameter phi [default %s]","phi determines the rate of change of noise","",&fAdaptPhi,(1.0f/5.0f));
  //AddParmFloat(&pCurAlg->parmList,"-theta","adjustment parameter theta [default %s]","theta determines the stagnation detection","",&fAdaptTheta,(1.0f/6.0f));
  
  CreateTrigger("PickNewJack",ChooseCandidate,PickNewJack,"","");
  CreateTrigger("InitNewJackConfig",PostParameters,InitNewJackConfig,"","");

  CreateTrigger("CreateNewJackFlopCount",CreateStateInfo,CreateNewJackFlopCount,"","");
  CreateTrigger("InitNewJackFlopCount",InitStateInfo,InitNewJackFlopCount,"","");
  CreateContainerTrigger("NewJackFlopCount","CreateNewJackFlopCount,InitNewJackFlopCount");

  //CreateTrigger("CreatePromFlipCount",CreateStateInfo,CreatePromFlipCount,"","");
  //CreateTrigger("InitPromFlipCount",InitStateInfo,InitPromFlipCount,"","");
  //CreateContainerTrigger("PromFlipCount","CreatePromFlipCount,InitPromFlipCount");

  //CreateTrigger("CreateClauseLitFlipCount",CreateStateInfo,CreateClauseLitFlipCount,"","");
  //CreateTrigger("InitClauseLitFlipCount",InitStateInfo,InitClauseLitFlipCount,"","");
  //CreateContainerTrigger("ClauseLitFlipCount","CreateClauseLitFlipCount,InitClauseLitFlipCount");

  CreateTrigger("InitAdaptNewJackDiv",PostInit,InitAdaptNewJackDiv,"","");
  CreateTrigger("AdaptNewJackDivAdjust",PostFlip,AdaptNewJackDivAdjust,"InitAdaptNewJackDiv","");

  pRepNewJackTrace = CreateReport("newjacktrace","NewJacktrace","Prints data for every var every step","stdout","");

  CreateTrigger("CreateNewJackScores",CreateStateInfo,CreateNewJackScores,"","");

  CreateTrigger("PreRunNewJack",PreRun,PreRunNewJack,"","");
  CreateTrigger("RunCalculationsNewJack",RunCalculations,RunCalculationsNewJack,"","");

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


void PreRunNewJack() {

  iNumPromFlips = 0;
  iSumSelClauseLen = 0;

}

void RunCalculationsNewJack() {
  fMeanSelClauseLen = ((FLOAT) (iSumSelClauseLen) / (FLOAT) (iStep - iNumPromFlips));
}


void InitNewJackConfig() {
  UINT32 j;

  fPropGWeightsSum = FLOATZERO;
  for (j=0; j < NUMGREEDYPROP; j++) {
    fPropGWeightsSum += aPropGWeights[j];
  }
  if (fPropGWeightsSum == FLOATZERO) {
    ReportPrint(pRepErr,"Warning! set Greedy Property weights \n");
    fPropGWeightsSum = 1.0;
    aPropGWeights[0] = 1.0;
  }

  fPropDWeightsSum = FLOATZERO;
  for (j=0; j < NUMDIVPROP; j++) {
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

void PickNewJack() {

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

  UINT32 iNumNewJackCandidates;

  fDiv = ProbToFloat(iNewJackDiv);
  
  iFlipCandidate = 0;

  if (iNumFalse==0) {
    return;
  }

  ////// PROM VARS

  if (bNewJackUsePromising) {
    if (iNumDecPromVars > 0) {
      PromisingSelectBest();
    }
    if (iFlipCandidate != 0) {
      //aLitPromFlipCount[GetFalseLit(iFlipCandidate)]++;
      iNumPromFlips++;
      return;
    }
  }

  // On to NewJack

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

  iNumNewJackCandidates = 0;
  //fPropValueSum = FLOATZERO;
  fMaxPropValue = -FLOATMAX;
  fMinPropValue = FLOATMAX;

  if ((bJustGreedy)||(bMixed)) {

    // determine which greedy property to use
    fSelectPos = RandomFloat()*fPropGWeightsSum;
    fSelectSum = FLOATZERO;
    iSelectProperty = 0;
    for (j=0;j<NUMGREEDYPROP;j++) {
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
            iNumNewJackCandidates = 0;
            fMaxPropValue = fPropValue;
          }
          aNewJackCandidates[iNumNewJackCandidates++] = iVar;
        }
      } else {
        //fPropValueSum += fPropValue;
        //if ( fPropValue > fMaxPropValue) {
        //  fMaxPropValue = fPropValue;
        //}
        if ( fPropValue < fMinPropValue) {
          fMinPropValue = fPropValue;
        }
        aNewJackGreedy[j] = fPropValue;
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
          aNewJackGreedy[j] += fPropertyAddOffset;
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////

  //fPropValueSum = FLOATZERO;
  //fMaxPropValue = -FLOATMAX;
  fMinPropValue = FLOATMAX;

  if ((bJustDiv)||(bMixed)) {
    // determine which Div to use

    fSelectPos = RandomFloat()*fPropDWeightsSum;
    fSelectSum = FLOATZERO;
    iSelectProperty = 0;
    for (j=0;j<(NUMDIVPROP);j++) {
      fSelectSum += aPropDWeights[j];
      if (fSelectPos <= fSelectSum) {
        iSelectProperty = j;
        break;
      }
    }

    pLit = pClauseLits[iClause];
    for (j=0;j<iClauseLen;j++) {
      iVar = GetVarFromLit(*pLit);
/*
  AddParmFloat(&pCurAlg->parmList,"-dnone","none/uniform [default %s]","","",&aPropDWeights[0],1);
  AddParmFloat(&pCurAlg->parmList,"-drand","random [default %s]","","",&aPropDWeights[1],1);
  AddParmFloat(&pCurAlg->parmList,"-dage","age [default %s]","","",&aPropDWeights[2],1);
  AddParmFloat(&pCurAlg->parmList,"-dagegap","floor(age/gap) [default %s]","","",&aPropDWeights[3],1);
  AddParmFloat(&pCurAlg->parmList,"-dtabu","tabu [default %s]","","",&aPropDWeights[4],1);
  AddParmFloat(&pCurAlg->parmList,"-dsage","sparrow age [default %s]","","",&aPropDWeights[5],1);
  AddParmFloat(&pCurAlg->parmList,"-dage2","age [default %s]","","",&aPropDWeights[6],1);
  AddParmFloat(&pCurAlg->parmList,"-dflip","flip [default %s]","","",&aPropDWeights[7],1);*/

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
          fPropValue = (FLOAT) aLitNewJackFlopCount[*pLit];
          break;
        case 11:
          fPropValue = (FLOAT) aLitNewJackNormFlopCount[*pLit];
          break;
        case 12:
          fPropValue = (FLOAT) aLitNewJackFlopCount[*pLit]  / ((FLOAT) aNumLitOcc[*pLit]);
          break;
        case 13:
          fPropValue = (FLOAT) aLitNewJackNormFlopCount[*pLit]  / ((FLOAT) aNumLitOcc[*pLit]);
          break;
        case 14:
          fPropValue = (FLOAT) aLitNewJackResetFlopCount[*pLit];
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
            iNumNewJackCandidates = 0;
            fMaxPropValue = fPropValue;
          }
          aNewJackCandidates[iNumNewJackCandidates++] = iVar;
        }
      } else {
        //fPropValueSum += fPropValue;
        //if ( fPropValue > fMaxPropValue) {
        //  fMaxPropValue = fPropValue;
        //}
        if ( fPropValue < fMinPropValue) {
          fMinPropValue = fPropValue;
        }
        aNewJackDiv[j] = fPropValue;
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
          aNewJackDiv[j] += fPropertyAddOffset;
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
      aNewJackGreedy[j] = aNewJackDiv[j];
    }
  }

  if (bMixed) {
    fMaxPropValue = -FLOATMAX;
    for (j=0;j<iClauseLen;j++) {
      aNewJackGreedy[j] *= aNewJackDiv[j];
      if (bUseBest) {
        if ( aNewJackGreedy[j] >= fMaxPropValue) {
          if (aNewJackGreedy[j] > fMaxPropValue) {
            iNumNewJackCandidates=0;
            fMaxPropValue = aNewJackGreedy[j];
          }
          aNewJackCandidates[iNumNewJackCandidates++] = GetVar(iClause,j);
        }
      }
    }
  }
  
  if (bUseBest) {
    if (iNumNewJackCandidates > 1) {
      iFlipCandidate = aNewJackCandidates[RandomInt(iNumNewJackCandidates)];
    } else {
      iFlipCandidate = aNewJackCandidates[0];
    }
  } else {
    fSelectSum = FLOATZERO;
    for (j=0;j<iClauseLen;j++) {
      if (aNewJackGreedy[j] < FLOATZERO) {
        aNewJackGreedy[j] = FLOATZERO;
      }
      fSelectSum += aNewJackGreedy[j];
    }
    if (fSelectSum == FLOATZERO) {
      j = RandomInt(iClauseLen);
      iFlipCandidate = GetVar(iClause,j);
    } else {
      fSelectPos = RandomFloat()*fSelectSum;
      fSelectSum = FLOATZERO;
      for (j=0;j<iClauseLen;j++) {
        fSelectSum += aNewJackGreedy[j];
        if (fSelectPos <= fSelectSum) {
          iFlipCandidate = GetVar(iClause,j);
          // pClauseLitFlipCount[iClause][j]++; // removed
          break;
        }
      }
    }
  }



      //if (iSelectProperty == 3) {
      //  fPropValue = (FLOAT) (iStep) / (FLOAT) (iNumVars);
      //  fPropValue /= (FLOAT) (aFlipCounts[iVar]+1);
      //}

  //  iAge = iStep - aVarLastChange[iVar];
  //  iFlipCount = aFlipCounts[iVar];
  //  iFlopCount = aLitNewJackFlopCount[*pLit];
  //  iPromFlipCount = aLitPromFlipCount[*pLit];
  //  iClauseLitFlipCount = pClauseLitFlipCount[iClause][j];
  //  
  //  aLitNewJackFlopCount[*pLit]++;

  //  ///////////////////////////////////////////////////////////////////////////////////
  //  
  //  ///////////////////////////////////////////////////////////////////////////////////
  //  if (bNewJackAge) {
  //    fBaseAgeProb = ((FLOAT) iAge) / fAgeDiv;
  //    fAgeProb = 1.0;
  //    for (k=0; k < iAgeExp; k++) {
  //      fAgeProb *= fBaseAgeProb;
  //    }
  //    fAgeProb += 1.0;
  //    aSparrowScores[j] *= fAgeProb;
  //  } else {
  //    if (fExtAge > FLOATZERO) {
  //      iAgeAdj = iAge / iAgeGap;
  //      fAgeRating = ((FLOAT) iAgeAdj) / (FLOAT) (iSumAge);
  //      aSparrowScores[j] *= (1- fExtAge) + fExtAge * fAgeRating;
  //    }
  //  }

  //  ///////////////////////////////////////////////////////////////////////////////////

  //  if (iTabuTenure > 0) {
  //    if (iAge < iTabuTenure) {
  //      aSparrowScores[j] *= fTabuPen;
  //    }
  //  }

  //  ///////////////////////////////////////////////////////////////////////////////////

  //  if (fExtFlip > FLOATZERO) {
  //    if (iSumFlipCount) {
  //      fFlipRating = (FLOAT) (iSumFlipCount - iFlipCount) / (FLOAT) (iSumFlipCount);
  //      aSparrowScores[j] *= (1- fExtFlip) + fExtFlip * fFlipRating;
  //    }
  //  }

  //  ///////////////////////////////////////////////////////////////////////////////////

  //  if (fExtFlop > FLOATZERO) {
  //    if (iSumFlopCount) {
  //      fFlopRating = (FLOAT) (iFlopCount) / (FLOAT) (iSumFlopCount);
  //      aSparrowScores[j] *= (1- fExtFlop) + fExtFlop * fFlopRating;
  //    }
  //  }

  //  ///////////////////////////////////////////////////////////////////////////////////

  //  if (fExtClauseFlip > FLOATZERO) {
  //    if (iSumClauseLitFlipCount > 0) {
  //      fFlipRating = (FLOAT) (iSumClauseLitFlipCount - iClauseLitFlipCount) / (FLOAT) (iSumClauseLitFlipCount);
  //      aSparrowScores[j] *= (1- fExtClauseFlip) + fExtClauseFlip * fClauseFlipRating;
  //    }
  //  }

  //  ///////////////////////////////////////////////////////////////////////////////////

  //  if (pRepNewJackTrace->bActive) {
  //    if (!bNewJackHeader) {
  //      ReportPrint(pRepNewJackTrace,"RUN,STEP,DIV,CLAUSE,VAR,PENSCORE,REALSCORE,AGE,FLIP,FLOP,CLAUSEFLIP,PROMFLIP,SCOREPROB,AGEPROB,AGERATING,FLIPRATING,FLOPRATING,CLAUSEFLIPRATING,FINALSCORE\n");
  //      bNewJackHeader = 1;
  //    }
  //    ReportPrint1(pRepNewJackTrace,"%"P32",",iRun);
  //    ReportPrint1(pRepNewJackTrace,"%"P64",",iStep);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fDiv);
  //    ReportPrint1(pRepNewJackTrace,"%"P32",",iClause);
  //    ReportPrint1(pRepNewJackTrace,"%"P32",",iVar);
  //    ReportPrint1(pRepNewJackTrace,"%"PS32",",iPenScore);
  //    ReportPrint1(pRepNewJackTrace,"%"PS32",",iRealScore);
  //    ReportPrint1(pRepNewJackTrace,"%"P64",",iAge);
  //    ReportPrint1(pRepNewJackTrace,"%"P64",",iFlipCount);
  //    ReportPrint1(pRepNewJackTrace,"%"P64",",iFlopCount);
  //    ReportPrint1(pRepNewJackTrace,"%"P64",",iClauseLitFlipCount);
  //    ReportPrint1(pRepNewJackTrace,"%"P64",",iPromFlipCount);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fScoreProb);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fAgeProb);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fAgeRating);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fFlipRating);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fFlopRating);
  //    ReportPrint1(pRepNewJackTrace,"%g,",fClauseFlipRating);
  //    ReportPrint1(pRepNewJackTrace,"%g\n",aSparrowScores[j]);

  //  }


  //  fScoreSum += aSparrowScores[j];
  //  pLit++;
  //}

  //fScorePos = RandomFloat()*fScoreSum;
  //fScoreSum = 0.0;

  //for (j=0;j<iClauseLen;j++) {
  //  fScoreSum += aSparrowScores[j];
  //  if (fScorePos <= fScoreSum) {
  //    iFlipCandidate = GetVar(iClause,j);
  //    pClauseLitFlipCount[iClause][j]++;
  //    break;
  //  }
  //}

  //if (bNewJackFlopCountReset) {
  //  aLitNewJackFlopCount[GetFalseLit(iFlipCandidate)] = 0;
  //} else {
  //  aLitNewJackFlopCount[GetFalseLit(iFlipCandidate)]--;
  //}
  fNormClauseLen = 1.0f / (FLOAT) iClauseLen;
  pLit = pClauseLits[iClause];
  for (j=0;j<iClauseLen;j++) {
    if (GetVarFromLit(*pLit) != iFlipCandidate) {
      aLitNewJackFlopCount[*pLit]++;
      aLitNewJackResetFlopCount[*pLit]++;
      aLitNewJackNormFlopCount[*pLit] += fNormClauseLen;
    }
    pLit++;
  }
  aLitNewJackResetFlopCount[GetFalseLit(iFlipCandidate)] = 0;
  aClauseLastFlipVar[iClause] = iFlipCandidate;

}

void CreateNewJackFlopCount() {
  aLitNewJackFlopCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT));
  aLitNewJackResetFlopCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT));
  aLitNewJackNormFlopCount = (FLOAT *) AllocateRAM((iNumVars+1)*2*sizeof(FLOAT));
}

void InitNewJackFlopCount() {
  memset(aLitNewJackFlopCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
  memset(aLitNewJackResetFlopCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
  memset(aLitNewJackNormFlopCount,0,(iNumVars+1)*2*sizeof(FLOAT));
}

//void CreatePromFlipCount() {
//  aLitPromFlipCount = (UBIGINT *) AllocateRAM((iNumVars+1)*2*sizeof(UBIGINT));
//}
//
//void InitPromFlipCount() {
//  memset(aLitPromFlipCount,0,(iNumVars+1)*2*sizeof(UBIGINT));
//}

//void CreateClauseLitFlipCount() {
//  UINT32 j;
//  UBIGINT *pCurPointer;
//  pClauseLitFlipCountData = (UBIGINT *) AllocateRAM(iNumLits * sizeof(UBIGINT));
//  pClauseLitFlipCount = (UBIGINT **) AllocateRAM(iNumClauses * sizeof(UBIGINT *));
//  pCurPointer = pClauseLitFlipCountData;
//  for (j=0;j<iNumClauses;j++) {
//    pClauseLitFlipCount[j] = pCurPointer;
//    pCurPointer += aClauseLen[j];
//  }
//}
//
//void InitClauseLitFlipCount() {
//  memset(pClauseLitFlipCountData,0,iNumLits * sizeof(UBIGINT));
//}

void InitAdaptNewJackDiv() {
  iLastAdaptStep=iStep;
  iLastAdaptNumFalse=iNumFalse;
  iNewJackDiv = 0;
}

void AdaptNewJackDivAdjust() {

  /* this varaint allows for different values of Phi & Theta */

  if (iStep-iLastAdaptStep > iNumClauses*fAdaptTheta) {
    iNewJackDiv += (PROBABILITY) ((UINT32MAX - iNewJackDiv)*fAdaptPhi);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  } else if (iNumFalse < iLastAdaptNumFalse) {
    iNewJackDiv -= (PROBABILITY) (iNewJackDiv * fAdaptPhi / 2);
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
