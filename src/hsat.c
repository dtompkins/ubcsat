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

void PickHSat();
void PickHSatW();

void AddHSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hsat","",FALSE,
    "HSAT",
    "Gent, Walsh [AAAI 93]",
    "PickHSat",
    "DefaultProcedures,Flip+VarScore,VarLastChange",
    "default","default");
  
  CreateTrigger("PickHSat",ChooseCandidate,PickHSat,"","");

  pCurAlg = CreateAlgorithm("hsat","",TRUE,
    "HSAT (weighted)",
    "Gent, Walsh [AAAI 93]",
    "PickHSatW",
    "DefaultProceduresW,Flip+VarScoreW,VarLastChange",
    "default_w","default");
  
  CreateTrigger("PickHSatW",ChooseCandidate,PickHSatW,"","");
 
}

void PickHSat() {
  
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

      /* if 2 variables are tied, and one is 'older' then choose older var */

      if ((iScore < iBestScore)||(aVarLastChange[j]<aVarLastChange[*aCandidateList])) {
        iNumCandidates=0;
        iBestScore = iScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }

  }
  
  /* select flip candidate uniformly from candidate list */
  
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
}

void PickHSatW() {
  
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
      
      /* if 2 variables are tied, and one is 'older' then choose older var */

      if ((fScore < fBestScore)||(aVarLastChange[j]<aVarLastChange[*aCandidateList])) {
        iNumCandidates=0;
        fBestScore = fScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }

  }
  
  /* select flip candidate uniformly from candidate list */
  
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];
}



