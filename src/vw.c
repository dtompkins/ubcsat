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

FLOAT fVW2Smooth;
FLOAT fVW2WeightFactor;
void PickVW1();
void PickVW2();

/***** Trigger VW2Weights *****/

void CreateVW2Weights();
void InitVW2Weights();
void UpdateVW2Weights();

FLOAT *aVW2Weights;
FLOAT fVW2WeightMean;


void AddVW() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("vw1","",FALSE,
    "VW1: Variable Weighting Scheme One",
    "Prestwich [SAT 05]",
    "PickVW1",
    "DefaultProcedures,Flip+FalseClauseList,FlipCounts",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWp,0.50);

  CreateTrigger("PickVW1",ChooseCandidate,PickVW1,"","");


  pCurAlg = CreateAlgorithm("vw2","",FALSE,
    "VW2: Variable Weighting Scheme Two",
    "Prestwich [SAT 05]",
    "PickVW2",
    "DefaultProcedures,Flip+FalseClauseList,VW2Weights",
    "default","default");
  
  CopyParameters(pCurAlg,"vw1","",FALSE);
  AddParmFloat(&pCurAlg->parmList,"-s","VW2 smoothing factor [default %s]","paramater to adjust variable weight smoothing~s = 0 is equivalent to VW1~s = 1 is more HWSAT-like","",&fVW2Smooth,0.01);
  AddParmFloat(&pCurAlg->parmList,"-c","VW2 weighting factor [default %s]","paramater to adjust variable weight scoring~c = 0 is equivalent to WalkSAT/SKC","",&fVW2WeightFactor,0.01);

  CreateTrigger("PickVW2",ChooseCandidate,PickVW2,"","");

  CreateTrigger("CreateVW2Weights",CreateStateInfo,CreateVW2Weights,"","");
  CreateTrigger("InitVW2Weights",InitStateInfo,InitVW2Weights,"","");
  CreateTrigger("UpdateVW2Weights",UpdateStateInfo,UpdateVW2Weights,"","");
  CreateContainerTrigger("VW2Weights","InitVW2Weights,CreateVW2Weights,UpdateVW2Weights");

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
  UINT32 iBestVarFlipCount;

  iNumCandidates = 0;
  iBestScore = iNumClauses;
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
  iBestScore = iNumClauses;
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


void CreateVW2Weights() {
  aVW2Weights = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
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

