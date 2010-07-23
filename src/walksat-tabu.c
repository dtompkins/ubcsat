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

void PickWalkSatTabu();
void PickWalkSatTabuW();
void PickWalkSatTabuNoNull();

void AddWalkSatTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("walksat-tabu","",FALSE,
    "WALKSAT-TABU: WalkSAT with TABU search",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickWalkSatTabu",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);

  CreateTrigger("PickWalkSatTabu",ChooseCandidate,PickWalkSatTabu,"","");


  pCurAlg = CreateAlgorithm("walksat-tabu","",TRUE,
    "WALKSAT-TABU: WalkSAT with TABU search (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickWalkSatTabuW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"walksat-tabu","",FALSE);

  CreateTrigger("PickWalkSatTabuW",ChooseCandidate,PickWalkSatTabuW,"","");

}

UINT32 iWalkSATTabuClause;

void PickWalkSatTabu()
{
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  
  UINT32 iTabuCutoff;

  iNumCandidates = 0;
  iBestScore = iNumClauses;

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

void PickWalkSatTabuW()
{
 
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  
  UINT32 iTabuCutoff;

  iNumCandidates = 0;
  fBestScore = fTotalWeight;

  /* calculation of tabu cutoff */

  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  /* select the clause according to a weighted scheme */

  iWalkSATTabuClause = PickClauseWCS();
  iClauseLen = aClauseLen[iWalkSATTabuClause];

  pLit = pClauseLits[iWalkSATTabuClause];

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values 
    
       note that in this case, score is the breakcount[] */

    fScore = FLOATZERO;
      
    pClause = pLitClause[GetNegatedLit(*pLit)];

    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        fScore += aClauseWeight[*pClause];
      }
      pClause++;
    }
    
    /* variables with breakcount (score) = 0 are never tabu */

    if ((fScore==FLOATZERO)||(aVarLastChange[iVar] < iTabuCutoff)) { 

      /* build candidate list of best vars */

      if (fScore <= fBestScore) {
        if (fScore < fBestScore) {
          iNumCandidates=0;
          fBestScore = fScore;
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

