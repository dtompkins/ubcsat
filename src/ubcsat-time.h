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

#ifdef WIN32
#include <sys/timeb.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

void InitSeed();

void StartTotalClock();
void StopTotalClock();

void StartRunClock();
void StopRunClock();

double TotalTimeElapsed();
double RunTimeElapsed();

extern double fTotalTime;
extern double fRunTime;


