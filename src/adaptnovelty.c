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

UINT32 iInvPhi=5;               /* = 1/phi   */
UINT32 iInvTheta=6;             /* = 1/theta */

FLOAT fAdaptPhi;
FLOAT fAdaptTheta;

UINT32 iLastAdaptStep;
UINT32 iLastAdaptNumFalse;
FLOAT fLastAdaptSumFalseW;

void InitAdaptNoveltyNoise();
void AdaptNoveltyNoise();
void AdaptNoveltyNoiseAdjust();
void AdaptNoveltyNoiseW();

void AddAdaptNoveltyPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("adaptnovelty+","",FALSE,
    "Adaptive Novelty+: Novelty+ with adaptive noise",
    "Hoos [AAAI 02]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyNoise,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);

  CreateTrigger("InitAdaptNoveltyNoise",PostInit,InitAdaptNoveltyNoise,"","");
  CreateTrigger("AdaptNoveltyNoise",PostFlip,AdaptNoveltyNoise,"InitAdaptNoveltyNoise","");

  pCurAlg = CreateAlgorithm("adaptnovelty+","params",FALSE,
    "Adaptive Novelty+ with changable adaptability parameters",
    "Hoos [AAAI 02]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyNoiseAdjust,VarLastChange",
    "default","default");

  CopyParameters(pCurAlg,"adaptnovelty+","",FALSE);
  AddParmFloat(&pCurAlg->parmList,"-phi","adjustment parameter phi [default %s]","phi determines the rate of change of noise","",&fAdaptPhi,(1.0f/5.0f));
  AddParmFloat(&pCurAlg->parmList,"-theta","adjustment parameter theta [default %s]","theta determines the stagnation detection","",&fAdaptTheta,(1.0f/6.0f));
  CreateTrigger("AdaptNoveltyNoiseAdjust",PostFlip,AdaptNoveltyNoiseAdjust,"InitAdaptNoveltyNoise","");

  pCurAlg = CreateAlgorithm("adaptnovelty+","",TRUE,
    "Adaptive Novelty+: Novelty+ with adaptive noise (weighted)",
    "Hoos [AAAI 02]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,AdaptNoveltyNoiseW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"adaptnovelty+","",FALSE);

  CreateTrigger("AdaptNoveltyNoiseW",PostFlip,AdaptNoveltyNoiseW,"InitAdaptNoveltyNoise","");
 
}

void InitAdaptNoveltyNoise() {
  iLastAdaptStep=iStep;
  iLastAdaptNumFalse=iNumFalse;
  fLastAdaptSumFalseW=fTotalWeight;
  iNovNoise = 0;
}

void AdaptNoveltyNoise() {
  
  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {
    
    /* if no improvement in a while, increase noise */

    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)/iInvPhi);

    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;

  } else if (iNumFalse < iLastAdaptNumFalse) {

    /* if improving, then decrease noise */
    /* note: this is how the original code was implemented: [wp := wp - wp * phi / 2] */
    /* it was incorrectly listed in AAAI02 paper as [wp := wp - wp * 2 * phi] */

    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);

    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  }
}

void AdaptNoveltyNoiseAdjust() {

  /* this varaint allows for different values of Phi & Theta */
  
  if (iStep-iLastAdaptStep > iNumClauses*fAdaptTheta) {
    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)*fAdaptPhi);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  } else if (iNumFalse < iLastAdaptNumFalse) {
    iNovNoise -= (PROBABILITY) (iNovNoise * fAdaptPhi / 2);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  }
}


void AdaptNoveltyNoiseW() {

  /* weighted varaint -- see regular algorithm for comments */

  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {

    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)/iInvPhi);
    iLastAdaptStep = iStep;
    fLastAdaptSumFalseW = fSumFalseW;
  
  } else if (fSumFalseW < fLastAdaptSumFalseW) {

    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);
    
    iLastAdaptStep = iStep;
    fLastAdaptSumFalseW = fSumFalseW;
  }
}

