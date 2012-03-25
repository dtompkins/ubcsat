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

#ifndef UBCSAT_TIME_H
#define UBCSAT_TIME_H

#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

#ifdef __cplusplus 
namespace ubcsat {
#endif


void InitSeed();

void StartAbsoluteClock();
void StartTotalClock();
void StopTotalClock();

void StartRunClock();
void StopRunClock();

double TotalTimeElapsed();
double RunTimeElapsed();

extern BOOL bUseAbsoluteTime;
extern BOOL bUseSystemTime;
extern double fTotalTime;
extern double fRunTime;


#ifdef __cplusplus
}
#endif
#endif
