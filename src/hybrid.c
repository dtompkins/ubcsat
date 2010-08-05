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

void InitHybridInfo();
void UpdateHybridInfo();
void PickHybrid();

void InitNoVWSmoothing();

void InitHybrid1();
void InitHybrid2();

FLOAT fHybridGamma;
FLOAT fVW2WeightMax;

void AddHybrid() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hybrid","",FALSE,
   "Hybrid: Switch between VW and adaptG2WSAT+ (paper version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid,InitHybridInfo,UpdateHybridInfo,InitNoVWSmoothing,ConfigureG2WSatGeneral",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");

  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSAT*~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,10);

  AddParmUInt(&pCurAlg->parmList,"-sub","G2WSAT sub-algortihm [default %s]","0: Novelty+~1:Novelty++~2:Novelty+p~3:Novelty++p~4:Novelty++0~5:Novelty++1~6:Novelty++2","",&iG2WsatSubAlgID,0);
  AddParmBool(&pCurAlg->parmList,"-psel","decreasing promising variable selection [default %s]","0: best~1:oldest~2:random","",&iG2WsatPromSelectID,1);
  // todo add auto-smoothing

  CreateTrigger("PickHybrid",ChooseCandidate,PickHybrid,"","");
  CreateTrigger("InitHybridInfo",InitStateInfo,InitHybridInfo,"","");
  CreateTrigger("UpdateHybridInfo",UpdateStateInfo,UpdateHybridInfo,"","UpdateVW2Weights");
  CreateTrigger("InitNoVWSmoothing",InitStateInfo,InitNoVWSmoothing,"","");


  pCurAlg = CreateAlgorithm("hybrid","2007",FALSE,
   "Hybrid1: Switch between VW and adaptG2WSATp (Sat07 version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid,InitHybridInfo,UpdateHybridInfo,InitHybrid1,InitNoVWSmoothing",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");
  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSAT*~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,15);
  CreateTrigger("InitHybrid1",InitStateInfo,InitHybrid1,"","");

  pCurAlg = CreateAlgorithm("hybrid","2009",FALSE,
   "Hybrid2: Switch between VW and adaptG2WSAT+ (Sat09 version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid,InitHybridInfo,UpdateHybridInfo,InitVW2Auto,UpdateVW2Auto,InitHybrid2",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");
  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSAT*~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,1.025);
  CreateTrigger("InitHybrid2",InitStateInfo,InitHybrid2,"","");

}

void InitHybridInfo() {
  fVW2WeightMax = FLOATZERO;
}

void UpdateHybridInfo() {
  FLOAT fPrevWeight;
  if (iFlipCandidate) {
    fPrevWeight = aVW2Weights[iFlipCandidate];
    aVW2Weights[iFlipCandidate] = (1.0f - fVW2Smooth) * (aVW2Weights[iFlipCandidate] + 1.0f) + (fVW2Smooth * (FLOAT) iStep);
    fVW2WeightMean += (aVW2Weights[iFlipCandidate] - fPrevWeight) / iNumVars;
    if (aVW2Weights[iFlipCandidate] > fVW2WeightMax) {
      fVW2WeightMax = aVW2Weights[iFlipCandidate];
    }
  }
}

void PickHybrid() {
  if (fVW2WeightMax >= fHybridGamma * fVW2WeightMean) {
    iMultiAlgCurrent = 3;
    PickVW2Auto();
  } else {
    PickG2WSatGeneral();
  }
}

void InitNoVWSmoothing() {
  iMaxExpProbability=2;
  fVW2Smooth = 0.0f;
}

void InitHybrid1() {
  iG2WsatSubAlgID = 2;
  iG2WsatPromSelectID = 1;
  ConfigureG2WSatGeneral();
  InitNoVWSmoothing();
}

void InitHybrid2() {
  iG2WsatSubAlgID = 0;
  iG2WsatPromSelectID = 1;
  ConfigureG2WSatGeneral();
}

#ifdef __cplusplus 
}
#endif
