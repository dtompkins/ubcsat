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

void PickSatenstein();
void PickSatensteinW();
void NoveltyProm();
void NoveltyPromisingProm();
void NoveltyPlusPlusProm();
void CreateDecPromVarsW();
void InitDecPromVarsW();
void UpdateDecPromVarsW();
UINT32 TieBreaking();

void EnableDisableTrigger();
BOOL CheckIfFreebie(UINT32);

UINT32 iHeuristic;
UINT32 iScoringMeasure;
UINT32 iSmoothingScheme;
UINT32 iDecStrategy;
UINT32 iRandomStep;
UINT32 iTieBreaking;
UINT32 iBestPromVar;
UINT32 iSecondBestPromVar;
UINT32 iLeastRecentlyFlippedPromVar;
void Smooth();
void UpdateClauseWeight();
UINT32 iUpdateSchemePromList;
UINT32 iAdaptiveNoiseScheme;
BOOL *aIsDecPromVar;
BOOL bNoise;
BOOL bPromNoise;
BOOL bPromisingList;
BOOL bPerformNoveltyAlternate;
BOOL bSingleClause;
BOOL bPerformRandomWalk;
BOOL bTabu;
BOOL bVarInFalse;
PROBABILITY iRWp;
PROBABILITY iRDp;
PROBABILITY iRWpWalk;
PROBABILITY iRFp;

PROBABILITY iPromNovNoise;
BOOL bPerformNovelty;
BOOL bNoveltyVariant;
UINT32 *aDecPromVarsListW;
UINT32 *aDecPromVarsListPosW;
UINT32 iNumDecPromVarsW;
BOOL *aIsDecPromVarW;

PROBABILITY iPromDp;
PROBABILITY iPromWp;

void AddSatenstein() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("satenstein","",FALSE,
    "Generalized local search algorithm",
    "Yet to be published",
    "PickSatenstein,InitRSAPS,PostFlipRSAPS,PostFlipSAPS,PostFlipPAWS",
    "DefaultProcedures,AdaptPromNoveltyNoise,Flip+TrackChanges+FCL,Flip+FalseClauseList,DecPromVars,FalseClauseList,VarLastChange,MakeBreak,AdaptNoveltyPlusNoise,FlipCounts,LookAhead,EnableDisableTrigger,VarInFalse,VarScore,VW2Weights,Flip+MBPFL+FCL+VIF,ClauseVarFlipCounts,AutoVW2Weights,Flip+TrackChanges+FCLPen,DecPromPenVars,ClausePen,VarPenScore,WeightedList,VarsShareClauses,Flip+MBPINT+FCL+VIF,PenClauseList,SpecialUpdate,SpecialUpdateMakeBreak",
    "default","default");

    CopyParameters(pCurAlg,"novelty+","",FALSE,0);

    //CopyParameters(pCurAlg,"novelty+p","",FALSE,3);
    //CopyParameters(pCurAlg,"novelty++","",FALSE,6);
    //CopyParameters(pCurAlg,"novelty++p","",FALSE,9);
	  //CopyParameters(pCurAlg,"novelty++'","",FALSE,12);
    CopyParameters(pCurAlg,"walksat-tabu","",FALSE,3);
    CopyParameters(pCurAlg,"vw2","",FALSE,4);


    CreateTrigger("EnableDisableTrigger",PreStart,EnableDisableTrigger,"","");
  AddParmProbability(&pCurAlg->parmList,"-dp","diversification probability [default %s]","with probability dp, select the least recently flipped~variable from an unsat clause","",&iDp,0.05);
  AddParmProbability(&pCurAlg->parmList,"-promdp","diversification probability [default %s]","with probability promdp, select the least recently flipped~variable from the promising list","",&iPromDp,0.05);
  AddParmProbability(&pCurAlg->parmList,"-promwp","diversification probability [default %s]","with probability promwp, select a random variable from the promising list","",&iPromWp,0.01);

   AddParmProbability(&pCurAlg->parmList,"-rwp","random walk probability [default %s]","with probability rwp, randomly pick a variable from an unsat clause","",&iRWp,0.01);
   AddParmProbability(&pCurAlg->parmList,"-rwpwalk","random walk probability [default %s]","with probability rwpwalk, randomly pick a variable from an unsat clause","",&iRWpWalk,0.50);
   AddParmProbability(&pCurAlg->parmList,"-rdp","random walk probability [default %s]","with probability rdp, select the least recently flipped~variable from an unsat clause","",&iRDp,0.05);
   AddParmProbability(&pCurAlg->parmList,"-rfp","random walk probability [default %s]","with probability Dp, select the least number of times flipped~variable from an unsat clause","",&iRFp,0.05);
  AddParmBool(&pCurAlg->parmList,"-adaptive","Adaptive strategy [default %s]","if true tunes noise parameter adaptively","",&bNoise, TRUE);
  AddParmBool(&pCurAlg->parmList,"-adaptiveprom","Adaptive strategy [default %s]","if true tunes noise parameter for promising list adaptively","",&bPromNoise, FALSE);

  AddParmBool(&pCurAlg->parmList,"-promisinglist","Promising list [default %s]","if true performs module 2 of SATenstein-LS","",&bPromisingList, TRUE);
  AddParmBool(&pCurAlg->parmList,"-singleclause","Single Clause [default %s]","if true performs module 3 of SATenstein-LS","",&bSingleClause, TRUE);
  AddParmBool(&pCurAlg->parmList,"-performrandomwalk","Perform random-walk [default %s]","if true perform module 1 of SATenstein-LS","",&bPerformRandomWalk, FALSE);
  AddParmBool(&pCurAlg->parmList,"-performalternatenovelty","Perform Alternate Novelty [default %s]","if true perform Novelty with additional flatmove","",&bPerformNoveltyAlternate, FALSE);

  AddParmBool(&pCurAlg->parmList,"-tabusearch","Perform random-walk [default %s]","if true cheks tabu-status of a variable","",&bTabu, FALSE);
  AddParmBool(&pCurAlg->parmList,"-clausepen","Perform random-walk [default %s]","if true assigns clause penalty to each of the clauses","",&bPen, FALSE);
  AddParmBool(&pCurAlg->parmList,"-varinfalse","Perform random-walk [default %s]","if true looks only at variables found in false clause","",&bVarInFalse, TRUE);

  AddParmUInt(&pCurAlg->parmList,"-heuristic","Heuristic choice [default %s]","selects among a list of available heuristic options","",&iHeuristic, 1);
  AddParmUInt(&pCurAlg->parmList,"-randomwalk","Random step [default %s]","specifies option for module 1 in SATenstein-LS","",&iRandomStep, 1);
  AddParmUInt(&pCurAlg->parmList,"-tiebreaking","Tie Breaking [default %s]","specifies how tiebreaking is performed","",&iTieBreaking, 1);
  AddParmUInt(&pCurAlg->parmList,"-updateschemepromlist","Updation scheme for promising list [default %s]","specifies the way promising list is updated","",&iUpdateSchemePromList, 1);
    AddParmUInt(&pCurAlg->parmList,"-adaptivenoisescheme","Updation scheme for promising list [default %s]","specifies the way promising list is updated","",&iAdaptiveNoiseScheme, 1);

  AddParmUInt(&pCurAlg->parmList,"-scoringmeasure","Scoring measure [default %s]","selects among a list of available scoring measures","",&iScoringMeasure, 1);
   AddParmUInt(&pCurAlg->parmList,"-smoothingscheme","Smoothing scheme [default %s]","selects among a list of smoothingscheme","",&iSmoothingScheme, 1);
  AddParmUInt(&pCurAlg->parmList,"-decreasingvariable","Decreasing variable selection choice [default %s]","selects among a list of available options for selecting a decreasing variable","",&iDecStrategy, 2);
  AddParmUInt(&pCurAlg->parmList,"-phi","Parameter for adaptive tuning [default %s]","parameter for adaptive tuning","",&iInvPhi, 5);
  AddParmUInt(&pCurAlg->parmList,"-theta","Parameter for adaptive tuning [default %s]","parameter for adaptive tuning","",&iInvTheta, 6);
  AddParmUInt(&pCurAlg->parmList,"-promphi","Parameter for adaptive tuning [default %s]","parameter for adaptive tuning","",&iPromInvPhi, 5);
  AddParmUInt(&pCurAlg->parmList,"-promtheta","Parameter for adaptive tuning [default %s]","parameter for adaptive tuning","",&iPromInvTheta, 6);

  AddParmBool(&pCurAlg->parmList,"-adaptwalkprob","Perform random-walk [default %s]","if true picks up a clause randomly and performs random-walk","",&bAdaptWalkProb, FALSE);
  AddParmBool(&pCurAlg->parmList,"-adaptpromwalkprob","Perform random-walk [default %s]","if true picks up a clause randomly and performs random-walk","",&bAdaptPromWalkProb, FALSE);

   AddParmFloat(&pCurAlg->parmList,"-alpha","scaling parameter alpha [default %s]","when a local minimum is encountered,~multiply all unsatisfied cluase penalties by FL","",&fAlpha,1.3f);
  AddParmFloat(&pCurAlg->parmList,"-rho","smoothing parameter rho [default %s]","when smoothing occurs, smooth penalties by a factor of FL","",&fRho,0.8f);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","when a local minimum is encountered,~smooth penalties with probability PR","",&iPs,0.40);
  AddParmProbability(&pCurAlg->parmList,"-promnovnoise","smooth probabilty [default %s]","when a local minimum is encountered,~smooth penalties with probability PR","",&iPromNovNoise,0.50);

  AddParmFloat(&pCurAlg->parmList,"-sapsthresh","threshold for detecting local minima [default %s]","the algorithm considers a local minima to occur when no~improvement greater than FL is possible~the default reflects the value used in SAPS 1.0","",&fPenaltyImprove,-1.0e-01f);
  AddParmUInt(&pCurAlg->parmList,"-maxinc","frequency of penalty reductions [default %s]","reduce (smooth) all clause penalties by 1~after every INT increases","",&iPAWSMaxInc,10);
  AddParmProbability(&pCurAlg->parmList,"-pflat","flat move probabilty [default %s]","when a local minimum is encountered,~take a 'flat' (sideways) step with probability PR","",&iPAWSFlatMove,0.15);

 
  CreateTrigger("PickSatenstein",ChooseCandidate,PickSatenstein,"","");
  




  pCurAlg = CreateAlgorithm("satenstein","",TRUE,
    "Generalized local search algorithm",
    "Yet to be published",
    "PickSatensteinW",
    "DefaultProceduresW,Flip+TrackChanges+FCL+W,DecPromVarsW,FalseClauseList,VarLastChange,MakeBreakW",
    "default_w","default");

  CopyParameters(pCurAlg,"satenstein","",FALSE,0);

  CreateTrigger("PickSatensteinW",ChooseCandidate,PickSatensteinW,"","");
  
  CreateTrigger("CreateDecPromVarsW",CreateStateInfo,CreateDecPromVarsW,"CreateTrackChangesW","");
  CreateTrigger("InitDecPromVarsW",InitStateInfo,InitDecPromVarsW,"InitTrackChangesW","");
  CreateTrigger("UpdateDecPromVarsW",UpdateStateInfo,UpdateDecPromVarsW,"UpdateTrackChangesW","");
  CreateContainerTrigger("DecPromVarsW","CreateDecPromVarsW,InitDecPromVarsW,UpdateDecPromVarsW,MakeBreakW,AdaptNoveltyPlusNoiseW,FlipCounts,LookAhead,EnableDisableTrigger,VW2Weights,VarInFalse,VarScoreW,VarLastChange");
  
}

/* This part of code manages all conflicting triggers of SATenstein-LS.
   Once the parameter configuration for a given instantiation of SATenstein-LS
   is obtained. This part of the code figures out triggers that need to 
   be disabled and enabled.
*/

void EnableDisableTrigger() {

  BOOL bNotThreeSat;
  int j;
  if (bNoise) {
    ActivateTriggers("AdaptNoveltyPlusNoise,InitAdaptNoveltyPlusNoise");
   }
   else {
    DeActivateTriggers("AdaptNoveltyPlusNoise,InitAdaptNoveltyPlusNoise");
  }

    if (bPromNoise) {
    ActivateTriggers("AdaptPromNoveltyNoise,InitAdaptPromNoveltyNoise");
   }
   else {
    DeActivateTriggers("AdaptPromNoveltyNoise,InitAdaptPromNoveltyNoise");
  }



//Trigger VW2Weight is required only iHeuristic is 8 


  if((((iHeuristic == 8)||(iHeuristic ==16))&&((bPromisingList)||(bSingleClause)))||(bPromisingList && iDecStrategy == 4)||(bPerformRandomWalk && iRandomStep ==5)||((iTieBreaking == 4)&&(bPromisingList &&((iDecStrategy ==1)||(iDecStrategy ==4))))){
    if(bNoise)
    ActivateTriggers("AutoVW2Weights");
    else
    ActivateTriggers("VW2Weights");
    
  } else {
    DeActivateTriggers("VW2Weights,AutoVW2Weights");
  }

 //Lookahead is never required if promising variant is not used 

 if((iHeuristic > 9)&&((bPromisingList)||(bSingleClause))){
   ActivateTriggers("LookAhead");
 } else {
  DeActivateTriggers("LookAhead");
 }


 //DecPromVar is used if promising list 
 if((bPromisingList)||((bSingleClause)&&(iHeuristic > 9))){
 if(bPen){ 
 ActivateTriggers("DecPromPenVars");
 DeActivateTriggers("DecPromVars");
 }
 else{
 ActivateTriggers("DecPromVars");
 DeActivateTriggers("DecPromPenVars");
}
} else {
  DeActivateTriggers("DecPromVars,DecPromPenVars");
 } 


if((bPromisingList)||((bSingleClause)&&(iHeuristic > 9))){
if(!bPen){
ActivateTriggers("VarScore");
DeActivateTriggers("VarPenScore");
}
else{
ActivateTriggers("VarPenScore");
DeActivateTriggers("VarScore");
}
}else {
DeActivateTriggers("VarScore,VarPenScore");
}


 if(((bPerformRandomWalk)&&(iRandomStep ==2))||((bPromisingList)||((bSingleClause)&&(iHeuristic > 9)))){
     if(bPen){
     ActivateTriggers("Flip+TrackChanges+FCLPen");
     DeActivateTriggers("Flip+TrackChanges+FCL");
     }
     else{
     ActivateTriggers("Flip+TrackChanges+FCL");
     DeActivateTriggers("Flip+TrackChanges+FCLPen");
     }
     DeActivateTriggers("Flip+FalseClauseList");
     
}else{
DeActivateTriggers("Flip+TrackChanges+FCL,Flip+TrackChanges+FCLPen");

ActivateTriggers("Flip+FalseClauseList");

}

/*SAPS and PAWS need some disjoint set of triggers that other algorithms don't need
  so they are treated separately. */
if(bPen&&(!bPromisingList)&&(!bSingleClause)){
 DeActivateTriggers("SpecialUpdate,SpecialUpdateMakeBreak");
 if(iSmoothingScheme == 2){

  ActivateTriggers("VarInFalse,MakeBreak,Flip+TrackChanges+FCL,PostFlipPAWS");
  DeActivateTriggers("Flip+FalseClauseList,Flip+MBPFL+FCL+VIF,PostFlipSAPS,PostFlipRSAPS");
 }

 if(iSmoothingScheme == 1){
  ActivateTriggers("VarInFalse,MakeBreak,Flip+TrackChanges+FCL");
  DeActivateTriggers("Flip+FalseClauseList,,Flip+MBPINT+FCL+VIF,PostFlipPAWS");
   if(bNoise){
    DeActivateTriggers("PostFlipSAPS,AdaptNoveltyPlusNoise,InitAdaptNoveltyPlusNoise");
    ActivateTriggers("InitRSAPS,PostFlipRSAPS");
   }
  else
   DeActivateTriggers("PostFlipRSAPS,InitRSAPS");
  }
}
else{

DeActivateTriggers("Flip+MBPINT+FCL+VIF,PenClauseList,PostFlipPAWS,Flip+MBPFL+FCL+VIF,PostFlipSAPS,PostFlipRSAPS");

  if((!bSingleClause && !bPen && !bPromisingList)||((bPerformRandomWalk)&&(iRandomStep ==2))){
   if(((bPerformRandomWalk)&&(iRandomStep ==2))||(bVarInFalse)){
    DeActivateTriggers("SpecialUpdateMakeBreak");
  } else {
    DeActivateTriggers("SpecialUpdate");
   }
 } 
else{
  DeActivateTriggers("SpecialUpdate,SpecialUpdateMakeBreak");
}

}


if((bPerformRandomWalk && iRandomStep ==4)||(iTieBreaking == 3)||((bPromisingList)&&(iDecStrategy == 5))||(((iHeuristic == 7)||(iHeuristic >=  14))&&((bPromisingList)||(bSingleClause)))||((!bSingleClause)&&(iScoringMeasure==3))){
ActivateTriggers("FlipCounts");
} else {
DeActivateTriggers("FlipCounts");
}

if(bPen && (bPromisingList || bSingleClause) && (iSmoothingScheme == 2))
{bNotThreeSat = FALSE;
 for (j=0;j<iNumClauses;j++) {
      if (aClauseLen[j]!=3) {
         bNotThreeSat = TRUE;
      }
    }
 
if(bNotThreeSat == TRUE)
{
 iPs = -1;  


}
else{
iPs =  1717986918;

}

}


/*If Promising list is not true 
  and heuristic is 10,11,12,13 and 15
  then updatescheme is hard coded back to 14 */

if(!bPromisingList &&((iHeuristic ==10)||(iHeuristic ==11) ||(iHeuristic ==12) ||(iHeuristic ==13)||(iHeuristic == 15)))
{
 iUpdateSchemePromList = 2;
}

if((iSelectClause == 3)||(iSelectClause == 4)||(iSelectClause == 5)||(iSelectClause == 6)){
ActivateTriggers("ClauseVarFlipCounts");
}
else
{
DeActivateTriggers("ClauseVarFlipCounts");
}


}


void PickSatenstein() {

  UINT32 j,i,k;
  UINT32 iVar;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iBestVarFlipCount;
  FLOAT fVWWeight;
  FLOAT fBestVWWeight;
  SINT32 iScore =0;
  SINT32 iBestScore;
  BOOL FreebieExist;
  BOOL CheckFreebie;
  BOOL bTestBool;
  LITTYPE litPick;
  LITTYPE *pLit;  
  double dRunTime;
  UINT32 iLastChange;
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

//  printf("%f ",fSumClauseVarFlipCount);
  bPerformNovelty = TRUE;
  if(bPerformRandomWalk)
  {
     
    

      switch(iRandomStep){
    

      case 1:
        if(RandomProb(iRWp)){
        if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
      bPerformNovelty = FALSE;
      return;
      } else {
      iFlipCandidate = 0;
      }
        }
     
      break;

      case 2:
      if(RandomProb(iRWpWalk))
      {
      if (iNumVarsInFalseList) {
      iFlipCandidate = aVarInFalseList[RandomInt(iNumVarsInFalseList)];
      bPerformNovelty = FALSE;
      return;
      } else {
      iFlipCandidate = 0;
      }
      

      }
      break;

      case 3:
      if(RandomProb(iRDp))
      {
      if (iNumFalse) {

      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];

      pLit = pClauseLits[iClause];

      iFlipCandidate = GetVarFromLit(*pLit);

      pLit++;

      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }
       pLit++;
      }
     bPerformNovelty = FALSE;
     return;
    } else {
      iFlipCandidate = 0;
    }
   }

      break;

      case 4:
      if(RandomProb(iRFp))
      {
      if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];

      pLit = pClauseLits[iClause];

      iFlipCandidate = GetVarFromLit(*pLit);

      pLit++;

      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        if (aFlipCounts[iVar] < aFlipCounts[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }

        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
      bPerformNovelty =FALSE;
      return;
      }
      break;

      case 5:
      if(RandomProb(iRFp))
      {
      if (iNumFalse) {
      iClause = SelectClause();
      iClauseLen = aClauseLen[iClause];

      pLit = pClauseLits[iClause];

      iFlipCandidate = GetVarFromLit(*pLit);

      pLit++;

      for (j=1;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);

        if (aVW2Weights[iVar] < aVW2Weights[iFlipCandidate]) {
          iFlipCandidate = iVar;
        }

        pLit++;
      }
    } else {
      iFlipCandidate = 0;
    }
      bPerformNovelty =FALSE;
      return;
      }
      break;
     

      }


 
  }

  
  if(bPromisingList) {
  bPerformNovelty = TRUE;
  if (iNumDecPromVars > 0 ) {

    
	  switch (iDecStrategy){

	  case 1:/* Checks for freebie (breakcount 0) variables in the
                    promising decreasing variable stack. If there are 
                    freebies then selects among them according to the
                    tie-breaking scheme. If there is not any free-bie
                    randomly picks one variable from the promising 
                    decreasing variable list. */

                    iNumCandidates = 0;
                    FreebieExist = FALSE;
                    iBestScore = iNumFalse;  //CWBIG;    //BIG;
                    iLastChange = iStep;
                    i = -1;
                    k = 0;
                    for (j = 0; j < iNumDecPromVars; j++) {
                         iVar = aDecPromVarsList[j];
                         iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
                         if (iScore < 0) {
                          if(CheckIfFreebie(iVar)){
                           if(iNumCandidates ==0){
                            FreebieExist = TRUE;
                           }
                          aCandidateList[iNumCandidates++] = iVar;
                         }
                       } else {
                            i = j;
                            aIsDecPromVar[iVar]=FALSE;
                            break;
                           }
                    }
                    j++;
                    if (i != -1) {
                     for (j = i+1; j < iNumDecPromVars; j++) {
                      iVar = aDecPromVarsList[j];
                      iScore = aVarScore[iVar];
                      if (iScore < 0) {
                       aDecPromVarsListPos[iVar] = i;
                       aDecPromVarsList[i++] =iVar;
                          if(CheckIfFreebie(iVar)){
                           if(iNumCandidates ==0){
                            FreebieExist = TRUE;
                           }
                          aCandidateList[iNumCandidates++] = iVar;
                       }
                      } else {
                         aIsDecPromVar[iVar] = FALSE;
                        }
                      }
                     iNumDecPromVars = i;
                    }
      
                  if(FreebieExist == TRUE){
                   if (iNumCandidates > 1)
                    iFlipCandidate = TieBreaking();
                   else
                    iFlipCandidate = aCandidateList[0];
                  }else {
                    if(iNumDecPromVars > 0) {
                     if(iNumDecPromVars == 1)
                      iFlipCandidate = aDecPromVarsList[0];
                     else
                      iFlipCandidate = aDecPromVarsList[RandomInt(iNumDecPromVars)];
                     }
                 }
                break;

	 case 2: /* Flip the variable with highest score 
                    breaking ties in favor of the least 
                    recently flipped variable. This strategy
                    is taken by G2WSAT, GNovelty+, adaptG2WSAT
                    preliminary version. */
 
                    iBestScore = iNumFalse;  //CWBIG;    //BIG;
                    iLastChange = iStep;
                    i = -1;
                    k = 0;
                    for (j = 0; j < iNumDecPromVars; j++) {
                         iVar = aDecPromVarsList[j];
                         if(!bPen)
                         iScore = aVarScore[iVar];
                         else
                         iScore =aVarPenScore[iVar];
                         if (iScore < 0) {
                          if (iScore < iBestScore) {
                           iBestScore = iScore;
                           iLastChange = aVarLastChange[iVar];
                           iFlipCandidate = iVar;
                        } else if (iScore == iBestScore) {
                           if (aVarLastChange[iVar] < iLastChange) {
                            iLastChange = aVarLastChange[iVar];
                            iFlipCandidate = iVar;
                           }
                          }
                         } else {
                            i = j;          
                            aIsDecPromVar[iVar]=FALSE;
                            break;
                           }          
                    }
                    j++;
                    if (i != -1) {
                     for (j = i+1; j < iNumDecPromVars; j++) {
                      iVar = aDecPromVarsList[j];
                      if(!bPen)
                      iScore = aVarScore[iVar];
                      else
                      iScore = aVarPenScore[iVar];
                      if (iScore < 0) {
                       aDecPromVarsListPos[iVar] = i;
                       aDecPromVarsList[i++] =iVar;
                      if (iScore < iBestScore) {
                       iBestScore = iScore;
                       iLastChange = aVarLastChange[iVar];
                       iFlipCandidate = iVar;
                      } else if (iScore == iBestScore) {
                         if (aVarLastChange[iVar] < iLastChange) {
                          iLastChange = aVarLastChange[iVar];
                          iFlipCandidate =iVar;
                         }
                       }
                      } else {
                         aIsDecPromVar[iVar] = FALSE;
                        }
                      }        
                     iNumDecPromVars = i;
                    }    
		 break;

	 case 3: /* Selects the least recently flipped variable
                    from the promising decreasing variable list.
                    This strategy is present in adaptG2WSAT0, 
                    adaptG2WSAT+ and adaptG2WSAT+p. */ 
                    iBestScore = iNumFalse;  //CWBIG;    //BIG;
                    iLastChange = iStep;
                    i = -1;
                    k = 0;
                    for (j = 0; j < iNumDecPromVars; j++) {
                         iVar = aDecPromVarsList[j];
                         iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
                         if (iScore < 0) {
                           if (aVarLastChange[iVar] < iLastChange) {
                            iLastChange = aVarLastChange[iVar];
                            iFlipCandidate = iVar;
                           }
                          }
                       else {
                            i = j;
                            aIsDecPromVar[iVar]=FALSE;
                            break;
                           }
                    }
                    j++;
                    if (i != -1) {
                     for (j = i+1; j < iNumDecPromVars; j++) {
                      iVar = aDecPromVarsList[j];
                      iScore = aVarScore[iVar];
                      if (iScore < 0) {
                       aDecPromVarsListPos[iVar] = i;
                       aDecPromVarsList[i++] =iVar;
                       if (aVarLastChange[iVar] < iLastChange) {
                        iLastChange = aVarLastChange[iVar];
                        iFlipCandidate =iVar;
                        }
                       }
                    else {
                     aIsDecPromVar[iVar] = FALSE;
                    }
                  }
                  iNumDecPromVars = i;
                }

	        break;

	 case 4:  /*Pick the variable that has been flipped least number of times 
                    Any remaining ties are broken according to the tie-breaking
                    policy. */ 
		    iNumCandidates = 0;
		    fBestVWWeight = (FLOAT)iStep;
                    iLastChange = iStep;
                    i = -1;
                    k = 0;
                    for (j = 0; j < iNumDecPromVars; j++) {
                         iVar = aDecPromVarsList[j];
                         iScore = bPen ? aVarPenScore[iVar] :aVarScore[iVar];
                         if (iScore < 0) {
                          if(aVW2Weights[iVar] <= fBestVWWeight) {
                           if(aVW2Weights[iVar] < fBestVWWeight) {
                            iNumCandidates=0;
                            fBestVWWeight = aVW2Weights[iVar];
                           }
                           aCandidateList[iNumCandidates++] = iVar;
                        }
                       }
                       else {
                            i = j;
                            aIsDecPromVar[iVar]=FALSE;
                            break;
                           }
                    }
                    j++;
                    if (i != -1) {
                     for (j = i+1; j < iNumDecPromVars; j++) {
                      iVar = aDecPromVarsList[j];
                      iScore = aVarScore[iVar];
                      if (iScore < 0) {
                       aDecPromVarsListPos[iVar] = i;
                       aDecPromVarsList[i++] =iVar;
                          if(aVW2Weights[iVar] <= fBestVWWeight) {
                           if(aVW2Weights[iVar] < fBestVWWeight) {
                            iNumCandidates=0;
                            fBestVWWeight = aVW2Weights[iVar];
                           }
                           aCandidateList[iNumCandidates++] = iVar;
                        }

                     }
                    else {
                     aIsDecPromVar[iVar] = FALSE;
                    }
                  }
                  iNumDecPromVars = i;
                }

		 
                if (iNumCandidates > 1)
	         iFlipCandidate = TieBreaking(); 
	        else
	    	 iFlipCandidate = aCandidateList[0];

		 break;
         case 5:  /*Pick the variable that has been flipped least number of times 
                    Any remaining ties are broken according to the tie-breaking
                    policy. */
                    iNumCandidates = 0;
                    iBestVarFlipCount = iStep;
                    iLastChange = iStep;
                    i = -1;
                    k = 0;
                    for (j = 0; j < iNumDecPromVars; j++) {
                         iVar = aDecPromVarsList[j];
                         iScore = bPen ? aVarPenScore[iVar] :aVarScore[iVar];
                         if (iScore < 0) {
                          if(aFlipCounts[iVar] <= iBestVarFlipCount) {
                           if(aFlipCounts[iVar] < iBestVarFlipCount) {
                            iNumCandidates=0;
                            iBestVarFlipCount = aFlipCounts[iVar];
                           }
                           aCandidateList[iNumCandidates++] = iVar;
                        }
                       }
                       else {
                            i = j;
                            aIsDecPromVar[iVar]=FALSE;
                            break;
                           }
                    }
                    j++;
                    if (i != -1) {
                     for (j = i+1; j < iNumDecPromVars; j++) {
                      iVar = aDecPromVarsList[j];
                      iScore = aVarScore[iVar];
                      if (iScore < 0) {
                       aDecPromVarsListPos[iVar] = i;
                       aDecPromVarsList[i++] =iVar;
                       if(aFlipCounts[iVar] <= iBestVarFlipCount) {
                        if(aFlipCounts[iVar] < iBestVarFlipCount) {
                         iNumCandidates=0;
                         iBestVarFlipCount = aFlipCounts[iVar];
                        }
                       aCandidateList[iNumCandidates++] = iVar;
                      }
                    }
                    else {
                     aIsDecPromVar[iVar] = FALSE;
                    }
                  }
                  iNumDecPromVars = i;
                }


                if (iNumCandidates > 1)
                 iFlipCandidate = TieBreaking();
                else
                 iFlipCandidate = aCandidateList[0];

                 break;
    

        
        case 6:  /* Select randomly a variable from the
                    promising decreasing variable. */
                    iBestScore = iNumFalse;  //CWBIG;    //BIG;
                    iLastChange = iStep;
                    i = -1;
                    k = 0;
                    for (j = 0; j < iNumDecPromVars; j++) {
                         iVar = aDecPromVarsList[j];
                         iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
                         if (iScore < 0) {
                         } else {
                            i = j;
                            aIsDecPromVar[iVar]=FALSE;
                            break;
                           }
                    }
                    j++;
                    if (i != -1) {
                     for (j = i+1; j < iNumDecPromVars; j++) {
                      iVar = aDecPromVarsList[j];
                      iScore = aVarScore[iVar];
                      if (iScore < 0) {
                       aDecPromVarsListPos[iVar] = i;
                       aDecPromVarsList[i++] =iVar;
                      } else {
                         aIsDecPromVar[iVar] = FALSE;
                        }
                      }
                     iNumDecPromVars = i;
                    }

                    if(iNumDecPromVars > 0) {
                     if(iNumDecPromVars == 1)
                      iFlipCandidate = aDecPromVarsList[0];
                     else
                      iFlipCandidate = aDecPromVarsList[RandomInt(iNumDecPromVars)];   
                    }


                    break;
           case 7:  NoveltyProm();
                    break; 

           case 8:  NoveltyPlusPlusProm();
                    if(iNumDecPromVars > 1){
                     if(RandomProb(iPromDp))
                      iFlipCandidate = iLeastRecentlyFlippedPromVar;
                    }
                    break;
           case 9:  NoveltyProm();
                    if(iNumDecPromVars > 1){
                     if(RandomProb(iPromWp))
                      iFlipCandidate = aDecPromVarsList[RandomInt(iNumDecPromVars)];
                    }
                    break;
           case 10:  NoveltyProm();
                    if(iNumDecPromVars > 1){
                     if(RandomProb(iPromDp)){
                      if(iNumDecPromVars == 2){
                       iFlipCandidate = aDecPromVarsList[RandomInt(iNumDecPromVars)];
                       } else{
                          iNumCandidates = 0;
                          for(j =0; j <iNumDecPromVars; j++){
                           iVar = aDecPromVarsList[j];
                           if((iVar!= iBestPromVar)&&(iVar != iSecondBestPromVar))
                            aCandidateList[iNumCandidates++] = iVar;
                           }
                         if(iNumCandidates != 0)
                         {
                         if(iNumCandidates == 1)
                          iFlipCandidate = aCandidateList[0];
                         else
                          iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)]; 
                        }
                       }
                      }
                    }
                    break;
            case 11: NoveltyPromisingProm();
                     break; 


	  }
  }
 if(iNumDecPromVars > 0)
{
 bPerformNovelty = FALSE;
}

if(bPerformNovelty)
{     if(bPen){
      if(iPs != -1){ 
      UpdateClauseWeight();
      if(RandomProb(iPs))      
      Smooth();
       }
      }
 
      switch (iHeuristic){
	  
	   case 1: if(bTabu)
             PickNoveltyTabu();
             else
             PickNovelty();
             
	   break;
     
     case 2:if(bTabu)
            PickNoveltyPlusTabu(); 
            else
            PickNoveltyPlus();
             
	   break;

	   case 3: if(bTabu)
             PickNoveltyPlusPlusTabu();
             else
             PickNoveltyPlusPlus();
             
	   break;

	   case 4:if(bTabu)
            PickNoveltyPlusPlusPrimeTabu(); 
            else
            PickNoveltyPlusPlusPrime();
	   break;

     case 5:if(bTabu)
            PickRNovelty();
            else
            PickRNovelty();

	   break;

     case 6:if(bTabu)
            PickRNoveltyPlus();
            else
            PickRNoveltyPlus();
	   break;


     case 7:if(bTabu)
             PickVW1Tabu();
             else
             PickVW1();

           break;

      case 8:if(bTabu)
             PickVW2Tabu();
             else{
             if(!bNoise)
             PickVW2();
             else
             PickVW2Automated();   
             }
           break;

     case 9:if(bTabu)
             PickWalkSatTabu();
             else
             PickWalkSatSKC();
           break;

     case 10:if(bTabu)
            PickNoveltyPromisingTabu();
            else
            PickNoveltyPromising();
	   break;

     case 11:if(bTabu)
            PickNoveltyPlusPromisingTabu();
            else
            PickNoveltyPlusPromising();
	   break;

    case 12: if(bTabu)
             PickNoveltyPlusPlusPromisingTabu();
             else
             PickNoveltyPlusPlusPromising();
	   break;

    case 13:if(bTabu) 
             PickNoveltyPlusPlusPrimePromisingTabu();
             else
             PickNoveltyPlusPlusPrimePromising();

	   break;  

      case 14:
             PickNoveltyPlusFC();
           break;

     case 15:
             PickNoveltyPlusPromisingFC();
           break;
    
     

	   }
   }
 }

 else
 {
   if(bSingleClause)
   { 
         if(bPen){
      if(iPs != -1){
      UpdateClauseWeight();
      if(RandomProb(iPs))
      Smooth();
       }
      }


      bPerformNovelty = TRUE;

      switch (iHeuristic){
	   case 1: if(bTabu)
             PickNoveltyTabu();
             else
             PickNovelty();
             
	   break;
     
     case 2:if(bTabu)
            PickNoveltyPlusTabu(); 
            else
            PickNoveltyPlus();
             
	   break;

	   case 3: if(bTabu)
             PickNoveltyPlusPlusTabu();
             else
             PickNoveltyPlusPlus();
             
	   break;

	   case 4:if(bTabu)
            PickNoveltyPlusPlusPrimeTabu(); 
            else
            PickNoveltyPlusPlusPrime();
	   break;

     case 5:if(bTabu)
            PickRNovelty();
            else
            PickRNovelty();

	   break;

     case 6:if(bTabu)
            PickRNoveltyPlus();
            else
            PickRNoveltyPlus();
	   break;


     case 7:if(bTabu)
             PickVW1Tabu();
             else
             PickVW1();

           break;
    case 8:if(bTabu)
             PickVW2Tabu();
             else{
             if(!bNoise)
             PickVW2();
             else
             PickVW2Automated();
             }
           break;



     case 9:if(bTabu)
             PickWalkSatTabu();
             else
             PickWalkSatSKC();
           break;

     case 10:if(bTabu)
            PickNoveltyPromisingTabu();
            else
            PickNoveltyPromising();
	   break;

     case 11:if(bTabu)
            PickNoveltyPlusPromisingTabu();
            else
            PickNoveltyPlusPromising();
	   break;

    case 12: if(bTabu)
             PickNoveltyPlusPlusPromisingTabu();
             else
             PickNoveltyPlusPlusPromising();
	   break;

    case 13:if(bTabu) 
             PickNoveltyPlusPlusPrimePromisingTabu();
             else
             PickNoveltyPlusPlusPrimePromising();

	   break;  
     case 14:
             PickNoveltyPlusFC();
           break;

     case 15:
             PickNoveltyPlusPromisingFC();
           break;

     case 16: 
                  if (RandomProb(iDp)) {
                   if (iNumFalse) {
                   iClause = SelectClause();
                   iClauseLen = aClauseLen[iClause];

                   pLit = pClauseLits[iClause];

                   iFlipCandidate = GetVarFromLit(*pLit);

                   pLit++;

                   for (j=1;j<iClauseLen;j++) {
                    iVar = GetVarFromLit(*pLit);

                     if (aVW2Weights[iVar] < aVW2Weights[iFlipCandidate]) {
                      iFlipCandidate = iVar;
                   }

                  pLit++;
                 }
                } else {
                  iFlipCandidate = 0;
               }
           } else {

            /* otherwise, use regular novelty */
            if(!bTabu)
             PickNovelty();
            else
             PickNoveltyTabu();
            }
 
           break;
 
           }
   }
   else
   {
     
     if(!bPen){
     // This part of code is never executed in our implementation for the paper
     // I will make a cleaner version by removing it. 

     if(!bVarInFalse){
     switch (iScoringMeasure){
       case 1:  iNumCandidates = 0;
                iBestScore = iNumClauses;
                /* check score of all variables */
                
                for (j=1;j<=iNumVars;j++) {
                
               /* use cached value of score */ 
                  
                 if(!bTabu){
                  iScore = aBreakCount[j] - aMakeCount[j];
                  
                  /* build candidate list of best vars */
  
                  if(iScore <= iBestScore) {
                    if (iScore < iBestScore) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                   aCandidateList[iNumCandidates++] = j;
                  }
                 }
                  
                else{
                    if (aVarLastChange[j] < iTabuCutoff) { 
                     iScore = aBreakCount[j] - aMakeCount[j];

                   /* build candidate list of best vars */
                   if(iScore <= iBestScore) {
                    if (iScore < iBestScore) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                     aCandidateList[iNumCandidates++] = j;
                   }
                  }
                 }
                }
             /* select flip candidate uniformly from candidate list */
             if (iNumCandidates > 1)
              iFlipCandidate = TieBreaking();
             else
              iFlipCandidate = aCandidateList[0];
             break;

         case 2:  iNumCandidates = 0;
                  iBestScore = iNumClauses;

                  /* check score of all variables */
                   for (j=1;j<=iNumVars;j++) {
                  /* use cached value of score */
                   
                   if(!bTabu){
                    iScore = -aMakeCount[j];
                   /* build candidate list of best vars */
                    if(iScore <= iBestScore) {
                     if (iScore < iBestScore) {
                      iNumCandidates=0;
                      iBestScore = iScore;
                     }
                      aCandidateList[iNumCandidates++] = j;
                    }
                  }
                   else{                     
                    if (aVarLastChange[j] < iTabuCutoff) { 
                     iScore = -aMakeCount[j];

                     /* build candidate list of best vars */
                     if(iScore <= iBestScore) {
                      if (iScore < iBestScore) {
                       iNumCandidates=0;
                       iBestScore = iScore;
                      }
                      aCandidateList[iNumCandidates++] = j;
                     }                    
                    }
                   }
                  }
                   
                 /* select flip candidate uniformly from candidate list */
                 if (iNumCandidates > 1)
                  iFlipCandidate = TieBreaking();
                 else
                  iFlipCandidate = aCandidateList[0];
                 break;

      case 3: 
             iNumCandidates = 0;
                iBestScore = iNumClauses;
                /* check score of all variables */
                
                for (j=1;j<=iNumVars;j++) {
                
               /* use cached value of score */ 
                  
                 if(!bTabu){
                  iScore = aBreakCount[j] - aMakeCount[j];
                  
                  /* build candidate list of best vars */
  
                  if(iScore <= iBestScore) {
                    if ((iScore < iBestScore)||(aVarLastChange[j]<aVarLastChange[*aCandidateList])) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                   aCandidateList[iNumCandidates++] = j;
                  }
                 }
                  
                else{
                    if (aVarLastChange[j] < iTabuCutoff) { 
                     iScore = aBreakCount[j] - aMakeCount[j];

                   /* build candidate list of best vars */
                   if ((iScore < iBestScore)||(aVarLastChange[j]<aVarLastChange[*aCandidateList])) {
                    if (iScore < iBestScore) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                     aCandidateList[iNumCandidates++] = j;
                   }
                  }
                 }
                }
             /* select flip candidate uniformly from candidate list */
             if (iNumCandidates > 1)
              iFlipCandidate = TieBreaking();
             else
              iFlipCandidate = aCandidateList[0];
             break;


     }
     }
else{
     switch (iScoringMeasure){
       case 1:  iNumCandidates = 0;
                iBestScore = iNumClauses;
                /* check score of all variables */
                
                for (j=0;j<iNumVarsInFalseList;j++) {
                 iVar = aVarInFalseList[j];
                
               /* use cached value of score */ 
                  
                 if(!bTabu){
                  iScore = aBreakCount[iVar] - aMakeCount[iVar];
                  
                  /* build candidate list of best vars */
  
                  if(iScore <= iBestScore) {
                    if (iScore < iBestScore) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                   aCandidateList[iNumCandidates++] = iVar;
                  }
                 }
                  
                else{
                    if (aVarLastChange[j] < iTabuCutoff) { 
                     iScore = aBreakCount[iVar] - aMakeCount[iVar];

                   /* build candidate list of best vars */
                   if(iScore <= iBestScore) {
                    if (iScore < iBestScore) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                     aCandidateList[iNumCandidates++] = iVar;
                   }
                  }
                 }
                }
             /* select flip candidate uniformly from candidate list */
             if (iNumCandidates > 1)
              iFlipCandidate = TieBreaking();
             else
              iFlipCandidate = aCandidateList[0];
             break;

         case 2:  iNumCandidates = 0;
                  iBestScore = iNumClauses;

                  /* check score of all variables */
                 for (j=0;j<iNumVarsInFalseList;j++) {
                  iVar = aVarInFalseList[j];
                  /* use cached value of score */
                   
                   if(!bTabu){
                    iScore = -aMakeCount[iVar];
                   /* build candidate list of best vars */
                    if(iScore <= iBestScore) {
                     if (iScore < iBestScore) {
                      iNumCandidates=0;
                      iBestScore = iScore;
                     }
                      aCandidateList[iNumCandidates++] = iVar;
                    }
                  }
                   else{                     
                    if (aVarLastChange[j] < iTabuCutoff) { 
                     iScore = -aMakeCount[iVar];

                     /* build candidate list of best vars */
                     if(iScore <= iBestScore) {
                      if (iScore < iBestScore) {
                       iNumCandidates=0;
                       iBestScore = iScore;
                      }
                      aCandidateList[iNumCandidates++] = iVar;
                     }                    
                    }
                   }
                  }
                   
                 /* select flip candidate uniformly from candidate list */
                 if (iNumCandidates > 1)
                  iFlipCandidate = TieBreaking();
                 else
                  iFlipCandidate = aCandidateList[0];
                 break;

      case 3: 
             iNumCandidates = 0;
                iBestScore = iNumClauses;
                /* check score of all variables */
                
                for (j=0;j<iNumVarsInFalseList;j++) {
                  iVar = aVarInFalseList[j];
               /* use cached value of score */ 
                  
                 if(!bTabu){
                  iScore = aBreakCount[iVar] - aMakeCount[iVar];
                  
                  /* build candidate list of best vars */
  
                  if(iScore <= iBestScore) {
                    if ((iScore < iBestScore)||(aVarLastChange[iVar]<aVarLastChange[*aCandidateList])) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                   aCandidateList[iNumCandidates++] = iVar;
                  }
                 }
                  
                else{
                    if (aVarLastChange[j] < iTabuCutoff) { 
                      iScore = aBreakCount[iVar] - aMakeCount[iVar];

                   /* build candidate list of best vars */
                   if (iScore <= iBestScore){
                   if ((iScore < iBestScore)||(aVarLastChange[j]<aVarLastChange[*aCandidateList])) {
                     iNumCandidates=0;
                     iBestScore = iScore;
                    }
                     aCandidateList[iNumCandidates++] = iVar;
                   }
                  }
                 }
                }
             /* select flip candidate uniformly from candidate list */
             if (iNumCandidates > 1)
              iFlipCandidate = TieBreaking();
             else
              iFlipCandidate = aCandidateList[0];
             break;
        
     }
  }
}
else{
  switch (iSmoothingScheme){

  case 1: 
   
         PickSAPS();
         break;

 case 2:         
        PickPAWS();
        break;

  }

}

  }

 }
}



void PickSatensteinW() {
/*Just to reduce the size of the file, and there is no need 
  to have a weighted version of Satenstein as it will only
  complicate things more. This portion is deleted now. 
  I have a preliminary version of Satenstein Weighted that works
  but after that so many things have been added to it. That has not 
  been incorporated into the weighted version. 
  */

  //TODO update Satenstein Weighted. 
 }



void CreateDecPromVarsW() {

  aDecPromVarsListW = AllocateRAM((iNumVars+1) * sizeof(UINT32));

}

void InitDecPromVarsW() {

  UINT32 j;

  iNumDecPromVarsW = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarScoreW[j] < FLOATZERO) {
      aDecPromVarsListW[iNumDecPromVarsW++] = j;
    }
  }
}

void UpdateDecPromVarsW() {

  UINT32 j,k;
  UINT32 iVar;

  for (j=0;j<iNumChangesW;j++) {
    iVar = aChangeListW[j];
    if ((aVarScoreW[iVar] < FLOATZERO)&&(aChangeOldScoreW[iVar] >= 0)) {
      aDecPromVarsListW[iNumDecPromVarsW++] = iVar;
    }
  }
  j=0;
  k=0;
  while (j < iNumDecPromVarsW) {
    iVar = aDecPromVarsListW[k];
    if ((aVarScoreW[iVar] >= 0)||(iVar == iFlipCandidate)) {
      iNumDecPromVarsW--;
    } else {
      aDecPromVarsListW[j++]=aDecPromVarsListW[k];
    }
    k++;
  }
}



UINT32 TieBreaking() {
    
    UINT32 j = 0;
    
    switch(iTieBreaking){
      case 1: return aCandidateList[RandomInt(iNumCandidates)];;
              break;

      case 2: iFlipCandidate = aCandidateList[0];
              for(j =0; j <iNumCandidates; j++)
              {
               if(aVarLastChange[iFlipCandidate] > aVarLastChange[aCandidateList[j]]){
			         iFlipCandidate = aCandidateList[j];
			
		           }
              }
             return iFlipCandidate;
             break;

     case 3: iFlipCandidate = aCandidateList[0];
              for(j =0; j <iNumCandidates; j++)
              {
               if(aFlipCounts[iFlipCandidate] > aFlipCounts[aCandidateList[j]]){
			         iFlipCandidate = aCandidateList[j];
			
		           }
              }
             return iFlipCandidate;
             break;
     case 4:iFlipCandidate = aCandidateList[0];
             for(j =0; j <iNumCandidates; j++)
             {
              if (aVW2Weights[aCandidateList[j]] < aVW2Weights[iFlipCandidate]) {
               iFlipCandidate = aCandidateList[j];
              }
            }
            return iFlipCandidate;
            break;
     }
    
     
    
      

    return 0;
}











BOOL CheckIfFreebie(UINT32 iLookVar)
{    
  UINT32 i;
  UINT32 j;
  SINT32 iScore =0;
  SINT32 iMakeScore =0;
  UINT32 iClause;
  UINT32 iClauseLen;
  UINT32 iVar;
  LITTYPE *pLit;
  LITTYPE *pClause;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 iNumOcc;

  if(iLookVar == 0){
    return FALSE;


  }

  litWasTrue = GetTrueLit(iLookVar);

 // printf("\n The lit is true %d",IsLitTrue(litWasTrue));

  litWasFalse = GetFalseLit(iLookVar);
  // printf("\n The lit is true %d",IsLitTrue(litWasFalse));
/*
    iNumOcc = aNumLitOcc[litWasFalse];
    pClause = pLitClause[litWasFalse];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==0) {
        iMakeScore++;
      }
      pClause++;
    }

*/

    iNumOcc = aNumLitOcc[litWasTrue];
    pClause = pLitClause[litWasTrue];

    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause]==1) {
        iScore++;
      }
      pClause++;
    }



/* 
  printf("\n Breakcount is %u",aBreakCount[iLookVar]);
  printf("\n Computed BreakCount is %d", iScore);
  printf("\n MakeCount is %u",aMakeCount[iLookVar]);
  printf("\n Computed MakeCount is %d", iMakeScore);
  printf("\n Varscore is %d", aVarScore[iLookVar]);
*/
  if(iScore ==0)
    return TRUE;
  else
    return FALSE;

//return FALSE;

}


void NoveltyProm()
  {
   int i,k,j;
   SINT32 iScore;
   SINT32 iBestScore = bPen? iSumFalsePen :iNumFalse;  
   SINT32 iSecondBestScore = bPen ? iSumFalsePen :iNumFalse;
   UINT32 iYoungestChange = 0;
   UINT32 iYoungestVar;
   UINT32 iVar;
   i = -1;
   k = 0;
   for (j = 0; j < iNumDecPromVars; j++) {
        iVar = aDecPromVarsList[j];
        iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
        if (iScore < 0) {
                if(aVarLastChange[iVar] > iYoungestChange){
                iYoungestChange = aVarLastChange[iVar];
                iYoungestVar = iVar;
                }

                if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestPromVar]))) {
                   iSecondBestPromVar = iBestPromVar;
                   iBestPromVar = iVar;
                   iSecondBestScore = iBestScore;
                   iBestScore = iScore;
               } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestPromVar]))) {
                   iSecondBestPromVar = iVar;
                   iSecondBestScore = iScore;
               }


        } else {
           i = j;          aIsDecPromVar[iVar]=FALSE;
           break;
           }
        }
    j++;
    if (i != -1){
          for (j = i+1; j < iNumDecPromVars; j++) {
               iVar = aDecPromVarsList[j];
               iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
               if (iScore < 0) {
                aDecPromVarsListPos[iVar] = i;
                aDecPromVarsList[i++] =iVar;
                if(aVarLastChange[iVar] > iYoungestChange){
                 iYoungestChange = aVarLastChange[iVar];
                 iYoungestVar = iVar;
                }
               if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestPromVar]))) {
                iSecondBestPromVar = iBestPromVar;
                iBestPromVar = iVar;
                iSecondBestScore = iBestScore;
                iBestScore = iScore;
               } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestPromVar]))) {
                        iSecondBestPromVar = iVar;
                        iSecondBestScore = iScore;
               }

         } else {
                  aIsDecPromVar[iVar] = FALSE;
                }
         }
    iNumDecPromVars = i;
   }
    if(iNumDecPromVars == 1){
         iFlipCandidate = iBestPromVar;
         return;
    }
    if(iNumDecPromVars > 1){
     iFlipCandidate = iBestPromVar;
     if(iBestPromVar == iYoungestVar){
      if(RandomProb(iPromNovNoise))
       iFlipCandidate = iSecondBestPromVar;
     }
   }


}


/* This is pure redundancy. But the Novelty++ heuristic requires to keep track of the 
   least recently flipped variable. And going over the list of promising decreasing variable 
   will be purely annoying and unfair to this heuristic. */

void NoveltyPlusPlusProm()
{
   int i,k,j;
   UINT32 iLastChange = iStep;
   SINT32 iScore;
   SINT32 iBestScore = bPen? iSumFalsePen :iNumFalse;
   SINT32 iSecondBestScore = bPen ? iSumFalsePen :iNumFalse;
   UINT32 iYoungestChange = 0;
   UINT32 iYoungestVar;
   UINT32 iVar;
   i = -1;
   k = 0;
   for (j = 0; j < iNumDecPromVars; j++) {
        iVar = aDecPromVarsList[j];
        iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
        if (iScore < 0) {
                if(aVarLastChange[iVar] > iYoungestChange){
                iYoungestChange = aVarLastChange[iVar];
                iYoungestVar = iVar;

                }
                if(aVarLastChange[iVar] < iLastChange){
                iLastChange = aVarLastChange[iVar];
                iLeastRecentlyFlippedPromVar = iVar;
                }

                if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestPromVar]))) {
                   iSecondBestPromVar = iBestPromVar;
                   iBestPromVar = iVar;
                   iSecondBestScore = iBestScore;
                   iBestScore = iScore;
               } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestPromVar]))) {
                   iSecondBestPromVar = iVar;
                   iSecondBestScore = iScore;
               }


        } else {
           i = j;          aIsDecPromVar[iVar]=FALSE;
           break;
           }
        }
    j++;
    if (i != -1){
          for (j = i+1; j < iNumDecPromVars; j++) {
               iVar = aDecPromVarsList[j];
               iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
               if (iScore < 0) {
                aDecPromVarsListPos[iVar] = i;
                aDecPromVarsList[i++] =iVar;
                if(aVarLastChange[iVar] > iYoungestChange){
                 iYoungestChange = aVarLastChange[iVar];
                 iYoungestVar = iVar;

                }
                if(aVarLastChange[iVar] < iLastChange){
                iLastChange = aVarLastChange[iVar];
                iLeastRecentlyFlippedPromVar = iVar;
                }

               if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestPromVar]))) {
                iSecondBestPromVar = iBestPromVar;
                iBestPromVar = iVar;
                iSecondBestScore = iBestScore;
                iBestScore = iScore;
               } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestPromVar]))) {
                        iSecondBestPromVar = iVar;
                        iSecondBestScore = iScore;
               }

         } else {
                  aIsDecPromVar[iVar] = FALSE;
                }
         }
    iNumDecPromVars = i;
   }
    if(iNumDecPromVars == 1){
         iFlipCandidate = iBestPromVar;
         return;
    }
    if(iNumDecPromVars > 1){
     iFlipCandidate = iBestPromVar;
     if(iBestPromVar == iYoungestVar){
      if(RandomProb(iPromNovNoise))
       iFlipCandidate = iSecondBestPromVar;
     }
   }


}


void UpdateClauseWeight()
{
register int i,j;
LITTYPE *pLit;
UINT32 iClauseLen;
register UINT32 iClause,iVar;

for(i=0;i <iNumFalse; i++)
{
 iClause = aFalseList[i];
 if(++aClausePen[iClause] == 2)
 {
  aWhereWeight[iClause] = iNumWeighted;
  aWeightedList[iNumWeighted]=iClause;
  iNumWeighted++;
 }
 iClauseLen = aClauseLen[iClause];
 pLit = pClauseLits[iClause];
 for (j=0; j<iClauseLen; j++) {
 iVar = GetVarFromLit(*pLit);
 --aVarPenScore[iVar];
        if (!aIsDecPromVar[iVar] && (aVarPenScore[iVar] < 0) && (aVarLastChange[iVar] <iStep -1 )) {
        aDecPromVarsListPos[iVar] = iNumDecPromVars;
        aDecPromVarsList[iNumDecPromVars++] = iVar;
        aIsDecPromVar[iVar] = TRUE;
   }
 pLit++;
 }

}
iSumFalsePen += iNumFalse;
iTotalWeight += iNumFalse;
}

void Smooth()
{
  register int i,j;
  LITTYPE *pLit;
  register UINT32 iClause,iVar;
  UINT32 iClauseLen;
  UINT32 iStartNumWeighted;
  iStartNumWeighted = iNumWeighted;
  iTotalWeight -= iNumWeighted;
  for(j=0;j<iStartNumWeighted;j++) {

         iClause = aWeightedList[j];
         if(--aClausePen[iClause]==1){
          --iNumWeighted;
          aWeightedList[aWhereWeight[iClause]]=aWeightedList[iNumWeighted];
          aWhereWeight[aWeightedList[iNumWeighted]] = aWhereWeight[iClause];

         }
         if (aNumTrueLit[iClause]==0) {
                 --iSumFalsePen;
                 iClauseLen = aClauseLen[iClause];
                 pLit = pClauseLits[iClause];
                  for (i=0;i<iClauseLen;i++) {
                         iVar = GetVarFromLit(*pLit);
                         ++aVarPenScore[iVar];
                         pLit++;
                         }
         }
                  if (aNumTrueLit[iClause]==1) {
                     iVar = aCritSat[iClause];
                     --aVarPenScore[iVar];
                     if (!aIsDecPromVar[iVar] && (aVarPenScore[iVar] < 0) && (aVarLastChange[iVar] <iStep -1 )) {
        aDecPromVarsListPos[iVar] = iNumDecPromVars;
        aDecPromVarsList[iNumDecPromVars++] = iVar;
        aIsDecPromVar[iVar] = TRUE;
   }
  

                  }

  }
}

void NoveltyPromisingProm()
{
   int i,k,j;
   SINT32 iScore;
   SINT32 iBestScore = bPen? iSumFalsePen :iNumFalse;
   SINT32 iSecondBestScore = bPen ? iSumFalsePen :iNumFalse;
   SINT32 iSecondBestLookAheadScore,iBestLookAheadScore;
   UINT32 iYoungestChange = 0;
   UINT32 iYoungestVar;
   UINT32 iVar;
   i = -1;
   k = 0;
   for (j = 0; j < iNumDecPromVars; j++) {
        iVar = aDecPromVarsList[j];
        iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
        if (iScore < 0) {
                if(aVarLastChange[iVar] > iYoungestChange){
                iYoungestChange = aVarLastChange[iVar];
                iYoungestVar = iVar;
                }

                if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestPromVar]))) {
                   iSecondBestPromVar = iBestPromVar;
                   iBestPromVar = iVar;
                   iSecondBestScore = iBestScore;
                   iBestScore = iScore;
               } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestPromVar]))) {
                   iSecondBestPromVar = iVar;
                   iSecondBestScore = iScore;
               }


        } else {
           i = j;          aIsDecPromVar[iVar]=FALSE;
           break;
           }
        }
    j++;
    if (i != -1){
          for (j = i+1; j < iNumDecPromVars; j++) {
               iVar = aDecPromVarsList[j];
               iScore = bPen ? aVarPenScore[iVar] : aVarScore[iVar];
               if (iScore < 0) {
                aDecPromVarsListPos[iVar] = i;
                aDecPromVarsList[i++] =iVar;
                if(aVarLastChange[iVar] > iYoungestChange){
                 iYoungestChange = aVarLastChange[iVar];
                 iYoungestVar = iVar;
                }
               if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestPromVar]))) {
                iSecondBestPromVar = iBestPromVar;
                iBestPromVar = iVar;
                iSecondBestScore = iBestScore;
                iBestScore = iScore;
               } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestPromVar]))) {
                        iSecondBestPromVar = iVar;
                        iSecondBestScore = iScore;
               }

         } else {
                  aIsDecPromVar[iVar] = FALSE;
                }
         }
    iNumDecPromVars = i;
   }
    if(iNumDecPromVars == 1){
         iFlipCandidate = iBestPromVar;
         return;
    }
    if(iNumDecPromVars > 1){
           /* choose the 'best' variable by default */

      iFlipCandidate = iBestPromVar;

      /* If the best is the youngest, with probability (iNovNoise) select the 2nd best */

      if (iFlipCandidate == iYoungestVar) {
        if (RandomProb(iPromNovNoise)) {
          iFlipCandidate = iSecondBestPromVar;
          return;
        }
      } else {

        /* If the best is older than then 2nd best, just choose the best */

        if (aVarLastChange[iSecondBestPromVar] >= aVarLastChange[iFlipCandidate]) {
          return;
        }
      }

      /* otherwise, determine the 'look ahead' score for the 2nd best variable */

      iSecondBestLookAheadScore = bPen ? aVarPenScore[iSecondBestPromVar] + BestLookAheadPenScore(iSecondBestPromVar) :aVarScore[iSecondBestPromVar] + BestLookAheadScore(iSecondBestPromVar);

      if (iSecondBestLookAheadScore > iBestScore) {
        iBestLookAheadScore = iBestScore;
      } else {

        /* if the 'look ahead' score for the 2nd variable is better than the regular score
           for the best variable, calculate the look ahead score for the best variable */

        iBestLookAheadScore = bPen ?  aVarPenScore[iFlipCandidate] + BestLookAheadPenScore(iFlipCandidate) : aVarScore[iFlipCandidate] + BestLookAheadScore(iFlipCandidate);
      }

      /* choose the variable with the best look ahead score */

      /* Note that this BREAKS TIES by selecting the 2nd best variable -- as in the paper and in the author's code */

      if (iBestLookAheadScore >= iSecondBestLookAheadScore) {
        iFlipCandidate = iSecondBestPromVar;
      }
    
  } else {
    iFlipCandidate = 0;
  }

   }




