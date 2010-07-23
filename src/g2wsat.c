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

void PickG2WSat();
void PickG2WSatW();

void PickG2WSatNoveltyPlusOldest();
void PickG2WSatNoveltyPlusOldestW();

void PickG2WSatP();

void InitAdaptG2WSatNoise();
void AdaptG2WSatNoise();


void AddG2WSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("g2wsat","",FALSE,
   "G2WSAT: Gradient-based Greedy WalkSAT (uses Novelty++)",
    "Li, Huang  [SAT 05]",
    "PickG2WSat",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty++","",FALSE);
  CreateTrigger("PickG2WSat",ChooseCandidate,PickG2WSat,"","");


  pCurAlg = CreateAlgorithm("g2wsat","",TRUE,
    "G2WSAT: Gradient-based Greedy WalkSAT (weighted)",
    "Li, Huang  [SAT 05]",
    "PickG2WSatW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVarsW,FalseClauseList,VarLastChange",
    "default_w","default");

  CopyParameters(pCurAlg,"g2wsat","",FALSE);
  CreateTrigger("PickG2WSatW",ChooseCandidate,PickG2WSatW,"","");


  pCurAlg = CreateAlgorithm("g2wsat","novelty+oldest",FALSE,
   "G2WSAT: Gradient-based Greedy WalkSAT (uses Nov+ & oldest dec. prom var)",
    "Li, Wei, and Zhang [SAT 07]",
    "PickG2WSatNoveltyPlusOldest",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE);
  CreateTrigger("PickG2WSatNoveltyPlusOldest",ChooseCandidate,PickG2WSatNoveltyPlusOldest,"","");


  pCurAlg = CreateAlgorithm("g2wsat","novelty+oldest",TRUE,
    "G2WSAT: uses Nov+ & oldest dec. prom var) (weighted)",
    "Li, Wei, and Zhang [SAT 07]",
    "PickG2WSatNoveltyPlusOldestW",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE);

  CreateTrigger("PickG2WSatNoveltyPlusOldestW",ChooseCandidate,PickG2WSatNoveltyPlusOldestW,"","");


  pCurAlg = CreateAlgorithm("g2wsat+p","",FALSE,
   "G2WSAT+p: Gradient-based Greedy WalkSAT with look-ahead (uses Novelty+p)",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatP",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+p","",FALSE);

  CreateTrigger("PickG2WSatP",ChooseCandidate,PickG2WSatP,"","");


  pCurAlg = CreateAlgorithm("adaptg2wsat","",FALSE,
   "Adaptive G2WSat: Adaptive G2WSAT (uses Nov+, Oldest DecPromVar)",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatNoveltyPlusOldest",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,AdaptG2WSatNoise",
    "default","default");
  
  CopyParameters(pCurAlg,"g2wsat","novelty+oldest",FALSE);

  CreateTrigger("InitAdaptG2WSatNoise",PostInit,InitAdaptG2WSatNoise,"","");
  CreateTrigger("AdaptG2WSatNoise",PostFlip,AdaptG2WSatNoise,"InitAdaptG2WSatNoise","");


  pCurAlg = CreateAlgorithm("adaptg2wsat+p","",FALSE,
   "Adapt+G2WSat+p: Adaptive G2WSAT+p",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatP",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead,AdaptG2WSatNoise",
    "default","default");

  CopyParameters(pCurAlg,"g2wsat+p","",FALSE);
}


void PickG2WSat() {
 
  UINT32 j;
  UINT32 iVar;
  SINT32 iScore;

  /* If there are Decreasing Promising Variables */

  if (iNumDecPromVars > 0 ) {

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
  } else {

    /* Otherwise, just use Novelty++ */

    PickNoveltyPlusPlusVarScore();
  }
}

void PickG2WSatW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 j;
  UINT32 iVar;
  FLOAT fScore;

  if (iNumDecPromVarsW > 0 ) {
    iFlipCandidate = aDecPromVarsListW[0];
    fBestScore = aVarScoreW[iFlipCandidate];
    for (j=1;j<iNumDecPromVarsW;j++) {
      iVar = aDecPromVarsListW[j];
      fScore = aVarScoreW[iVar];
      if (fScore < fBestScore) {
        iFlipCandidate = iVar;
        fBestScore = aVarScoreW[iVar];
      } else {
        if (fScore == fBestScore) {
          if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
            iFlipCandidate = iVar;
          }
        }
      }
    }
  } else {
    PickNoveltyPlusPlusW();
  }
}

void PickG2WSatNoveltyPlusOldest() {
 
  UINT32 j;
  UINT32 iVar;

  /* If there are Decreasing Promising Variables */

  if (iNumDecPromVars > 0 ) {

    /* Find the one with that is the 'oldest' */

    iFlipCandidate = aDecPromVarsList[0];
    for (j=1;j<iNumDecPromVars;j++) {
      iVar = aDecPromVarsList[j];
      if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
        iFlipCandidate = iVar;
      }
    }
  } else {

    /* Otherwise, just use Novelty+ (not Novelty++ as with regular G2WSat */

    PickNoveltyPlusVarScore();
  }
}


void PickG2WSatNoveltyPlusOldestW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 j;
  UINT32 iVar;

  if (iNumDecPromVars > 0 ) {
    iFlipCandidate = aDecPromVarsList[0];
    for (j=1;j<iNumDecPromVars;j++) {
      iVar = aDecPromVarsList[j];
      if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
        iFlipCandidate = iVar;
      }
    }
  } else {
    PickNoveltyPlusW();
  }
}


void PickG2WSatP() {
 
  UINT32 j;

  /* If there are Decreasing Promising Variables */

  if (iNumDecPromVars > 0 ) {

    /* Find the one with that is the 'oldest' */

    iFlipCandidate = aDecPromVarsList[0];
    for (j=1;j<iNumDecPromVars;j++) {
      if (aVarLastChange[aDecPromVarsList[j]] < aVarLastChange[iFlipCandidate]) {
        iFlipCandidate = aDecPromVarsList[j];
      }
    }
  } else {

    /* Otherwise, just use Novelty+p */

    PickNoveltyPlusP();
  }
}

void InitAdaptG2WSatNoise() {
  InitAdaptNoveltyNoise();
  iWp = 0;
  fAdaptTheta = 0.2f;
  fAdaptPhi = 0.1f;
}

void AdaptG2WSatNoise() {
  AdaptNoveltyNoiseAdjust();
  iWp = iNovNoise/10;
}
