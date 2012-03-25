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
 
 UINT32 iPromInvPhi=5;               /* = 1/phi   */
 UINT32 iPromInvTheta=6; 
 
//const UINT32 iInvPhi=10;               /* = 1/phi   */
//const UINT32 iInvTheta=5;             /* = 1/theta */

UINT32 iLastAdaptStep;
UINT32 iLastAdaptNumFalse;
UINT32 iLastPromAdaptStep;
UINT32 iLastPromAdaptNumFalse;
int intNovNoise;
int intDp;

FLOAT fLastAdaptSumFalseW;
BOOL bAdaptWalkProb = FALSE;
BOOL bAdaptPromWalkProb = FALSE;
void InitAdaptNoveltyPlusNoise();
void AdaptNoveltyPlusNoise();
void InitAdaptPromNoveltyNoise();
void AdaptPromNoveltyNoise();

void AdaptNoveltyPlusNoiseW();

void InitAdaptNoveltyNoise();
void AdaptNoveltyNoise();

void AddAdaptNoveltyPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("adaptnovelty+","",FALSE,
    "Adaptive Novelty+: Novelty+ with adaptive noise",
    "Hoos [AAAI 02]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyPlusNoise,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.01);
  AddParmUInt(&pCurAlg->parmList,"-selectclause","select clause [default %s]","specifies choice to select clause","",&iSelectClause,1);

  CreateTrigger("InitAdaptNoveltyPlusNoise",PostInit,InitAdaptNoveltyPlusNoise,"","");
  CreateTrigger("AdaptNoveltyPlusNoise",PostFlip,AdaptNoveltyPlusNoise,"InitAdaptNoveltyPlusNoise","");

  CreateTrigger("InitAdaptPromNoveltyNoise",PostInit,InitAdaptPromNoveltyNoise,"","");
  CreateTrigger("AdaptPromNoveltyNoise",PostFlip,AdaptPromNoveltyNoise,"InitAdaptPromNoveltyNoise","");


  pCurAlg = CreateAlgorithm("adaptnovelty+","",TRUE,
    "Adaptive Novelty+: Novelty+ with adaptive noise (weighted)",
    "Hoos [AAAI 02]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,AdaptNoveltyPlusNoiseW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"adaptnovelty+","",FALSE,0);
  AddParmUInt(&pCurAlg->parmList,"-selectclause","select clause [default %s]","specifies choice to select clause","",&iSelectClause,1);

  CreateTrigger("AdaptNoveltyPlusNoiseW",PostFlip,AdaptNoveltyPlusNoiseW,"InitAdaptNoveltyPlusNoise","");

}
 

void AddAdaptNovelty() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("adaptnovelty","",FALSE,
    "Adaptive Novelty: Novelty with adaptive noise",
    "Hoos [AAAI 02]",
    "PickNovelty",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyNoise,VarLastChange",
    "default","default");
  AddParmUInt(&pCurAlg->parmList,"-selectclause","select clause [default %s]","specifies choice to select clause","",&iSelectClause,1);
  
  CreateTrigger("InitAdaptNoveltyNoise",PostInit,InitAdaptNoveltyNoise,"","");
  CreateTrigger("AdaptNoveltyNoise",PostFlip,AdaptNoveltyPlusNoise,"InitAdaptNoveltyNoise","");
 
}

void InitAdaptNoveltyPlusNoise() {
  iLastAdaptStep=iStep;
  iLastAdaptNumFalse=iNumFalse;
  fLastAdaptSumFalseW=fTotalWeight;
  iNovNoise = 0;
  intNovNoise = 0;
  if(bAdaptWalkProb){
  iDp = 0;
  iWp = 0;
  intDp = 0;
}
  iWpWalk=0;
}

void AdaptNoveltyPlusNoise() {
  if(iAdaptiveNoiseScheme ==1 && bPerformNovelty &&bPromisingList)
   return; 
  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {
    
    /* if no improvement in a while, increase noise */

    iNovNoise += (PROBABILITY) ((0xFFFFFFFF - iNovNoise)/iInvPhi);
    iWpWalk += (PROBABILITY) ((0xFFFFFFFF - iWpWalk)/iInvPhi);
    intNovNoise += (int) ((100 - intNovNoise)/iInvPhi); 
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
    if(bAdaptWalkProb){	 
    iWp = iNovNoise / 10;
    iDp = iNovNoise / 10;
    intDp = intNovNoise/10;
    }
    iWpWalk = iNovNoise;
    
  } else if (iNumFalse < iLastAdaptNumFalse) {

    /* if improving, then decrease noise */

    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);
    iWpWalk -= (PROBABILITY) (iWpWalk / iInvPhi / 2);
    intNovNoise -= (int) (intNovNoise /iInvPhi / 2);
    /* note: this is how the original code was implemented: [wp := wp - wp * phi / 2] */
    /* it was incorrectly listed in AAAI02 paper as [wp := wp - wp * 2 * phi] */
    
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
    if(bAdaptWalkProb){	
   iWp = iNovNoise/10;
   iDp = iNovNoise/10;
   intDp = intNovNoise/10;
   }
   iWpWalk = iNovNoise;
  }
}

void AdaptNoveltyPlusNoiseW() {

  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {

    /* if no improvement in a while, increase noise */

    iNovNoise += (PROBABILITY) ((0xFFFFFFFF - iNovNoise)/iInvPhi);
    iWpWalk += (PROBABILITY) ((0xFFFFFFFF - iWpWalk)/iInvPhi);
    iLastAdaptStep = iStep;
    fLastAdaptSumFalseW = fSumFalseW;
    iWp = iNovNoise / 10;

  
  } else if (fSumFalseW < fLastAdaptSumFalseW) {

    /* if improving, then decrease noise */
    
    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);
    iWpWalk -= (PROBABILITY) (iWpWalk / iInvPhi / 2);
    
    /* note: this is how the original code was implemented: [wp := wp - wp * phi / 2] */
    /* it was incorrectly listed in the paper as [wp := wp - wp * 2 * phi] */

    iLastAdaptStep = iStep;
    fLastAdaptSumFalseW = fSumFalseW;
    iWp = iNovNoise / 10;

  }
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

    iNovNoise += (PROBABILITY) ((0xFFFFFFFF - iNovNoise)/iInvPhi);
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


void InitAdaptPromNoveltyNoise() {
  iLastPromAdaptStep=iStep;
  iLastPromAdaptNumFalse=iNumFalse;
  //fLastAdaptSumFalseW=fTotalWeight;
  iPromNovNoise = 0;
  if(bAdaptPromWalkProb){
  iPromDp = 0;
  iPromWp = 0;
}
}

void AdaptPromNoveltyNoise() {
  if (iStep-iLastPromAdaptStep > iNumClauses/iPromInvTheta) {
 
    /* if no improvement in a while, increase noise */

    iPromNovNoise += (PROBABILITY) ((0xFFFFFFFF - iPromNovNoise)/iPromInvPhi);
    iLastPromAdaptStep = iStep;
    iLastPromAdaptNumFalse = iNumFalse;
    if(bAdaptPromWalkProb){
    iPromWp = iPromNovNoise / 10;
    iPromDp = iPromNovNoise / 10;
    }
  } else if (iNumFalse < iLastPromAdaptNumFalse) {

    /* if improving, then decrease noise */

    iPromNovNoise -= (PROBABILITY) (iPromNovNoise / iPromInvPhi / 2);

    /* note: this is how the original code was implemented: [wp := wp - wp * phi / 2] */
    /* it was incorrectly listed in AAAI02 paper as [wp := wp - wp * 2 * phi] */

    iLastPromAdaptStep = iStep;
    iLastPromAdaptNumFalse = iNumFalse;
    if(bAdaptPromWalkProb){
   iPromWp = iPromNovNoise/10;
   iPromDp = iPromNovNoise/10;
   }
  }
}

