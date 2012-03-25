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

void PickWalkSatTabu();
void PickWalkSatTabuNoNull();

void AddWalkSatTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("walksat-tabu","",0,
    "WALKSAT-TABU: WalkSAT with TABU search",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickWalkSatTabu",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);

  CreateTrigger("PickWalkSatTabu",ChooseCandidate,PickWalkSatTabu,"","");

}

UINT32 iWalkSATTabuClause;

void PickWalkSatTabu() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  
  UBIGINT iTabuCutoff;

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;

  /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  /* select an unsatisfied clause uniformly at random */

  iWalkSATTabuClause = aFalseList[RandomInt(iNumFalse)];
  iClauseLen = aClauseLen[iWalkSATTabuClause];

  pLit = pClauseLits[iWalkSATTabuClause];

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values 
    
       note that in this case, score is the breakcount[] */

    iScore = 0;

    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }

    /* variables with breakcount (score) = 0 are never tabu */

    if ((iScore==0)||(aVarLastChange[iVar] < iTabuCutoff)) { 

      /* build candidate list of best vars */

      if (iScore <= iBestScore) {
        if (iScore < iBestScore) {
          iNumCandidates=0;
          iBestScore = iScore;
        }
        aCandidateList[iNumCandidates++] = iVar;
      }
    }
    pLit++;
  }

  /* perform a null flip if no candidates exist */

  if (iNumCandidates == 0) {
    iFlipCandidate = 0;
    return;
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
