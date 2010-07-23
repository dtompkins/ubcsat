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

const UINT32 iInvPhi=5;               /* = 1/phi   */
const UINT32 iInvTheta=6;             /* = 1/theta */

UINT32 iLastAdaptStep;
UINT32 iLastAdaptNumFalse;
FLOAT fLastAdaptSumFalseW;

void InitAdaptNoveltyNoise();
void AdaptNoveltyNoise();
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

    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);

    /* note: this is how the original code was implemented: [wp := wp - wp * phi / 2] */
    /* it was incorrectly listed in AAAI02 paper as [wp := wp - wp * 2 * phi] */
    
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
  }
}

void AdaptNoveltyNoiseW() {

  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {

    /* if no improvement in a while, increase noise */

    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)/iInvPhi);
    iLastAdaptStep = iStep;
    fLastAdaptSumFalseW = fSumFalseW;
  
  } else if (fSumFalseW < fLastAdaptSumFalseW) {

    /* if improving, then decrease noise */
    
    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);
    
    /* note: this is how the original code was implemented: [wp := wp - wp * phi / 2] */
    /* it was incorrectly listed in the paper as [wp := wp - wp * 2 * phi] */

    iLastAdaptStep = iStep;
    fLastAdaptSumFalseW = fSumFalseW;
  }
}

