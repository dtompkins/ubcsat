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

void PickRNovelty();
void PickRNoveltyPlus();

void AddRNovelty() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rnovelty","",FALSE,
    "RNovelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickRNovelty",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  CreateTrigger("PickRNovelty",ChooseCandidate,PickRNovelty,"","");

}

void AddRNoveltyPlus() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rnovelty+","",FALSE,
    "RNovelty+: RNovelty with random walk",
    "Hoos [AAAI 99]",
    "PickRNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
    
  CopyParameters(pCurAlg,"novelty+","",FALSE);

  CreateTrigger("PickRNoveltyPlus",ChooseCandidate,PickRNoveltyPlus,"","");

}


void PickRNoveltyCore() {

  /* see the Novelty() code for general comments */
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;

  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  SINT32 iScoreMargin;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
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
        iScore--;
      }
      pClause++;
    }
    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }
    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
      iYoungestVar = iVar;
    }
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
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar) {
    return;
  }

  /* otherwise, calculate difference between 'best' and 'second best' */

  iScoreMargin = iSecondBestScore - iBestScore;

  /* if novnoise < 0.5 always choose best if the margin is > 1,
     and choose the second best with noise (novnoise * 2) if margin is 1 */

  if ((iNovNoise < 0x7FFFFFFF)) {
    if (iScoreMargin > 1) {
      return;
    }
    if (iScoreMargin == 1) {
      if (RandomProb(iNovNoise << 1)) {
        iFlipCandidate = iSecondBestVar;
      }
      return;
    }
  }

  /* if novnoise >= 0.5 always choose second best if the margin is = 1,
     and choose the second best with noise (1 - novnoise) * 2 if margin is > 1 */

  if (iScoreMargin == 1) {
    iFlipCandidate = iSecondBestVar;
    return;
  } 

  if (RandomProb(((iNovNoise - 0x7FFFFFFF)<<1))) {
    iFlipCandidate = iSecondBestVar;
  }
}

void PickRNovelty()
{
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;

  /* every 100 steps uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if ((iStep % 100) == 0) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
    return;
  }

  /* otherwise, perform a regular RNovelty step */

  PickRNoveltyCore();
}


void PickRNoveltyPlus()
{
 
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
    return;
  }

  /* otherwise, perform a regular RNovelty step */

  PickRNoveltyCore();

}

