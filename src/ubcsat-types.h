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

/* Column Statistic Fields */

#define STATCODE_all          0xFFFFFFFF
#define STATCODE_mean         0x00000002
#define STATCODE_stddev       0x00000004
#define STATCODE_cv           0x00000008
#define STATCODE_var          0x00000010
#define STATCODE_stderr       0x00000020
#define STATCODE_vmr          0x00000040
#define STATCODE_sum          0x00000080
#define STATCODE_median       0x00000100
#define STATCODE_min          0x00000200
#define STATCODE_max          0x00000400
#define STATCODE_q05          0x00000800
#define STATCODE_q10          0x00001000
#define STATCODE_q25          0x00002000
#define STATCODE_q75          0x00004000
#define STATCODE_q90          0x00008000
#define STATCODE_q95          0x00010000
#define STATCODE_q98          0x00020000
#define STATCODE_qr7525       0x00040000
#define STATCODE_qr9010       0x00080000
#define STATCODE_qr9505       0x00100000
#define STATCODE_stepmean     0x00200000
#define STATCODE_solvemean    0x00400000
#define STATCODE_failmean     0x00800000
#define STATCODE_solvemedian  0x01000000
#define STATCODE_failmedian   0x02000000
#define STATCODE_solvemin     0x04000000
#define STATCODE_failmin      0x08000000
#define STATCODE_solvemax     0x10000000
#define STATCODE_failmax      0x20000000

#define STATCODE_RAMMASK      0x3FFFFF00
#define STATCODE_SORTMASK     0x001FFF00
#define STATCODE_CALCMASK     0x0000007E
#define STATCODE_SFMASK       0x3FE00000

#define FLOAT double
#define FLOATMAX (1E+300)
#define FLOATZERO (0.0f)

#define FLOATSTATSMIN (1E-8)

#define PROBABILITY unsigned long

#ifndef BOOL
#define BOOL unsigned long
#endif

#ifndef UINT32
#define UINT32 unsigned long
#endif

#define UINT32MAX 0xFFFFFFFF

#ifndef SINT32
#define SINT32 signed long
#endif

#define SINT32MAX 0x7FFFFFFF
#define SINT32MIN 0x80000000

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL 0
#endif

enum EVENTPOINT
{
  PostParameters,
  ReadInInstance,
  PostRead,
  CreateData,
  CreateStateInfo,
  PreStart,
  PreRun,
  CheckRestart,
  PreInit,
  InitData,
  InitStateInfo,
  PostInit,
  PreStep,
  ChooseCandidate,
  PreFlip,
  FlipCandidate,
  UpdateStateInfo,
  PostFlip,
  PostStep,
  StepCalculations,
  CheckTerminate,
  RunCalculations,
  PostRun,
  FinalCalculations,
  FinalReports,
  NUMEVENTPOINTS
};

enum PARMTYPE {
   PTypeUInt,
   PTypeSInt,
   PTypeBool,
   PTypeString,
   PTypeProbability,
   PTypeFloat,
   PTypeReport
};

enum CDATATYPE {
  DTypeUInt,
  DTypeSInt,
  DTypeFloat,
  DTypeString
};

enum COLTYPE {
  ColTypeFinal,
  ColTypeMean,
  ColTypeStddev,
  ColTypeCV,
  ColTypeMin,
  ColTypeMax,
  ColTypeFinalDivStep,
  ColTypeFinalDivStep100
};

typedef void (*FXNPTR)();
typedef void (*CALLBACKPTR)(UINT32, const char *sItem); 
typedef UINT32 (*FXNRAND32)();

typedef struct typeITEM {
  char *sID;
  BOOL bContainer;
  char *sContainerList;
} ITEM;

typedef struct typeITEMLIST {
  UINT32 iNumItems;
  ITEM aItems[MAXITEMLIST];
} ITEMLIST;

typedef struct typeTRIGGER {
  
  enum EVENTPOINT eEventPoint;
  FXNPTR pProcedure;
  
  BOOL bActive;
  BOOL bDisabled;

  char *sDependencyList;
  char *sDeactivateList;

} TRIGGER;

typedef union typePARAMETERDEFAULT {
  UINT32 iUInt;
  SINT32 iSInt;
  BOOL bBool;
  char *sString;
  PROBABILITY iProb;
  FLOAT fFloat;
} PARAMETERDEFAULT;

typedef struct typeALGPARM {
  enum PARMTYPE eType;
  char *sSwitch;
  char *sTerseDescription;        /* use less than 59 characters */
  char *sVerboseDescription;      /* pad with 25 spaces for each line */
  void *pParmValue;
  BOOL bSpecified;
  char *sTriggers;
  PARAMETERDEFAULT defDefault;
} ALGPARM;

typedef struct ALGPARAMETERS {
  UINT32 iNumParms;
  ALGPARM aParms[MAXALGPARMS];
} ALGPARMLIST;

typedef struct typeALGORITHM {
  
  char *sName;
  char *sVariant;
  BOOL bWeighted;

  char *sDescription;
  char *sAuthors;

  char *sHeuristicTriggers;
  char *sDataTriggers;

  char *sDefaultOutput;
  char *sDefaultStats;

  ALGPARMLIST parmList;

} ALGORITHM;

typedef struct typeREPORT {
  FILE *fileOut;
  char *sID;
  char *sDescription;
  char *sVerboseDescription;
  BOOL bActive;
  BOOL bSpecialFileIO;
  char *sOutputFile;
  char *sTriggers;
  UINT32 iNumParms;
  char *aParmName[MAXREPORTPARMS];
  enum PARMTYPE aParmTypes[MAXREPORTPARMS];
  void *aParameters[MAXREPORTPARMS];
} REPORT;

typedef struct typeREPORTCOL {
  BOOL bActive;

  BOOL bAllocateColumnRAM;

  char *sDescription;
  char *sHeader1;
  char *sHeader2;
  char *sHeader3;
  char *sPrintFormat;

  UINT32 *puiCurValue;
  SINT32 *psiCurValue;
  FLOAT *pfCurValue;

  UINT32 *puiColumnData;
  SINT32 *psiColumnData;
  FLOAT *pfColumnData;

  UINT32 uiCurRowValue;
  SINT32 siCurRowValue;
  FLOAT fCurRowValue;

  char *sTriggers;

  enum CDATATYPE eSourceDataType;
  enum CDATATYPE eFinalDataType;
  enum COLTYPE eColType;

  SINT32 siMinMaxVal;
  UINT32 uiMinMaxVal;
  FLOAT  fMinMaxVal;

  FLOAT fRowSum;
  FLOAT fRowSum2;

  FLOAT fColSum;
  FLOAT fColSum2;

} REPORTCOL;


typedef struct typeREPORTSTAT {
  
  unsigned char bActive;
  UINT32 iActiveID;

  char *sDataColumn;

  BOOL bCustomField;  
  char *sCustomDescription;
  void *pCustomValue;
  enum CDATATYPE eCustomType;
  char *sPrintCustomFormat;

  char *sBaseDescription;

  char *sStatParms;

  UINT32 iStatFlags;
  
  char *sTriggers;

  BOOL bSortByStep;

} REPORTSTAT;

#ifndef BYTE
#define BYTE unsigned char
#endif

typedef BYTE* VARSTATE;

typedef struct typeVARSTATELIST {

  VARSTATE vsState;

  struct typeVARSTATELIST* pNext;

} VARSTATELIST;

typedef struct typeDYNAMICPARM {
  
  void *pTarget;
  enum CDATATYPE eDataType;
  UINT32 *pBase;
  FLOAT fFactor;

} DYNAMICPARM;

