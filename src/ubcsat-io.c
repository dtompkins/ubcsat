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

#define MERSENNE

#ifdef MERSENNE

#ifdef __cplusplus 
  FXNRAND32 fxnRandUInt32 = (FXNRAND32) mersenne::genrand_int32;
  #define fxnRandSeed(A) mersenne::init_genrand(A)
#else
  FXNRAND32 fxnRandUInt32 = genrand_int32;
  #define fxnRandSeed(A) init_genrand(A)
#endif
#else
  #ifdef _WIN32
    FXNRAND32 fxnRandUInt32 = (UINT32 (*)()) rand;
    #define fxnRandSeed(A) srand(A)
  #else
    FXNRAND32 fxnRandUInt32 = (UINT32 (*)()) random;
    #define fxnRandSeed(A) srandom(A)
  #endif
#endif

FLOAT ProbToFloat(PROBABILITY iProb) {
  return (iProb*(1.0/4294967295.0));
}

PROBABILITY FloatToProb(FLOAT fProb) {
  return ((PROBABILITY) (fProb * 4294967295.0));
}

UINT32 ProbToInvInt(PROBABILITY iProb) {
  FLOAT fInv;
  fInv = 4294967295.0 / iProb;
  return ((UINT32) (fInv + 0.5));
}

FLOAT RandomFloat() {
  UINT32 iNum;
  FLOAT fNum;
  iNum = fxnRandUInt32();
  fNum = (FLOAT) iNum;
  fNum /= 4294967295.0;
  return(fNum);
}

UINT32 RandomInt(UINT32 iMax) {
  return(fxnRandUInt32() % iMax);
}

BOOL RandomProb(PROBABILITY iProb) {
  if (iProb==0) {
    return(0);
  }
  if (fxnRandUInt32() <= iProb) {
    return(1);
  } else {
    return(0);
  }
}

UINT32 RandomMax() {
  return(fxnRandUInt32());
}

void RandomSeed(UINT32 iSeed) {
  fxnRandSeed(iSeed);
}

UBIGINT iNumRandomCalls;
FXNRAND32 fxnRandOrig;

UINT32 CountRandom() {
  iNumRandomCalls++;
  return(fxnRandOrig());
}

void SetupCountRandom() {
  fxnRandOrig = fxnRandUInt32;
  fxnRandUInt32 = CountRandom;
}

void InitCountRandom() {
  iNumRandomCalls = 0;
}

void SetupFile(FILE **fFil,const char *sOpenType, const char *sFilename, FILE *filDefault, BOOL bAllowNull) {

  if (*sFilename) {
    if (strcmp(sFilename,"null")==0) {
      if (bAllowNull) {
        (*fFil) = NULL;  
      } else {
        fprintf(stderr,"Fatal Error: Invalid use of null i/o\n");
        AbnormalExit();
        exit(1);
      }
    } else {
      (*fFil) = fopen(sFilename,sOpenType);
      if ((*fFil) == NULL) {
        printf("Fatal Error: Invalid filename [%s] specified \n",sFilename);
        AbnormalExit();
        exit(1);
      }
    }
  } else {
    (*fFil) = filDefault;
  }
}

void ActivateReportTriggers() {

  UINT32 j;

  for (j=0;j<iNumReports;j++) {
    if (strcmp(aReports[j].sOutputFile,"null")==0) {
      aReports[j].bActive = 0;
      aReports[j].fileOut = 0;
    } else if (aReports[j].bUserActivated) {
      aReports[j].bActive = 1;
    } else if ((aReports[j].bDefaultActivate)&&(!bReportQuiet)) {
      aReports[j].bActive = 1;
    }
    if (aReports[j].bActive) {
      if (strcmp(aReports[j].sOutputFile,"stdout")==0) {
        aReports[j].fileOut = stdout;
      } else if (strcmp(aReports[j].sOutputFile,"stderr")==0) {
        aReports[j].fileOut = stderr;
      } else {
        if (!aReports[j].bSpecialFileIO) {
          aReports[j].fileOut = fopen(aReports[j].sOutputFile,"w");
          if (aReports[j].fileOut == 0) {
            printf("Fatal Error: Invalid filename [%s] specified \n",aReports[j].sOutputFile);
            AbnormalExit();
            exit(1);
          }
        }
      }
      ActivateTriggers(aReports[j].sTriggers);
    }
  }

}

void CloseSingleFile(FILE *filToClose) {
  if ((filToClose)&&(filToClose != stdout)&&(filToClose != stderr)) {
    fclose(filToClose);
  }
}

void CloseReports() {
  UINT32 j;
  for (j=0;j<iNumReports;j++) {
    if (aReports[j].bActive) {
      if (!aReports[j].bSpecialFileIO) {
        CloseSingleFile(aReports[j].fileOut);
      }
    }
  }
}

void AbnormalExit() {
  CloseReports();  
  FreeRAM();
}

void CleanExit() {
  CloseReports();
  FreeRAM();
}

FILE *filReportPrint;


FILE *filRandomData;
char *sFilenameRandomData;
char *sFilenameAbort;
BYTE *pRandomDataBuffer;
UINT32 iRandomBufferRemaining;
BYTE *pNextRandomData;
BOOL bCycleData;
UINT32 iCycleDataLen;

UINT32 FileRandomUInt32() {
  UINT32 iReturn = 0;
  UINT32 j;
  if (iRandomBufferRemaining >= 4) {
    iReturn = *pNextRandomData++;
    iReturn = (iReturn << 8) | *pNextRandomData++;
    iReturn = (iReturn << 8) | *pNextRandomData++;
    iReturn = (iReturn << 8) | *pNextRandomData++;
    iRandomBufferRemaining -= 4;
  } else {
    j = 4;
    while (j--) {
      if (iRandomBufferRemaining==0) {
        if (bCycleData) {
          iRandomBufferRemaining = iCycleDataLen;
          pNextRandomData = pRandomDataBuffer;
        } else {
          iRandomBufferRemaining = (UINT32) fread(pRandomDataBuffer,1,RANDOMFILEBUFFERSIZE,filRandomData);
          if (iRandomBufferRemaining==0) {
            CloseSingleFile(filRandomData);
            SetupFile(&filRandomData,"rb",sFilenameRandomData,NULL,0);
            iRandomBufferRemaining = (UINT32) fread(pRandomDataBuffer,1,RANDOMFILEBUFFERSIZE,filRandomData);
          }
          pNextRandomData = pRandomDataBuffer;
        }
      }
      iReturn = (iReturn << 8) | *pNextRandomData++;
      iRandomBufferRemaining--;
    }
  }
  return(iReturn);
}

void CreateFileRandom() {

  pRandomDataBuffer = (BYTE *) AllocateRAM(RANDOMFILEBUFFERSIZE, HeapAdmin);
  
  SetupFile(&filRandomData,"rb",sFilenameRandomData,NULL,0);

  if (filRandomData==NULL) {
    ReportPrint(pRepErr,"Error! Unable to read from random data file\n");
    AbnormalExit();
    exit(1);
  }

  iRandomBufferRemaining = (UINT32) fread(pRandomDataBuffer,1,RANDOMFILEBUFFERSIZE,filRandomData);

  if ((iRandomBufferRemaining < RANDOMFILEBUFFERSIZE)||(feof(filRandomData))) {
    bCycleData = 1;
    iCycleDataLen = iRandomBufferRemaining;
  } else {
    bCycleData = 0;
  }

  pNextRandomData = pRandomDataBuffer;

  fxnRandUInt32 = FileRandomUInt32;
}

void CloseFileRandom() {
  CloseSingleFile(filRandomData);
}

void FileAbort() {

  FILE *fileAbort;
  
  fileAbort = fopen(sFilenameAbort,"r");

  if (fileAbort) {
    bTerminateAllRuns = 1;
    fclose(fileAbort);
  }
}

#ifdef __cplusplus

}
#endif
