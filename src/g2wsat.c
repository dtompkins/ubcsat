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

void PickG2WSatGeneral();
void ConfigureG2WSatGeneral();

void InitAdaptG2WSatNoise();
void AdaptG2WSatNoise();

FXNPTR fxnG2WsatNovelty;

BOOL bG2WsatNovPlusPlus;
BOOL bG2WsatNovLookAhead;
BOOL bG2WsatSelectOldest;

void AddG2WSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("g2wsat","",FALSE,
   "G2WSAT: Gradient-based Greedy WalkSAT",
    "Li, Huang  [SAT 05] and Li, Wei, Zhang [SAT 07] (-lookahead)",
    "PickG2WSatGeneral,ConfigureG2WSatGeneral",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmProbability(&pCurAlg->parmList,"-wpdp","walk / diversification probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.05);

  AddParmBool(&pCurAlg->parmList,"-nov++","use Novelty++ instead of Novelty+","if true, use Novelty++~if false, use Novelty+","",&bG2WsatNovPlusPlus,TRUE);
  AddParmBool(&pCurAlg->parmList,"-lookahead","use lookahead (Novelty+p or Novelty++p)","if true, use Novelty+p or Novelty++p (see -nov++)~if false, use Novelty+ or Novelty++","",&bG2WsatNovLookAhead,FALSE);
  AddParmBool(&pCurAlg->parmList,"-oldest","select oldest or best promising variable","if true, select oldest promising variable~if false, select best promising variable","",&bG2WsatSelectOldest,FALSE);

  CreateTrigger("PickG2WSatGeneral",ChooseCandidate,PickG2WSatGeneral,"","");
  CreateTrigger("ConfigureG2WSatGeneral",PostParameters,ConfigureG2WSatGeneral,"","");


  pCurAlg = CreateAlgorithm("adaptg2wsat","",FALSE,
   "Adaptive G2WSat",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatGeneral,ConfigureG2WSatGeneral,InitAdaptG2WSatNoise,AdaptG2WSatNoise",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");

  AddParmBool(&pCurAlg->parmList,"-nov++","use Novelty++ instead of Novelty+","if true, use Novelty++~if false, use Novelty+","",&bG2WsatNovPlusPlus,FALSE);
  AddParmBool(&pCurAlg->parmList,"-lookahead","use lookahead (Novelty+p or Novelty++p)","if true, use Novelty+p or Novelty++p (see -nov++)~if false, use Novelty+ or Novelty++","",&bG2WsatNovLookAhead,FALSE);
  AddParmBool(&pCurAlg->parmList,"-oldest","select oldest or best promising variable","if true, select oldest promising variable~if false, select best promising variable","",&bG2WsatSelectOldest,FALSE);

  CreateTrigger("InitAdaptG2WSatNoise",PostInit,InitAdaptG2WSatNoise,"","");
  CreateTrigger("AdaptG2WSatNoise",PostFlip,AdaptG2WSatNoise,"InitAdaptG2WSatNoise","");
}

void ConfigureG2WSatGeneral() {
  if (bG2WsatNovLookAhead) {
    ActivateTriggers("LookAhead");
    if (bG2WsatNovPlusPlus) {
      fxnG2WsatNovelty = PickNoveltyPlusPlusP;
    } else {
      fxnG2WsatNovelty = PickNoveltyPlusP;
    }
  } else {
    if (bG2WsatNovPlusPlus) {
      fxnG2WsatNovelty = PickNoveltyPlusPlus;
    } else {
      fxnG2WsatNovelty = PickNoveltyPlus;
    }
  }
}

void PickG2WSatGeneral() {
 
  UINT32 j;
  UINT32 iVar;
  SINT32 iScore;

  /* If there are Decreasing Promising Variables */

  if (iNumDecPromVars > 0 ) {

    if (bG2WsatSelectOldest) {
      iFlipCandidate = aDecPromVarsList[0];
      for (j=1;j<iNumDecPromVars;j++) {
        iVar = aDecPromVarsList[j];
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
      }
    } else {

      /* Find the one with the 'best' score */

      iFlipCandidate = aDecPromVarsList[0];
      iBestScore = aVarScore[iFlipCandidate];        
      for (j=1;j<iNumDecPromVars;j++) {
        iVar = aDecPromVarsList[j];
        iScore = aVarScore[iVar];
        if (iScore < iBestScore) {
          iFlipCandidate = iVar;
          iBestScore = iScore;
        } else {
          /* If there is a tie, prefer the 'oldest' variable */
          if (iScore == iBestScore) {
            if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
              iFlipCandidate = iVar;
            }
          }
        }
      }
    }
  } else {
    fxnG2WsatNovelty();
  }
}

void InitAdaptG2WSatNoise() {
  InitAdaptNoveltyNoise();
  iNovWpDp = 0;
  fAdaptTheta = 0.2f;
  fAdaptPhi = 0.1f;
}

void AdaptG2WSatNoise() {
  AdaptNoveltyNoiseAdjust();
  iNovWpDp = iNovNoise/10;
}

#ifdef __cplusplus
}
#endif

