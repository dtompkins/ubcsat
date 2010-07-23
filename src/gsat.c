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

void PickGSatSimple();
void PickGSatWithBSL();
void PickGSatW();

void AddGSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gsat","",FALSE,
    "GSAT: Greedy search for SAT",
    "Selman, Levesque, Mitchell [AAAI 93]",
    "PickGSatWithBSL",
    "DefaultProcedures,Flip+TrackChanges,BestScoreList",
    "default","default");

  CreateTrigger("PickGSatWithBSL",ChooseCandidate,PickGSatWithBSL,"","");  

  pCurAlg = CreateAlgorithm("gsat","simple",FALSE,
    "GSAT: Greedy search for SAT (simple, but slower version)",
    "Selman, Levesque, Mitchell [AAAI 93]",
    "PickGSatSimple",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  
  CreateTrigger("PickGSatSimple",ChooseCandidate,PickGSatSimple,"","");

  pCurAlg = CreateAlgorithm("gsat","",TRUE,
    "GSAT: Greedy search for SAT (weighted)",
    "Selman, Levesque, Mitchell [AAAI 93]",
    "PickGSatW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  
  CreateTrigger("PickGSatW",ChooseCandidate,PickGSatW,"","");

}


void PickGSatSimple() {
  
  UINT32 j;
  SINT32 iScore;

  iNumCandidates = 0;
  iBestScore = iNumClauses;

  /* check score of all variables */

  for (j=1;j<=iNumVars;j++) {

    /* use cached value of score */

    iScore = aVarScore[j];

    /* build candidate list of best vars */

    if (iScore <= iBestScore) {
      if (iScore < iBestScore) {
        iNumCandidates=0;
        iBestScore = iScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }
  }

  /* select flip candidate uniformly from candidate list */

  if (iNumCandidates > 1) {
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  } else {
    iFlipCandidate = aCandidateList[0];
  }
}

void PickGSatWithBSL() {

  /* this is a fancier 'prototype' gsat implementation,
      which caches a list of the 'best' variables at all times */

  if (iNumBestScoreList > 1) {
    iFlipCandidate = aBestScoreList[RandomInt(iNumBestScoreList)];
  } else {
    iFlipCandidate = aBestScoreList[0];
  }
}


void PickGSatW() {
  
  UINT32 j;
  FLOAT fScore;

  iNumCandidates = 0;
  fBestScore = fTotalWeight;

  /* check score of all variables */

  for (j=1;j<=iNumVars;j++) {
    
    /* use cached value of weighted score */

    fScore = aVarScoreW[j];

    /* build candidate list of best vars */

    if (fScore <= fBestScore) {
      if (fScore < fBestScore) {
        iNumCandidates=0;
        fBestScore = fScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }
  }
  
  /* select flip candidate uniformly from candidate list */
  
  if (iNumCandidates > 1) {
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  } else {
    iFlipCandidate = aCandidateList[0];
  }
}
