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

void PickNoveltyPlusP();
void InitLookAhead();
void CreateLookAhead();
SINT32 BestLookAheadScore(UINT32 iLookVar);

void AddNoveltyPlusP() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+p","",FALSE,
    "Novelty+p: Novelty with random walk and look-ahead",
    "Li, Wei, and Zhang [SAT 07]",
    "PickNoveltyPlusP",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange,LookAhead",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE);

  CreateTrigger("PickNoveltyPlusP",ChooseCandidate,PickNoveltyPlusP,"","");

  CreateTrigger("CreateLookAhead",CreateStateInfo,CreateLookAhead,"","");
  CreateTrigger("InitLookAhead",InitStateInfo,InitLookAhead,"","");
  CreateContainerTrigger("LookAhead","CreateLookAhead,InitLookAhead");

}

void PickNoveltyPlusP() {

  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  SINT32 iScore;
  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  LITTYPE litPick;

  SINT32 iSecondBestLookAheadScore;
  SINT32 iBestLookAheadScore;

  if (iNumFalse) {

    /* select random unsatisfied clause */

    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];

    if (RandomProb(iWp)) {

      /* with probability (iWp) uniformly choose a literal from that clause */

      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);

    } else {

      iBestScore = iNumClauses;
      iSecondBestScore = iNumClauses;
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);

      /* for each literal in the clause */

      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        /* use the cached score value for that literal */

        iScore = aVarScore[iVar];

        /* keep track of which literal was the 'youngest' */

        if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
          iYoungestVar = iVar;
        }

        /* keep track of the 'best' and the 'second best' variables,
           breaking ties by selecting the younger variables */

        if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
          iSecondBestVar = iBestVar;
          iBestVar = iVar;
          iSecondBestScore = iBestScore;
          iBestScore = iScore;
        } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
          iSecondBestVar = iVar;
          iSecondBestScore = iScore;
        }
        pLit++;
      }

      /* choose the 'best' variable by default */
      
      iFlipCandidate = iBestVar;

      /* If the best is the youngest, with probability (iNovNoise) select the 2nd best */

      if (iFlipCandidate == iYoungestVar) {
        if (RandomProb(iNovNoise)) {
          iFlipCandidate = iSecondBestVar;
          return;
        }
      } else {

        /* If the best is older than then 2nd best, just choose the best */

        if (aVarLastChange[iSecondBestVar] >= aVarLastChange[iFlipCandidate]) {
          return;
        }
      }

      /* otherwise, determine the 'look ahead' score for the 2nd best variable */

      iSecondBestLookAheadScore = aVarScore[iSecondBestVar] + BestLookAheadScore(iSecondBestVar);

      if (iSecondBestLookAheadScore > iBestScore) {
        iBestLookAheadScore = iBestScore;
      } else {

        /* if the 'look ahead' score for the 2nd variable is better than the regular score
           for the best variable, calculate the look ahead score for the best variable */

        iBestLookAheadScore = aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
      }

      /* choose the variable with the best look ahead score */

      /* Note that this BREAKS TIES by selecting the 2nd best variable -- as in the paper and in the author's code */

      if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
        iFlipCandidate = iSecondBestVar;
      }
    }
  } else {
    iFlipCandidate = 0;
  }
}


UINT32 *aIsLookAhead;
UINT32 *aLookAheadList;
SINT32 *aLookAheadScoreChange;

#define UpdateLookAhead(var,diff) {if(aIsLookAhead[var]==FALSE) {aIsLookAhead[var]=TRUE; aLookAheadList[iNumLookAhead++] = var; aLookAheadScoreChange[var] = (diff);} else {aLookAheadScoreChange[var] += (diff);}};

void CreateLookAhead() {
  aIsLookAhead = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aLookAheadList = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aLookAheadScoreChange = AllocateRAM((iNumVars+1) * sizeof(SINT32));
}

void InitLookAhead() {
  memset(aIsLookAhead,0,(iNumVars+1) * sizeof(UINT32));
}

SINT32 BestLookAheadScore(UINT32 iLookVar) {
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;
  UINT32 iNumLookAhead;
  SINT32 iScore;
  SINT32 iBestLookAheadScore;

  if (iLookVar == 0) {
    return(0);
  }

  iNumLookAhead = 0;

  /* Add all Decreasing Promising variables to the 'best lookahead' list */

  for (j=0;j<iNumDecPromVars;j++) {
    UpdateLookAhead(aDecPromVarsList[j],0);
  }

  litWasTrue = GetTrueLit(iLookVar);
  litWasFalse = GetFalseLit(iLookVar);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {

    /* for each clause the variable critically satisfied,
       the make count for other variables increases by one (score -1) */

    if (aNumTrueLit[*pClause]==1) { 
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,-1);
        pLit++;
      }
    }

    /* for each 2-satisfied clause the variable occured in,
       the break count for the other variable increases by one (score +1) */

    if (aNumTrueLit[*pClause]==2) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          if (iVar != iLookVar) {
            UpdateLookAhead(iVar,+1);
            break;
          }
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {

    /* for each clause the variable will now make true
       the make count for other variables decreases by one (score +1) */

    if (aNumTrueLit[*pClause]==0) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateLookAhead(iVar,+1);
        pLit++;
      }
    }

    /* for each clause the variable will now no make critically satisfied 
       the break count for the critical variable decreases by one (score -1) */

    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      UpdateLookAhead(iVar,-1);
    }
    pClause++;
  }

  iBestLookAheadScore = iNumClauses;

  /* for each varaible that has 'changed' and/or is a Decreasing promising varaible */

  for (j=0;j<iNumLookAhead;j++) {
    iVar = aLookAheadList[j];
    if (iVar != iLookVar) {

      /* determine the combined score for flipping both variables */
     
      iScore = aVarScore[iVar] + aLookAheadScoreChange[iVar];

      /* only consider promising variables -- to match the paper and the author's software */

      if ((j < iNumDecPromVars)||(aVarScore[iVar]>=0)) {

        /* find the best 'combined' score */

        if (iScore < iBestLookAheadScore) {
          iBestLookAheadScore = iScore;
        }
      }
    }
    aIsLookAhead[iVar] = FALSE;
  }
  
  /* only consider 'improving' look ahead scores */

  if (iBestLookAheadScore < 0) {
    return(iBestLookAheadScore);
  } else {
    return(0);
  }
}
