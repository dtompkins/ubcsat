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

UINT32 iTabuTenureMedian;
UINT32 iTabuTenureInterval;
UINT32 iTabuTenureLow;
UINT32 iTabuTenureHigh;

void CreateRoTS();
void InitRoTS();
void PickRoTS();

void AddRoTS() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rots","",0,
    "RoTS: Robust TABU Search",
    "Taillard [Parallel Computing 1991], based on implementation by Stuetzle",
    "PickRoTS",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-tabu","target (median) tabu tenure [default %s]","","",&iTabuTenureMedian,10);
  AddParmUInt(&pCurAlg->parmList,"-tabuinterval","interval size: percent of tabu tenure [default %s]","range of tabu tenure is: tt +/- tt * (INT/100)","",&iTabuTenureInterval,25);

  CreateTrigger("CreateRoTS",PostParameters,CreateRoTS,"","");
  CreateTrigger("InitRoTS",PreRun,InitRoTS,"","");
  CreateTrigger("PickRoTS",ChooseCandidate,PickRoTS,"CreateRoTS,InitRoTS,VarLastChange,BestFalse","");

}

void CreateRoTS() {
  iTabuTenureLow = iTabuTenureMedian - (iTabuTenureInterval * iTabuTenureMedian) / 100;
  iTabuTenureHigh = iTabuTenureMedian + (iTabuTenureInterval * iTabuTenureMedian) / 100;
}

void InitRoTS() {
  iTabuTenure = iTabuTenureMedian;
}

void PickRoTS() {
  
  UINT32 j;
  SINT32 iScore;
  UBIGINT iTabuCutoff;

  /* every N steps, choose the tabu tenure uniformly from the tabu range */

  if (iTabuTenureLow != iTabuTenureHigh) {
    if ((iStep % iNumVars)==0) {
      iTabuTenure = iTabuTenureLow + RandomInt(iTabuTenureHigh - iTabuTenureLow);
    }
  }

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

  /* check all variables */

  for (j=1;j<=iNumVars;j++) {

    /* use cached value of score */    
    
    iScore = aVarScore[j];

    if (aVarLastChange[j] >= iTabuCutoff) { 

      /* check for aspiration (early exit) */

      if ((iNumFalse + iScore) < iBestNumFalse) {
        iFlipCandidate = j;
        return;
      }

    } else if ((iStep - aVarLastChange[j]) > (iNumVars * 10)) {

      /* check for variable stagnation (early exit) */

      iFlipCandidate = j;
      return;

    } else { 

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
