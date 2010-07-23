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
  
  bFound = FALSE;

  for (j=0;j<iNumHeap;j++) {
    if (aHeap[j].iBytesFree >= size) {
      bFound = TRUE;
      iHeapID = j;
      break;
    }
  }
  if (!bFound) {
    if (size > DEFAULTHEAPSIZE) {
      aHeap[iNumHeap].pHeap = malloc(size);
      aHeap[iNumHeap].pFree = aHeap[iNumHeap].pHeap;
      aHeap[iNumHeap].iBytesFree = size;
    } else {
      aHeap[iNumHeap].pHeap = malloc(DEFAULTHEAPSIZE);
      aHeap[iNumHeap].pFree = aHeap[iNumHeap].pHeap;
      aHeap[iNumHeap].iBytesFree = DEFAULTHEAPSIZE;
    }
    iHeapID = iNumHeap;
    iNumHeap++;
    if (iNumHeap == MAXHEAPS) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXHEAPS [%u]\n",MAXHEAPS);
      AbnormalExit();
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
  (*sNew) = AllocateRAM(strlen(sSrc)+1);
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
