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
void DistributeDDFW();

UINT32 iDDFWInitWeight;
PROBABILITY iDDFW_TL;

void AddDDFW() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("ddfw","",FALSE,
    "DDFW: Divide and Distribute Fixed Weights",
    "Ishtaiwi, Thornton, Sattar, Pham [CP 05]",
    "PickPAWS,DistributeDDFW,SetupDDFW",
    "DefaultProcedures,Flip+MBPINT+FCL+VIF",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-pflat","flat move probabilty [default %s]","when a local minimum is encountered,~take a 'flat' (sideways) step with probability PR","",&iPAWSFlatMove,0.15);
  AddParmProbability(&pCurAlg->parmList,"-tl","deterministically select neighbour [default %s]","select a random neighbour to distribute weight from with~probability (1-PR)","",&iDDFW_TL,0.99);
  AddParmUInt(&pCurAlg->parmList,"-winit","initial clause weights [default %s]","","",&iDDFWInitWeight,8);
  
  CreateTrigger("DistributeDDFW",PostFlip,DistributeDDFW,"","");
  CreateTrigger("SetupDDFW",PreStart,SetupDDFW,"","");

}

void SetupDDFW() {
  iInitPenaltyINT = iDDFWInitWeight;
}

void DistributeDDFW() {

  UINT32 j;
  UINT32 k;
  UINT32 m;

  UINT32 iCurrentClause;
  LITTYPE *pLit;

  UINT32 *pNeighbourClause;

  UINT32 iSourceClause = 0;
  UINT32 iSourceClausePenalty;

  UINT32 iPenaltyChange;

  BOOL bFoundClause;

  /* only perform distribution for null flips */

  if (iFlipCandidate) {
    return;
  }

  /* for each false clause, distribute penalty from another clause */

  for(j=0;j<iNumFalse;j++) {
    
    iCurrentClause = aFalseList[j];

    bFoundClause = FALSE;
    iSourceClausePenalty = iDDFWInitWeight;

    /* for each literal in the current clause... */

    pLit = pClauseLits[iCurrentClause];
    for (k=0;k<aClauseLen[iCurrentClause];k++) {

      /* check all neighbouring clauses */

      pNeighbourClause = pLitClause[*pLit];
      for (m=0;m<aNumLitOcc[*pLit];m++) {

        /* if the clause is satisfied */

        if (aNumTrueLit[*pNeighbourClause] > 0) {

          /* find the satisfied clause with highest weight */

          if (aClausePenaltyINT[*pNeighbourClause] >= iSourceClausePenalty) {
            iSourceClause = *pNeighbourClause;
            iSourceClausePenalty = aClausePenaltyINT[iSourceClause];
            bFoundClause = TRUE;
          }
        }
        pNeighbourClause++;
      }
      pLit++;
    }

    /* This is not mentioned in the DDFW paper, but was found in the original DDFW code from the authors */

    /* with probability (1-iDDFW_TL), ignore the found clause and choose randomly */

    if (bFoundClause) {
      if (!RandomProb(iDDFW_TL)) {
        bFoundClause = FALSE;
      }
    }

    /* if no clause has been found, keep randomly selecting a clause until 
       a) it is satisfied, and
       b) it has increased weight
    */

    while (!bFoundClause) {
      iSourceClause = RandomInt(iNumClauses);
      if (aNumTrueLit[iSourceClause] > 0) {
        if (aClausePenaltyINT[iSourceClause] >= iDDFWInitWeight) {
          iSourceClausePenalty = aClausePenaltyINT[iSourceClause];
          bFoundClause = TRUE;
        }
      }
    }

    /* the amount of weight to transfer depends on the source clause weight */

    if (iSourceClausePenalty > iDDFWInitWeight) {
      iPenaltyChange = 2;
    } else {
      iPenaltyChange = 1;
    }

    /* transfer the weight between the clauses  */

    aClausePenaltyINT[iCurrentClause] += iPenaltyChange;
    aClausePenaltyINT[iSourceClause] -= iPenaltyChange;

    /* For the current clause, the 'make' score for each variable has to be increased */

    pLit = pClauseLits[iCurrentClause];
    for (k=0;k<aClauseLen[iCurrentClause];k++) {
      aMakePenaltyINT[GetVarFromLit(*pLit)] += iPenaltyChange;
      pLit++;
    }

    /* For the source clause, if it is critically sat, the 'break' has to be decreased */

    if (aNumTrueLit[iSourceClause]==1) {
      aBreakPenaltyINT[aCritSat[iSourceClause]] -= iPenaltyChange;
    }
  }
}

