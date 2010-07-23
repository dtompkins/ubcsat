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

void PickG2WSat();
void PickG2WSatW();

/***** Trigger DecPromVars *****/

void CreateDecPromVars();
void InitDecPromVars();
void UpdateDecPromVars();

void CreateDecPromVarsW();
void InitDecPromVarsW();
void UpdateDecPromVarsW();

UINT32 *aDecPromVarsList;
UINT32 *aDecPromVarsListPos;
UINT32 iNumDecPromVars;
BOOL *aIsDecPromVar;

UINT32 *aDecPromVarsListW;
UINT32 *aDecPromVarsListPosW;
UINT32 iNumDecPromVarsW;
BOOL *aIsDecPromVarW;

// TODO: remove
UINT32 iTempNumDecPromSteps;

void AddG2WSat() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("g2wsat","",FALSE,
    "G2WSAT: Gradient-based Greedy WalkSAT",
    "Li, Huang  [SAT 05]",
    "PickG2WSat",
    "DefaultProcedures,Flip+TrackChanges,DecPromVars,FalseClauseList,VarLastChange",
    "default","default");

  CopyParameters(pCurAlg,"novelty++","",FALSE);

  CreateTrigger("PickG2WSat",ChooseCandidate,PickG2WSat,"","");
  
  CreateTrigger("CreateDecPromVars",CreateStateInfo,CreateDecPromVars,"CreateTrackChanges","");
  CreateTrigger("InitDecPromVars",InitStateInfo,InitDecPromVars,"InitTrackChanges","");
  CreateTrigger("UpdateDecPromVars",UpdateStateInfo,UpdateDecPromVars,"UpdateTrackChanges","");
  CreateContainerTrigger("DecPromVars","CreateDecPromVars,InitDecPromVars,UpdateDecPromVars");

// TODO: remove
  AddColumnUInt("percentprom","Percent of Steps with a Promising Decreasing Variable",
    "Percent",
    "Steps w",
    "PromDec",
    "%7.4f",
    &iTempNumDecPromSteps,"",ColTypeFinalDivStep100);


  pCurAlg = CreateAlgorithm("g2wsat","",TRUE,
    "G2WSAT: Gradient-based Greedy WalkSAT (weighted)",
    "Li, Huang  [SAT 05]",
    "PickG2WSatW",
    "DefaultProceduresW,Flip+TrackChangesW,DecPromVarsW,FalseClauseList,VarLastChange",
    "default_w","default");

  CopyParameters(pCurAlg,"g2wsat","",FALSE);

  CreateTrigger("PickG2WSatW",ChooseCandidate,PickG2WSatW,"","");
  
  CreateTrigger("CreateDecPromVarsW",CreateStateInfo,CreateDecPromVarsW,"CreateTrackChangesW","");
  CreateTrigger("InitDecPromVarsW",InitStateInfo,InitDecPromVarsW,"InitTrackChangesW","");
  CreateTrigger("UpdateDecPromVarsW",UpdateStateInfo,UpdateDecPromVarsW,"UpdateTrackChangesW","");
  CreateContainerTrigger("DecPromVarsW","CreateDecPromVarsW,InitDecPromVarsW,UpdateDecPromVarsW");

}


void PickG2WSat() {

  UINT32 j;
  UINT32 iVar;
  SINT32 iScore;
  SINT32 iBestScore;

  if (iNumDecPromVars > 0 ) {

    iTempNumDecPromSteps++;

    iFlipCandidate = aDecPromVarsList[0];
    iBestScore = aVarScore[iFlipCandidate];

    for (j=1;j<iNumDecPromVars;j++) {

      iVar = aDecPromVarsList[j];
      iScore = aVarScore[iVar];

      if (iScore < iBestScore) {
        iFlipCandidate = iVar;
        iBestScore = aVarScore[iVar];
      } else {
        if (iScore == iBestScore) {
          if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
            iFlipCandidate = iVar;
          }
        }
      }
    }
  } else {
    PickNoveltyPlusPlus();
  }
}

void CreateDecPromVars() {

  aDecPromVarsList = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aDecPromVarsListPos = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aIsDecPromVar = AllocateRAM((iNumVars+1) * sizeof(BOOL));

}

void InitDecPromVars() {

  UINT32 j;

  iNumDecPromVars = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarScore[j] < 0) {
      aDecPromVarsListPos[j] = iNumDecPromVars;
      aDecPromVarsList[iNumDecPromVars++] = j;
      aIsDecPromVar[j] = TRUE;
    } else {
      aIsDecPromVar[j] = FALSE;
    }
  }

  // TODO: remove
  iTempNumDecPromSteps = 0;
}

void UpdateDecPromVars() {

  UINT32 j;
  UINT32 iVar;

  for (j=0;j<iNumChanges;j++) {
    iVar = aChangeList[j];

    if (iVar == iFlipCandidate) {
      if (aIsDecPromVar[iVar]) {
        aIsDecPromVar[iVar] = FALSE;
        aDecPromVarsList[aDecPromVarsListPos[iVar]] = aDecPromVarsList[--iNumDecPromVars];
        aDecPromVarsListPos[aDecPromVarsList[iNumDecPromVars]] = aDecPromVarsListPos[iVar];
      }
    } else {
      if ((aChangeOldScore[iVar] >= 0) && (aVarScore[iVar] < 0)) {
        aIsDecPromVar[iVar] = TRUE;
        aDecPromVarsListPos[iVar] = iNumDecPromVars;
        aDecPromVarsList[iNumDecPromVars++] = iVar;
      }

      if ((aVarScore[iVar] >= 0) && aIsDecPromVar[iVar]) {
        aIsDecPromVar[iVar] = FALSE;
        aDecPromVarsList[aDecPromVarsListPos[iVar]] = aDecPromVarsList[--iNumDecPromVars];
        aDecPromVarsListPos[aDecPromVarsList[iNumDecPromVars]] = aDecPromVarsListPos[iVar];
      }
    }
  }
}





void PickG2WSatW() {

  UINT32 j;
  UINT32 iVar;
  FLOAT fScore;
  FLOAT fBestScore;

  if (iNumDecPromVarsW > 0 ) {

    iFlipCandidate = aDecPromVarsListW[0];
    fBestScore = aVarScoreW[iFlipCandidate];

    for (j=1;j<iNumDecPromVarsW;j++) {

      iVar = aDecPromVarsListW[j];
      fScore = aVarScoreW[iVar];

      if (fScore < fBestScore) {
        iFlipCandidate = iVar;
        fBestScore = aVarScoreW[iVar];
      } else {
        if (fScore == fBestScore) {
          if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
            iFlipCandidate = iVar;
          }
        }
      }
    }
  } else {
    PickNoveltyPlusPlusW();
  }
}


void CreateDecPromVarsW() {

  aDecPromVarsListW = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aDecPromVarsListPosW = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aIsDecPromVarW = AllocateRAM((iNumVars+1) * sizeof(BOOL));

}

void InitDecPromVarsW() {

  UINT32 j;

  iNumDecPromVarsW = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarScoreW[j] < FLOATZERO) {
      aDecPromVarsListPosW[j] = iNumDecPromVarsW;
      aDecPromVarsListW[iNumDecPromVarsW++] = j;
      aIsDecPromVarW[j] = TRUE;
    } else {
      aIsDecPromVarW[j] = FALSE;
    }
  }
}

void UpdateDecPromVarsW() {

  UINT32 j;
  UINT32 iVar;

  for (j=0;j<iNumChangesW;j++) {
    iVar = aChangeListW[j];

    if (iVar == iFlipCandidate) {
      if (aIsDecPromVarW[iVar]) {
        aIsDecPromVarW[iVar] = FALSE;
        aDecPromVarsListW[aDecPromVarsListPosW[iVar]] = aDecPromVarsListW[--iNumDecPromVarsW];
        aDecPromVarsListPosW[aDecPromVarsListW[iNumDecPromVarsW]] = aDecPromVarsListPosW[iVar];
      }
    } else {
      if ((aChangeOldScoreW[iVar] >= FLOATZERO) && (aVarScoreW[iVar] < FLOATZERO)) {
        aIsDecPromVarW[iVar] = TRUE;
        aDecPromVarsListPosW[iVar] = iNumDecPromVarsW;
        aDecPromVarsListW[iNumDecPromVarsW++] = iVar;
      }

      if ((aVarScoreW[iVar] >= FLOATZERO) && aIsDecPromVarW[iVar]) {
        aIsDecPromVarW[iVar] = FALSE;
        aDecPromVarsListW[aDecPromVarsListPosW[iVar]] = aDecPromVarsListW[--iNumDecPromVarsW];
        aDecPromVarsListPosW[aDecPromVarsListW[iNumDecPromVarsW]] = aDecPromVarsListPosW[iVar];
      }
    }
  }
}
