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

#ifndef UBCSAT_IO_H
#define UBCSAT_IO_H

#include <stdio.h>
#ifndef _WIN32
#ifdef __cplusplus
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>
#endif
#include "ubcsat-types.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif


void RandomSeed(UINT32 iSeed);
UINT32 RandomMax();
UINT32 RandomInt(UINT32 iMax);
BOOL RandomProb(PROBABILITY);
FLOAT RandomFloat();

FLOAT ProbToFloat(PROBABILITY iProb);
PROBABILITY FloatToProb(FLOAT fProb);
UINT32 ProbToInvInt(PROBABILITY iProb);

void ActivateReportTriggers();
void AbnormalExit();
void CleanExit();

void CloseSingleFile(FILE *filToClose);

void SetupFile(FILE **fFil,const char *sOpenType, const char *sFilename, FILE *filDefault, BOOL bAllowNull);

extern UBIGINT iNumRandomCalls;
void SetupCountRandom();
void InitCountRandom();

void InitVarsFromFile();

extern FILE *filReportPrint;

#ifdef _WIN32
#define P32 "I32u"
#define PS32 "I32d"
#define P64 "I64u"
#define PS64 "I64d"
#define SCAN32 "I32u"
#define SCANS32 "I32d"
#define SCAN64 "I64u"
#define SCANS64 "I64d"
#else
#define P32 PRIu32
#define PS32 PRId32
#define P64 PRIu64
#define PS64 PRId64
#define SCAN32 SCNu32
#define SCANS32 SCNd32
#define SCAN64 SCNu64
#define SCANS64 SCNd64
#endif

#define ReportPrint(pRep, sFormat) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat); }
#define ReportPrint1(pRep, sFormat, pVal1) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1); }
#define ReportPrint2(pRep, sFormat, pVal1, pVal2) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1,pVal2); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1,pVal2); }
#define ReportPrint3(pRep, sFormat, pVal1, pVal2, pVal3) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1,pVal2,pVal3); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1,pVal2,pVal3); }


#define ReportHdrPrefix(pRep) {if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,"%s ",sCommentString); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,"%s ",sCommentString); }}
#define ReportHdrPrint(pRep, sFormat) { if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat); }}
#define ReportHdrPrint1(pRep, sFormat, pVal1) { if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1); }}
#define ReportHdrPrint2(pRep, sFormat, pVal1, pVal2) { if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1,pVal2); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1,pVal2); }}

#define ReportFlush(pRep) {if (pRep) fflush(pRep->fileOut);}

extern char *sFilenameRandomData;
extern char *sFilenameAbort;
void CreateFileRandom();
void CloseFileRandom();
void FileAbort();


#ifdef __cplusplus
}
#endif
#endif
