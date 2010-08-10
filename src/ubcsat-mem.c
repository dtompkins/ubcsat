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

#ifdef __cplusplus 
namespace ubcsat {
#endif

/*  
    This file contains the simple memory managment of ubcsat
*/


UINT32 iNumHeap;
size_t iLastRequestSize;
UINT32 iLastHeap;

typedef struct typeUBCSATHEAP {
  char *pHeap;
  char *pFree;
  size_t iBytesFree;
} UBCSATHEAP;

UBCSATHEAP aHeap[MAXHEAPS];

void *AllocateRAM( size_t size ) {
  UINT32 j;
  BOOL bFound;
  UINT32 iHeapID = 0;
  void *pReturn;

  size = size + (4-(size % 4));

  iLastRequestSize = size;
  
  bFound = 0;

  for (j=0;j<iNumHeap;j++) {
    if (aHeap[j].iBytesFree >= size) {
      bFound = 1;
      iHeapID = j;
      break;
    }
  }
  if (!bFound) {
    if (size > DEFAULTHEAPSIZE) {
      aHeap[iNumHeap].pHeap = (char *) malloc(size);
      aHeap[iNumHeap].pFree = aHeap[iNumHeap].pHeap;
      aHeap[iNumHeap].iBytesFree = size;
    } else {
      aHeap[iNumHeap].pHeap = (char *) malloc(DEFAULTHEAPSIZE);
      aHeap[iNumHeap].pFree = aHeap[iNumHeap].pHeap;
      aHeap[iNumHeap].iBytesFree = DEFAULTHEAPSIZE;
    }
    iHeapID = iNumHeap;
    iNumHeap++;
    if (iNumHeap == MAXHEAPS) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXHEAPS [%d]\n",MAXHEAPS);
      AbnormalExit();
      exit(1);
    }
  }

  pReturn = (void *) aHeap[iHeapID].pFree;
  aHeap[iHeapID].pFree += size;
  aHeap[iHeapID].iBytesFree -= size;
  iLastHeap = iHeapID;
  return(pReturn);
}

void AdjustLastRAM( size_t size ) {
  size = size + (4-(size % 4));
  aHeap[iLastHeap].iBytesFree += (iLastRequestSize - size);
  aHeap[iLastHeap].pFree -= (iLastRequestSize - size);
}

void SetString(char **sNew, const char *sSrc) {
  (*sNew) = (char *) AllocateRAM(strlen(sSrc)+1);
  strcpy(*sNew,sSrc);
}

void FreeRAM() {
  
  UINT32 j;

  for (j=0;j<iNumHeap;j++) {
    if (aHeap[j].pHeap) {
      free(aHeap[j].pHeap);
    }
  }
}

#ifdef __cplusplus

}
#endif

