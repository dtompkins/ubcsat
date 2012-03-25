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
