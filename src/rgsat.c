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

void PickRGSat();

void AddRGSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rgsat","",0,
    "RGSAT: Restarting GSAT (poor algorithm -- academic use only)",
    "Tompkins, Hoos [SAIM 04]",
    "PickRGSat",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  CreateTrigger("PickRGSat",CheckRestart,PickRGSat,"","");

}

void PickRGSat() {
  
  UINT32 j;
  SINT32 iScore;

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;

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
    bRestart = 1;
  }
}

#ifdef __cplusplus

}
#endif
