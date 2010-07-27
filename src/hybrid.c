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

void PickHybrid1();

void AddHybrid() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("hybrid1","",FALSE,
   "Hybrid1: Switch between VW and adaptG2WSATp",
    "Wei, Li, Zhang  [JSAT 08]",
    "PickHybrid1",
    "DefaultProcedures",
    "default","default");
  
  CreateTrigger("PickHybrid1",ChooseCandidate,PickHybrid1,"","");

}


void PickHybrid1() {
  printf("TODO!\n"); 
}
