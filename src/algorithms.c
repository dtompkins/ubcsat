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


void AddAlgorithms() {

  AddGSat();  
  AddGWSat();
  AddGSatTabu();

  AddHSat();
  AddHWSat();

  AddJack();

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
