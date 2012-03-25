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
void PickRNoveltyPlusW();
void PickRNoveltyCoreW();

void AddRNovelty() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rnovelty","",FALSE,
    "RNovelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickRNovelty",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  CreateTrigger("PickRNovelty",ChooseCandidate,PickRNovelty,"","");

   pCurAlg = CreateAlgorithm("rnovelty","",TRUE,
    "RNovelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickRNoveltyCoreW",
    "DefaultProcedures,Flip+FalseClauseListW,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE,0);

  CreateTrigger("PickRNoveltyCoreW",ChooseCandidate,PickRNoveltyCoreW,"","");

}

void AddRNoveltyPlus() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rnovelty+","",FALSE,
    "RNovelty+: RNovelty with random walk",
    "Hoos [AAAI 99]",
    "PickRNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
    
  CopyParameters(pCurAlg,"novelty+","",FALSE,0);

  CreateTrigger("PickRNoveltyPlus",ChooseCandidate,PickRNoveltyPlus,"","");


   pCurAlg = CreateAlgorithm("rnovelty+","",TRUE,
    "RNovelty+: RNovelty with random walk",
    "Hoos [AAAI 99]",
    "PickRNoveltyPlusW",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
    
  CopyParameters(pCurAlg,"novelty+","",FALSE,0);

  CreateTrigger("PickRNoveltyPlusW",ChooseCandidate,PickRNoveltyPlusW,"","");
}


void PickRNoveltyCore()
{
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  SINT32 iScoreWithoutPen;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;

  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  SINT32 iScoreMargin;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;

  UINT32 iTabuCutoff;   
       if(bTabu)
  {
      if (iStep > iTabuTenure) {
       iTabuCutoff = iStep - iTabuTenure;
      if (iVarLastChangeReset > iTabuCutoff) {
       iTabuCutoff = iVarLastChangeReset;
     }
   } else {
    iTabuCutoff = 1;
   }
  }

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = SelectClause();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iScore = 0;

    iVar = GetVarFromLit(*pLit);
   
    if(!bTabu)
    { 
    iScore = 0;
    iScoreWithoutPen = 0;
    iVar = GetVarFromLit(*pLit);
    /*The computation of score is necessary only when
      the varscore trigger is not activated. Also,
      we need to check whether clause penalty is
      activated. If it is activated then, we need
      to take account of the clause weight*/
    if(!bPromisingList && !bPen){
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
        iScoreWithoutPen --;
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
        iScoreWithoutPen++;
      }
      pClause++;
    }
    }else if(!bPromisingList && bPen){
    //Promising List is not used but Clause Penalty is activated
        iNumOcc = aNumLitOcc[*pLit];
        pClause = pLitClause[*pLit];
        for (i=0;i<iNumOcc;i++) {
         if (aNumTrueLit[*pClause]==0) {
          iScore -= aClausePen[*pClause];
          iScoreWithoutPen--;
         }
        pClause++;
      }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
        iScoreWithoutPen++;
      }
      pClause++;
    }


   } else {

   iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
   iScoreWithoutPen = aVarScore[iVar];
  }



    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }
   } 
  else{
       iScore = 0;
    iScoreWithoutPen = 0;
    iVar = GetVarFromLit(*pLit);
    /*The computation of score is necessary only when
      the varscore trigger is not activated. Also,
      we need to check whether clause penalty is
      activated. If it is activated then, we need
      to take account of the clause weight*/
    if(!bPromisingList && !bPen){
    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iScore--;
        iScoreWithoutPen --;
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
        iScoreWithoutPen++;
      }
      pClause++;
    }
    }else if(!bPromisingList && bPen){
    //Promising List is not used but Clause Penalty is activated
        iNumOcc = aNumLitOcc[*pLit];
        pClause = pLitClause[*pLit];
        for (i=0;i<iNumOcc;i++) {
         if (aNumTrueLit[*pClause]==0) {
          iScore -= aClausePen[*pClause];
          iScoreWithoutPen--;
         }
        pClause++;
      }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore+= aClausePen[*pClause];
        iScoreWithoutPen++;
      }
      pClause++;
    }


   } else {

   iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
   iScoreWithoutPen = aVarScore[iVar];
  }



    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */
 if((aVarLastChange[iVar] < iTabuCutoff)||(iScore ==0)){
 
    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }

    }
   }
    pLit++;
  }
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, calculate difference between 'best' and 'second best' */

  iScoreMargin = iSecondBestScore - iBestScore;

  /* if novnoise < 0.5 always choose best if the margin is > 1,
     and choose the second best with noise (novnoise * 2) if margin is 1 */

  if ((iNovNoise < 0x7FFFFFFF)) {
    if (iScoreMargin > 1)
      return;
    if (iScoreMargin == 1) {
      if (RandomProb(iNovNoise << 1))
        iFlipCandidate = iSecondBestVar;
      return;
    }
  }

  /* if novnoise >= 0.5 always choose second best if the margin is = 1,
     and choose the second best with noise (1 - novnoise) * 2 if margin is > 1 */

  if (iScoreMargin == 1) {
    iFlipCandidate = iSecondBestVar;
    return;
  } 

  if (RandomProb(((iNovNoise - 0x7FFFFFFF)<<1)))
    iFlipCandidate = iSecondBestVar;
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





void PickRNoveltyCoreW()
{
 
  UINT32 i;
  UINT32 j;
  FLOAT  fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;

  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  FLOAT  fSecondBestScore;
  FLOAT  fScoreMargin;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  fBestScore = fTotalWeight;
  fSecondBestScore = fTotalWeight;

  /* select an unsatisfied clause uniformly at random */

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

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    fScore = FLOATZERO;

    iVar = GetVarFromLit(*pLit);

    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        fScore -= aClauseWeight[*pClause];
      }
      pClause++;
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        fScore += aClauseWeight[*pClause];
      }
      pClause++;
    }

    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((fScore < fBestScore) || ((fScore == fBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      fSecondBestScore = fBestScore;
      fBestScore = fScore;
    } else if ((fScore < fSecondBestScore) || ((fScore == fSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      fSecondBestScore = fScore;
    }

    pLit++;
  }
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, calculate difference between 'best' and 'second best' */

  fScoreMargin = fSecondBestScore - fBestScore;

  /* if novnoise < 0.5 always choose best if the margin is > 1,
     and choose the second best with noise (novnoise * 2) if margin is 1 */

  if ((iNovNoise < 0x7FFFFFFF)) {
    if (fScoreMargin > 1)
      return;
    if (fScoreMargin == 1) {
      if (RandomProb(iNovNoise << 1))
        iFlipCandidate = iSecondBestVar;
      return;
    }
  }

  /* if novnoise >= 0.5 always choose second best if the margin is = 1,
     and choose the second best with noise (1 - novnoise) * 2 if margin is > 1 */

  if (fScoreMargin == 1) {
    iFlipCandidate = iSecondBestVar;
    return;
  } 

  if (RandomProb(((iNovNoise - 0x7FFFFFFF)<<1)))
    iFlipCandidate = iSecondBestVar;
}


void PickRNoveltyPlusW()
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

  PickRNoveltyCoreW();

}



