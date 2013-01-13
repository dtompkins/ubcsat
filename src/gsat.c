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

void PickGSatSimple();
void PickGSatWithBSL();

void AddGSat() {

  CreateAlgorithm("gsat","",0,
    "GSAT: Greedy search for SAT",
    "Selman, Levesque, Mitchell [AAAI 93]",
    "PickGSatWithBSL",
    "DefaultProcedures,Flip+TrackChanges,BestScoreList",
    "default","default");

  CreateTrigger("PickGSatWithBSL",ChooseCandidate,PickGSatWithBSL,"","");  

  CreateAlgorithm("gsat","simple",0,
    "GSAT: Greedy search for SAT (simple, but slower version)",
    "Selman, Levesque, Mitchell [AAAI 93]",
    "PickGSatSimple",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  
  CreateTrigger("PickGSatSimple",ChooseCandidate,PickGSatSimple,"","");

}


void PickGSatSimple() {
  
  UINT32 j;
  SINT32 iScore;

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;

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

#ifdef __cplusplus
}
#endif
