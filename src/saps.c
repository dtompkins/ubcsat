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

const FLOAT fMaxClausePenalty = 1000.0f;


void PickSAPS();
void PostFlipSAPS();

void PostFlipSAPSWSmooth();

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
    "DefaultProcedures,Flip+MBPFL+FCL+VIF",
    "default","default");
  
  AddParmFloat(&pCurAlg->parmList,"-alpha","scaling parameter alpha [default %s]","when a local minimum is encountered,~multiply all unsatisfied cluase penalties by FL","",&fAlpha,1.3f);
  AddParmFloat(&pCurAlg->parmList,"-rho","smoothing parameter rho [default %s]","when smoothing occurs, smooth penalties by a factor of FL","",&fRho,0.8f);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","when a local minimum is encountered,~smooth penalties with probability PR","",&iPs,0.05);
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","when a local minimum is encountered,~flip a random variable with probability PR","",&iWp,0.01);
  AddParmFloat(&pCurAlg->parmList,"-sapsthresh","threshold for detecting local minima [default %s]","the algorithm considers a local minima to occur when no~improvement greater than FL is possible~the default reflects the value used in SAPS 1.0","",&fPenaltyImprove,-1.0e-01f);

  CreateTrigger("PickSAPS",ChooseCandidate,PickSAPS,"","");
  CreateTrigger("PostFlipSAPS",PostFlip,PostFlipSAPS,"","");



  pCurAlg = CreateAlgorithm("saps","winit",TRUE,
    "SAPS: (weighted -- init to weights)",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,PostFlipSAPS",
    "DefaultProceduresW,InitClausePenaltyFLW,Flip+MBPFL+FCL+VIF+W",
    "default_w","default");
  
  CopyParameters(pCurAlg,"saps","",FALSE);



  pCurAlg = CreateAlgorithm("saps","wsmooth",TRUE,
    "SAPS: (weighted -- init and smooth to weights)",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,PostFlipSAPSWSmooth",
    "DefaultProceduresW,InitClausePenaltyFLW,Flip+MBPFL+FCL+VIF+W",
    "default_w","default");
  
  CopyParameters(pCurAlg,"saps","",FALSE);

  CreateTrigger("PostFlipSAPSWSmooth",PostFlip,PostFlipSAPSWSmooth,"","");



  pCurAlg = CreateAlgorithm("rsaps","",FALSE,
    "RSAPS: Reactive Scaling And Probabilistic Smoothing",
    "Hutter, Tompkins, Hoos [CP 02]",
    "PickSAPS,InitRSAPS,PostFlipRSAPS",
    "DefaultProcedures,Flip+MBPFL+FCL+VIF",
    "default","default");

  CopyParameters(pCurAlg,"saps","",FALSE);

  CreateTrigger("PostFlipRSAPS",PostFlip,PostFlipRSAPS,"","");
  CreateTrigger("InitRSAPS",PostInit,InitRSAPS,"","");



  pCurAlg = CreateAlgorithm("sapsnr","",FALSE,
  "SAPS/NR: De-randomized version of SAPS",
  "Tompkins, Hoos [SAIM 04]",
  "PickSAPSNR,PostFlipSAPSNR",
  "DefaultProcedures,Flip+MBPFL+FCL+VIF",
  "default","default");

  AddParmFloat(&pCurAlg->parmList,"-alpha","scaling parameter alpha [default %s]","when a local minimum is encountered,~multiply all unsatisfied cluase penalties by FL","",&fAlpha,1.3f);
  AddParmFloat(&pCurAlg->parmList,"-rho","smoothing parameter rho [default %s]","when smoothing occurs, smooth penalties by a factor of FL","",&fRho,0.8f);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth periodicity [default %s]","peridically smooth every 1/PR local minima","",&iPs,0.05);
  AddParmFloat(&pCurAlg->parmList,"-sapsthresh","threshold for detecting local minima [default %s]","the algorithm considers a local minima to occur when no~improvement greater than FL is possible~the default reflects the value used in SAPS 1.0","",&fPenaltyImprove,-1.0e-01f);

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

    fScore = aBreakPenaltyFL[iVar] - aMakePenaltyFL[iVar];

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

  fAveragePenalty = fTotalPenaltyFL / (FLOAT) iNumClauses;
  fAveragePenalty *= (1.0f - fRho);

  /* add the average penalty to each clause penalty */

  for(j=0;j<iNumClauses;j++) {
    aClausePenaltyFL[j] += fAveragePenalty;
  }

  /* update cached values */

  fBasePenaltyFL += fAveragePenalty;
    
  fTotalPenaltyFL += (fAveragePenalty * (FLOAT) iNumClauses);

  for (j=1;j<=iNumVars;j++) {
    aMakePenaltyFL[j] += fAveragePenalty * (FLOAT) aMakeCount[j];
    aBreakPenaltyFL[j] += fAveragePenalty * (FLOAT) aBreakCount[j];
  }

}

void AdjustPenalties() {
  
  UINT32 j;
  UINT32 k;
  BOOL bReScale = FALSE;
  FLOAT fDiff;
  FLOAT fOld;
  LITTYPE *pLit;

  /* as per the original SAPS implementation, if a single clause
     penalty exceeds the value (1000), divide all penalties by 1000 */

  for(j=0;j<iNumFalse;j++) {
    if (aClausePenaltyFL[aFalseList[j]] > fMaxClausePenalty) {
      bReScale = TRUE;
      break;
    }
  }
  
  if (bReScale) {

    fTotalPenaltyFL = 0;

    fBasePenaltyFL /= fMaxClausePenalty;

    for(j=0;j<iNumClauses;j++) {

      fOld = aClausePenaltyFL[j];
      aClausePenaltyFL[j] /= fMaxClausePenalty;
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

    fOld = aClausePenaltyFL[iClause];

    aClausePenaltyFL[iClause] *= fAlpha;

    fDiff = aClausePenaltyFL[iClause] - fOld;

    /* update cached values */

    pLit = pClauseLits[iClause];
    for (k=0;k<aClauseLen[iClause];k++) {
      aMakePenaltyFL[GetVarFromLit(*pLit)] += fDiff;
      pLit++;
    }
    fTotalPenaltyFL += fDiff;
  }
}


void PostFlipSAPS() {

  if (iFlipCandidate) {
    return;
  }

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
  if (iFlipCandidate) {
    return;
  }

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

    fScore = aBreakPenaltyFL[iVar] - aMakePenaltyFL[iVar];

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


