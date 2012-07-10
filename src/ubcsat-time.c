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

/*  
    This file contains the code for timestamping.
    Hopefully you won't have any problems compiling in your environment
*/

void StartAbsoluteClock();
void StartTotalClock();
void StopTotalClock();

void StartRunClock();
void StopRunClock();

double TotalTimeElapsed();
double RunTimeElapsed();

BOOL bUseAbsoluteTime;
BOOL bUseSystemTime;
double fTotalTime;
double fRunTime;

double fStartAbsoluteTimeStamp;
double fStartTotalTimeStamp;
double fStartRunTimeStamp;

double fCurrentTimeStamp;

#ifdef _WIN32

  struct _timeb tstruct;

#else

  struct timeval tv;
  struct timezone tzp;
  struct tms prog_tms;

#endif

#ifdef _WIN32

  void CurrentTime() {
    _ftime( &tstruct );
    fCurrentTimeStamp = ((double) tstruct.time + ((double)tstruct.millitm)/1000.0);
  }

  void InitSeed() {
    _ftime( &tstruct );
    iSeed = (( ((UINT32) tstruct.time) & 0x001FFFFF ) * 1000) + tstruct.millitm;
  }

#else

  void CurrentTime() {
    times(&prog_tms);
    if (bUseSystemTime) {
      fCurrentTimeStamp = ((double)(prog_tms.tms_utime + prog_tms.tms_stime)) / ((double)sysconf(_SC_CLK_TCK));
    } else {
      fCurrentTimeStamp = (double)prog_tms.tms_utime / ((double)sysconf(_SC_CLK_TCK));
    }
  }

  void InitSeed() {
  
    gettimeofday(&tv,&tzp);
    iSeed = (( tv.tv_sec & 0x000007FF ) * 1000000) + tv.tv_usec;

  }

#endif

double TimeElapsed(double fStart) {
  CurrentTime();
  if ((fCurrentTimeStamp - fStart) <= FLOATZERO) {
    return(FLOATZERO);
  } else {
    return(fCurrentTimeStamp - fStart);
  }
}
  
void StartAbsoluteClock() {
  CurrentTime();
  fStartAbsoluteTimeStamp = fCurrentTimeStamp;
}

void StartTotalClock() {
  if (bUseAbsoluteTime) {
    fStartTotalTimeStamp = fStartAbsoluteTimeStamp;
  } else {
    CurrentTime();
    fStartTotalTimeStamp = fCurrentTimeStamp;
  }
}

void StartRunClock() {
  CurrentTime();
  fStartRunTimeStamp = fCurrentTimeStamp;
}

double AbsoluteTimeElapsed() {
  return(TimeElapsed(fStartAbsoluteTimeStamp));
}

double TotalTimeElapsed() {
  return(TimeElapsed(fStartTotalTimeStamp));
}

double RunTimeElapsed() {
  return(TimeElapsed(fStartRunTimeStamp));
}


void StopTotalClock() {
  fTotalTime = TotalTimeElapsed();
}

void StopRunClock() {
  fRunTime = RunTimeElapsed();
}

#ifdef __cplusplus

}
#endif

