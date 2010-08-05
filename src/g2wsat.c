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

UINT32 iG2WsatSubAlgID;
UINT32 iG2WsatPromSelectID;

void AddG2WSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("g2wsat","",FALSE,
    "G2WSAT: Gradient-based Greedy WalkSAT (generalized)",
    "Li, Huang  [SAT 05] and Li, Wei, Zhang [SAT 07]",
    "PickG2WSatGeneral,ConfigureG2WSatGeneral",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmProbability(&pCurAlg->parmList,"-wpdp","walk / diversification probability [default %s]","with probability PR, perform diversification~according to -subalg setting","",&iNovWpDp,0.05);

  AddParmUInt(&pCurAlg->parmList,"-sub","G2WSAT sub-algortihm [default %s]","0: Novelty+~1:Novelty++~2:Novelty+p~3:Novelty++p~4:Novelty++0~5:Novelty++1~6:Novelty++2","",&iG2WsatSubAlgID,1);
  AddParmBool(&pCurAlg->parmList,"-psel","decreasing promising variable selection [default %s]","0: best~1:oldest~2:random","",&iG2WsatPromSelectID,0);

  CreateTrigger("PickG2WSatGeneral",ChooseCandidate,PickG2WSatGeneral,"","");
  CreateTrigger("ConfigureG2WSatGeneral",PostParameters,ConfigureG2WSatGeneral,"","");

  pCurAlg = CreateAlgorithm("adaptg2wsat","",FALSE,
    "Adaptive G2WSat (generalized)",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatGeneral,ConfigureG2WSatGeneral,InitAdaptG2WSatNoise,AdaptG2WSatNoise",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");

  AddParmUInt(&pCurAlg->parmList,"-sub","G2WSAT sub-algortihm [default %s]","0: Novelty+~1:Novelty++~2:Novelty+p~3:Novelty++p~4:Novelty++0~5:Novelty++1~6:Novelty++2","",&iG2WsatSubAlgID,0);
  AddParmBool(&pCurAlg->parmList,"-psel","decreasing promising variable selection [default %s]","0: best~1:oldest~2:random","",&iG2WsatPromSelectID,0);

  CreateTrigger("InitAdaptG2WSatNoise",PostInit,InitAdaptG2WSatNoise,"","");
  CreateTrigger("AdaptG2WSatNoise",PostFlip,AdaptG2WSatNoise,"InitAdaptG2WSatNoise","");

  //todo... add theta & phi
}

void ConfigureG2WSatGeneral() {
  switch (iG2WsatSubAlgID) {
    case 0: // Novelty+
      fxnG2WsatNovelty = PickNoveltyPlus;
      break;
    case 1: // Novelty++
      fxnG2WsatNovelty = PickNoveltyPlusPlus;
      break;
    case 2: // Novelty+p
      fxnG2WsatNovelty = PickNoveltyPlusP;
      ActivateTriggers("LookAhead");
      break;
    case 3: // Novelty++p
      fxnG2WsatNovelty = PickNoveltyPlusPlusP;
      ActivateTriggers("LookAhead");
      break;
    case 4:
    case 5:
    case 6:
      ReportPrint1(pRepErr,"Not Supported Yet: -sub %lu\n",iG2WsatSubAlgID);
      AbnormalExit();
      exit(1);
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -sub %lu\n",iG2WsatSubAlgID);
      AbnormalExit();
      exit(1);
  }
  if (iG2WsatPromSelectID > 2) {
      ReportPrint1(pRepErr,"Unexpected Error: unknown -psel %lu\n",iG2WsatPromSelectID);
      AbnormalExit();
      exit(1);
  }
}

void PickG2WSatGeneral() {
 
  UINT32 j;
  UINT32 iVar;
  SINT32 iScore;

  /* If there are Decreasing Promising Variables */

  if (iNumDecPromVars > 0 ) {
    if (iG2WsatPromSelectID==0) { //best
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
    } else if (iG2WsatPromSelectID==1) { //oldest
      iFlipCandidate = aDecPromVarsList[0];
      for (j=1;j<iNumDecPromVars;j++) {
        iVar = aDecPromVarsList[j];
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
      }
    } else { // random or first ???
      iFlipCandidate = aDecPromVarsList[0];
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

