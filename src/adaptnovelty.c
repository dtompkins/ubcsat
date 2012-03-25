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

UINT32 iInvPhi=5;               /* = 1/phi   */
UINT32 iInvTheta=6;             /* = 1/theta */

FLOAT fAdaptPhi;
FLOAT fAdaptTheta;

UBIGINT iLastAdaptStep;
UINT32 iLastAdaptNumFalse;
UBIGINT iLastAdaptSumFalseWeight;

void InitAdaptNoveltyNoise();
void AdaptNoveltyNoise();
void AdaptNoveltyNoiseAdjust();

void AddAdaptNoveltyPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("adaptnovelty+","",0,
    "Adaptive Novelty+: Novelty+ with adaptive noise",
    "Hoos [AAAI 02]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyNoise,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.01);

  CreateTrigger("InitAdaptNoveltyNoise",PostInit,InitAdaptNoveltyNoise,"","");
  CreateTrigger("AdaptNoveltyNoise",PostFlip,AdaptNoveltyNoise,"InitAdaptNoveltyNoise","");

  pCurAlg = CreateAlgorithm("adaptnovelty+","params",0,
    "Adaptive Novelty+ with changable adaptability parameters",
    "Hoos [AAAI 02]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyNoiseAdjust,VarLastChange",
    "default","default");

  CopyParameters(pCurAlg,"adaptnovelty+","",0);
  AddParmFloat(&pCurAlg->parmList,"-phi","adjustment parameter phi [default %s]","phi determines the rate of change of noise","",&fAdaptPhi,(1.0f/5.0f));
  AddParmFloat(&pCurAlg->parmList,"-theta","adjustment parameter theta [default %s]","theta determines the stagnation detection","",&fAdaptTheta,(1.0f/6.0f));
  CreateTrigger("AdaptNoveltyNoiseAdjust",PostFlip,AdaptNoveltyNoiseAdjust,"InitAdaptNoveltyNoise","");

}

void InitAdaptNoveltyNoise() {
  iLastAdaptStep=iStep;
  iLastAdaptNumFalse=iNumFalse;
  iLastAdaptSumFalseWeight = UBIGINTMAX;
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


#ifdef __cplusplus

}
#endif
