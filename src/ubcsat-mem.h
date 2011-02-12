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

#ifndef UBCSAT_MEM_H
#define UBCSAT_MEM_H

#include "ubcsat-types.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif


void *AllocateRAM( size_t size );
void AdjustLastRAM( size_t size );
void PrintMemUsage();
void SetString(char **sNew, const char *sSrc);
void FreeRAM();

extern UINT32 iNumHeap;


#ifdef __cplusplus
}
#endif
#endif
