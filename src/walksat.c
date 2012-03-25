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

void PickWalkSatSKC();
void PickWalkSatSKCW();
UINT32 PickClauseWCSPen();
PROBABILITY iWpWalk; 

void AddWalkSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("walksat","",FALSE,
    "WALKSAT: Original WalkSAT algorithm (SKC variant)",
    "Selman, Kautz, Cohen [AAAI 94]",
    "PickWalkSatSKC",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wpwalk","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWpWalk,0.50);

  CreateTrigger("PickWalkSatSKC",ChooseCandidate,PickWalkSatSKC,"","");


  pCurAlg = CreateAlgorithm("walksat","",TRUE,
    "WALKSAT: Original WalkSAT algorithm (SKC variant) (weighted)",
    "Selman, Kautz, Cohen [AAAI 94]",
    "PickWalkSatSKCW",
    "DefaultProceduresW,Flip+FalseClauseListW",
    "default_w","default");
  
  CopyParameters(pCurAlg,"walksat","",FALSE,0);

  CreateTrigger("PickWalkSatSKCW",ChooseCandidate,PickWalkSatSKCW,"","");
 
}

void PickWalkSatSKC() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  LITTYPE *pClause;
  LITTYPE litPick;
  UINT32 iNumOcc;

  iNumCandidates = 0;
  iBestScore = iNumClauses;

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
    
       note that in this case, score is the breakcount[] */

    iScore = 0;
    
    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
  
    if(!bPen)
    {  
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }
 
    }
    else{

      for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore += aClausePen[*pClause];
      }
      pClause++;
    }
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
    if (RandomProb(iWpWalk)) {
      litPick = pClauseLits[iClause][RandomInt(iClauseLen)];
      iFlipCandidate = GetVarFromLit(litPick);
      return;
    }
  }

  /* select flip candidate uniformly from candidate list */
  
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];

}



UINT32 PickClauseWCS() {

  /* this routine randomly selects a weighted clause so that
     clauses with larger weights are more likely to be selected 
     ('roulette' selection) */

  UINT32 j;
  FLOAT fRandClause;
  FLOAT fClauseSum;
  UINT32 iClause = 0;

  fRandClause = RandomFloat() * fSumFalseW;

  fClauseSum = FLOATZERO;

  for (j=0;j<iNumFalse;j++) {
    iClause = aFalseList[j];
    fClauseSum += aClauseWeight[iClause];
    if (fRandClause < fClauseSum) {
      break;
    }
  }
  return(iClause);
}

UINT32 PickClauseWCSPen() {

  /* this routine randomly selects a weighted clause so that
     clauses with larger weights are more likely to be selected
     ('roulette' selection) */

  UINT32 j;
  UINT32 iRandClause;
  UINT32 iClauseSum;
  UINT32 iClause = 0;
  iRandClause = RandomFloat() * iSumFalsePen;
  iClauseSum = 0;
  for (j=0;j<iNumFalse;j++) {
    iClause = aFalseList[j];
    iClauseSum += aClausePen[iClause];
    if (iRandClause < iClauseSum) {
      break;
    }
  }
  return(iClause);
}



void PickWalkSatSKCW() {

  /* for general comments, review the unweighted version */

  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  LITTYPE *pClause;
  LITTYPE litPick;
  UINT32 iNumOcc;

  iNumCandidates = 0;
  fBestScore = fTotalWeight;

  /* select the clause according to a weighted scheme */

  if (iNumFalse) {
    iClause = PickClauseWCS();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  for (j=0;j<iClauseLen;j++) {
    fScore = 0.0;
    
    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        fScore += aClauseWeight[*pClause];
      }
      pClause++;
    }

    if (fScore <= fBestScore) {
      if (fScore < fBestScore) {
        iNumCandidates=0;
        fBestScore = fScore;
      }
      aCandidateList[iNumCandidates++] = iVar;
    }

    pLit++;
  }

  if (fBestScore > 0) {
    if (RandomProb(iWpWalk)) {
      litPick = pClauseLits[iClause][RandomInt(iClauseLen)];
      iFlipCandidate = GetVarFromLit(litPick);
      return;
    }
  }

  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];

}

