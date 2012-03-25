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

void PickWalkSatSKC();

void AddWalkSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("walksat","",0,
    "WALKSAT: Original WalkSAT algorithm (SKC variant)",
    "Selman, Kautz, Cohen [AAAI 94]",
    "PickWalkSatSKC",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.50);

  CreateTrigger("PickWalkSatSKC",ChooseCandidate,PickWalkSatSKC,"","");
   
}

void PickWalkSatSKC() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 *pClause;
  LITTYPE litPick;
  UINT32 iNumOcc;

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values 
    
       note that in this case, score is just the breakcount[] */

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

    /* build candidate list of best vars */

    if (iScore <= iBestScore) {
      if (iScore < iBestScore) {
        iNumCandidates=0;
        iBestScore = iScore;
      }
      aCandidateList[iNumCandidates++] = iVar;
    }

    pLit++;
  }

  /* if the best step is a worsening step, then with
     probability (iWp) randomly choose the literal to flip */

  if (iBestScore > 0) {
    if (RandomProb(iWp)) {
      litPick = pClauseLits[iClause][RandomInt(iClauseLen)];
      iFlipCandidate = GetVarFromLit(litPick);
      return;
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
