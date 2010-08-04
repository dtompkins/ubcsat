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

void InitHybrdInfo();
void UpdateHybridInfo();

void PickHybrid1();
void InitHybrd1();

void PickHybrid2();

FLOAT fHybridGamma;
FLOAT fVW2WeightMax;

UINT32 iNumAlg1;
UINT32 iNumAlg2;
UINT32 iNumAlg3;

void AddHybrid() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hybrid","2007",FALSE,
   "Hybrid1: Switch between VW and adaptG2WSATp (Sat07 version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid1,InitHybrdInfo,UpdateHybridInfo,InitHybrid1",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");

  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid1 switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSATp~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,15);

  CreateTrigger("InitHybrdInfo",InitStateInfo,InitHybrdInfo,"","");
  CreateTrigger("UpdateHybridInfo",UpdateStateInfo,UpdateHybridInfo,"","UpdateVW2Weights");

  CreateTrigger("PickHybrid1",ChooseCandidate,PickHybrid1,"","");
  CreateTrigger("InitHybrd1",InitStateInfo,InitHybrd1,"","");


  pCurAlg = CreateAlgorithm("hybrid","2009",FALSE,
   "Hybrid2: Switch between VW and adaptG2WSAT+ (Sat09 version)",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid2,InitHybrdInfo,UpdateHybridInfo,InitVW2Auto,UpdateVW2Auto",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise,VW2Weights",
    "default","default");
  AddParmFloat(&pCurAlg->parmList,"-gamma","Hybrid2 switching criteria [default %s]","paramater to adjust selecting VW over AdaptG2WSAT+~use VW if max.vw2w > gamma * avg.vw2w","",&fHybridGamma,1.025);
  CreateTrigger("PickHybrid2",ChooseCandidate,PickHybrid2,"","");

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

  AddColumnUInt("numalg3","# of Search Steps Algorithm 3",
    "  # Search",
    "     Steps",
    "     Alg 3",
    "%10u",
    &iNumAlg3,"",ColTypeFinal);
}

void InitHybrdInfo() {
  fVW2WeightMax = FLOATZERO;
  iNumAlg1 = 0;
  iNumAlg2 = 0;
  iNumAlg3 = 0;
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
    PickVW2Auto();
    iNumAlg1++;
  } else {
    PickG2WSatP();
    iNumAlg2++;
  }
}

void InitHybrd1() {
  iMaxExpProbability=2;
  fVW2Smooth = 0.0f;
}

void PickHybrid2() {
  if (fVW2WeightMax > fHybridGamma * fVW2WeightMean) {
    PickVW2Auto();
    iNumAlg1++;
  } else {
    PickG2WSatNoveltyPlusOldest();
    iNumAlg2++;
  }
}
