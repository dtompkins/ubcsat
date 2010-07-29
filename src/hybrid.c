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
void InitHybrd1Info();
void UpdateHybrid1Info();

FLOAT fHybridGamma;
FLOAT fVW2WeightMax;

UINT32 iNumAlg1;
UINT32 iNumAlg2;

void AddHybrid() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hybrid1","",FALSE,
   "Hybrid1: Switch between VW and adaptG2WSATp",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid1,InitHybrd1Info,UpdateHybrid1Info",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");

  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid1 switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSATp~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,10);
  
  CreateTrigger("PickHybrid1",ChooseCandidate,PickHybrid1,"","");
  CreateTrigger("InitHybrd1Info",InitStateInfo,InitHybrd1Info,"","");
  CreateTrigger("UpdateHybrid1Info",UpdateStateInfo,UpdateHybrid1Info,"","UpdateVW2Weights");

  AddColumnUInt("numalg1","# of Search Steps Algorithm 1",
    "  # Search",
    "     Steps",
    "     Alg 1",
    "%10u",
    &iNumAlg1,"",ColTypeFinal);

  AddColumnUInt("numalg2","# of Search Steps Algorithm 2",
    "  # Search",
    "     Steps",
    "     Alg 2",
    "%10u",
    &iNumAlg2,"",ColTypeFinal);
}

void PickHybrid1() {
  if (fVW2WeightMax > fHybridGamma * fVW2WeightMean) {
    PickVW2Auto();
    iNumAlg1++;
  } else {
    PickG2WSatP();
    iNumAlg2++;
  }
}

void InitHybrd1Info() {
  fVW2WeightMax = FLOATZERO;
  iMaxExpProbability=2;
  fVW2Smooth = 0.0f;
  iNumAlg1 = 0;
  iNumAlg2 = 0;
}

void UpdateHybrid1Info() {
  FLOAT fPrevWeight = aVW2Weights[iFlipCandidate];
  //aVW2Weights[iFlipCandidate] = (1.0f - fVW2Smooth) * (aVW2Weights[iFlipCandidate] + 1.0f) + (fVW2Smooth * (FLOAT) iStep);
  aVW2Weights[iFlipCandidate] = aVW2Weights[iFlipCandidate] + 1.0f;
  fVW2WeightMean += (aVW2Weights[iFlipCandidate] - fPrevWeight) / iNumVars;
  if (aVW2Weights[iFlipCandidate] > fVW2WeightMax) {
    fVW2WeightMax = aVW2Weights[iFlipCandidate];
  }
}
