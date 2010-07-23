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

UINT32 iTabuTenure;

void PickGSatTabu();
void PickGSatTabuW();

void AddGSatTabu() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gsat-tabu","",FALSE,
    "GSAT-TABU: GSAT with Tabu search",
    "Mazure, Sais, Gregoire [AAAI 97]",
    "PickGSatTabu",
    "DefaultProcedures,Flip+VarScore,VarLastChange",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-tabu","tabu tenure [default %s]","variables flipped within the last INT steps are tabu","",&iTabuTenure,10);

  CreateTrigger("PickGSatTabu",ChooseCandidate,PickGSatTabu,"","");

  pCurAlg = CreateAlgorithm("gsat-tabu","",TRUE,
    "GSAT-TABU: GSAT with Tabu search (weighted)",
    "Mazure, Sais, Gregoire [AAAI 97]",
    "PickGSatTabuW",
    "DefaultProceduresW,Flip+VarScoreW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"gsat-tabu","",FALSE);

  CreateTrigger("PickGSatTabuW",ChooseCandidate,PickGSatTabuW,"","");

}

void PickGSatTabu() {
  
  UINT32 j;
  SINT32 iScore;
  UINT32 iTabuCutoff;

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
  iBestScore = iNumClauses;

  for (j=1;j<=iNumVars;j++) {

    /* check score of all non-tabu variables */

    if (aVarLastChange[j] < iTabuCutoff) { 

      /* use cached value of score */
      
      iScore = aVarScore[j];

      /* build candidate list of best vars */

      if (iScore <= iBestScore) {
        if (iScore < iBestScore) {
          iNumCandidates=0;
          iBestScore = iScore;
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

void PickGSatTabuW() {
  
  UINT32 j;
  FLOAT fScore;
  UINT32 iTabuCutoff;

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
  fBestScore = fTotalWeight;

  for (j=1;j<=iNumVars;j++) {
    
    /* check score of all non-tabu variables */

    if (aVarLastChange[j] < iTabuCutoff) { 
      
      /* use cached value of weighted score */

      fScore = aVarScoreW[j];

      /* build candidate list of best vars */

      if (fScore <= fBestScore) {
        if (fScore < fBestScore) {
          iNumCandidates=0;
          fBestScore = fScore;
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

