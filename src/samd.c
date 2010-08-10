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

void SAMDUpdateVarLastChange();

void AddSAMD() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("samd","",0,
    "SAMD: Steepest Ascent Mildest Descent",
    "Hansen and Jaumard [Computing 1990]",
    "PickGSatTabu,SAMDUpdateVarLastChange",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  
  CopyParameters(pCurAlg,"gsat-tabu","",0);

  CreateTrigger("SAMDUpdateVarLastChange",UpdateStateInfo,SAMDUpdateVarLastChange,"VarLastChange","UpdateVarLastChange");

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

#ifdef __cplusplus

}
#endif
