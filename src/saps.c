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

FLOAT fAlpha;
FLOAT fRho;
FLOAT fPenaltyImprove;
PROBABILITY iPs;
PROBABILITY iRPs;

FLOAT fMaxClausePenalty = 1000.0f;


void PickSAPS();
void PostFlipSAPS();

void InitRSAPS();
void PickRSAPS();
void PostFlipRSAPS();

void PickSAPSNR();
void PostFlipSAPSNR();


void AddSAPS() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("saps","",FALSE,
    "SAPS: Scaling And Probabilistic Smoothing",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,PostFlipSAPS",
    "DefaultProcedures,Flip+MBP+FCL+VIF",
    "default","default");
  
  AddParmFloat(&pCurAlg->parmList,"-alpha","alpha","Algorithm scaling parameter alpha","",&fAlpha,1.3f);
  AddParmFloat(&pCurAlg->parmList,"-rho","rho","Algorithm smoothing parameter rho","",&fRho,0.8f);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty","When a local minimum is encountered, Smooth Penalties with prob. [ps]","",&iPs,0.05);
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability","When a local minimum is encountered, walk with prob. [wp]","",&iWp,0.01);
  AddParmFloat(&pCurAlg->parmList,"-sapsthresh","SAPS Threshold for Improvement","Sets threshold for detecting a local minimum","",&fPenaltyImprove,-1.0e-01f);

  CreateTrigger("PickSAPS",ChooseCandidate,PickSAPS,"","");
  CreateTrigger("PostFlipSAPS",PostFlip,PostFlipSAPS,"","");


  pCurAlg = CreateAlgorithm("rsaps","",FALSE,
    "RSAPS: Reactive Scaling And Probabilistic Smoothing",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,InitRSAPS,PostFlipRSAPS",
    "DefaultProcedures,Flip+MBP+FCL+VIF",
    "default","default");

  CopyParameters(pCurAlg,"saps","",FALSE);

  CreateTrigger("PostFlipRSAPS",PostFlip,PostFlipRSAPS,"","");
  CreateTrigger("InitRSAPS",PostInit,InitRSAPS,"","");


  pCurAlg = CreateAlgorithm("sapsnr","",FALSE,
  "SAPS/NR: De-randomised version of SAPS",
  "Tompkins, Hoos [SAIM 04]",
  "PickSAPSNR,PostFlipSAPSNR",
  "DefaultProcedures,Flip+MBP+FCL+VIF",
  "default","default");

  AddParmFloat(&pCurAlg->parmList,"-alpha","alpha","Algorithm scaling parameter alpha","",&fAlpha,1.3f);
  AddParmFloat(&pCurAlg->parmList,"-rho","rho","Algorithm smoothing parameter rho","",&fRho,0.8f);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth periodicity","when a local minimum is encountered, peridically smooth every 1/[ps] steps","",&iPs,0.05);
  AddParmFloat(&pCurAlg->parmList,"-sapsthresh","SAPS Threshold for Improvement","Sets threshold for detecting a local minimum","",&fPenaltyImprove,-1.0e-01f);

  CreateTrigger("PickSAPSNR",ChooseCandidate,PickSAPSNR,"","");
  CreateTrigger("PostFlipSAPSNR",PostFlip,PostFlipSAPSNR,"NullFlips","");
}

void PickSAPS() {
  
  UINT32 j;
  UINT32 iVar;
  FLOAT fScore;
  FLOAT fBestScore;

  iNumCandidates = 0;
  fBestScore = FLOATMAX;

  /* look at all variables that appear in false clauses */

  for (j=0;j<iNumVarsInFalseList;j++) {
    iVar = aVarInFalseList[j];

    /* use cached value of breakcount - makecount */

    fScore = aBreakPenalty[iVar] - aMakePenalty[iVar];

    /* build candidate list of best vars */

    if (fScore <= fBestScore) {
      if (fScore < fBestScore) {
        iNumCandidates = 0;
        fBestScore = fScore;
      }
      aCandidateList[iNumCandidates++] = iVar;
    }
  }

  /* if bestScore is not below a threshold */

  if (fBestScore >= fPenaltyImprove) {

    /* with probability (iWp) flip a variable at random,
       otherwise it's a null flip */

    if (RandomProb(iWp)) {
      iFlipCandidate = RandomInt(iNumVars) + 1;
    } else {
      iFlipCandidate = 0;
    }
  } else {
    
    /* select flip candidate uniformly from candidate list */
    
    if (iNumCandidates > 1) {
      iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
    } else {
      iFlipCandidate = *aCandidateList;
    }
  }
}

void SmoothSAPS() {
  
  FLOAT fAveragePenalty;
  UINT32 j;

  /* calculate the average penalty, and adjust by rho */

  fAveragePenalty = fTotalPenalty / (FLOAT) iNumClauses;
  fAveragePenalty *= (1.0f - fRho);

  /* add the average penalty to each clause penalty */

  for(j=0;j<iNumClauses;j++) {
    aClausePenalty[j] += fAveragePenalty;
  }

  /* update cached values */

  fBasePenalty += fAveragePenalty;
    
  fTotalPenalty += (fAveragePenalty * (FLOAT) iNumClauses);

  for (j=1;j<=iNumVars;j++) {
    aMakePenalty[j] += fAveragePenalty * (FLOAT) aMakeCount[j];
    aBreakPenalty[j] += fAveragePenalty * (FLOAT) aBreakCount[j];
  }

}


void AdjustPenalties() {
  
  UINT32 j;
  UINT32 k;
  BOOL bReScale = 0;
  FLOAT fDiff;
  FLOAT fOld;
  LITTYPE *pLit;

  /* as per the original SAPS implementation, if a single clause
     penalty exceeds the value (1000), divide all penalties by 1000 */

  for(j=0;j<iNumFalse;j++) {
    if (aClausePenalty[aFalseList[j]] > fMaxClausePenalty) {
      bReScale = 1;
      break;
    }
  }
  
  if (bReScale) {

    fTotalPenalty = 0;

    fBasePenalty /= fMaxClausePenalty;

    for(j=0;j<iNumClauses;j++) {

      fOld = aClausePenalty[j];
      aClausePenalty[j] /= fMaxClausePenalty;
      fDiff = aClausePenalty[j] - fOld;

      if (aNumTrueLit[j]==0) {
        pLit = pClauseLits[j];
        for (k=0;k<aClauseLen[j];k++) {
          aMakePenalty[GetVarFromLit(*pLit)] += fDiff;
          pLit++;
        }
      }

      if (aNumTrueLit[j]==1) {
        aBreakPenalty[aCritSat[j]] += fDiff;
      }

      fTotalPenalty += aClausePenalty[j];
    }
  }

}

void ScaleSAPS() {
  UINT32 j;
  UINT32 k;
  UINT32 iClause;
  FLOAT fOld;
  FLOAT fDiff;
  LITTYPE *pLit;

  /* for each false clause, increae the clause penalty by alpha */

  for(j=0;j<iNumFalse;j++) {
    
    iClause = aFalseList[j];

    fOld = aClausePenalty[iClause];

    aClausePenalty[iClause] *= fAlpha;

    fDiff = aClausePenalty[iClause] - fOld;

    /* update cached values */

    pLit = pClauseLits[iClause];
    for (k=0;k<aClauseLen[iClause];k++) {
      aMakePenalty[GetVarFromLit(*pLit)] += fDiff;
      pLit++;
    }
		fTotalPenalty += fDiff;
  }
}


void PostFlipSAPS() {

  if (iFlipCandidate)
    return;

  /* if a 'null flip' */

  /* smooth with probibility (iPs) */

  if (RandomProb(iPs)) {
    SmoothSAPS();
  }

  /* possibly adjust all penalties (see above) */

  AdjustPenalties();

  /* scale all unsatisfied clauses */
  
  ScaleSAPS();

}

void InitRSAPS() {
	iLastAdaptStep=0;
	iLastAdaptNumFalse=iNumFalse;
	iRPs = iPs;
}

void RSAPSAdaptSmooth() {
	
  UINT32 iInvTheta = 6;
  FLOAT fDelta = 0.1f;
  FLOAT fProb;

  if (iStep-iLastAdaptStep > iNumClauses / iInvTheta) {

    /* if no improvement, decrease smoothing probability */

    fProb = ProbToFloat(iRPs);
    fProb *= fDelta;
    iRPs = FloatToProb(fProb);

	  iLastAdaptStep=iStep;
	  iLastAdaptNumFalse = iNumFalse;

	} else if (iNumFalse < iLastAdaptNumFalse) {

    /* if improvement, increase smoothing probability */

    fProb = ProbToFloat(iRPs);
    fProb += (1.0f - fProb) * 2.0f * fDelta;
    iRPs = FloatToProb(fProb);

	  iLastAdaptStep=iStep;
	  iLastAdaptNumFalse = iNumFalse;
  }

}

void PostFlipRSAPS() {

  if (iFlipCandidate == 0) {

    if (RandomProb(iRPs)) {
      SmoothSAPS();
      iRPs = 0;
    }

    AdjustPenalties();
  
    ScaleSAPS();

  }

  RSAPSAdaptSmooth();

}

void PostFlipSAPSNR() {
  if (iFlipCandidate)
    return;

  /* with SAPS/NR, smoothing is made periodic instead of probabilistic */

  if (iPs) {
    if ((iNumNullFlips % ProbToInvInt(iPs))==0) {
      SmoothSAPS();
    }
  }

  AdjustPenalties();
  
  ScaleSAPS();
}

void PickSAPSNR() {
  
  UINT32 j;
  UINT32 iVar;
  FLOAT fScore;
  FLOAT fBestScore;

  iNumCandidates = 0;
  fBestScore = FLOATMAX;

  /* look at all variables that appear in false clauses */

  for (j=0;j<iNumVarsInFalseList;j++) {

    iVar = aVarInFalseList[j];

    /* use cached value of breakcount - makecount */

    fScore = aBreakPenalty[iVar] - aMakePenalty[iVar];

    /* build candidate list of best vars */

    if (fScore <= fBestScore) {
      if (fScore < fBestScore) {
        iNumCandidates = 0;
      }
      fBestScore = fScore;
      aCandidateList[iNumCandidates++] = iVar;
    }
  }

  /* if no improvement over threshold, then it's a null flip */
  /* if multiple candidates, then just select the first candidate */

  if (fBestScore >= fPenaltyImprove) {
    iFlipCandidate = 0;
  } else {
    iFlipCandidate = *aCandidateList;
  }
}


