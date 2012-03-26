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
    This file contains the simple memory managment of ubcsat
*/

#define UBCSAT_MALLOC_ALIGN 16

UINT32 iNumHeap;
size_t iLastRequestSize;
UINT32 iLastHeap;

typedef struct typeUBCSATHEAP {
  char *pHeap;
  char *pFree;
  size_t iBytesFree;
  enum HEAPTYPE eHeapType;
} UBCSATHEAP;

UBCSATHEAP aHeap[MAXHEAPS];

// #define UBCSAT_NO_HEAPS

#ifdef UBCSAT_NO_HEAPS

void *AllocateRAM( size_t size , enum HEAPTYPE type) {
  NOREF(type);
  return(malloc(size));
}

void AdjustLastRAM( size_t size ) {
  NOREF(size);
}

#else

void *AllocateRAM( size_t size , enum HEAPTYPE type) {
  UINT32 j;
  BOOL bFound;
  UINT32 iHeapID = 0;
  void *pReturn;

  size = size + (UBCSAT_MALLOC_ALIGN - (size % UBCSAT_MALLOC_ALIGN));

  iLastRequestSize = size;
  
  bFound = 0;

  for (j=0;j<iNumHeap;j++) {
    if ((aHeap[j].eHeapType == type)&&(aHeap[j].iBytesFree >= size)) {
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
      aHeap[iNumHeap].eHeapType = type;
    } else {
      aHeap[iNumHeap].pHeap = (char *) malloc(DEFAULTHEAPSIZE);
      aHeap[iNumHeap].pFree = aHeap[iNumHeap].pHeap;
      aHeap[iNumHeap].iBytesFree = DEFAULTHEAPSIZE;
      aHeap[iNumHeap].eHeapType = type;
    }
    if (aHeap[iNumHeap].pHeap == NULL) {
      ReportPrint(pRepErr,"Unexpected Error: malloc failed\n");
      AbnormalExit();
      exit(1);
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
  size = size + (UBCSAT_MALLOC_ALIGN - (size % UBCSAT_MALLOC_ALIGN));
  aHeap[iLastHeap].iBytesFree += (iLastRequestSize - size);
  aHeap[iLastHeap].pFree -= (iLastRequestSize - size);
}

#endif

void SetString(char **sNew, const char *sSrc) {
  (*sNew) = (char *) AllocateRAM(strlen(sSrc)+1, HeapString);
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

