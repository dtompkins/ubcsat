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


void AddAlgorithms() {

  AddGSat();  
  AddGWSat();
  AddGSatTabu();

  AddHSat();
  AddHWSat();

  AddWalkSat();
  AddWalkSatTabu();

  AddNovelty();
  AddNoveltyPlus();
  AddNoveltyPlusPlus();

  AddNoveltyPlusP();

  AddAdaptNoveltyPlus();

  AddRNovelty();
  AddRNoveltyPlus();

  AddSAPS();

  AddPAWS();

  AddDDFW();

  AddG2WSat();

  AddSparrow();

  AddVW();

  AddRoTS();
  AddIRoTS();

  AddSAMD();

  AddRandom();

  AddDerandomized();

  AddRGSat();

  AddWeighted();

}


#ifdef __cplusplus

}
#endif
