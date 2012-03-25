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

PROBABILITY iWp;

void PickGWSat();

void AddGWSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gwsat","",0,
    "GWSAT: GSAT with random walk",
    "Selman, Kautz [IJCAI 93]",
    "PickGWSat",
    "DefaultProcedures,Flip+VarInFalse",
    "default","default");

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from those~that appear in unsat clauses","",&iWp,0.50);

  CreateTrigger("PickGWSat",ChooseCandidate,PickGWSat,"","");

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
    iBestScore = (SINT32) iNumClauses;

    /* check score of all variables */

    for (j=1;j<=iNumVars;j++) {

      /* Maintaing VarFalseList requires MakeCount[],
         so use makecount & breakcount to calculate score */

      iScore = (SINT32) aBreakCount[j] - (SINT32) aMakeCount[j];

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

#ifdef __cplusplus
}
#endif

