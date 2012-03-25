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

void SetupDDFW();
void PickDDFW();
void DistributeDDFW();

UINT32 iDDFWInitWeight;
PROBABILITY iDDFW_TL;

void AddDDFW() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("ddfw","",FALSE,
    "DDFW: Divide and Distribute Fixed Weights",
    "Ishtaiwi, Thornton, Sattar, Pham [CP 05]",
    "PickDDFW,DistributeDDFW,SetupDDFW",
    "DefaultProcedures,Flip+MBPINT+FCL+VIF",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-pflat","flat move probabilty [default %s]","when a local minimum is encountered,~take a 'flat' (sideways) step with probability PR","",&iPAWSFlatMove,0.15);
  AddParmProbability(&pCurAlg->parmList,"-tl","deterministically select neighbour [default %s]","select a random neighbour to distribute weight from with~probability (1-PR)","",&iDDFW_TL,0.99);
  AddParmUInt(&pCurAlg->parmList,"-winit","initial clause weights [default %s]","","",&iDDFWInitWeight,8);
  
  CreateTrigger("PickDDFW",ChooseCandidate,PickDDFW,"","");
  CreateTrigger("DistributeDDFW",PostFlip,DistributeDDFW,"","");
  CreateTrigger("SetupDDFW",PreStart,SetupDDFW,"","");

}

void SetupDDFW() {
  iInitPenaltyINT = iDDFWInitWeight;
}

void PickDDFW() {
  
  UINT32 j;
  UINT32 iVar;
  SINT32 iScore;
  SINT32 iBestScore;
  UINT32 iLoopEnd;

  iNumCandidates = 0;
  iBestScore = 0x7FFFFFFF;

  /* look at all variables that appear in false clauses */

  for (j=0;j<iNumVarsInFalseList;j++) {
    iVar = aVarInFalseList[j];

    /* use cached value of breakcount - makecount */

    iScore = aBreakPenaltyINT[iVar] - aMakePenaltyINT[iVar];

    /* build candidate list of best vars */

    if (iScore <= iBestScore) {
      if (iScore < iBestScore) {
        iNumCandidates = 0;
        iBestScore = iScore;
      }

      /* using the "Monte Carlo" method */

      iLoopEnd = iNumCandidates + aMakeCount[iVar];
      
      if (iLoopEnd >= iMaxCandidates) {
        ReportPrint1(pRepErr,"Unexpected Error: increase iMaxCandidates [%u]\n",iMaxCandidates);
        AbnormalExit();
      }

      while (iNumCandidates < iLoopEnd) {
        aCandidateList[iNumCandidates++] = iVar;
      }
    }
  }

  iFlipCandidate = 0;

  if (iBestScore < 0) {

    /* select flip candidate uniformly from candidate list */
    
    if (iNumCandidates > 1) {
      iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
    } else {
      iFlipCandidate = *aCandidateList;
    }
  } else {
    
    if (iBestScore == 0) {

      /* with probability (iPAWSFlatMove) flip candidate from candidate list,
         otherwise it's a null flip */
    
      if (RandomProb(iPAWSFlatMove)) {
        if (iNumCandidates > 1) {
          iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
        } else {
          iFlipCandidate = *aCandidateList;
        }
      }
    }
  }
}


void DistributeDDFW() {

  UINT32 j;
  UINT32 k;
  UINT32 m;

  UINT32 iCurrentClause;
  LITTYPE *pLit;

  UINT32 *pNeighbourClause;

  UINT32 iSelectedClause = 0;
  UINT32 iSelectedClausePenalty;

  UINT32 iPenaltyChange;

  BOOL bFoundClause;

  /* only perform distribution for null flips */

  if (iFlipCandidate)
    return;

  /* for each false clause, distribute penalty from another clause */

  for(j=0;j<iNumFalse;j++) {
    
    iCurrentClause = aFalseList[j];

    bFoundClause = FALSE;
    iSelectedClausePenalty = iDDFWInitWeight;

    /* for each literal in the current clause... */

    pLit = pClauseLits[iCurrentClause];
    for (k=0;k<aClauseLen[iCurrentClause];k++) {

      /* check all neighbouring clauses for the one with maximum penalty */

      pNeighbourClause = pLitClause[*pLit];
      for (m=0;m<aNumLitOcc[*pLit];m++) {
        if (aNumTrueLit[*pNeighbourClause] > 0) {
          if (aClausePenaltyINT[*pNeighbourClause] >= iSelectedClausePenalty) {
            iSelectedClause = *pNeighbourClause;
            iSelectedClausePenalty = aClausePenaltyINT[iSelectedClause];
            bFoundClause = TRUE;
          }
        }
        pNeighbourClause++;
      }
      pLit++;
    }

    if (bFoundClause) {
      if (!RandomProb(iDDFW_TL)) {
        bFoundClause = FALSE;
      }
    } 

    while (!bFoundClause) {
      iSelectedClause = RandomInt(iNumClauses);
      if (aNumTrueLit[iSelectedClause] > 0) {
        if (aClausePenaltyINT[iSelectedClause] >= iDDFWInitWeight) {
          iSelectedClausePenalty = aClausePenaltyINT[iSelectedClause];
          bFoundClause = TRUE;
        }
      }
    }

    if (iSelectedClausePenalty > iDDFWInitWeight) {
      iPenaltyChange = 2;
    } else {
      iPenaltyChange = 1;
    }

    aClausePenaltyINT[iCurrentClause] += iPenaltyChange;
    aClausePenaltyINT[iSelectedClause] -= iPenaltyChange;

    /* update cached values */

    pLit = pClauseLits[iCurrentClause];
    for (k=0;k<aClauseLen[iCurrentClause];k++) {
      aMakePenaltyINT[GetVarFromLit(*pLit)] += iPenaltyChange;
      pLit++;
    }

    if (aNumTrueLit[iSelectedClause]==1) {
      aBreakPenaltyINT[aCritSat[iSelectedClause]] -= iPenaltyChange;
    }
  }

}

