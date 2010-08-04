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

void InitHybridInfo();
void UpdateHybridInfo();

void PickHybrid1();
void InitHybrid1();

void PickHybrid2();

FLOAT fHybridGamma;
FLOAT fVW2WeightMax;

void AddHybrid() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hybrid","2007",FALSE,
   "Hybrid1: Switch between VW and adaptG2WSATp (Sat07 version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid1,InitHybridInfo,UpdateHybridInfo,InitHybrid1",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");

  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid1 switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSATp~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,15);

  CreateTrigger("InitHybridInfo",InitStateInfo,InitHybridInfo,"","");
  CreateTrigger("UpdateHybridInfo",UpdateStateInfo,UpdateHybridInfo,"","UpdateVW2Weights");

  CreateTrigger("PickHybrid1",ChooseCandidate,PickHybrid1,"","");
  CreateTrigger("InitHybrid1",InitStateInfo,InitHybrid1,"","");


  pCurAlg = CreateAlgorithm("hybrid","2009",FALSE,
   "Hybrid2: Switch between VW and adaptG2WSAT+ (Sat09 version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid2,InitHybridInfo,UpdateHybridInfo,InitVW2Auto,UpdateVW2Auto",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");
  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid2 switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSAT+~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,1.025);
  CreateTrigger("PickHybrid2",ChooseCandidate,PickHybrid2,"","");

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

void PickHybrid1() {
  if (fVW2WeightMax >= fHybridGamma * fVW2WeightMean) {
    iMultiAlgCurrent = 3;
    PickVW2Auto();
  } else {
    PickG2WSatP();
  }
}

void InitHybrid1() {
  iMaxExpProbability=2;
  fVW2Smooth = 0.0f;
}

void PickHybrid2() {
  if (fVW2WeightMax > fHybridGamma * fVW2WeightMean) {
    iMultiAlgCurrent = 3;
    PickVW2Auto();
  } else {
    PickG2WSatNoveltyPlusOldest();
  }
}
