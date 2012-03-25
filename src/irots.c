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

/* note that this IRoTS implementation differs slightly from
   the previously published software... in mostly unimportant
   ways.  in this implementation we have strived to match the
   text of the AI 2003 paper exactly */

UINT32 iIrotsEscapeSteps;
UINT32 iIrotsPerturbSteps;

UINT32 iLSTabuTenure;
UINT32 iPerturbTabuTenure;

PROBABILITY iIrotsNoise;

UINT32 iLSTabuTenureLow;
UINT32 iLSTabuTenureHigh;
UINT32 iPerturbTabuTenureLow;
UINT32 iPerturbTabuTenureHigh;

UBIGINT iIrotsLSBestStep;
UBIGINT iIrotsLSBestValue;
UBIGINT iIrotsLSBestValueWeight;

UINT32 iIrotsSavedValue;
UBIGINT iIrotsSavedValueWeight;

UINT32 iIrotsMode;

BOOL *aIrotsBackup;

void InitIRoTSParms();
void InitIRoTS();
void PostStepIRoTS();

void CreateIRoTSBackup();
void IRoTSBackup();
void IRoTSRestore();

void AddIRoTS() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("irots","",0,
    "IRoTS: Iterated Robust TABU Search",
    "Smyth, Hoos, Stuetzle [AI 2003]",
    "PickRoTS,PostStepIRoTS",
    "DefaultProcedures,Flip+VarScore,CreateIRoTSBackup",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-ltabu","local search phase median tabu tenure","[default is 0.1n + 4]","",&iLSTabuTenure,0);
  AddParmUInt(&pCurAlg->parmList,"-tabuinterval","interval size: percent of tabu tenure [default %s]","range of tabu tenure is: tt +/- tt * (INT/100)","",&iTabuTenureInterval,25);
  AddParmUInt(&pCurAlg->parmList,"-esteps","perturb after INT steps beyond last improving step", "[default is n^2/4]","",&iIrotsEscapeSteps,0);
  AddParmUInt(&pCurAlg->parmList,"-psteps","perturb for exactly INT steps", "[default is 0.9n]","",&iIrotsPerturbSteps,0);
  AddParmUInt(&pCurAlg->parmList,"-ptabu","perturb phase median tabu tenure", "[default to psteps/2]","",&iPerturbTabuTenure,0);
  AddParmProbability(&pCurAlg->parmList,"-pnoise","irots noise [default %s]","probability of selecting the worst of the two candidates","",&iIrotsNoise,0.10);

  CreateTrigger("InitIRoTSParms",PreStart,InitIRoTSParms,"","");
  CreateTrigger("InitIRoTS",PreRun,InitIRoTS,"","");
  CreateTrigger("PostStepIRoTS",PostStep,PostStepIRoTS,"InitIRoTS,InitIRoTSParms","");

  CreateTrigger("CreateIRoTSBackup",CreateStateInfo,CreateIRoTSBackup,"","");

}

void InitIRoTSParms() {

  /* this variable initialization scheme is from the paper */
  
  if (iLSTabuTenure == 0) iLSTabuTenure = iNumVars / 10 + 4;
  if (iIrotsEscapeSteps == 0) iIrotsEscapeSteps = iNumVars * iNumVars / 4;
  if (iIrotsPerturbSteps == 0) iIrotsPerturbSteps = iNumVars * 9 / 10;
  if (iPerturbTabuTenure == 0) iPerturbTabuTenure = iIrotsPerturbSteps / 2;

  /* use the iTabuTenureInterval to set the low & high range for the tabu tenure */

  iLSTabuTenureLow = iLSTabuTenure - (iTabuTenureInterval * iLSTabuTenure) / 100;
  iLSTabuTenureHigh = iLSTabuTenure + (iTabuTenureInterval * iLSTabuTenure) / 100;

  iPerturbTabuTenureLow = iPerturbTabuTenure - (iTabuTenureInterval * iPerturbTabuTenure) / 100;
  iPerturbTabuTenureHigh = iPerturbTabuTenure + (iTabuTenureInterval * iPerturbTabuTenure) / 100;
}

void InitIRoTS() {

  /* mode 0 ==> initial local search phase */
  
  iIrotsMode = 0;

  /* set RoTS parameters to the LS mode  */

  iTabuTenureLow = iLSTabuTenureLow;
  iTabuTenureHigh = iLSTabuTenureHigh;
  iTabuTenure = iLSTabuTenure;

  iIrotsLSBestStep = 1;
  iIrotsLSBestValue = iNumClauses + 1;
  iIrotsLSBestValueWeight = UBIGINTMAX;

  iVarLastChangeReset = 0;
}


void PostStepIRoTS() {

  BOOL bSwitchMode = 0;
  BOOL bSave = 0;
  BOOL bRestore = 0;

  /* after a regular IRoTS step, the process depends on what
     'mode' the solver is in */

  switch (iIrotsMode) {

  /* mode 0 ==> initial local search phase */

  case 0: 

    /* keep track of the 'step' with last best improvement */

    if (bWeighted) {
      if (iSumFalseWeight < iIrotsLSBestValueWeight) {
        iIrotsLSBestValueWeight = iSumFalseWeight;
        iIrotsLSBestStep = iStep;
      }
    } else {
      if (iNumFalse < iIrotsLSBestValue) {
        iIrotsLSBestValue = iNumFalse;
        iIrotsLSBestStep = iStep;
      }
    }

    /* in no improvement past the best in 'esteps' steps, go to perturbation phase */

    if (iStep == iIrotsLSBestStep + iIrotsEscapeSteps + 1) {

      /* because this is the initial phase, we must 'save' the candidate solution */

      bSave = 1;
      iIrotsMode++;
      bSwitchMode = 1;
    }
    
    break;

  /* mode 1 ==> perturbation search phase */

  case 1:

    /* stay in this phase for exactly 'psteps' steps */

    if (iStep == iVarLastChangeReset + iIrotsPerturbSteps + 1) {
      bSwitchMode = 1;
      iIrotsMode++;
      iIrotsLSBestValue = iNumFalse;
      iIrotsLSBestValueWeight = iSumFalseWeight;
      iIrotsLSBestStep = iStep;
    }
    
    break;

  /* mode 2 ==> ongoing local search phase */

  case 2:

    /* keep track of the 'step' with last best improvement */

    if (bWeighted) {
      if (iSumFalseWeight < iIrotsLSBestValueWeight) {
        iIrotsLSBestValueWeight = iSumFalseWeight;
        iIrotsLSBestStep = iStep;
      }
    } else {
      if (iNumFalse < iIrotsLSBestValue) {
        iIrotsLSBestValue = iNumFalse;
        iIrotsLSBestStep = iStep;
      }
    }

    /* in no improvement past the best in 'esteps' steps, go to perturbation phase */

    if (iStep == iIrotsLSBestStep + iIrotsEscapeSteps + 1) {
      iIrotsMode--;
      bSwitchMode = 1;

      /* compare current candidate solution against the last 'saved' candidate solution
      choose the worst solution with probability (iWp)
      if we choose the current solution, 'save' it and continue 
      otherwise, 'restore' the previously saved solution */
      
      if (bWeighted) {
        if (iSumFalseWeight <= iIrotsSavedValueWeight) {
          if (RandomProb(iIrotsNoise)) {
            bRestore = 1;
          } else {
            bSave = 1;
          }
        } else {
          if (RandomProb(iIrotsNoise)) {
            bSave = 1;
          } else {
            bRestore = 1;
          }
        }
      } else {
        if (iNumFalse <= iIrotsSavedValue) {
          if (RandomProb(iIrotsNoise)) {
            bRestore = 1;
          } else {
            bSave = 1;
          }
        } else {
          if (RandomProb(iIrotsNoise)) {
            bSave = 1;
          } else {
            bRestore = 1;
          }
        }
      }
    }
    break;

  default:
    break;
  }

  /* if switching modes, then set IRoTS parameters appropriately */

  if (bSwitchMode) {
    iVarLastChangeReset = iStep;

    if (iIrotsMode == 1) {
      iTabuTenureLow = iPerturbTabuTenureLow;
      iTabuTenureHigh = iPerturbTabuTenureHigh;
      iTabuTenure = iPerturbTabuTenure;
    } else {
      iTabuTenureLow = iLSTabuTenureLow;
      iTabuTenureHigh = iLSTabuTenureHigh;
      iTabuTenure = iLSTabuTenure;
    }

    /* choose the tabu tenure uniformly from the tabu range */

    if (iTabuTenureLow != iTabuTenureHigh) {
      iTabuTenure = iTabuTenureLow + RandomInt(iTabuTenureHigh - iTabuTenureLow);
    }
  }

  if (bSave) {
    
    IRoTSBackup();

    if (bWeighted) {
      iIrotsSavedValueWeight = iSumFalseWeight;
    } else {
      iIrotsSavedValue = iNumFalse;
    }
  }

  if (bRestore) {
    
    IRoTSRestore();

    /* after restoring the variables, we must reset all state information */

    RunProcedures2(InitStateInfo);
    RunProcedures2(PostInit);
  }

}


void CreateIRoTSBackup() {
  aIrotsBackup = (BOOL *) AllocateRAM((iNumVars+1)*sizeof(BOOL), HeapData);
}

void IRoTSBackup() {
  UINT32 j;
  for (j=1;j<=iNumVars;j++) {
    aIrotsBackup[j] = aVarValue[j];
  }
}

void IRoTSRestore() {
  UINT32 j;
  for (j=1;j<=iNumVars;j++) {
    aVarValue[j] = aIrotsBackup[j];
  }
}

#ifdef __cplusplus

}
#endif
