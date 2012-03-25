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
void PickVW1W();
void PickVW2();
void PickVW2W();
void PickVW1Tabu();
void PickVW2Tabu();
void CreateAutoVW2Weights();
void InitAutoVW2Weights();
void UpdateAutoVW2Weights();



/***** Trigger VW2Weights *****/

void CreateVW2Weights();
void InitVW2Weights();
void UpdateVW2Weights();

FLOAT *aVW2Weights;
FLOAT fVW2WeightMean;
char expchance(int);
int Intensity;
double Interval,Adjustment;



void AddVW1() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("vw1","",FALSE,
    "VW1: Variable Weighting Scheme One",
    "Prestwich [SAT 05]",
    "PickVW1",
    "DefaultProcedures,Flip+FalseClauseList,FlipCounts",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-wpwalk","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWpWalk,0.50);

  CreateTrigger("PickVW1",ChooseCandidate,PickVW1,"","");

   pCurAlg = CreateAlgorithm("vw1","",TRUE,
    "VW1: Variable Weighting Scheme One",
    "Prestwich [SAT 05]",
    "PickVW1W",
    "DefaultProceduresW,Flip+FalseClauseListW,FlipCounts",
    "default_w","default");

  CopyParameters(pCurAlg,"vw1","",FALSE,0);

  CreateTrigger("PickVW1W",ChooseCandidate,PickVW1W,"","");
  
   pCurAlg = CreateAlgorithm("vw1tabu","",FALSE,
    "VW1: Variable Weighting Scheme One with Tabu",
    "Prestwich [SAT 05]",
    "PickVW1Tabu",
    "DefaultProcedures,Flip+FalseClauseList,FlipCounts,VarLastChange",
    "default","default");

  AddParmProbability(&pCurAlg->parmList,"-wpwalk","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iWpWalk,0.50);
   AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);

  CreateTrigger("PickVW1Tabu",ChooseCandidate,PickVW1Tabu,"","");


   



}



void AddVW2() {
  
    ALGORITHM *pCurAlg;
    pCurAlg = CreateAlgorithm("vw2","",FALSE,
    "VW2: Variable Weighting Scheme Two",
    "Prestwich [SAT 05]",
    "PickVW2",
    "DefaultProcedures,Flip+FalseClauseList,VW2Weights",
    "default","default");
  
  CopyParameters(pCurAlg,"vw1","",FALSE,0);
  AddParmFloat(&pCurAlg->parmList,"-s","VW2 smoothing factor [default %s]","paramater to adjust variable weight smoothing~s = 0 is equivalent to VW1~s = 1 is more HWSAT-like","",&fVW2Smooth,0.01);
  AddParmFloat(&pCurAlg->parmList,"-c","VW2 weighting factor [default %s]","paramater to adjust variable weight scoring~c = 0 is equivalent to WalkSAT/SKC","",&fVW2WeightFactor,0.01);

  CreateTrigger("PickVW2",ChooseCandidate,PickVW2,"","");

  CreateTrigger("CreateVW2Weights",CreateStateInfo,CreateVW2Weights,"","");
  CreateTrigger("InitVW2Weights",InitStateInfo,InitVW2Weights,"","");
  CreateTrigger("UpdateVW2Weights",UpdateStateInfo,UpdateVW2Weights,"","");
  CreateContainerTrigger("VW2Weights","InitVW2Weights,CreateVW2Weights,UpdateVW2Weights");

  
    pCurAlg = CreateAlgorithm("vw2","",TRUE,
    "VW2: Variable Weighting Scheme Two",
    "Prestwich [SAT 05]",
    "PickVW2W",
    "DefaultProceduresW,Flip+FalseClauseList+W,VW2Weights",
    "default_w","default");

     CopyParameters(pCurAlg,"vw2","",FALSE,0);
     CreateTrigger("PickVW2W",ChooseCandidate,PickVW2W,"","");


  // TODO: Weighted variants
   
      pCurAlg = CreateAlgorithm("vw2tabu","",FALSE,
    "VW2: Variable Weighting Scheme Two with Tabu",
    "Prestwich [SAT 05]",
    "PickVW2Tabu",
    "DefaultProcedures,Flip+FalseClauseList,VW2Weights,VarLastChange",
    "default","default");

  CopyParameters(pCurAlg,"vw2","",FALSE,0);

   CreateTrigger("PickVW2Tabu",ChooseCandidate,PickVW2Tabu,"","");


  CreateTrigger("CreateAutoVW2Weights",CreateStateInfo,CreateAutoVW2Weights,"","");
  CreateTrigger("InitAutoVW2Weights",InitStateInfo,InitAutoVW2Weights,"","");
  CreateTrigger("UpdateAutoVW2Weights",UpdateStateInfo,UpdateAutoVW2Weights,"","");
  CreateContainerTrigger("AutoVW2Weights","InitAutoVW2Weights,CreateAutoVW2Weights,UpdateAutoVW2Weights");

  pCurAlg = CreateAlgorithm("autovw2","",FALSE,
    "VW2: Variable Weighting Scheme Two",
    "Prestwich [SAT 05]",
    "PickVW2Automated",
    "DefaultProcedures,Flip+FalseClauseList,AutoVW2Weights",
    "default","default");
    CopyParameters(pCurAlg,"vw2","",FALSE,0);
    CreateTrigger("PickVW2Automated",ChooseCandidate,PickVW2Automated,"","");

 

}

void PickVW1() {
 
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
  UINT32 iBestVarFlipCount;

  iNumCandidates = 0;
  iBestScore = iNumClauses;
  iBestVarFlipCount = iStep;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
   // iClause = aFalseList[RandomInt(iNumFalse)];
   iClause = SelectClause(); 
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

    /*For clause penalties we need to take account of that*/
    
    if(!bPen)
    {    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }
    }
    else {
        for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
      }
      pClause++;
    }

    }
    /* build candidate list of best vars */

    if (iScore <= iBestScore) {
      if (iScore < iBestScore) {
        iNumCandidates=0;
        iBestScore = iScore;
        aCandidateList[iNumCandidates++] = iVar;
      } else {
	      if(iBestScore == 0) {
          aCandidateList[iNumCandidates++] = iVar;
        } else {

          /* if it's a tie and not a 'freebie', 
             prefer variables that have have been 
             flipped less frequently */

	        if(aFlipCounts[iVar] <= iBestVarFlipCount) {
		        if(aFlipCounts[iVar] < iBestVarFlipCount) {
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

void PickVW2() {
 
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
  FLOAT fCurVW2Score;
  FLOAT fBestVW2Score;

  iNumCandidates = 0;
  iBestScore = iNumClauses;
  fBestVW2Score = FLOATMAX;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
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
    else {
        for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
      }
      pClause++;
    }

    }

 

    /* build candidate list of best vars */

    if (iScore == 0) {
      if (iBestScore > 0) {
        iNumCandidates=0;
        iBestScore = 0;
      } 
      aCandidateList[iNumCandidates++] = iVar;
    } else {
      if(iBestScore > 0) {
        fCurVW2Score = (FLOAT) iScore + fVW2WeightFactor * (aVW2Weights[iVar] - fVW2WeightMean);

        if(fCurVW2Score <= fBestVW2Score) {
	        if(fCurVW2Score < fBestVW2Score) {
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


void CreateVW2Weights() {
  aVW2Weights = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
}

void InitVW2Weights() {
  memset(aVW2Weights,0,(iNumVars+1)*sizeof(FLOAT));
  fVW2WeightMean = 0.0f;
}

void UpdateVW2Weights() {
  FLOAT fPrevWeight = aVW2Weights[iFlipCandidate];
  aVW2Weights[iFlipCandidate] = (1.0f - fVW2Smooth) * (aVW2Weights[iFlipCandidate] + 1.0f) + (fVW2Smooth * (FLOAT) iStep);
  fVW2WeightMean += (aVW2Weights[iFlipCandidate] - fPrevWeight) / iNumVars;
}


void PickVW1W() {
 
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
  UINT32 iBestVarFlipCount;

  iNumCandidates = 0;
  fBestScore = fTotalWeight;
  iBestVarFlipCount = iStep;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause();
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

    fScore = FLOATZERO;
    
    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        fScore += aClauseWeight[*pClause];
      }
      pClause++;
    }

    /* build candidate list of best vars */

    if (fScore <= fBestScore) {
      if (fScore < fBestScore) {
        iNumCandidates=0;
        fBestScore = fScore;
        aCandidateList[iNumCandidates++] = iVar;
      } else {
	      if(fBestScore == FLOATZERO) {
          aCandidateList[iNumCandidates++] = iVar;
        } else {

          /* if it's a tie and not a 'freebie', 
             prefer variables that have have been 
             flipped less frequently */

	        if(aFlipCounts[iVar] <= iBestVarFlipCount) {
		        if(aFlipCounts[iVar] < iBestVarFlipCount) {
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





void PickVW2W() {
 
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
  FLOAT fCurVW2Score;
  FLOAT fBestVW2Score;

  iNumCandidates = 0;
  fBestScore = fTotalWeight;
  fBestVW2Score = FLOATMAX;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause();
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

    fScore = FLOATZERO;
    
    iVar = GetVarFromLit(*pLit);
    
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        fScore += aClauseWeight[*pClause];
        // POSSIBLE TODO: EARLY EXIT IF FREEBIE WAS PREVIOUSLY FOUND?
      }
      pClause++;
    }

    /* build candidate list of best vars */

    if (fScore == 0) {
      if (fBestScore > 0) {
        iNumCandidates=0;
        fBestScore = 0;
      } 
      aCandidateList[iNumCandidates++] = iVar;
    } else {
      if(fBestScore > 0) {
        fCurVW2Score = fScore + fVW2WeightFactor * (aVW2Weights[iVar] - fVW2WeightMean);

        if(fCurVW2Score <= fBestVW2Score) {
	        if(fCurVW2Score < fBestVW2Score) {
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

  if (fBestScore > 0) {
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


void PickVW1Tabu()
{

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
  UINT32 iBestVarFlipCount;
  UINT32 iTabuCutoff;  


  iNumCandidates = 0;
  iBestScore = iNumClauses;
  iBestVarFlipCount = iStep;

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

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
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
    else {
        for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
      }
      pClause++;
    }

    }



    /* build candidate list of best vars */
     if ((iScore==0)||(aVarLastChange[iVar] < iTabuCutoff)){
    if (iScore <= iBestScore) {
      if (iScore < iBestScore) {
        iNumCandidates=0;
        iBestScore = iScore;
        aCandidateList[iNumCandidates++] = iVar;
      } else {
              if(iBestScore == 0) {
          aCandidateList[iNumCandidates++] = iVar;
        } else {

          /* if it's a tie and not a 'freebie',
             prefer variables that have have been
             flipped less frequently */

                if(aFlipCounts[iVar] <= iBestVarFlipCount) {
                        if(aFlipCounts[iVar] < iBestVarFlipCount) {
                          iNumCandidates=0;
                          iBestVarFlipCount = aFlipCounts[iVar];
                        }
                        aCandidateList[iNumCandidates++] = iVar;
                      }
                  }
            }
    }
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





void PickVW2Tabu() {

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
  FLOAT fCurVW2Score;
  FLOAT fBestVW2Score;
  UINT32 iTabuCutoff;  

  iNumCandidates = 0;
  iBestScore = iNumClauses;
  fBestVW2Score = FLOATMAX;
  
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

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
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
    else {
        for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
      }
      pClause++;
    }

    }


    /* build candidate list of best vars */

    if (iScore == 0) {
      if (iBestScore > 0) {
        iNumCandidates=0;
        iBestScore = 0;
      }
      aCandidateList[iNumCandidates++] = iVar;
    } else {
     if (aVarLastChange[iVar] < iTabuCutoff){
      if(iBestScore > 0) {
        fCurVW2Score = (FLOAT) iScore + fVW2WeightFactor * (aVW2Weights[iVar] - fVW2WeightMean);

        if(fCurVW2Score <= fBestVW2Score) {
                if(fCurVW2Score < fBestVW2Score) {
                  iNumCandidates=0;
                  fBestVW2Score = fCurVW2Score;
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


void CreateAutoVW2Weights() {
  aVW2Weights = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
}

void InitAutoVW2Weights() {
  memset(aVW2Weights,0,(iNumVars+1)*sizeof(FLOAT));
  fVW2WeightMean = FLOATZERO;
  Intensity=1;  fVW2Smooth=0.1;  Interval=1000.0;  Adjustment=1000.0;
  //printf("IINITIALIZED");
}

void UpdateAutoVW2Weights() {
    int I;

  FLOAT fPrevWeight = aVW2Weights[iFlipCandidate];
  aVW2Weights[iFlipCandidate] = (1.0f - fVW2Smooth) * (aVW2Weights[iFlipCandidate] + 1.0f) + (fVW2Smooth * (FLOAT) iStep);
  fVW2WeightMean += (aVW2Weights[iFlipCandidate] - fPrevWeight) / iNumVars;
          if (iStep>Adjustment)
      {  Intensity=1+rand()%6;  I=rand()%7;
         if (I==6) fVW2Smooth =0.0; else fVW2Smooth =pow(10.0,0.0-I)*0.2;
         //printf("\n %d",I);
         //printf("\n%f",fVW2Smooth);
         Interval*=1.1;  Adjustment=iStep+Interval;
      }


}




void PickVW2Automated() {

  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  LITTYPE *pRandomLit;
  UINT32 randomInt;
  LITTYPE *pClause;
  LITTYPE litPick;
  UINT32 iNumOcc;
  FLOAT fCurVW2Score;
  FLOAT fBestVW2Score;
  SINT32 V, BestV, C, BestC, B, BestB;

  iNumCandidates = 0;
  iBestScore = iNumClauses;
  fBestVW2Score = FLOATMAX;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    //iClause = aFalseList[RandomInt(iNumFalse)];
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }


  pLit = pClauseLits[iClause];
  randomInt = RandomInt(iClauseLen);
  pRandomLit = pLit + randomInt;

  BestV = GetVarFromLit(*pRandomLit);
  BestB = 0;

  iNumOcc = aNumLitOcc[GetNegatedLit(*pRandomLit)];
  pClause = pLitClause[GetNegatedLit(*pRandomLit)];
 if(!bPen){
  for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        BestB++;
        // POSSIBLE TODO: EARLY EXIT IF FREEBIE WAS PREVIOUSLY FOUND?
      }
      pClause++;
   }
}
 else{
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        BestB+= aClausePen[*pClause];
        // POSSIBLE TODO: EARLY EXIT IF FREEBIE WAS PREVIOUSLY FOUND?
      }
      pClause++;
   }
}
   //printf("\n %d",BestB);
  if(BestB ==0){
     iFlipCandidate = BestV;
     //printf("\n %u",aBreakCount[BestV]);
     //printf(" I have luckily found a freebie");
     return;
  }

  BestC = aVW2Weights[BestV];
  //printf("\n %d",BestC);
   pLit = pClauseLits[iClause];

  for (j=0;j<iClauseLen;j++) {

     randomInt++;
     if(randomInt>=iClauseLen)
     randomInt =0;

     pRandomLit = pLit + randomInt;


    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values

       note that in this case, score is the breakcount[] */

    B = 0;

    iVar = GetVarFromLit(*pRandomLit);

    iNumOcc = aNumLitOcc[GetNegatedLit(*pRandomLit)];
    pClause = pLitClause[GetNegatedLit(*pRandomLit)];

  if(!bPen){
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        B++;
        // POSSIBLE TODO: EARLY EXIT IF FREEBIE WAS PREVIOUSLY FOUND?
      }
      pClause++;
    }
 }
 else{
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        B+= aClausePen[*pClause];
        // POSSIBLE TODO: EARLY EXIT IF FREEBIE WAS PREVIOUSLY FOUND?
      }
      pClause++;
    }
}




    /* build candidate list of best vars */

    if (B == 0) {
      iFlipCandidate = iVar;
      return;
    }

    C = aVW2Weights[iVar];

    if (B<BestB || C<BestC && (B==BestB || expchance(B-BestB)))
      {  BestV=iVar;  BestB=B;  BestC=C;
      }





  }

 iFlipCandidate = BestV;
 return;
}




char expchance(int X)
{
   if (X>Intensity) X=Intensity;
   if (X<1) return 1;
   X=1<<X;
   if (rand()%X==0) return 1;
   return 0;
}
               
