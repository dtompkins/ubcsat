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

void PickRGSat();

void AddRGSat() {

  CreateAlgorithm("rgsat","",0,
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
