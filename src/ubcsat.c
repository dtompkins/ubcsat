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

int ubcsatmain(int argc, char *argv[]) {

  StartAbsoluteClock();
  
  InitSeed();

  SetupUBCSAT();

  AddAlgorithms();
  AddParameters();
  AddReports();
  AddDataTriggers();
  AddReportTriggers();

  AddLocal();
  
  ParseAllParameters(argc,argv);

  ActivateAlgorithmTriggers();
  ActivateReportTriggers();

  RandomSeed(iSeed);

  RunProcedures(PostParameters);

  RunProcedures(ReadInInstance);

  RunProcedures(PostRead);

  RunProcedures(CreateData);
  RunProcedures(CreateStateInfo);

  iRun = 0;
  iNumSolutionsFound = 0;
  bTerminateAllRuns = 0;

  RunProcedures(PreStart);

  StartTotalClock();

  while ((iRun < iNumRuns) && (! bTerminateAllRuns)) {

    iRun++;

    iStep = 0;
    bSolutionFound = 0;
    bTerminateRun = 0;
    bRestart = 1;

    RunProcedures(PreRun);

    StartRunClock();
    
    while ((iStep < iCutoff) && (! bSolutionFound) && (! bTerminateRun)) {

      iStep++;
      iFlipCandidate = 0;

      RunProcedures(PreStep);
      RunProcedures(CheckRestart);

      if (bRestart) {
        RunProcedures(PreInit);
        RunProcedures(InitData);
        RunProcedures(InitStateInfo);
        RunProcedures(PostInit);
        bRestart = 0;
      } else {
        RunProcedures(ChooseCandidate);
        RunProcedures(PreFlip);
        RunProcedures(FlipCandidate);
        RunProcedures(UpdateStateInfo);
        RunProcedures(PostFlip);
      }
      
      RunProcedures(PostStep);

      RunProcedures(StepCalculations);

      RunProcedures(CheckTerminate);
    }

    StopRunClock();

    RunProcedures(RunCalculations);
    
    RunProcedures(PostRun);

    if (bSolutionFound) {
      iNumSolutionsFound++;
      if (iNumSolutionsFound == iFind) {
        bTerminateAllRuns = 1;
      }
    }
  }

  StopTotalClock();

  RunProcedures(FinalCalculations);

  RunProcedures(FinalReports);

  CleanExit();

  return(0);
  
}

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

int main(int argc, char *argv[]) {
  return(ubcsat::ubcsatmain(argc,argv));
}

#else

int main(int argc, char *argv[]) {
  return(ubcsatmain(argc,argv));
}

#endif
