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

void PickAdaptG2WSatPlus();


/***** Trigger DecPromVars *****/



UINT32 *aDecPromVarsList;
UINT32 *aDecPromVarsListPos;
UINT32 iNumDecPromVars;
UINT32 iHeuristic;
UINT32 iDecStrategy;
BOOL *aIsDecPromVar;


UINT32 *aDecPromVarsListW;
UINT32 *aDecPromVarsListPosW;
UINT32 iNumDecPromVarsW;
BOOL *aIsDecPromVarW;

void AddAdaptG2WSatPlus() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("adaptg2wsatplus","",FALSE,
    "AdaptG2WSATPlus: G2WSAT with adaptive noise mechanism",
    "Li, Huang  [SAT 07]",
    "PickAdaptG2WSatPlus",
    "DefaultProcedures,Flip+TrackChanges,DecPromVars,FalseClauseList,VarLastChange,AdaptNoveltyPlusNoise",
    "default","default");

    CopyParameters(pCurAlg,"novelty+","",FALSE,0);
  
  CreateTrigger("PickAdaptG2WSatPlus",ChooseCandidate,PickAdaptG2WSatPlus,"","");
  
  

 //TODO Weighted version.
}


void PickAdaptG2WSatPlus(){

  UINT32 j;
  UINT32 iVar;
  UINT32 iBestVarFlipCount;
  SINT32 iScore;
  SINT32 iBestScore;
  BOOL FreebieExist;

  
  //pick the least recently flipped variable
  if (iNumDecPromVars > 0 ) {

	  
         iFlipCandidate = aDecPromVarsList[0];
		 for (j=1;j<iNumDecPromVars;j++) {

		 iVar = aDecPromVarsList[j];
		 if(aVarLastChange[iFlipCandidate] > aVarLastChange[iVar]){
			iFlipCandidate = iVar;
			
		}
				 		
	  }

  } else {	       
	   PickNoveltyPlus();	   	    
	}
}


