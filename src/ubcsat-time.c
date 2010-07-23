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

/*  
    This file contains the code for timestamping.
    Hopefully you won't have any problems compiling in your environment
*/

void StartTotalClock();
void StopTotalClock();

void StartRunClock();
void StopRunClock();

double TotalTimeElapsed();
double RunTimeElapsed();

double fTotalTime;
double fRunTime;

double fStartTotalTimeStamp;
double fStartRunTimeStamp;

double fCurrentTimeStamp;

#ifdef WIN32

  struct _timeb tstruct;

#else

  struct timeval tv;
  struct timezone tzp;
  struct tms prog_tms;

#endif

#ifdef WIN32

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
    fCurrentTimeStamp = (double)prog_tms.tms_utime / ((double)sysconf(_SC_CLK_TCK));
  }

  void InitSeed() {
  
    gettimeofday(&tv,&tzp);
    iSeed = (( tv.tv_sec & 0x000007FF ) * 1000000) + tv.tv_usec;

  }

#endif

double TimeElapsed(double fStart)
{
  CurrentTime();
  if ((fCurrentTimeStamp - fStart) <= FLOATZERO) {
    return(FLOATZERO);
  } else {
    return(fCurrentTimeStamp - fStart);
  }
}
  
void StartTotalClock() {
  CurrentTime();
  fStartTotalTimeStamp = fCurrentTimeStamp;
}

void StartRunClock() {
  CurrentTime();
  fStartRunTimeStamp = fCurrentTimeStamp;
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

