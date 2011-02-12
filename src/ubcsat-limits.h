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

#ifndef UBCSAT_LIMITS_H
#define UBCSAT_LIMITS_H

#ifdef __cplusplus 
namespace ubcsat {
#endif


#define DEFAULTHEAPSIZE 4194304
#define MAXHEAPS 1024

#define LITSPERCHUNK 262144

#define MAXNUMALG 128
#define MAXALGPARMS 128

#define MAXFXNLIST 32

#define MAXCNFLINELEN 16384

#define MAXREPORTS 64
#define MAXREPORTPARMS 8

#define MAXPARMLINELEN 16384
#define MAXTOTALPARMS 128

#define MAXITEMLIST 512
#define MAXITEMLISTSTRINGLENGTH 1024

#define MAXREPORTHEADERSTRING 256

#define RANDOMFILEBUFFERSIZE 1048576

#define HELPSTRINGLENGTH 128

#define MAXDYNAMICPARMS 16

#ifdef __cplusplus
}
#endif
#endif

