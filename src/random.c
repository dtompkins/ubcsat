/*

      ##  ##  #####    #####   $$$$$   $$$$   $$$$$$    
      ##  ##  ##  ##  ##      $$      $$  $$    $$      
      ##  ##  #####   ##       $$$$   $$$$$$    $$      
      ##  ##  ##  ##  ##          $$  $$  $$    $$      
       ####   #####    #####  $$$$$   $$  $$    $$      
  ======================================================
  SLS SAT Solver from The University of British Columbia
  ======================================================
  ...... Developed & Maintained by Dave Tompkins .......
  ------------------------------------------------------
  ...... consult legal.txt for legal information .......
  ------------------------------------------------------
  .... project website: http://ubcsat.dtompkins.com ....
  ------------------------------------------------------
  source repository: https://github.com/dtompkins/ubcsat
  ------------------------------------------------------
  ..... contact us at ubcsat [@] googlegroups.com ......
  ------------------------------------------------------

*/

#include "ubcsat.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif

void PickURWalk();
void PickCRWalk();
void SchoeningRestart();

void InitLLLWalk();
void PickLLLWalk();
PROBABILITY iLLLWalkVarFlipProb;

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
    "DefaultProcedures,Flip+FalseClauseList,CheckForRestarts",
    "default","default");

  pCurAlg = CreateAlgorithm("lllwalk","",0,
    "Lovasz-Local-Lemma Rnd Walk: choose unsat clause, flip each var with prob.",
    "Mosser [STOC 09]",
    "PickLLLWalk",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");

  AddParmProbability(&pCurAlg->parmList,"-vfp","var flip probability [default %s]","flip each var in clause with probability PR","",&iLLLWalkVarFlipProb,0.5);

  CreateTrigger("InitLLLWalk",InitStateInfo,InitLLLWalk,"","");
  CreateTrigger("PickLLLWalk",ChooseCandidate,PickLLLWalk,"InitLLLWalk","");

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
    ReportHdrPrefix(pRepErr);
    ReportHdrPrint(pRepErr,"Warning! Changing -prestart value to match Schoening's Algorithm\n");
  }

  iPeriodicRestart = iNumVars * 3;

}


UINT32 iLLLWalkClausePosition; // next literal in the clause to select; 0 for no clause selected

void InitLLLWalk() {
  iLLLWalkClausePosition = 0;
}

void PickLLLWalk() {

  LITTYPE litPick;

  if (iNumFalse) {

    if (iLLLWalkClausePosition == 0) {
      iClausePick = aFalseList[RandomInt(iNumFalse)];
    }

    if (RandomProb(iLLLWalkVarFlipProb)) {
      litPick = pClauseLits[iClausePick][iLLLWalkClausePosition];
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }

    iLLLWalkClausePosition++;

    if (iLLLWalkClausePosition == aClauseLen[iClausePick]) {
      iLLLWalkClausePosition = 0;
    }
  }
}

#ifdef __cplusplus

}
#endif
