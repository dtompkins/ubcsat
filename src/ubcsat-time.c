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

double fTotalTime;
double fStartTimeStamp;
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
    iSeed = (( tstruct.time & 0x001FFFFF ) * 1000) + tstruct.millitm;
  }

  double TimeElapsed()
  {
    double answer;

    CurrentTime();
    answer = fCurrentTimeStamp - fStartTimeStamp;
    return answer;
  }

#else

  void CurrentTime() {
    times(&prog_tms);
    fCurrentTimeStamp = (double)prog_tms.tms_utime;
  }

  void InitSeed() {
  
    gettimeofday(&tv,&tzp);
    iSeed = (( tv.tv_sec & 0x000007FF ) * 1000000) + tv.tv_usec;

  }

  double TimeElapsed() {

    double answer;

    times(&prog_tms);
    answer = (((double)prog_tms.tms_utime-fStartTimeStamp)/((double)sysconf(_SC_CLK_TCK)));
    return answer;
  }

#endif

void StartClock() {
  CurrentTime();
  fStartTimeStamp = fCurrentTimeStamp;
}

void StopClock() {
  fTotalTime = TimeElapsed();
}

