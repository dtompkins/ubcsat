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

PROBABILITY iWp;

void PickGWSat();
void PickGWSatW();

void AddGWSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gwsat","",FALSE,
    "GWSAT: GSAT with random walk",
    "Selman, Kautz [IJCAI 93]",
    "PickGWSat",
    "DefaultProcedures,Flip+VarInFalse",
    "default","default");

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from those~that appear in unsat clauses","",&iWp,0.50);

  CreateTrigger("PickGWSat",ChooseCandidate,PickGWSat,"","");

  pCurAlg = CreateAlgorithm("gwsat","",1,
    "GWSAT: GSAT with random walk (weighted)",
    "Selman, Kautz [IJCAI 93]",
    "PickGWSatW",
    "DefaultProceduresW,VarScoreW,VarInFalse",
    "default_w","default");

  CopyParameters(pCurAlg,"gwsat","",FALSE);

  CreateTrigger("PickGWSatW",ChooseCandidate,PickGWSatW,"","");


}

void PickGWSat() {
  UINT32 j;
  SINT32 iScore;

  /* with probability (iWp) uniformly choose a variable from all
     variables that appear in false clauses */

  if (RandomProb(iWp)) {
    if (iNumVarsInFalseList) {
      iFlipCandidate = aVarInFalseList[RandomInt(iNumVarsInFalseList)];
    } else {
      iFlipCandidate = 0;
    }

  } else {

    iNumCandidates = 0;
    iBestScore = iNumClauses;

    /* check score of all variables */

    for (j=1;j<=iNumVars;j++) {

      /* Maintaing VarFalseList requires MakeCount[],
         so use makecount & breakcount to calculate score */

      iScore = aBreakCount[j] - aMakeCount[j];

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
}

void PickGWSatW() {
  UINT32 j;
  FLOAT fScore;

  /* with probability (iWp) uniformly choose a variable from all
     variables that appear in false clauses */

  if (RandomProb(iWp)) {
    if (iNumVarsInFalseList) {
      iFlipCandidate = aVarInFalseList[RandomInt(iNumVarsInFalseList)];
    } else {
      iFlipCandidate = 0;
    }
  } else {

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
}

