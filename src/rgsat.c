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

void PickRGSat();
void PickRGSatW();

void AddRGSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rgsat","",FALSE,
    "RGSAT: Restarting GSAT (poor algorithm -- academic use only)",
    "Tompkins, Hoos [SAIM 04]",
    "PickRGSat",
    "DefaultProcedures,Flip+VarScore",
    "default","default");

  CreateTrigger("PickRGSat",CheckRestart,PickRGSat,"","");

  pCurAlg = CreateAlgorithm("rgsat","",TRUE,
    "RGSAT: Restarting GSAT (poor algorithm -- academic use only) (weighted)",
    "Tompkins, Hoos [SAIM 04]",
    "PickRGSatW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  
  CreateTrigger("PickRGSatW",CheckRestart,PickRGSatW,"","");

}

void PickRGSat() {
  
  UINT32 j;
  SINT32 iScore;

  iNumCandidates = 0;
  iBestScore = iNumClauses;

  /* Algorithm is essentially GSAT */

  for (j=1;j<=iNumVars;j++) {
    iScore = aVarScore[j];
    if (iScore <= iBestScore) {
      if (iScore < iBestScore) {
        iNumCandidates=0;
        iBestScore = iScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }
  }

  /* If improving step possible, select flip candidate uniformly from candidate list */

  if (iBestScore < 0) {   
    if (iNumCandidates > 1) {
      iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
    } else {
      iFlipCandidate = aCandidateList[0];
    }
  } else {

    /* Otherwise, restart  */

    iFlipCandidate = 0;
    bRestart = TRUE;
  }
}


void PickRGSatW() {

  /* weighted varaint -- see regular algorithm for comments */
  
  UINT32 j;
  FLOAT fScore;

  iNumCandidates = 0;
  fBestScore = fTotalWeight;
  for (j=1;j<=iNumVars;j++) {
    fScore = aVarScoreW[j];
    if (fScore <= fBestScore) {
      if (fScore < fBestScore) {
        iNumCandidates=0;
        fBestScore = fScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }
  }
  if (fBestScore < FLOATZERO) {   
    if (iNumCandidates > 1) {
      iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
    } else {
      iFlipCandidate = aCandidateList[0];
    }
  } else {
    iFlipCandidate = 0;
    bRestart = TRUE;
  }
}


