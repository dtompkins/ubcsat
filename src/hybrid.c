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

void PickHybrid1();
void InitHybrdInfo();
void UpdateHybridInfo();

FLOAT fHybridGamma;
FLOAT fVW2WeightMax;

void AddHybrid() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hybrid1","",FALSE,
   "Hybrid1: Switch between VW and adaptG2WSATp",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid1,InitHybrdInfo,UpdateHybridInfo",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,InitVW2Auto,UpdateVW2Auto,VW2Weights",
    "default","default");

  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid1 switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSATp~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,10);
  
  CreateTrigger("PickHybrid1",ChooseCandidate,PickHybrid1,"","");
  CreateTrigger("InitHybrdInfo",InitStateInfo,InitHybrdInfo,"","");
  CreateTrigger("UpdateHybridInfo",UpdateStateInfo,UpdateHybridInfo,"","UpdateVW2Weights");

  //CreateTrigger("AdaptG2WSatNoise",PostFlip,AdaptG2WSatNoise,"InitAdaptG2WSatNoise","");


}

void PickHybrid1() {
  if (fVW2WeightMax > fHybridGamma * fVW2WeightMean) {
    PickVW2Auto();
  } else {
    PickG2WSatP();
  }
}

void InitHybrdInfo() {
  fVW2WeightMax = FLOATZERO;
}

void UpdateHybridInfo() {
  FLOAT fPrevWeight = aVW2Weights[iFlipCandidate];
  //aVW2Weights[iFlipCandidate] = (1.0f - fVW2Smooth) * (aVW2Weights[iFlipCandidate] + 1.0f) + (fVW2Smooth * (FLOAT) iStep);
  aVW2Weights[iFlipCandidate] = aVW2Weights[iFlipCandidate] + 1.0f;
  fVW2WeightMean += (aVW2Weights[iFlipCandidate] - fPrevWeight) / iNumVars;
  if (aVW2Weights[iFlipCandidate] > fVW2WeightMax) {
    fVW2WeightMax = aVW2Weights[iFlipCandidate];
  }
}
