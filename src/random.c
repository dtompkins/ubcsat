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

void PickURWalk();
void PickCRWalk();
void SchoeningRestart();

void AddRandom() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("urwalk","",FALSE,
    "Uninformed Random Walk: flip any var at random",
    "",
    "PickURWalk",
    "DefaultProcedures",
    "default","default");

  CreateTrigger("PickURWalk",ChooseCandidate,PickURWalk,"","");  

  pCurAlg = CreateAlgorithm("urwalk","",TRUE,
    "Uninformed Random Walk: flip any var at random (weighted)",
    "",
    "PickURWalk",
    "DefaultProceduresW",
    "default_w","default");

  pCurAlg = CreateAlgorithm("crwalk","",FALSE,
    "Conflict-Directed Random Walk: choose unsat clause, then literal at random",
    "Papadimitriou [FOCS 91]",
    "PickCRWalk",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");

  CreateTrigger("PickCRWalk",ChooseCandidate,PickCRWalk,"","");

  pCurAlg = CreateAlgorithm("crwalk","",TRUE,
    "Conflict-Directed Random Walk (weighted)",
    "Papadimitriou [FOCS 91]",
    "PickCRWalk",
    "DefaultProceduresW,Flip+FalseClauseListW",
    "default_w","default");

  CreateTrigger("SchoeningRestart",CreateData,SchoeningRestart,"","");

  pCurAlg = CreateAlgorithm("crwalk","schoening",FALSE,
    "Conflict-Directed Random Walk, restart every 3n steps",
    "Schoening [FOCS 99]",
    "PickCRWalk,SchoeningRestart",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");

  pCurAlg = CreateAlgorithm("crwalk","schoening",TRUE,
    "Conflict-Directed Random Walk, restart every 3n steps (weighted)",
    "Schoening [FOCS 99]",
    "PickCRWalk,SchoeningRestart",
    "DefaultProceduresW,Flip+FalseClauseListW",
    "default_w","default");

}


void PickURWalk() {
  iFlipCandidate = RandomInt(iNumVars) + 1;
}

void PickCRWalk() {
  
  UINT32 iClause;
  UINT32 iClauseLen;

  LITTYPE litPick;

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
    litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
    iFlipCandidate = GetVarFromLit(litPick);
  }
}

void SchoeningRestart() {

  if (iPeriodicRestart != 0) {
    ReportPrint(pRepErr,"Warning! Changing -prestart value to match Schoening's Algorithm\n");
  }

  iPeriodicRestart = iNumVars * 3;

}

