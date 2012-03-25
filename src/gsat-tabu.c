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

UINT32 iTabuTenure;

void PickGSatTabu();

void AddGSatTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gsat-tabu","",0,
    "GSAT-TABU: GSAT with Tabu search",
    "Mazure, Sais, Gregoire [AAAI 97]",
    "PickGSatTabu",
    "DefaultProcedures,Flip+VarScore,VarLastChange",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);

  CreateTrigger("PickGSatTabu",ChooseCandidate,PickGSatTabu,"","");

}

void PickGSatTabu() {
  
  UINT32 j;
  SINT32 iScore;
  UBIGINT iTabuCutoff;

  /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;

  for (j=1;j<=iNumVars;j++) {

    /* check score of all non-tabu variables */

    if (aVarLastChange[j] < iTabuCutoff) { 

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
  }

  /* select flip candidate uniformly from candidate list */

  if (iNumCandidates > 1) {
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  } else {
    iFlipCandidate = aCandidateList[0];
  }
}

#ifdef __cplusplus

}
#endif
