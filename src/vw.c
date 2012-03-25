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

FLOAT fVW2Smooth;
FLOAT fVW2WeightFactor;
void PickVW1();
void PickVW2();

void PickVW2Auto();
void InitVW2Auto();
void UpdateVW2Auto();
SINT32 iMaxExpProbability;
FLOAT fVW2AutoAdjustInterval;
UBIGINT iNextVW2AutoAdjustStep;
FLOAT VW2AutoSmooth[7] = {0.2f, 0.02f, 0.002f, 0.0002f, 0.00002f, 0.000002f, 0.0f};

/***** Trigger VW2Weights *****/

void CreateVW2Weights();
void InitVW2Weights();
void UpdateVW2Weights();

FLOAT *aVW2Weights;
FLOAT fVW2WeightMean;


void AddVW() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("vw1","",0,
    "VW1: Variable Weighting Scheme One",
    "Prestwich [SAT 05]",
    "PickVW1",
    "DefaultProcedures,Flip+FalseClauseList,FlipCounts",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.50);

  CreateTrigger("PickVW1",ChooseCandidate,PickVW1,"","");


  pCurAlg = CreateAlgorithm("vw2","",0,
    "VW2: Variable Weighting Scheme Two",
    "Prestwich [SAT 05]",
    "PickVW2",
    "DefaultProcedures,Flip+FalseClauseList,VW2Weights",
    "default","default");
  
  CopyParameters(pCurAlg,"vw1","",0);
  AddParmFloat(&pCurAlg->parmList,"-s","VW2 smoothing factor [default %s]","paramater to adjust variable weight smoothing~s = 0 is equivalent to VW1~s = 1 is more HWSAT-like","",&fVW2Smooth,0.01);
  AddParmFloat(&pCurAlg->parmList,"-c","VW2 weighting factor [default %s]","paramater to adjust variable weight scoring~c = 0 is equivalent to WalkSAT/SKC","",&fVW2WeightFactor,0.01);

  CreateTrigger("PickVW2",ChooseCandidate,PickVW2,"","");

  CreateTrigger("CreateVW2Weights",CreateStateInfo,CreateVW2Weights,"","");
  CreateTrigger("InitVW2Weights",InitStateInfo,InitVW2Weights,"","");
  CreateTrigger("UpdateVW2Weights",UpdateStateInfo,UpdateVW2Weights,"","");
  CreateContainerTrigger("VW2Weights","InitVW2Weights,CreateVW2Weights,UpdateVW2Weights");

  pCurAlg = CreateAlgorithm("vw2","2005",0,
    "VW2/2005: VW2 with randomized parameter settings",
    "Prestwich [SAT 05], modified for 2005 competition",
    "PickVW2Auto,InitVW2Auto,UpdateVW2Auto",
    "DefaultProcedures,Flip+FalseClauseList,VW2Weights",
    "default","default");
  
  CreateTrigger("PickVW2Auto",ChooseCandidate,PickVW2Auto,"","");
  CreateTrigger("InitVW2Auto",InitStateInfo,InitVW2Auto,"","");
  CreateTrigger("UpdateVW2Auto",UpdateStateInfo,UpdateVW2Auto,"","");

}

void PickVW1() {
 
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
  UBIGINT iBestVarFlipCount;

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;
  iBestVarFlipCount = iStep;

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
        iBestVarFlipCount = aFlipCounts[iVar];
        aCandidateList[iNumCandidates++] = iVar;
      } else {
	      if (iBestScore == 0) {
          aCandidateList[iNumCandidates++] = iVar;
        } else {

          /* if it's a tie and not a 'freebie', 
             prefer variables that have have been 
             flipped less frequently */

	        if (aFlipCounts[iVar] <= iBestVarFlipCount) {
		        if (aFlipCounts[iVar] < iBestVarFlipCount) {
		          iNumCandidates=0;
		          iBestVarFlipCount = aFlipCounts[iVar];
		        }
		        aCandidateList[iNumCandidates++] = iVar;
		      }
        } 
	    }
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

void PickVW2() {
 
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
  FLOAT fCurVW2Score;
  FLOAT fBestVW2Score;

  iNumCandidates = 0;
  iBestScore = (SINT32) iNumClauses;
  fBestVW2Score = FLOATMAX;

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
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }

    if (iScore == 0) {

      /* Note: in author's original source code, it exits on the first freebie variable it discovers */

      if (iBestScore > 0) {
        iNumCandidates=0;
        iBestScore = 0;
      } 
      aCandidateList[iNumCandidates++] = iVar;
    } else {
      if (iBestScore > 0) {

        /* if no freebie found yet, use the VW2 weighted score */

        fCurVW2Score = (FLOAT) iScore + fVW2WeightFactor * (aVW2Weights[iVar] - fVW2WeightMean);

        if (fCurVW2Score <= fBestVW2Score) {
	        if (fCurVW2Score < fBestVW2Score) {
	          iNumCandidates=0;
	          fBestVW2Score = fCurVW2Score;
	        }
	        aCandidateList[iNumCandidates++] = iVar;
	      }
      } 
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


void InitVW2Auto() {
  iMaxExpProbability=1;
  fVW2Smooth = 0.1f;
  fVW2AutoAdjustInterval=1000.0f;
  iNextVW2AutoAdjustStep=1000;
}

void UpdateVW2Auto() {
  if ( iStep > iNextVW2AutoAdjustStep ) {
    iMaxExpProbability = 1 + (SINT32) RandomInt(6);
    fVW2Smooth = VW2AutoSmooth[RandomInt(7)];
    fVW2AutoAdjustInterval *= 1.1f;
    iNextVW2AutoAdjustStep = iStep + (UBIGINT) fVW2AutoAdjustInterval;
  }
}

BOOL BoundedExpProbability (SINT32 iExpProbability) {
  if (iExpProbability < 0) {
    return 1;
  }
  if (iExpProbability > iMaxExpProbability) {
    iExpProbability = iMaxExpProbability;
  }
  if (RandomInt((UINT32) (1 << iExpProbability))==0) {
    return 1;
  }
  return 0;
}

void PickVW2Auto() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  UINT32 iStartLit;
  LITTYPE curLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  SINT32 iPrevScore;
  FLOAT fPrevVW2Weight;

  iNumCandidates = 0;

  iPrevScore = (SINT32) iNumClauses;
  iBestScore = (SINT32) iNumClauses;
  fPrevVW2Weight = FLOATMAX;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  iStartLit = RandomInt(iClauseLen);

  for (j=0;j<iClauseLen;j++) {

    curLit = pClauseLits[iClause][(iStartLit + j) % iClauseLen];
    iVar = GetVarFromLit(curLit);

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values 
    
       note that in this case, score is the breakcount[] */

    iScore = 0;
    
    iNumOcc = aNumLitOcc[GetNegatedLit(curLit)];
    pClause = pLitClause[GetNegatedLit(curLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }

    if (iScore == 0) {
      iBestScore = 0;
      iFlipCandidate = iVar;
      return;
    } else {
      /* note: we distinguish between Best & "Prev" for other global Best statistics */
      if (iScore < iBestScore) {
        iBestScore = iScore;
      }

      if ((iScore < iPrevScore) || ((aVW2Weights[iVar] < fPrevVW2Weight) && BoundedExpProbability(iScore - iPrevScore))) {
        iFlipCandidate = iVar;
        iPrevScore = iScore;
        fPrevVW2Weight = aVW2Weights[iVar];
      }
    }
  }
}


void CreateVW2Weights() {
  aVW2Weights = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT), HeapData);
}

void InitVW2Weights() {
  memset(aVW2Weights,0,(iNumVars+1)*sizeof(FLOAT));
  fVW2WeightMean = FLOATZERO;
}

void UpdateVW2Weights() {
  FLOAT fPrevWeight = aVW2Weights[iFlipCandidate];
  aVW2Weights[iFlipCandidate] = (1.0f - fVW2Smooth) * (aVW2Weights[iFlipCandidate] + 1.0f) + (fVW2Smooth * (FLOAT) iStep);
  fVW2WeightMean += (aVW2Weights[iFlipCandidate] - fPrevWeight) / iNumVars;
}

#ifdef __cplusplus

}
#endif
