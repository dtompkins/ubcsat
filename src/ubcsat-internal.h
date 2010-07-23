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


#define NOFXN 0
#define NOREF(A) (A)=(A)

#define RunProcedures(A) {if (aNumActiveProcedures[A]) {for (iRunProceduresLoop=0;iRunProceduresLoop<aNumActiveProcedures[A];iRunProceduresLoop++) aActiveProcedures[A][iRunProceduresLoop]();} }
#define RunProcedures2(A) {if (aNumActiveProcedures[A]) {for (iRunProceduresLoop2=0;iRunProceduresLoop2<aNumActiveProcedures[A];iRunProceduresLoop2++) aActiveProcedures[A][iRunProceduresLoop2]();} }
/* Use RunProcedures2 when calling from outside of the main loop in ubcsat.c */

#define NUMVALIDSTATCODES 30
extern const char *sValidStatCodes[];

extern ALGORITHM aAlgorithms[];

extern ALGPARMLIST parmAlg;
extern ALGPARMLIST parmHelp;
extern ALGPARMLIST parmIO;
extern ALGPARMLIST parmUBCSAT;

extern BOOL bShowHelp;
extern BOOL bShowHelpA;
extern BOOL bShowHelpW;
extern BOOL bShowHelpP;
extern BOOL bShowHelpV;
extern BOOL bShowHelpT;
extern BOOL bShowHelpR;
extern BOOL bShowHelpC;
extern BOOL bShowHelpS;
extern BOOL bHelpHeaderShown;

extern const char sVersion[];
extern char *pVersion;
extern char *sCommentString;
extern FXNPTR aActiveProcedures[][MAXFXNLIST];

extern BOOL bReportOutputSuppress;

extern BOOL bReportStateLMOnly;
extern BOOL bReportDistanceLMOnly;
extern FLOAT fReportStateQuality;
extern BOOL bReportBestStepVars;
extern BOOL bReportOptClausesSol;
extern UINT32 iReportFalseHistCount;
extern BOOL bReportDistanceLMOnly;
extern UINT32 iReportDistHistCount;
extern BOOL bReportStateQuality;
extern UINT32 iReportStateQuality;

extern BOOL bReportPenaltyEveryLM;
extern BOOL bReportPenaltyReNormBase;
extern BOOL bReportPenaltyReNormFraction;

extern BOOL bReportTriggersAll;

extern FLOAT fDummy;
extern FLOAT fFlipsPerSecond;
extern FLOAT fPercentSuccess;
extern char *sStringAlgParms;

extern ITEMLIST listColumns;
extern ITEMLIST listStats;
extern ITEMLIST listTriggers;

extern REPORT aReports[];
extern REPORTCOL aColumns[];
extern DYNAMICPARM aDynamicParms[];
extern REPORTSTAT aStats[];
extern TRIGGER aTriggers[];

extern UINT32 aNumActiveProcedures[];

extern UINT32 aActiveCalcColumns[];
extern UINT32 aOutputColumns[];
extern UINT32 aRTDColumns[];

extern UINT32 iMobFixedWindow;
extern BOOL bMobilityFixedIncludeStart;
extern UINT32 iNumActiveCalcColumns;
extern UINT32 iNumAlg;
extern UINT32 iNumDynamicParms;
extern UINT32 iNumOutputColumns;
extern UINT32 iNumReports;
extern UINT32 iNumRTDColumns;
extern UINT32 iNumStatsActive;
extern UINT32 iReportMobilityDisplay;
extern BOOL bReportMobilityNormalized;
extern UINT32 iRunProceduresLoop;
extern UINT32 iRunProceduresLoop2;
extern BOOL bSortedByStepsValid;

extern BOOL bMobilityColNActive;
extern BOOL bMobilityColXActive;
extern FLOAT fMobilityColNMean;
extern FLOAT fMobilityColXMean;
extern FLOAT fMobilityColNMeanNorm;
extern FLOAT fMobilityColXMeanNorm;
extern FLOAT fMobilityColNCV;
extern FLOAT fMobilityColXCV;

extern char *sFilenameSoln;
extern char sLine[];

void ActivateDynamicParms();
void ActivateAlgorithmTriggers();
void ActivateColumnID(UINT32 iColID, const char *sItem);
void ActivateStatID(UINT32 iStatID, const char *sItem);
void ActivateTriggerID(UINT32 iFxnID, const char *sItem);

void ActivateColumns(char *sColumns);
void ActivateStats(char *sStats);

void AddAllocateRAMColumnID(UINT32 j, const char *sItem);
void AddOutputColumnID(UINT32 j, const char *sItem);
void AddParameters();
void AddParmReport(ALGPARMLIST *pParmList,const char *sSwitch,const char *sName,const char *sDescription,const char *sTriggers);
void AddReportTriggers();
void AddRTDColumnID(UINT32 j, const char *sItem);
void CalculateStats(FLOAT *fMean, FLOAT *fStddev, FLOAT *fCV, FLOAT fSum, FLOAT fSum2, UINT32 iCount);
FLOAT CorrelationCoeff(FLOAT fSumA, FLOAT fSumA2,FLOAT fSumB, FLOAT fSumB2, FLOAT fSumAB, UINT32 iCount);
void CheckPrintHelp();
UINT32 FindItem(ITEMLIST *pList,char *sID);
void HelpBadParm();
void ParseAllParameters(int argc, char *argv[]);
void PrintAlgParmSettings(REPORT *pRep, ALGPARMLIST *pParmList);
void ParseItemList(ITEMLIST *pList, char *sItems, CALLBACKPTR ItemFunction);
void PrintUBCSATHeader(REPORT *pRep);
void SetupUBCSAT();

#define GetVarStateBit(VS,B) ((((VS)[(B)>>3])>>(7 - ((B)&0x07)))&0x01)
VARSTATE NewVarState();
VARSTATE NewCopyVarState(VARSTATE vsCopy);
void SetCurVarState(VARSTATE vsIn);
BOOL SetCurVarStateString(VARSTATE vsIn, char *sVarState);
void SetArrayFromVarState(UINT32 *aOut, VARSTATE vsIn);
UINT32 HammingDistVarState(VARSTATE vsA, VARSTATE vsB);
BOOL IsVarStateEqual(VARSTATE vsA, VARSTATE vsB);
BOOL IsVarStateInList(VARSTATELIST *vsList, VARSTATE vsIn);
UINT32 MinHammingVarStateList(VARSTATELIST *vsList, VARSTATE vsIn);
void AddToVarStateList(VARSTATELIST *vsList, VARSTATE vsAdd);
BOOL AddUniqueToVarStateList(VARSTATELIST *vsList, VARSTATE vsAdd);

