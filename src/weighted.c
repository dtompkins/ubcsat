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

UINT32 PickClauseWCS() {

  /* this routine randomly selects a weighted clause so that
     clauses with larger weights are more likely to be selected 
     ('roulette' selection) */

  UINT32 j;
  FLOAT fRandClause;
  FLOAT fClauseSum;
  UINT32 iClause = 0;

  fRandClause = RandomFloat() * iSumFalseWeight;

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

void PickNoveltyW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 i;
  UINT32 j;
  SBIGINT iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  UINT32 iYoungestVar;
  SBIGINT iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScoreWeight = SBIGINTMAX;
  iSecondBestScore = SBIGINTMAX;

  /* select the clause according to a weighted scheme */

  if (iNumFalse) {
    iClause = PickClauseWCS();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }
  pLit = pClauseLits[iClause];
  iYoungestVar = GetVarFromLit(*pLit);
  for (j=0;j<iClauseLen;j++) {
    iScore = 0;
    iVar = GetVarFromLit(*pLit);
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore -= aClauseWeight[*pClause];
      }
      pClause++;
    }
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore += aClauseWeight[*pClause];
      }
      pClause++;
    }
    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
      iYoungestVar = iVar;
    }
    if ((iScore < iBestScoreWeight) || ((iScore == iBestScoreWeight) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScoreWeight;
      iBestScoreWeight = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }
    pLit++;
  }
  iFlipCandidate = iBestVar;
  if (iFlipCandidate != iYoungestVar) {
    return;
  }
  if (RandomProb(iNovNoise)) {
    iFlipCandidate = iSecondBestVar;
  }
}

void PickNoveltyPlusW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  
  if (RandomProb(iNovWpDp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyW();
  }
}

void PickNoveltyPlusPlusW() {

  /* weighted varaint -- see regular algorithm for comments */
 
  UINT32 j;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;

  if (RandomProb(iNovWpDp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iFlipCandidate = GetVarFromLit(*pLit);
      pLit++;
      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyW();
  }
}

void AdaptNoveltyNoiseW() {

  /* weighted varaint -- see regular algorithm for comments */

  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {
    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)/iInvPhi);
    iLastAdaptStep = iStep;
    iLastAdaptSumFalseWeight = iSumFalseWeight;
  } else if (iSumFalseWeight < iLastAdaptSumFalseWeight) {
    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);
    iLastAdaptStep = iStep;
    iLastAdaptSumFalseWeight = iSumFalseWeight;
  }
}

void AdaptNoveltyNoiseAdjustW() {

  /* this varaint allows for different values of Phi & Theta */
  
  if (iStep-iLastAdaptStep > iNumClauses*fAdaptTheta) {
    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)*fAdaptPhi);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
    iLastAdaptSumFalseWeight = iSumFalseWeight;
  } else if (iSumFalseWeight < iLastAdaptSumFalseWeight) {
    iNovNoise -= (PROBABILITY) (iNovNoise * fAdaptPhi / 2);
    iLastAdaptStep = iStep;
    iLastAdaptSumFalseWeight = iSumFalseWeight;
  }
}

void AdaptG2WSatNoiseW() {
  AdaptNoveltyNoiseAdjustW();
  iNovWpDp = iNovNoise/10;
}

void ConfigureG2WSatGeneralW() {
  switch (iG2WsatPromisingSelectID) {
    case 1: // Oldest
      fxnG2WsatPromisingSelect = PromisingSelectOldest;
      break;
    case 0: // Best
      //fxnG2WsatPromisingSelect = PromisingSelectBestW;
    case 2: // Longest
      // fxnG2WsatPromisingSelect = PromisingSelectOldest;
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -prom %"P32"\n",iG2WsatPromisingSelectID);
      AbnormalExit();
      exit(1);
  }
  switch (iG2WsatDiversificationSelectID) {
    case 0: // Random
      fxnG2WsatDiversificationSelect = DiversificationRandom;
      break;
    case 1: // Oldest
      fxnG2WsatDiversificationSelect = DiversificationOldest;
      break;
    case 2: // MinusOne
      fxnG2WsatDiversificationSelect = DiversificationMinusOne;
      break;
    case 3: // MinusTwo
      // fxnG2WsatDiversificationSelect = DiversificationMinusTwo;
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -div %"P32"\n",iG2WsatDiversificationSelectID);
      AbnormalExit();
      exit(1);
  }
  switch (iG2WsatBaseAlgSelectID) {
    case 0: // Novelty
      fxnG2WsatBaseAlgSelect = PickNoveltyW;
      break;
    case 1: // Novelty+p
      //fxnG2WsatBaseAlgSelect = PickNoveltyPW;
    case 2: // Top2
      //fxnG2WsatBaseAlgSelect = Top2W;
      break;
    default:
      ReportPrint1(pRepErr,"Unexpected Error: unknown -base %"P32"\n",iG2WsatBaseAlgSelectID);
      AbnormalExit();
      exit(1);
  }
}

void PickG2WSatGeneralW() {
  iFlipCandidate = 0;
  if (iNumFalse) {
    if (iNumDecPromVarsW > 0 ) {
      fxnG2WsatPromisingSelect();
    } else {
      if (RandomProb(iNovWpDp)) {
        fxnG2WsatDiversificationSelect();
      } else {
        fxnG2WsatBaseAlgSelect();
      }
    }
  }
}


void PickGSatW() {
  
  UINT32 j;
  SBIGINT iScore;

  iNumCandidates = 0;
  iBestScoreWeight = SBIGINTMAX;

  /* check score of all variables */

  for (j=1;j<=iNumVars;j++) {
    
    /* use cached value of weighted score */

    iScore = aVarScoreWeight[j];

    /* build candidate list of best vars */

    if (iScore <= iBestScoreWeight) {
      if (iScore < iBestScoreWeight) {
        iNumCandidates=0;
        iBestScoreWeight = iScore;
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

void PickGSatTabuW() {
  
  UINT32 j;
  SBIGINT iScore;
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
  iBestScoreWeight = SBIGINTMAX;

  for (j=1;j<=iNumVars;j++) {
    
    /* check score of all non-tabu variables */

    if (aVarLastChange[j] < iTabuCutoff) { 
      
      /* use cached value of weighted score */

      iScore = aVarScoreWeight[j];

      /* build candidate list of best vars */

      if (iScore <= iBestScoreWeight) {
        if (iScore < iBestScoreWeight) {
          iNumCandidates=0;
          iBestScoreWeight = iScore;
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

void PickGWSatW() {
  UINT32 j;
  SBIGINT iScore;

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
    iBestScoreWeight = SBIGINTMAX;

    /* check score of all variables */

    for (j=1;j<=iNumVars;j++) {
      
      /* use cached value of weighted score */

      iScore = aVarScoreWeight[j];

      /* build candidate list of best vars */

      if (iScore <= iBestScoreWeight) {
        if (iScore < iBestScoreWeight) {
          iNumCandidates=0;
          iBestScoreWeight = iScore;
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

void PickHSatW() {
  
  UINT32 j;
  SBIGINT iScore;

  iNumCandidates = 0;
  iBestScoreWeight = SBIGINTMAX;

  /* check score of all variables */

  for (j=1;j<=iNumVars;j++) {

    /* use cached value of weighted score */

    iScore = aVarScoreWeight[j];

    /* build candidate list of best vars */

    if (iScore <= iBestScoreWeight) {
      
      /* if 2 variables are tied, and one is 'older' then choose older var */

      if ((iScore < iBestScoreWeight)||(aVarLastChange[j]<aVarLastChange[*aCandidateList])) {
        iNumCandidates=0;
        iBestScoreWeight = iScore;
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

void PickHWSatW() {
  UINT32 iClause;
  LITTYPE litPick;

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      litPick = pClauseLits[iClause][RandomInt(aClauseLen[iClause])];
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {
    
     /* otherwise, perform a regular HWSAT step */

     PickHSatW();
  }
}

void PickRGSatW() {

  /* weighted varaint -- see regular algorithm for comments */
  
  UINT32 j;
  SBIGINT iScore;

  iNumCandidates = 0;
  iBestScoreWeight = SBIGINTMAX;
  for (j=1;j<=iNumVars;j++) {
    iScore = aVarScoreWeight[j];
    if (iScore <= iBestScoreWeight) {
      if (iScore < iBestScoreWeight) {
        iNumCandidates=0;
        iBestScoreWeight = iScore;
      }
      aCandidateList[iNumCandidates++] = j;
    }
  }
  if (iBestScoreWeight < FLOATZERO) {
    if (iNumCandidates > 1) {
      iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
    } else {
      iFlipCandidate = aCandidateList[0];
    }
  } else {
    iFlipCandidate = 0;
    bRestart = 1;
  }
}

void PickRoTSW() {

  /* weighted varaint -- see regular algorithm for comments */
  
  UINT32 j;
  SBIGINT iScore;
  UBIGINT iTabuCutoff;

  if (iTabuTenureLow != iTabuTenureHigh) {
    if ((iStep % iNumVars)==0) {
      iTabuTenure = iTabuTenureLow + RandomInt(iTabuTenureHigh - iTabuTenureLow);
    }
  }
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }
  iNumCandidates = 0;
  iBestScoreWeight = SBIGINTMAX;
  for (j=1;j<=iNumVars;j++) {
    iScore = aVarScoreWeight[j];
    if (aVarLastChange[j] >= iTabuCutoff) { 
      if ((iSumFalseWeight + iScore) < iBestSumFalseWeight) {
        iFlipCandidate = j;
        return;
      }
    } else if ((iStep - aVarLastChange[j]) > (iNumVars * 10)) {
      iFlipCandidate = j;
      return;
    } else { 
      if (iScore <= iBestScoreWeight) {
        if (iScore < iBestScoreWeight) {
          iNumCandidates=0;
          iBestScoreWeight = iScore;
        }
        aCandidateList[iNumCandidates++] = j;
      }
    }
  }
  if (iNumCandidates > 1) {
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  } else {
    iFlipCandidate = aCandidateList[0];
  }
}

void SAMDUpdateVarLastChangeW() {
  if (iBestScoreWeight >= FLOATZERO) {
    UpdateVarLastChange();
  }
}

void SmoothSAPSWSmooth() {
  
  UINT32 j;
  UINT32 k;
  FLOAT fOld;
  FLOAT fDiff;
  LITTYPE *pLit;

  fTotalPenaltyFL = FLOATZERO;
 
  for(j=0;j<iNumClauses;j++) {

  /* smooth penalties back towards original clause weights */

    fOld = aClausePenaltyFL[j];
    aClausePenaltyFL[j] = aClausePenaltyFL[j] * fRho + aClauseWeight[j] * (1-fRho);
    fDiff = aClausePenaltyFL[j] - fOld;

    if (aNumTrueLit[j]==0) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        aMakePenaltyFL[GetVarFromLit(*pLit)] += fDiff;
        pLit++;
      }
    }
    if (aNumTrueLit[j]==1) {
      aBreakPenaltyFL[aCritSat[j]] += fDiff;
    }
    fTotalPenaltyFL += aClausePenaltyFL[j];
  }

}

void PostFlipSAPSWSmooth() {
  if (iFlipCandidate) {
    return;
  }

  if (RandomProb(iPs)) {
    SmoothSAPSWSmooth();
  }

  /* no call to AdjustPenalties() for WSmooth variant*/
  
  ScaleSAPS();

}

void PickWalkSatSKCW() {

  /* weighted varaint -- see regular algorithm for comments */

  UINT32 i;
  UINT32 j;
  SBIGINT iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  UINT32 *pClause;
  LITTYPE litPick;
  UINT32 iNumOcc;

  iNumCandidates = 0;
  iBestScoreWeight = SBIGINTMAX;

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
    iScore = 0;
    iVar = GetVarFromLit(*pLit);
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore += aClauseWeight[*pClause];
      }
      pClause++;
    }
    if (iScore <= iBestScoreWeight) {
      if (iScore < iBestScoreWeight) {
        iNumCandidates=0;
        iBestScoreWeight = iScore;
      }
      aCandidateList[iNumCandidates++] = iVar;
    }
    pLit++;
  }
  if (iBestScoreWeight > FLOATZERO) {
    if (RandomProb(iWp)) {
      litPick = pClauseLits[iClause][RandomInt(iClauseLen)];
      iFlipCandidate = GetVarFromLit(litPick);
      return;
    }
  }
  if (iNumCandidates > 1) {
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  } else {
    iFlipCandidate = aCandidateList[0];
  }
}

void PickWalkSatTabuW() {
 
  UINT32 i;
  UINT32 j;
  SBIGINT iScore;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;
  
  UBIGINT iTabuCutoff;

  iNumCandidates = 0;
  iBestScoreWeight = SBIGINTMAX;

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

    iScore = 0;
      
    pClause = pLitClause[GetNegatedLit(*pLit)];

    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore += aClauseWeight[*pClause];
      }
      pClause++;
    }
    
    /* variables with breakcount (score) = 0 are never tabu */

    if ((iScore==FLOATZERO)||(aVarLastChange[iVar] < iTabuCutoff)) {

      /* build candidate list of best vars */

      if (iScore <= iBestScoreWeight) {
        if (iScore < iBestScoreWeight) {
          iNumCandidates=0;
          iBestScoreWeight = iScore;
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




void AddWeighted() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("adaptnovelty+","",1,
    "Adaptive Novelty+: Novelty+ with adaptive noise (weighted)",
    "Hoos [AAAI 02]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,AdaptNoveltyNoiseW,VarLastChange",
    "default_w","default");
  CopyParameters(pCurAlg,"adaptnovelty+","",0);
  CreateTrigger("AdaptNoveltyNoiseW",PostFlip,AdaptNoveltyNoiseW,"InitAdaptNoveltyNoise","");

  pCurAlg = CreateAlgorithm("g2wsat","",1,
    "G2WSAT: Gradient-based Greedy WalkSAT (generalized) (weighted)",
    "Li, Huang  [SAT 05] and Li, Wei, Zhang [SAT 07]",
    "PickG2WSatGeneralW,ConfigureG2WSatGeneralW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVarsW,FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmProbability(&pCurAlg->parmList,"-wpdp","walk / diversification probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.05);

  AddParmUInt(&pCurAlg->parmList,"-prom","G2WSAT Decreasing Promising Variable ID [default %s]","0: best~1: oldest variable~2: longest promising","",&iG2WsatPromisingSelectID,0);
  AddParmUInt(&pCurAlg->parmList,"-div","G2WSAT Diversification ID [default %s]","0: random (Novelty+)~1: oldest (Novelty++)~2: random[n-1] excl. youngest (Novelty-1)~3: random[n-2] excl. best 2  (Novelty-2)","",&iG2WsatDiversificationSelectID,1);
  AddParmUInt(&pCurAlg->parmList,"-base","G2WSAT Base Algorithm ID [default %s]","0: Novelty~2: Novelty+p~3: Top2","",&iG2WsatBaseAlgSelectID,0);

  CreateTrigger("PickG2WSatGeneralW",ChooseCandidate,PickG2WSatGeneralW,"","");
  CreateTrigger("ConfigureG2WSatGeneralW",PostParameters,ConfigureG2WSatGeneralW,"","");

  pCurAlg = CreateAlgorithm("adaptg2wsat","",1,
    "Adaptive G2WSat (generalized) (weighted)",
    "Li, Wei, Zhang  [SAT 07]",
    "PickG2WSatGeneralW,ConfigureG2WSatGeneralW,InitAdaptG2WSatNoise,AdaptG2WSatNoiseW",
    "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");

  AddParmUInt(&pCurAlg->parmList,"-prom","G2WSAT Decreasing Promising Variable ID [default %s]","0: best~1: oldest variable~2: longest promising","",&iG2WsatPromisingSelectID,0);
  AddParmUInt(&pCurAlg->parmList,"-div","G2WSAT Diversification ID [default %s]","0: random (Novelty+)~1: oldest (Novelty++)~2: random[n-1] excl. youngest (Novelty-1)~3: random[n-2] excl. best 2  (Novelty-2)","",&iG2WsatDiversificationSelectID,1);
  AddParmUInt(&pCurAlg->parmList,"-base","G2WSAT Base Algorithm ID [default %s]","0: Novelty~2: Novelty+p~3: Top2","",&iG2WsatBaseAlgSelectID,0);
  CreateTrigger("AdaptG2WSatNoiseW",PostFlip,AdaptG2WSatNoiseW,"InitAdaptG2WSatNoise","");

  pCurAlg = CreateAlgorithm("gsat-tabu","",1,
    "GSAT-TABU: GSAT with Tabu search (weighted)",
    "Mazure, Sais, Gregoire [AAAI 97]",
    "PickGSatTabuW",
    "DefaultProceduresW,Flip+VarScoreW,VarLastChange",
    "default_w","default");
  CopyParameters(pCurAlg,"gsat-tabu","",0);
  CreateTrigger("PickGSatTabuW",ChooseCandidate,PickGSatTabuW,"","");

  pCurAlg = CreateAlgorithm("gsat","",1,
    "GSAT: Greedy search for SAT (weighted)",
    "Selman, Levesque, Mitchell [AAAI 93]",
    "PickGSatW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  CreateTrigger("PickGSatW",ChooseCandidate,PickGSatW,"","");

  pCurAlg = CreateAlgorithm("gwsat","",1,
    "GWSAT: GSAT with random walk (weighted)",
    "Selman, Kautz [IJCAI 93]",
    "PickGWSatW",
    "DefaultProceduresW,VarScoreW,VarInFalse",
    "default_w","default");
  CopyParameters(pCurAlg,"gwsat","",0);
  CreateTrigger("PickGWSatW",ChooseCandidate,PickGWSatW,"","");

  pCurAlg = CreateAlgorithm("hsat","",1,
    "HSAT (weighted)",
    "Gent, Walsh [AAAI 93]",
    "PickHSatW",
    "DefaultProceduresW,Flip+VarScoreW,VarLastChange",
    "default_w","default");
  CreateTrigger("PickHSatW",ChooseCandidate,PickHSatW,"","");

  pCurAlg = CreateAlgorithm("hwsat","",1,
    "HWSAT: HSAT with random walk (weighted)",
    "Gent, Walsh [Hybrid Problems... 95]",
    "PickHWSatW",
    "DefaultProceduresW,Flip+VarScoreW,VarLastChange,FalseClauseList",
    "default_w","default");
  CopyParameters(pCurAlg,"hwsat","",0);
  CreateTrigger("PickHWSatW",ChooseCandidate,PickHWSatW,"","");

  pCurAlg = CreateAlgorithm("irots","",1,
    "IRoTS: Iterated Robust TABU Search (weighted)",
    "Smyth, Hoos, Stuetzle [AI 2003]",
    "PickRoTSW,PostStepIRoTS",
    "DefaultProceduresW,Flip+VarScoreW,CreateIRoTSBackup",
    "default_w","default");
  CopyParameters(pCurAlg,"irots","",0);

  pCurAlg = CreateAlgorithm("novelty","",1,
    "Novelty: (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  CopyParameters(pCurAlg,"novelty","",0);
  CreateTrigger("PickNoveltyW",ChooseCandidate,PickNoveltyW,"","");

  pCurAlg = CreateAlgorithm("novelty+","",1,
    "Novelty+: Novelty with random walk (weighted)",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  CopyParameters(pCurAlg,"novelty+","",0);
  CreateTrigger("PickNoveltyPlusW",ChooseCandidate,PickNoveltyPlusW,"","");
 
  pCurAlg = CreateAlgorithm("novelty++","",1,
    "Novelty++: Novelty+ with a modified diversification mechanism (weighted)",
    "Li, Huang  [SAT 05]",
    "PickNoveltyPlusPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  CopyParameters(pCurAlg,"novelty++","",0);
  CreateTrigger("PickNoveltyPlusPlusW",ChooseCandidate,PickNoveltyPlusPlusW,"","");

  pCurAlg = CreateAlgorithm("urwalk","",1,
    "Uninformed Random Walk: flip any var at random (weighted)",
    "",
    "PickURWalk",
    "DefaultProceduresW",
    "default_w","default");

  pCurAlg = CreateAlgorithm("crwalk","",1,
    "Conflict-Directed Random Walk (weighted)",
    "Papadimitriou [FOCS 91]",
    "PickCRWalk",
    "DefaultProceduresW,Flip+FalseClauseListW",
    "default_w","default");
  
  pCurAlg = CreateAlgorithm("crwalk","schoening",1,
    "Conflict-Directed Random Walk, restart every 3n steps (weighted)",
    "Schoening [FOCS 99]",
    "PickCRWalk,SchoeningRestart",
    "DefaultProceduresW,Flip+FalseClauseListW",
    "default_w","default");

  pCurAlg = CreateAlgorithm("rgsat","",1,
    "RGSAT: Restarting GSAT (poor algorithm -- academic use only) (weighted)",
    "Tompkins, Hoos [SAIM 04]",
    "PickRGSatW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  CreateTrigger("PickRGSatW",CheckRestart,PickRGSatW,"","");

  pCurAlg = CreateAlgorithm("rots","",1,
    "RoTS: Robust TABU Search (weighted)",
    "Taillard [Parallel Computing 1991], based on implementation by Stuetzle",
    "PickRoTSW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  CopyParameters(pCurAlg,"rots","",0);
  CreateTrigger("PickRoTSW",ChooseCandidate,PickRoTSW,"InitRoTS,VarLastChange,BestFalse","");

  pCurAlg = CreateAlgorithm("samd","",1,
    "SAMD: Steepest Ascent Mildest Descent (weighted)",
    "Hansen and Jaumard [Computing 1990]",
    "PickGSatTabuW,SAMDUpdateVarLastChangeW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  CopyParameters(pCurAlg,"gsat-tabu","",1);
  CreateTrigger("SAMDUpdateVarLastChangeW",UpdateStateInfo,SAMDUpdateVarLastChangeW,"VarLastChange","UpdateVarLastChange");

   pCurAlg = CreateAlgorithm("saps","winit",1,
    "SAPS: (weighted -- init to weights)",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,PostFlipSAPS",
    "DefaultProceduresW,InitClausePenaltyFLW,Flip+MBPFL+FCL+VIF+W",
    "default_w","default");
  CopyParameters(pCurAlg,"saps","",0);

  pCurAlg = CreateAlgorithm("saps","wsmooth",1,
    "SAPS: (weighted -- init and smooth to weights)",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,PostFlipSAPSWSmooth",
    "DefaultProceduresW,InitClausePenaltyFLW,Flip+MBPFL+FCL+VIF+W",
    "default_w","default");
  CopyParameters(pCurAlg,"saps","",0);
  CreateTrigger("PostFlipSAPSWSmooth",PostFlip,PostFlipSAPSWSmooth,"","");

  pCurAlg = CreateAlgorithm("walksat","",1,
    "WALKSAT: Original WalkSAT algorithm (SKC variant) (weighted)",
    "Selman, Kautz, Cohen [AAAI 94]",
    "PickWalkSatSKCW",
    "DefaultProceduresW,Flip+FalseClauseListW",
    "default_w","default");
  CopyParameters(pCurAlg,"walksat","",0);
  CreateTrigger("PickWalkSatSKCW",ChooseCandidate,PickWalkSatSKCW,"","");

  pCurAlg = CreateAlgorithm("walksat-tabu","",1,
    "WALKSAT-TABU: WalkSAT with TABU search (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickWalkSatTabuW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  CopyParameters(pCurAlg,"walksat-tabu","",0);
  CreateTrigger("PickWalkSatTabuW",ChooseCandidate,PickWalkSatTabuW,"","");

 
}


#ifdef __cplusplus

}
#endif
