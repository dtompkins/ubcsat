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

void SAMDUpdateVarLastChange();
void SAMDUpdateVarLastChangeW();

void AddSAMD() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("samd","",FALSE,
    "SAMD: Steepest Ascent Mildest Descent",
    "Hansen and Jaumard [Computing 1990]",
    "PickGSatTabu,SAMDUpdateVarLastChange",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  
  CopyParameters(pCurAlg,"gsat-tabu","",FALSE);

  CreateTrigger("SAMDUpdateVarLastChange",UpdateStateInfo,SAMDUpdateVarLastChange,"VarLastChange","UpdateVarLastChange");

  pCurAlg = CreateAlgorithm("samd","",TRUE,
    "SAMD: Steepest Ascent Mildest Descent (weighted)",
    "Hansen and Jaumard [Computing 1990]",
    "PickGSatTabuW,SAMDUpdateVarLastChangeW",
    "DefaultProceduresW,Flip+VarScoreW",
    "default_w","default");
  
  CopyParameters(pCurAlg,"gsat-tabu","",1);

  CreateTrigger("SAMDUpdateVarLastChangeW",UpdateStateInfo,SAMDUpdateVarLastChangeW,"VarLastChange","UpdateVarLastChange");
}

/* SAMD is essentially the same as GSAT-TABU, 
    except the age of flipped variables are not changed (and hence not tabu)
   if they were flipped in an improving search step
*/

void SAMDUpdateVarLastChange() {
  if (iBestScore >= 0) {
    UpdateVarLastChange();
  }
}

void SAMDUpdateVarLastChangeW() {
  if (fBestScore >= FLOATZERO) {
    UpdateVarLastChange();
  }
}



