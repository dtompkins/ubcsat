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

extern UINT32 iNumRandomCalls;
void SetupCountRandom();
void InitCountRandom();

void InitVarsFromFile();

extern FILE *filReportPrint;

#define ReportPrint(pRep, sFormat) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat); }
#define ReportPrint1(pRep, sFormat, pVal1) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1); }
#define ReportPrint2(pRep, sFormat, pVal1, pVal2) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1,pVal2); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1,pVal2); }
#define ReportPrint3(pRep, sFormat, pVal1, pVal2, pVal3) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1,pVal2,pVal3); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1,pVal2,pVal3); }


#define ReportHdrPrefix(pRep) {if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,"%s ",sCommentString); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,"%s ",sCommentString); }}
#define ReportHdrPrint(pRep, sFormat) { if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat); }}
#define ReportHdrPrint1(pRep, sFormat, pVal1) { if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1); }}
#define ReportHdrPrint2(pRep, sFormat, pVal1, pVal2) { if (!bReportClean) { filReportPrint = 0; if (pRep) filReportPrint = pRep->fileOut; if ((filReportPrint)&&(pRep->bActive)) fprintf(filReportPrint,sFormat,pVal1,pVal2); if ((bReportEcho) && (pRep->bActive) && (filReportPrint != stdout)) fprintf(stdout,sFormat,pVal1,pVal2); }}

extern char *sFilenameRandomData;
extern char *sFilenameAbort;
void CreateFileRandom();
void CloseFileRandom();
void FileAbort();


