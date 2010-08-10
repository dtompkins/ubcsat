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

#ifdef __cplusplus 
namespace ubcsat {
#endif

void PickURWalk();
void PickCRWalk();
void SchoeningRestart();

void AddRandom() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("urwalk","",0,
    "Uninformed Random Walk: flip any var at random",
    "",
    "PickURWalk",
    "DefaultProcedures",
    "default","default");

  CreateTrigger("PickURWalk",ChooseCandidate,PickURWalk,"","");  

  pCurAlg = CreateAlgorithm("crwalk","",0,
    "Conflict-Directed Random Walk: choose unsat clause, then literal at random",
    "Papadimitriou [FOCS 91]",
    "PickCRWalk",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");

  CreateTrigger("PickCRWalk",ChooseCandidate,PickCRWalk,"","");

  CreateTrigger("SchoeningRestart",CreateData,SchoeningRestart,"","");

  pCurAlg = CreateAlgorithm("crwalk","schoening",0,
    "Conflict-Directed Random Walk, restart every 3n steps",
    "Schoening [FOCS 99]",
    "PickCRWalk,SchoeningRestart",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");

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

#ifdef __cplusplus

}
#endif
