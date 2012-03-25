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

void AddSatGen() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("generalized local search","",FALSE,
    "Sat Gen",
    "Yet to be published",
    "PickSatGen",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);

  CreateTrigger("PickNovelty",ChooseCandidate,PickNovelty,"","");


  pCurAlg = CreateAlgorithm("novelty","",TRUE,
    "Novelty: (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "default_w","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  CreateTrigger("PickNoveltyW",ChooseCandidate,PickNoveltyW,"","");

}