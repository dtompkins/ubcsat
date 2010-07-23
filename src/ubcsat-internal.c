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
    This file is the kitchen sink of ubcsat... 
    Ideally, you shouldn't have to ever look at any of this code.
    
    In fact... stop looking at it now... go away.
*/

const char *sValidStatCodes[NUMVALIDSTATCODES] = {"all","mean","stddev","cv","var","stderr","vmr","sum","median","min","max","q05","q10","q25","q75","q90","q95","q98","qr75/25","qr90/10","qr95/05","stepmean","solvemean","failmean","solvemedian","failmedian","solvemin","failmin","solvemax","failmax"};

UINT32 aActiveCalcColumns[MAXITEMLIST];
FXNPTR aActiveProcedures[NUMEVENTPOINTS][MAXFXNLIST];
ALGORITHM aAlgorithms[MAXNUMALG];
REPORTCOL aColumns[MAXITEMLIST];
DYNAMICPARM aDynamicParms[MAXDYNAMICPARMS];
UINT32 aNumActiveProcedures[NUMEVENTPOINTS];
UINT32 aOutputColumns[MAXITEMLIST];
REPORT aReports[MAXREPORTS];
UINT32 aRTDColumns[MAXITEMLIST];
REPORTSTAT aStats[MAXITEMLIST];
char **aTotalParms;
TRIGGER aTriggers[MAXITEMLIST];
BOOL aParmValid[MAXTOTALPARMS];
BOOL bReportClean;
BOOL bReportFlush;
BOOL bReportEcho;
BOOL bRestart;
BOOL bSolutionFound;
BOOL bSolveMode;
BOOL bTerminateAllRuns;
BOOL bTerminateRun;
BOOL bWeighted;
FLOAT fDummy;
FLOAT fFlipsPerSecond;
FLOAT fBestScore;
FLOAT fTargetW;
SINT32 iBestScore;
UINT32 iCutoff;
UINT32 iFind;
UINT32 iFindUnique;
UINT32 iFlipCandidate;
UINT32 iNumActiveCalcColumns;
UINT32 iNumAlg;
UINT32 iNumDynamicParms;
UINT32 iNumOutputColumns;
UINT32 iNumReports;
UINT32 iNumRTDColumns;
UINT32 iNumRuns;
UINT32 iNumSolutionsFound;
UINT32 iNumStatsActive;
UINT32 iNumTotalParms;
UINT32 iPeriodicRestart;
PROBABILITY iProbRestart;
UINT32 iRun;
UINT32 iRunProceduresLoop;
UINT32 iRunProceduresLoop2;
UINT32 iSeed;
UINT32 iStagnateRestart;
UINT32 iStep;
UINT32 iTarget;
FLOAT fTimeOut;
FLOAT fGlobalTimeOut;
ITEMLIST listColumns;
ITEMLIST listStats;
ITEMLIST listTriggers;
ALGORITHM *pActiveAlgorithm;
ALGPARMLIST parmAlg;
ALGPARMLIST parmHelp;
ALGPARMLIST parmIO;
ALGPARMLIST parmUBCSAT;
char *pVersion;
char *sAlgName;
char *sCommentString;
char *sFilenameIn;
char *sFilenameParms;
char *sFilenameVarInit;
char *sFilenameSoln;
char sMasterString[MAXITEMLISTSTRINGLENGTH];
const char sNull = 0;
char sParmLine[MAXPARMLINELEN];
char sStringParm[MAXPARMLINELEN];
char *sVarName;

BOOL bReportStateLMOnly;
FLOAT fReportStateQuality;
BOOL bReportBestStepVars;
BOOL bReportOptClausesSol;
UINT32 iReportFalseHistCount;
BOOL bReportDistanceLMOnly;
UINT32 iReportDistHistCount;
BOOL bReportStateQuality;
UINT32 iReportStateQuality;
BOOL bReportTriggersAll;

void AddContainerItem(ITEMLIST *pList,const char *sID, const char *sList);
void AddItem(ITEMLIST *pList,const char *sID);
ALGPARM *AddParmCommon(ALGPARMLIST *pParmList,const char *sSwitch,const char *sTerseDescription,const char *sVerboseDescription,const char *sTriggers);
void AddReportParmCommon(REPORT *pRep, const char *sParmName);
void DeActivateTriggerID(UINT32 iFxnID, const char *sItem);
ALGORITHM *FindAlgorithm(const char *sFindName, const char *sFindVar, BOOL bFindWeighted);
UINT32 MatchParameter(char *sSwitch,char *sParm);
void ParseParameters(ALGPARMLIST *pParmList);
void SetAlgorithmDefaultReports();
void SetDefaultParms(ALGPARMLIST *pParmList);
UINT32 SetColStatFlags (char *sStatsParms);


void ActivateTriggers(char *sTriggers) {
  ParseItemList(&listTriggers,sTriggers,ActivateTriggerID);
}

void DeActivateTriggers(char *sTriggers) {
  ParseItemList(&listTriggers,sTriggers,DeActivateTriggerID);
}


void ActivateColumns(char *sColumns) {
  ParseItemList(&listColumns,sColumns,ActivateColumnID);
}

void ActivateStats(char *sStats) {
  ParseItemList(&listStats,sStats,ActivateStatID);
}

void ActivateAlgorithmTriggers() {
  ActivateTriggers(pActiveAlgorithm->sDataTriggers);
  ActivateTriggers(pActiveAlgorithm->sHeuristicTriggers);
}

void ActivateColumnID(UINT32 iColID, const char *sItem) {
  
  REPORTCOL *pCol;
  NOREF(sItem);
 
  pCol = &aColumns[iColID];

  if (pCol->bActive == FALSE) {
  
    pCol->bActive = TRUE;

    ActivateTriggers(pCol->sTriggers);

    switch (pCol->eColType) {
      case ColTypeMean:
      case ColTypeStddev:
      case ColTypeCV:
      case ColTypeMin:
      case ColTypeMax:
        ActivateTriggers("ColumnStepCalculation");
        aActiveCalcColumns[iNumActiveCalcColumns++] = iColID;
      case ColTypeFinal:
      case ColTypeFinalDivStep:
      case ColTypeFinalDivStep100:
        ActivateTriggers("ColumnRunCalculation");
        break;
    }
  }
}  

void ActivateStatID(UINT32 iStatID, const char *sItem) {

  REPORTSTAT *pStat;
  char *pPos;
  char *pPos2;

  pStat = &aStats[iStatID];

  if (pStat->bActive == FALSE) {
    pStat->bActive = TRUE;
    pStat->iActiveID = iNumStatsActive++;

    pPos = strchr(sItem,'[');
    if (pPos) {
      pPos++;
      pPos2 = strchr(pPos,']');
      if (pPos2) {
        *(pPos2) = 0;
        SetString(&pStat->sStatParms,pPos);
      }
    }

    if (pStat->bCustomField==FALSE) {

      pStat->iStatFlags = SetColStatFlags(pStat->sStatParms);
      ActivateColumns(pStat->sDataColumn);

      if (pStat->iStatFlags & STATCODE_RAMMASK) {
        ParseItemList(&listColumns,pStat->sDataColumn,AddAllocateRAMColumnID);
      }

      if (pStat->iStatFlags & STATCODE_SFMASK) {
        ActivateTriggers("SortByStepPerformance");
      }

    } else {
      ActivateColumns(pStat->sDataColumn);
      ParseItemList(&listColumns,pStat->sDataColumn,AddAllocateRAMColumnID);
      ActivateTriggers(pStat->sTriggers);
    }
  
  }
}

void ActivateTriggerID(UINT32 iFxnID, const char *sItem) {
  
  UINT32 j;
  BOOL bAlready;
  enum EVENTPOINT eEventPoint;
  TRIGGER *pTrigger;

  NOREF(sItem);

  pTrigger = &aTriggers[iFxnID];

  if (pTrigger->bActive == TRUE) {
    return;
  }

  pTrigger->bActive = TRUE;

  if (pTrigger->bDisabled == TRUE) {
    return;
  }

  ParseItemList(&listTriggers,pTrigger->sDependencyList,ActivateTriggerID);
  ParseItemList(&listTriggers,pTrigger->sDeactivateList,DeActivateTriggerID);

  eEventPoint = pTrigger->eEventPoint;

  bAlready = FALSE;
  for (j=0;j<aNumActiveProcedures[eEventPoint];j++) {
    if (aActiveProcedures[eEventPoint][j]==pTrigger->pProcedure) {
      bAlready = TRUE;
      break;
    }
  }

  if (bAlready==FALSE) {
    aActiveProcedures[eEventPoint][aNumActiveProcedures[eEventPoint]] = pTrigger->pProcedure;
    aNumActiveProcedures[eEventPoint]++;
    if (aNumActiveProcedures[eEventPoint] == MAXFXNLIST) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXFXNLIST [%u]\n",MAXFXNLIST);
      AbnormalExit();
    }
  }
}


void ActivateDynamicParms() {

  UINT32 j;
  DYNAMICPARM *pCurParm;

  UINT32 *iTarget;
  SINT32 *siTarget;
  FLOAT *fTarget;
  
  for (j=0;j<iNumDynamicParms;j++) {
    pCurParm = &(aDynamicParms[j]);
    
    switch (pCurParm->eDataType) {
      case DTypeUInt:
        iTarget = (UINT32 *) pCurParm->pTarget;
        if (*iTarget==0) {
          *iTarget = (UINT32) (*(pCurParm->pBase) * pCurParm->fFactor);
        }
        break;

      case DTypeSInt:
        siTarget = (SINT32 *) pCurParm->pTarget;
        if (*siTarget==0) {
          *siTarget = (SINT32) (*(pCurParm->pBase) * pCurParm->fFactor);
        }        
        break;

      case DTypeFloat:
        fTarget = (FLOAT *) pCurParm->pTarget;
        if (*fTarget==FLOATZERO) {
          *fTarget = (*(pCurParm->pBase) * pCurParm->fFactor);
        }
        break;
    }
  }
}

void AddAllocateRAMColumnID(UINT32 j, const char *sItem) {
  REPORTCOL *pCol;
  NOREF(sItem);

  pCol = &aColumns[j];
  pCol->bAllocateColumnRAM = TRUE;
  ActivateTriggers("AllocateColumnRAM");
}

void AddDynamicParm(void *pTarget, enum CDATATYPE eDataType, UINT32 *pBase, FLOAT fFactor) {

  if (iNumDynamicParms == 0) {
    ActivateTriggers("DynamicParms");
  }

  aDynamicParms[iNumDynamicParms].pTarget = pTarget;
  aDynamicParms[iNumDynamicParms].eDataType = eDataType;
  aDynamicParms[iNumDynamicParms].pBase = pBase;
  aDynamicParms[iNumDynamicParms].fFactor = fFactor;

  iNumDynamicParms++;

  if (iNumDynamicParms==MAXDYNAMICPARMS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXDYNAMICPARMS [%u] \n",MAXDYNAMICPARMS);
    AbnormalExit();
  }
}


void AddColumnComposite(const char *sID, 
                        const char *sList)
{
  AddContainerItem(&listColumns,sID,sList);
}

void AddColumnFloat(const char *sID, 
                  const char *sDescription, 
                  char *sHeader1,  
                  char *sHeader2,  
                  char *sHeader3, 
                  char *sPrintFormat,
                  FLOAT *pCurValue,
                  char *sTriggers,
                  enum COLTYPE eColType
                  )
{

  REPORTCOL *pCol;
  
  pCol = &aColumns[listColumns.iNumItems];

  pCol->bActive = FALSE;
  pCol->bAllocateColumnRAM = FALSE;

  SetString(&pCol->sDescription,sDescription);
  SetString(&pCol->sHeader1,sHeader1);
  SetString(&pCol->sHeader2,sHeader2);
  SetString(&pCol->sHeader3,sHeader3);
  SetString(&pCol->sPrintFormat,sPrintFormat);
  
  pCol->pfCurValue = pCurValue;

  pCol->eSourceDataType = DTypeFloat;
  pCol->eFinalDataType = DTypeFloat;

  SetString(&pCol->sTriggers,sTriggers);

  pCol->pfColumnData = 0;

  pCol->eColType = eColType;

  AddItem(&listColumns,sID);

}

void AddColumnUInt(const char *sID, 
                  const char *sDescription, 
                  char *sHeader1,  
                  char *sHeader2,  
                  char *sHeader3, 
                  char *sPrintFormat, 
                  UINT32 *pCurValue,
                  char *sTriggers,
                  enum COLTYPE eColType
                  )
{

  REPORTCOL *pCol;
  
  pCol = &aColumns[listColumns.iNumItems];

  pCol->bActive = FALSE;
  pCol->bAllocateColumnRAM = FALSE;

  SetString(&pCol->sDescription,sDescription);
  SetString(&pCol->sHeader1,sHeader1);
  SetString(&pCol->sHeader2,sHeader2);
  SetString(&pCol->sHeader3,sHeader3);
  SetString(&pCol->sPrintFormat,sPrintFormat);
  pCol->puiCurValue = pCurValue;

  pCol->eSourceDataType = DTypeUInt;

  SetString(&pCol->sTriggers,sTriggers);
  
  pCol->puiColumnData = NULL;
  pCol->pfColumnData = NULL;

  pCol->eColType = eColType;

  if ((eColType == ColTypeMean)||
      (eColType == ColTypeStddev)||
      (eColType == ColTypeCV)||
      (eColType == ColTypeFinalDivStep)||
      (eColType == ColTypeFinalDivStep100)) {
    pCol->eFinalDataType = DTypeFloat;
  } else {
    pCol->eFinalDataType = DTypeUInt;
  }

  pCol->fColSum = FLOATZERO;
  pCol->fColSum2 = FLOATZERO;

  AddItem(&listColumns,sID);
}

void AddContainerItem(ITEMLIST *pList,const char *sID, const char *sList) {
  SetString(&pList->aItems[pList->iNumItems].sID,sID);
  pList->aItems[pList->iNumItems].bContainer = TRUE;
  SetString(&pList->aItems[pList->iNumItems].sContainerList,sList);
  pList->iNumItems++;
  if (pList->iNumItems==MAXITEMLIST) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXITEMLIST [%u] \n",MAXITEMLIST);
    AbnormalExit();
  }
}

void AddItem(ITEMLIST *pList,const char *sID) {
  SetString(&pList->aItems[pList->iNumItems].sID,sID);
  pList->aItems[pList->iNumItems].bContainer = FALSE;
  pList->iNumItems++;
  if (pList->iNumItems==MAXITEMLIST) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXITEMLIST [%u] \n",MAXITEMLIST);
    AbnormalExit();
  }
}

void AddOutputColumnID(UINT32 j, const char *sItem) {
  
  aOutputColumns[iNumOutputColumns++] = j;

  if (bReportOutputSuppress) {
    AddAllocateRAMColumnID(j,sItem);
  }
}

void AddParmBool(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers,
                  UINT32 *pBool,
                  BOOL bDefBool)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->pParmValue = (void *) pBool;
  p->defDefault.bBool = bDefBool;
  p->eType = PTypeBool;
}

void AddParmFloat(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers,
                  FLOAT *pFloat,
                  FLOAT fDefFloat)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->pParmValue = (void *) pFloat;
  p->defDefault.fFloat = fDefFloat;
  p->eType = PTypeFloat;
}

ALGPARM *AddParmCommon(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers)
{
  ALGPARM *p;
  
  if (pParmList->iNumParms >= MAXALGPARMS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXALGPARMS [%u]\n",MAXALGPARMS);
    AbnormalExit();
  }

  p = &pParmList->aParms[pParmList->iNumParms++];

  SetString(&p->sSwitch,sSwitch);
  SetString(&p->sTerseDescription,sTerseDescription);
  SetString(&p->sVerboseDescription,sVerboseDescription);
  SetString(&p->sTriggers,sTriggers);
  p->bSpecified = FALSE;
  return(p);
}

void AddParmUInt(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers,
                  UINT32 *pInt,
                  UINT32 iDefInt)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->pParmValue = (void *) pInt;
  p->defDefault.iUInt = iDefInt;
  p->eType = PTypeUInt;
}

void AddParmProbability(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers,
                  PROBABILITY *pProb,
                  FLOAT fProb)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->pParmValue = (void *) pProb;
  p->defDefault.iProb = FloatToProb(fProb);
  p->eType = PTypeProbability;
}

void AddParmReport(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers
                  )
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->eType = PTypeReport;
}

void AddParmSInt(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers,
                  SINT32 *pSInt,
                  SINT32 iDefSInt)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->pParmValue = (void *) pSInt;
  p->defDefault.iSInt = iDefSInt;
  p->eType = PTypeSInt;
}

void AddParmString(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sTerseDescription, 
                  const char *sVerboseDescription,
                  const char *sTriggers,
                  char **pString,
                  char *sDefString)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sTerseDescription,sVerboseDescription,sTriggers);
  p->pParmValue = (void *) pString;
  SetString(&p->defDefault.sString,sDefString);
  p->eType = PTypeString;
}

void AddReportParmCommon(REPORT *pRep, const char *sParmName) {
  SetString(&pRep->aParmName[pRep->iNumParms],sParmName);
  pRep->iNumParms++;
  if (pRep->iNumParms==MAXREPORTPARMS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXREPORTPARMS [%u]\n",MAXREPORTPARMS);
    AbnormalExit();
  }
}

void AddReportParmFloat(REPORT *pRep, const char *sParmName, FLOAT *pParmValFloat, FLOAT fDefault) {
  pRep->aParmTypes[pRep->iNumParms] = PTypeFloat;
  pRep->aParameters[pRep->iNumParms] = (void *) pParmValFloat;
  *pParmValFloat = fDefault;
  AddReportParmCommon(pRep,sParmName);
}

void AddReportParmString(REPORT *pRep, const char *sParmName, const char *pDefault) {
  pRep->aParmTypes[pRep->iNumParms] = PTypeString;
  SetString((char **) &pRep->aParameters[pRep->iNumParms],pDefault);
  AddReportParmCommon(pRep,sParmName);
}

void AddReportParmUInt(REPORT *pRep, const char *sParmName, UINT32 *pParmValUInt, UINT32 iDefault) {
  pRep->aParmTypes[pRep->iNumParms] = PTypeUInt;
  pRep->aParameters[pRep->iNumParms] = (void *) pParmValUInt;
  *pParmValUInt = iDefault;
  AddReportParmCommon(pRep,sParmName);
}

void AddRTDColumnID(UINT32 j, const char *sItem) {

  aRTDColumns[iNumRTDColumns++] = j;

  AddAllocateRAMColumnID(j,sItem);
}

void AddStatCol(const char *sID, 
             const char *sBaseDescription, 
             const char *sDefParm,
             BOOL bSortByStep)
{

  REPORTSTAT *pStat;

  pStat = &aStats[listStats.iNumItems];

  pStat->bActive = FALSE;

  pStat->bCustomField = FALSE;

  SetString(&pStat->sBaseDescription,sBaseDescription);
  
  SetString(&pStat->sDataColumn,sID);
  SetString(&pStat->sStatParms,sDefParm);

  pStat->bSortByStep = bSortByStep;

  AddItem(&listStats,sID);
}

void AddContainerStat(const char *sID, 
                      const char *sList) {

  AddContainerItem(&listStats,sID,sList);
}

void AddStatCustom(const char *sID, 
                  const char *sCustomDescription,
                  const char *sBaseDescription, 
                  const char *sPrintCustomFormat,
                  void *pCurValue,
                  enum CDATATYPE eCustomType,
                  const char *sDataColumn,
                  const char *sTriggers
                )
{

  REPORTSTAT *pStat;

  pStat = &aStats[listStats.iNumItems];

  pStat->bActive = FALSE;

  pStat->bCustomField = TRUE;

  SetString(&pStat->sCustomDescription,sCustomDescription);

  SetString(&pStat->sBaseDescription,sBaseDescription);
  
  SetString(&pStat->sPrintCustomFormat,sPrintCustomFormat);
  
  pStat->pCustomValue = pCurValue;
  pStat->eCustomType = eCustomType;

  SetString(&pStat->sDataColumn,sDataColumn);

  SetString(&pStat->sTriggers,sTriggers);
  
  AddItem(&listStats,sID);

}

void CalculateStats(FLOAT *fMean, FLOAT *fStddev, FLOAT *fCV, FLOAT fSum, FLOAT fSum2, UINT32 iCount) {
  
  FLOAT fCountMul;
  FLOAT fCountMulMinusOne;
  FLOAT fStddev2;

  *fStddev = FLOATZERO;
  *fCV = FLOATZERO;

  if (iCount == 0) {
    *fMean = FLOATZERO;
  } else if (iCount == 1) {
    *fMean = fSum;
  } else {

    fCountMul = 1.0f / ((FLOAT) iCount);
    fCountMulMinusOne = 1.0f / ((FLOAT) (iCount - 1));

    *fMean = fSum * fCountMul;
    fStddev2 = (fSum2 - fCountMul * (fSum * fSum)) * fCountMulMinusOne;

    if (fStddev2 > FLOATSTATSMIN) {
      
      *fStddev = sqrt(fStddev2);

      if (*fMean > FLOATSTATSMIN) {
        *fCV = (*fStddev) / (*fMean);
      }
    } else {
      *fStddev = FLOATZERO;
      *fCV = FLOATZERO;
    }
  }
}

FLOAT CorrelationCoeff(FLOAT fSumA, FLOAT fSumA2,FLOAT fSumB, FLOAT fSumB2, FLOAT fSumAB, UINT32 iCount) {

  FLOAT fMeanA;
  FLOAT fStddevA;
  FLOAT fCVA;
  FLOAT fMeanB;
  FLOAT fStddevB;
  FLOAT fCVB;

  FLOAT fCountMul;
  FLOAT fCountMulMinusOne;

  FLOAT fCC;

  CalculateStats(&fMeanA, &fStddevA, &fCVA, fSumA, fSumA2, iCount);
  CalculateStats(&fMeanB, &fStddevB, &fCVB, fSumB, fSumB2, iCount);

  if (iCount == 0) {
    fCC = FLOATZERO;
  } else if (iCount == 1) {
    fCC = FLOATZERO;
  } else {
    fCountMul = 1.0f / ((FLOAT) iCount);
    fCountMulMinusOne = 1.0f / ((FLOAT) (iCount - 1));

    if ((fStddevA != FLOATZERO)&&(fStddevB != FLOATZERO)) {
      fCC = fCountMulMinusOne * (fSumAB - fCountMul * fSumA * fSumB);
      fCC /= (fStddevA * fStddevB);
    } else {
      fCC = FLOATZERO;
    }
  }
  return(fCC);
}


void CheckInvalidParamters() {

  UINT32 j;

  for (j=0;j<iNumTotalParms;j++) {
    if (!aParmValid[j]) {
      HelpBadParm(aTotalParms[j]);
    }
  }
}

void CheckParamterFile(int iCommandLineCount,char **aCommandLineArgs) {
  int j;
  UINT32 k;
  char *sParameterFilename = 0;
  FILE *filParm;
  char *pStart;
  char *pPos;

  UINT32 iNumParmFiles = 0;

  for (j=0;j<(iCommandLineCount - 1);j++) {
    if (MatchParameter("-param,-fp",aCommandLineArgs[j])) {
      iNumParmFiles++;
    }
  }

  if (iNumParmFiles==0) {
    iNumTotalParms = iCommandLineCount - 1;
    if (iNumTotalParms>=MAXTOTALPARMS) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
      AbnormalExit();
    }
    aTotalParms = &aCommandLineArgs[1];
  } else {

    aTotalParms = AllocateRAM(MAXTOTALPARMS * sizeof(char *));

    for (j=0;j<(iCommandLineCount - 1);j++) {
      if (MatchParameter("-param,-fp",aCommandLineArgs[j])) {
      
        sParameterFilename = aCommandLineArgs[j+1];

        SetupFile(&filParm,"r",sParameterFilename,stdin,0);

        while (!feof(filParm)) {
          if (fgets(sParmLine,MAXPARMLINELEN,filParm)) {
            if (strlen(sParmLine)==MAXPARMLINELEN-1) {
              ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXPARMLINELEN [%u]\n",MAXPARMLINELEN);
              AbnormalExit();
            }
            if ((*sParmLine)&&(*sParmLine != '#')) {
              pStart = sParmLine;
              pPos = strchr(pStart,' ');
              while (pPos) {
                if (pPos==pStart) {
                  pStart++;
                } else {
                  *pPos++=0;
                  SetString(&aTotalParms[iNumTotalParms++],pStart);
                  if (iNumTotalParms==MAXTOTALPARMS) {
                    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
                    AbnormalExit();
                  }
                  pStart = pPos;
                }
                pPos = strchr(pStart,' ');
              }
              pPos = strchr(pStart,10);
              if (pPos) {
                *pPos = 0;
              }
              pPos = strchr(pStart,13);
              if (pPos) {
                *pPos = 0;
              }
              if (strlen(pStart)) {
                SetString(&aTotalParms[iNumTotalParms++],pStart);
                if (iNumTotalParms==MAXTOTALPARMS) {
                  ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
                  AbnormalExit();
                }
              }
            }
          }
        }

        CloseSingleFile(filParm);

      }
    }

    for (j=1;j<(iCommandLineCount);j++) {
      SetString(&aTotalParms[iNumTotalParms++],aCommandLineArgs[j]);;
      if (iNumTotalParms==MAXTOTALPARMS) {
        ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
        AbnormalExit();
      }
    }
  }

  for (k=0;k<iNumTotalParms;k++) {
    aParmValid[k] = FALSE;
  }
  return;


}

void ClearActiveProcedures() {
  memset(aNumActiveProcedures,0,sizeof(UINT32) * NUMEVENTPOINTS);
}

void CopyParameters(ALGORITHM *pDest, const char *sName, const char *sVar, BOOL bWeighted) {
  UINT32 j;
  ALGPARMLIST *pParmList;
  ALGORITHM *pSrc = FindAlgorithm(sName,sVar,bWeighted);
  if (pSrc==0) {
    ReportPrint1(pRepErr,"Unexpected Error: Can't find algorithm %s\n",(char *) sName);
    AbnormalExit();
  }
  pParmList = &pSrc->parmList;
  pDest->parmList.iNumParms;
  for (j=0;j<pParmList->iNumParms;j++) {
    pDest->parmList.aParms[pDest->parmList.iNumParms] = pParmList->aParms[j];
    pDest->parmList.iNumParms++;
  }
}

ALGORITHM *CreateAlgorithm (const char *sName, const char *sVariant, BOOL bWeighted, 
                            const char *sDescription, 
                            const char *sAuthors,
                            const char *sHeuristicTriggers,
                            const char *sDataTriggers,
                            const char *sDefaultOutput,
                            const char *sDefaultStats) {
  
  ALGORITHM *pCurAlg;

  pCurAlg = &aAlgorithms[iNumAlg];

  SetString(&pCurAlg->sName,sName);
  SetString(&pCurAlg->sVariant,sVariant);
  pCurAlg->bWeighted = bWeighted;

  SetString(&pCurAlg->sDescription,sDescription);
  SetString(&pCurAlg->sAuthors,sAuthors);

  SetString(&pCurAlg->sHeuristicTriggers,sHeuristicTriggers);
  SetString(&pCurAlg->sDataTriggers,sDataTriggers);

  SetString(&pCurAlg->sDefaultOutput,sDefaultOutput);
  SetString(&pCurAlg->sDefaultStats,sDefaultStats);

  pCurAlg->parmList.iNumParms = 0;

  iNumAlg++;

  if (iNumAlg==MAXNUMALG) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXNUMALG [%u]\n",MAXNUMALG);
    AbnormalExit();
  }

  return(pCurAlg);
}


void CreateContainerTrigger(const char *sID, const char *sList) {
  AddContainerItem(&listTriggers,sID,sList);
}

REPORT *CreateReport(const char *sID, const char *sDescription, const char *sVerboseDescription, const char *sOutputFile, const char *sTriggers) {
  REPORT *pRep;

  pRep = &aReports[iNumReports++];

  if (iNumReports==MAXREPORTS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXREPORTS [%u] \n",MAXREPORTS);
    AbnormalExit();
  }


  SetString(&pRep->sID,sID);
  pRep->bActive = FALSE;
  pRep->bSpecialFileIO = FALSE;
  pRep->fileOut = stdout;
  SetString(&pRep->sOutputFile,sOutputFile);
  pRep->iNumParms = 0;
  SetString(&pRep->sTriggers,sTriggers);
  SetString(&pRep->sDescription,sDescription);
  SetString(&pRep->sVerboseDescription,sVerboseDescription);

  return (pRep);
}

void CreateTrigger(const char *sID, enum EVENTPOINT eEventPoint, FXNPTR pProcedure, char *sDependencyList, char *sDeactivateList) {
  TRIGGER *pNewTrig;

  pNewTrig = &aTriggers[listTriggers.iNumItems];

  pNewTrig->eEventPoint = eEventPoint;
  pNewTrig->pProcedure = pProcedure;
  SetString(&pNewTrig->sDependencyList,sDependencyList);
  SetString(&pNewTrig->sDeactivateList,sDeactivateList);

  AddItem(&listTriggers,sID);
}

void DeActivateTriggerID(UINT32 iFxnID, const char *sItem) {
  
  UINT32 j;
  UINT32 k;
  enum EVENTPOINT eEventPoint;
  TRIGGER *pTrigger;

  NOREF(sItem);

  pTrigger = &aTriggers[iFxnID];

  pTrigger->bDisabled = TRUE;

  if (pTrigger->bActive == TRUE) {

    eEventPoint = pTrigger->eEventPoint;

    for (j=0;j<aNumActiveProcedures[eEventPoint];j++) {
      if (aActiveProcedures[eEventPoint][j]==pTrigger->pProcedure) {
        for (k=j;k<aNumActiveProcedures[eEventPoint]-1;k++) {
          aActiveProcedures[eEventPoint][k] = aActiveProcedures[eEventPoint][k+1];
        }
        aNumActiveProcedures[eEventPoint]--;
        break;
      }
    }
  }
}

ALGORITHM *FindAlgorithm(const char *sFindName, const char *sFindVar, BOOL bFindWeighted) {
  UINT32 j;
  if (*sFindName != 0) {
    for (j=0;j<iNumAlg;j++) {
      if (strcmp(sFindName,aAlgorithms[j].sName)==0) {
        if (strcmp(sFindVar,aAlgorithms[j].sVariant)==0) {
          if (aAlgorithms[j].bWeighted == bFindWeighted) {
            return(&aAlgorithms[j]);
          }
        }
      }
    }
  }
  return(0);
}

UINT32 FindItem(ITEMLIST *pList,char *sID) {
  SINT32 j;
  char *pPos;
  char *pPos2;
  UINT32 iLen;

  pPos = strchr(sID,'[');
  if (pPos) {
    pPos2 = strchr(sID,']');
    if (pPos2) {
      iLen = pPos - sID;
      for (j=0;j<(SINT32)pList->iNumItems;j++) {
        if (strlen(pList->aItems[j].sID) == iLen) {
          if (strncmp(sID,pList->aItems[j].sID,iLen)==0) {
            return(j);
          }
        }
      }
    } else {
      ReportPrint1(pRepErr,"Error: unbalanced [] in (%s)\n",sID);
    }
  } else {
    for (j=0;j<(SINT32)pList->iNumItems;j++) {
        if (strcmp(sID,pList->aItems[j].sID)==0) {
          return(j);
        }
    }
  }
  ReportPrint1(pRepErr,"Error: reference to (%s) is unknown\n",sID);
  AbnormalExit();
  return(0);
}

ALGPARM *FindParm(ALGPARMLIST *pParmList, char *sSwitch) {

  UINT32 j;

  for (j=0;j<pParmList->iNumParms;j++) {
    if (MatchParameter(sSwitch,pParmList->aParms[j].sSwitch)) {
      return(&pParmList->aParms[j]);
    }
  }
  return(0);
}

ALGORITHM *GetAlgorithm() {
  UINT32 j;
  if (*sAlgName) {
    for (j=0;j<iNumAlg;j++) {
      if (strcmp(sAlgName,aAlgorithms[j].sName)==0) {
        if (strcmp(sVarName,aAlgorithms[j].sVariant)==0) {
          if (aAlgorithms[j].bWeighted == bWeighted) {
            return(&aAlgorithms[j]);
          }
        }
      }
    }
  }
  return(0);
}

void InheritDataTriggers(ALGORITHM *pDest, const char *sName, const char *sVar, BOOL bWeighted) {
  ALGORITHM *pSrc = FindAlgorithm(sName,sVar,bWeighted);
  if (pSrc==0) {
    ReportPrint1(pRepErr,"Unexpected Error: Can't find algorithm %s\n",(char *) sName);
    AbnormalExit();
  }
  if (pDest->sDataTriggers) {
    *sMasterString = 0;
    strcat(sMasterString,pDest->sDataTriggers);
    strcat(sMasterString,",");
    strcat(sMasterString,pSrc->sDataTriggers);
    SetString(&pDest->sDataTriggers,sMasterString);
  } else {
    SetString(&pDest->sDataTriggers,pSrc->sDataTriggers);
  }
}

BOOL IsLocalMinimum(BOOL bUseWeighted) {
  UINT32 j,k;
  UINT32 iNumOcc;
  UINT32 *pClause;
  LITTYPE litCur;
  SINT32 iScore;
  FLOAT fScore;

  if (bUseWeighted) {

    for (j=1;j<=iNumVars;j++) {

      fScore = FLOATZERO;
    
      litCur = GetFalseLit(j);

      iNumOcc = aNumLitOcc[litCur];
      pClause = pLitClause[litCur];
  
      for (k=0;k<iNumOcc;k++) {
        if (aNumTrueLit[*pClause]==0) {
          fScore -= aClauseWeight[*pClause];
        }
        pClause++;
      }

      iNumOcc = aNumLitOcc[GetNegatedLit(litCur)];
      pClause = pLitClause[GetNegatedLit(litCur)];
  
      for (k=0;k<iNumOcc;k++) {
        if (aNumTrueLit[*pClause]==1) {
          fScore += aClauseWeight[*pClause];
        }
        pClause++;
      }

      if (fScore < FLOATZERO) {
        return(FALSE);
      }
    }
  
  } else {

    for (j=1;j<=iNumVars;j++) {

      iScore = 0;
    
      litCur = GetFalseLit(j);

      iNumOcc = aNumLitOcc[litCur];
      pClause = pLitClause[litCur];
  
      for (k=0;k<iNumOcc;k++) {
        if (aNumTrueLit[*pClause]==0) {
          iScore--;
        }
        pClause++;
      }

      iNumOcc = aNumLitOcc[GetNegatedLit(litCur)];
      pClause = pLitClause[GetNegatedLit(litCur)];
  
      for (k=0;k<iNumOcc;k++) {
        if (aNumTrueLit[*pClause]==1) {
          iScore++;
        }
        pClause++;
      }

      if (iScore < 0) {
        return(FALSE);
      }
    }
  }
  return(TRUE);
}

UINT32 MatchParameter(char *sSwitch,char *sParm) {
  
  char *pPos;
  char *pEndParm;
  
  pPos = strstr(sSwitch,sParm);
  while (pPos) {
    pEndParm = pPos + strlen(sParm);

    if ((*pEndParm==0)||(*pEndParm=='|')||(*pEndParm==',')) {
      if (pPos==sSwitch) {
        return(1);
      } else {
        if ((*(pPos-1)=='|')||(*(pPos-1)==',')) {
          return(1);
        }
      }
    }
    
    pPos = strstr(pPos + 1,sParm);
  }
  return(0);
}

void ParseAllParameters(int argc, char *argv[]) {

  if (argc==1) {
    bShowHelp = TRUE;
    CheckPrintHelp();
  }
  
  CheckParamterFile(argc,argv);

  ParseParameters(&parmHelp);
  ParseParameters(&parmAlg);
  
  pActiveAlgorithm = GetAlgorithm();

  CheckPrintHelp();
  
  SetAlgorithmDefaultReports();

  ParseParameters(&parmUBCSAT);
  ParseParameters(&parmIO);
  ParseParameters(&(pActiveAlgorithm->parmList));

  CheckInvalidParamters();
}

void ParseItemList(ITEMLIST *pList, char *sItems, CALLBACKPTR ItemFunction) {

  char *pPos;
  SINT32 j;

  if (*sItems==0) {
    return;
  }

  pPos = strchr(sItems,',');
  if (pPos) {
    if (strlen(sItems) > MAXITEMLISTSTRINGLENGTH-1) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXITEMLISTSTRINGLENGTH [%u] \n",MAXITEMLISTSTRINGLENGTH);
      AbnormalExit();
    }
    strcpy(sMasterString,sItems);
    sMasterString[pPos-sItems] = 0;
    pPos++;
    ParseItemList(pList,sMasterString,ItemFunction);
    ParseItemList(pList,pPos,ItemFunction);
    return;
  }
  j = FindItem(pList,sItems);
  if (pList->aItems[j].bContainer) {
    ParseItemList(pList,pList->aItems[j].sContainerList,ItemFunction);
  } else {
    ItemFunction((UINT32) j,sItems);
  }
}

void ParseParameters(ALGPARMLIST *pParmList) {

  UINT32 j;

  UINT32 iCurParm = 0;
  SINT32 iCurReport;
  REPORT *pRep;
  UINT32 iNumRepParms;
  ALGPARM *pParm;

  
  float fTemp;
  float fTemp2 = 100.0;
  BOOL bRatioParm;

  SetDefaultParms(pParmList);

  while (iCurParm < iNumTotalParms) {
    pParm = 0;
    for (j=0;j<pParmList->iNumParms;j++) {
      
      if (MatchParameter(pParmList->aParms[j].sSwitch,aTotalParms[iCurParm])) {
        pParm = &pParmList->aParms[j];
        aParmValid[iCurParm++] = TRUE;
        break;
      }
    }

    if (pParm) {
      switch (pParm->eType)
      {
        case PTypeUInt:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          if (strcmp(aTotalParms[iCurParm],"max")==0) {
            *((UINT32 *) pParm->pParmValue) = UINT32MAX;
          } else {
            if (aTotalParms[iCurParm][strlen(aTotalParms[iCurParm])-1] == 'n') {
              if (strlen(aTotalParms[iCurParm]) > 1 ) {
                if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                  HelpBadParm(aTotalParms[iCurParm-1]);
                }
              } else {
                fTemp = 1.0f;
              }
              AddDynamicParm(pParm->pParmValue,DTypeUInt,&iNumVars,fTemp);
              *((UINT32 *) pParm->pParmValue) = 0;
            } else {
              if (sscanf(aTotalParms[iCurParm],"%u",pParm->pParmValue)==0) {
                HelpBadParm(aTotalParms[iCurParm-1]);
              }
            }
          }
          aParmValid[iCurParm++] = TRUE;
          break;
        case PTypeSInt:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          if (aTotalParms[iCurParm][strlen(aTotalParms[iCurParm])-1] == 'n') {
            if (strlen(aTotalParms[iCurParm]) > 1 ) {
              if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                HelpBadParm(aTotalParms[iCurParm-1]);
              }
            } else {
              fTemp = 1.0f;
            }
            AddDynamicParm(pParm->pParmValue,DTypeSInt,&iNumVars,fTemp);
            *((SINT32 *) pParm->pParmValue) = 0;
          } else {
            if (sscanf(aTotalParms[iCurParm],"%d",pParm->pParmValue)==0) {
              HelpBadParm(aTotalParms[iCurParm-1]);
            }
          }
          aParmValid[iCurParm++] = TRUE;
          break;
        case PTypeBool:
          *((UINT32 *)pParm->pParmValue) = 1;
          if (iCurParm < iNumTotalParms) {
            if (sscanf(aTotalParms[iCurParm],"%i",pParm->pParmValue)) {
              aParmValid[iCurParm++] = TRUE;
            }
          }
          break;

        case PTypeProbability:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          
          if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }

          bRatioParm = FALSE;
          aParmValid[iCurParm++] = TRUE;
          if (iCurParm < iNumTotalParms) {
            if (sscanf(aTotalParms[iCurParm],"%f",&fTemp2)) {
              bRatioParm = TRUE;
              aParmValid[iCurParm++] = TRUE;
              if (fTemp2 == 0.000000) {
                ReportPrint(pRepErr,"\n\nProbability invalid: The 2nd parameter to a Probability can not be 0\n\n");
                HelpBadParm(aTotalParms[iCurParm-3]);
                AbnormalExit();
              }
            }
          }
          
          if (bRatioParm == FALSE) {
            if (strcmp("1",aTotalParms[iCurParm-1])==0) {
              ReportPrint(pRepErr,"\n\nProbability ambiguous: 1/100 or 1.0? specify 0.01 or 1 100 or 1.0\n\n");
              HelpBadParm(aTotalParms[iCurParm-2]);
              AbnormalExit();
            }
            if (fTemp > 1.0) {
              fTemp /= 100;
              if (fTemp > 1.0) {
                ReportPrint(pRepErr,"\n\nProbability invalid: specify range 0.0-1.0 or 0-100\n\n");
                HelpBadParm(aTotalParms[iCurParm-2]);
                AbnormalExit();
              }
            }
          } else {
            fTemp = fTemp / fTemp2;
            if (fTemp > 1.0) {
              ReportPrint(pRepErr,"\n\nProbability ratio invalid: numerator > denominator \n\n");
              HelpBadParm(aTotalParms[iCurParm-3]);
              AbnormalExit();
            }
          }

          *((PROBABILITY *)pParm->pParmValue) = FloatToProb (fTemp);

          break;

        case PTypeString:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          SetString((char **) pParm->pParmValue,aTotalParms[iCurParm]);
          aParmValid[iCurParm++] = TRUE;
          break;

        case PTypeFloat:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          if (aTotalParms[iCurParm][strlen(aTotalParms[iCurParm])-1] == 'n') {
            if (strlen(aTotalParms[iCurParm]) > 1 ) {
              if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                HelpBadParm(aTotalParms[iCurParm-1]);
              }
            } else {
              fTemp = 1.0f;
            }
            AddDynamicParm(pParm->pParmValue,DTypeFloat,&iNumVars,fTemp);
            *((FLOAT *)pParm->pParmValue) = FLOATZERO;
          } else {
            if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
              HelpBadParm(aTotalParms[iCurParm-1]);
            }
            *((FLOAT *)pParm->pParmValue) = (FLOAT) fTemp;
          }
          aParmValid[iCurParm++] = TRUE;
          break;

        case PTypeReport:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          iCurReport = -1;          
          for (j=0;j<iNumReports;j++) {
            if (strcmp(aTotalParms[iCurParm],aReports[j].sID)==0) {
              iCurReport = j;
              break;
            }
          }
          if (iCurReport==-1) {
             HelpBadParm(aTotalParms[iCurParm-1]);
          }
          pRep = &aReports[iCurReport];
          
          pRep->bActive = TRUE;
          aParmValid[iCurParm++] = TRUE;

          if (iCurParm < iNumTotalParms) {
            if (*aTotalParms[iCurParm] != '-') {
              SetString(&pRep->sOutputFile,aTotalParms[iCurParm]);
              aParmValid[iCurParm++] = TRUE;
            }
            iNumRepParms = 0;
            while (iNumRepParms < pRep->iNumParms) {
              if (iCurParm < iNumTotalParms) {
                if (*aTotalParms[iCurParm] != '-') {
                  switch (pRep->aParmTypes[iNumRepParms])
                  {
                    case PTypeUInt:
                      if (aTotalParms[iCurParm][strlen(aTotalParms[iCurParm])-1] == 'n') {
                        if (strlen(aTotalParms[iCurParm]) > 1 ) {
                          if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                            HelpBadParm(aTotalParms[iCurParm]);
                          }
                        } else {
                          fTemp = 1.0f;
                        }
                        AddDynamicParm(pRep->aParameters[iNumRepParms],DTypeUInt,&iNumVars,fTemp);
                        *((UINT32 *) pRep->aParameters[iNumRepParms]) = 0;
                      } else {
                        if (sscanf(aTotalParms[iCurParm],"%i",pRep->aParameters[iNumRepParms])==0) {
                          HelpBadParm(aTotalParms[iCurParm]);
                        }
                      }
                      aParmValid[iCurParm++] = TRUE;
                      break;
                    case PTypeFloat:
                      if (aTotalParms[iCurParm][strlen(aTotalParms[iCurParm])-1] == 'n') {
                        if (strlen(aTotalParms[iCurParm]) > 1 ) {
                          if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                            HelpBadParm(aTotalParms[iCurParm]);
                          }
                        } else {
                          fTemp = 1.0f;
                        }
                        AddDynamicParm(pRep->aParameters[iNumRepParms],DTypeFloat,&iNumVars,fTemp);
                        *((FLOAT *) pRep->aParameters[iNumRepParms]) = FLOATZERO;
                      } else {
                        if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                          HelpBadParm(aTotalParms[iCurParm]);
                        }
                        *((FLOAT *)pRep->aParameters[iNumRepParms]) = (FLOAT) fTemp;
                      }
                      aParmValid[iCurParm++] = TRUE;
                      break;
                    case PTypeString:
                      SetString((char **) &pRep->aParameters[iNumRepParms],aTotalParms[iCurParm]);
                      aParmValid[iCurParm++] = TRUE;
                      break;
                  }
                }
              }
              iNumRepParms++;
            }
          }
          break;      
      }
      
      pParm->bSpecified = TRUE;

      ParseItemList(&listTriggers,pParm->sTriggers,ActivateTriggerID);
      
    } else {
      iCurParm++;
    }
  }
}

void PrintAlgParmSettings(REPORT *pRep, ALGPARMLIST *pParmList) {
  
  UINT32 j;
  ALGPARM *pCurParm;

  for (j=0;j<pParmList->iNumParms;j++) {
    pCurParm = &pParmList->aParms[j];
    ReportHdrPrefix(pRep);
    ReportHdrPrint1(pRep," %s ",pCurParm->sSwitch);
    switch(pCurParm->eType)
    {
      case PTypeUInt:
        ReportHdrPrint1(pRep,"%u ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeSInt:
        ReportHdrPrint1(pRep,"%d ", *(int *)pCurParm->pParmValue);
        break;
      case PTypeProbability:
        ReportHdrPrint1(pRep,"%.4g ", ProbToFloat(*(PROBABILITY *)pCurParm->pParmValue));
        break;
      case PTypeString:
        if (**(char **)pCurParm->pParmValue ==0) {
          ReportHdrPrint1(pRep,"%s ","[null]");
        } else {
          ReportHdrPrint1(pRep,"%s ", *(char **)pCurParm->pParmValue);
        }
        break;
      case PTypeFloat:
        ReportHdrPrint1(pRep,"%.6g ", *(FLOAT *)pCurParm->pParmValue);
        break;
      case PTypeBool:
        ReportHdrPrint1(pRep,"%u ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeReport:
        break;
    }  
    ReportHdrPrint(pRep,"\n");
  }
}

void SetAlgorithmDefaultReports() {
  SetString((char **) &pRepOut->aParameters[0],pActiveAlgorithm->sDefaultOutput);
  SetString((char **) &pRepStats->aParameters[0],pActiveAlgorithm->sDefaultStats);
}

void SetDefaultParms(ALGPARMLIST *pParmList) {
  UINT32 j;
  ALGPARM *pParm;

  for (j=0;j<pParmList->iNumParms;j++) {
    pParm = &pParmList->aParms[j];

    switch (pParm->eType)
    {
      case PTypeUInt:
        *((UINT32 *)pParm->pParmValue) = pParm->defDefault.iUInt;
        break;
      case PTypeSInt:
        *((SINT32 *)pParm->pParmValue) = pParm->defDefault.iSInt;
        break;
      case PTypeBool:
        *((UINT32 *)pParm->pParmValue) = pParm->defDefault.bBool;
        break;
      case PTypeProbability:
        *((PROBABILITY *)pParm->pParmValue) = pParm->defDefault.iProb;
        break;
      case PTypeString:
        SetString((char **) pParm->pParmValue,pParm->defDefault.sString);
        break;
      case PTypeFloat:
        *((FLOAT *)pParm->pParmValue) = pParm->defDefault.fFloat;
        break;
      case PTypeReport:
        break;
    }
  }
}


void SetupUBCSAT() {

  /* Initialize all global variables here */

  bClausePenaltyCreated = FALSE;
  bHelpHeaderShown = FALSE;
  bKnownSolutions = FALSE;
  bMobilityColNActive = FALSE;
  bMobilityColXActive = FALSE;

  bSortedByStepsValid = FALSE;
  bWeighted = FALSE;

  fDummy = FLOATZERO;

  iLastUnique = 0;
  iVarFlipHistoryLen = 0;

  iNumActiveCalcColumns = 0;
  iNumAlg = 0;
  iNumDynamicParms = 0;
  iNumOutputColumns = 0;
  iNumReports = 0;
  iNumRTDColumns = 0;
  iNumStatsActive = 0;
  iNumUniqueSolutions = 0;

  iNumVars = 0;
  iNumClauses = 0;
  iNumLits = 0;
  iVARSTATELen = 0;

  iNumHeap = 0;

  listColumns.iNumItems = 0;
  listStats.iNumItems = 0;
  listTriggers.iNumItems = 0;

  pActiveAlgorithm = NULL;
  pVersion = (char *) &sVersion[0];

  parmAlg.iNumParms = 0;
  parmHelp.iNumParms = 0;
  parmIO.iNumParms = 0;
  parmUBCSAT.iNumParms = 0;

  vslKnownSoln.pNext = NULL;
  vslKnownSoln.vsState = NULL;
  vslUnique.pNext = NULL;
  vslUnique.vsState = NULL;

  /* initialize reports */

  SetString(&sCommentString,"#");

  pRepHelp = CreateReport("help","Help Report"," All help messages that are displayed (Defaults to stdout)","stdout","");
  pRepHelp->bActive = TRUE;
  pRepHelp->fileOut = stdout;

  pRepErr = CreateReport("err","Error Report","All errors that are encountered (Defaults to stderr)","stderr","");
  pRepErr->bActive = TRUE;
  pRepErr->fileOut = stderr;

  ClearActiveProcedures();
}


UINT32 SetColStatFlags (char *sStatsParms) {

  UINT32 j;
  char *pPos;
  char *pPos2;
  UINT32 iLen;
  UINT32 bValid;

  UINT32 iFlags = 0x00000000;
  
  pPos = sStatsParms;

  while (*pPos != 0) {
    bValid = FALSE;
    pPos2 = strstr(pPos,"+");

    if (pPos2) {
      iLen = pPos2 - pPos;
    } else {
      iLen = strlen(pPos);
    }
    
    if (iLen) {
      for (j=0;j<NUMVALIDSTATCODES;j++) {
        if (strlen(sValidStatCodes[j])==iLen) {
          if (strncmp(pPos,sValidStatCodes[j],iLen)==0) {
            bValid = TRUE;
            
            if (j==0) {
              iFlags = STATCODE_all;
            } else {
              iFlags |= (0x00000001 << j);              
            }
          }
        }
      }
      if (bValid==FALSE) {
        ReportPrint1(pRepErr,"Error: reference to (%s) is unknown\n",pPos);        
        AbnormalExit();
      }
    }

    pPos += iLen;
    if (pPos2) {
      pPos++;
    }
  }

  return(iFlags);
}


/*** VARSTATE support functions ***/

const UINT32 aNumOnes[256] = {
  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
};

VARSTATE NewVarState() {
  VARSTATE vsNew;
  UINT32 j;
  if (iVARSTATELen == 0) {
    ReportPrint(pRepErr,"Unexpected Error: iVARSTATELen = 0\n");
    AbnormalExit();
    return(NULL);
  } else {
    vsNew = AllocateRAM(iVARSTATELen * sizeof(BYTE));
    for (j=0;j<iVARSTATELen;j++) {
      vsNew[j] = 0x00;
    }
  }
  return(vsNew);
}

VARSTATE NewCopyVarState(VARSTATE vsCopy) {
  VARSTATE vsNew;
  UINT32 j;
  vsNew = AllocateRAM(iVARSTATELen * sizeof(BYTE));
  for (j=0;j<iVARSTATELen;j++) {
    vsNew[j] = vsCopy[j];
  }
  return(vsNew);
}

void SetCurVarState(VARSTATE vsIn) {
  UINT32 j;
  
  BYTE *pCur = vsIn;
  BYTE byteMask = 0x80;

  for (j=0;j<iVARSTATELen;j++) {
    vsIn[j] = 0x00;
  }

  for (j=1;j<=iNumVars;j++) {

    if (aVarValue[j]) {
      *pCur |= byteMask;
    }
    
    byteMask >>= 1;

    if (((j-1)&(0x07))==0x07) {
      pCur++;
      byteMask = 0x80;
    }
  }
}

BOOL SetCurVarStateString(VARSTATE vsIn, char *sVarState) {
  UINT32 j;
  
  BYTE *pCur = vsIn;
  BYTE byteMask = 0x80;

  for (j=0;j<iVARSTATELen;j++) {
    vsIn[j] = 0x00;
  }

  if (strlen(sVarState) < iNumVars) {
    ReportPrint1(pRepErr,"Warning! Ignoring variable state string: (too short) %s \n",sVarState);
    return(FALSE);
  }

  for (j=1;j<=iNumVars;j++) {

    if (sVarState[j-1] == '1') {
      *pCur |= byteMask;
    } else if (sVarState[j-1] != '0') {
      ReportPrint1(pRepErr,"Warning! Ignoring variable state string: (invalid input) %s \n",sVarState);
      return(FALSE);
    }
    
    byteMask >>= 1;

    if (((j-1)&(0x07))==0x07) {
      pCur++;
      byteMask = 0x80;
    }
  }
  return(TRUE);
}


void SetArrayFromVarState(UINT32 *aOut, VARSTATE vsIn) {
  UINT32 j;
  for (j=0;j<iNumVars;j++) {
    aOut[j+1] = GetVarStateBit(vsIn,j);
  }  
}

UINT32 HammingDistVarState(VARSTATE vsA, VARSTATE vsB) {
  UINT32 j;
  UINT32 iDist = 0;
  BYTE *pA = vsA;
  BYTE *pB = vsB;
  if ((vsA == NULL)||(vsB == NULL)) {
    return(iNumVars);
  }

  for (j=0;j<iVARSTATELen;j++) {
    iDist += aNumOnes[*pA++ ^ *pB++];
  }
  return(iDist);
}


BOOL IsVarStateEqual(VARSTATE vsA, VARSTATE vsB) {
  UINT32 j;
  BYTE *pA = vsA;
  BYTE *pB = vsB;
  if ((vsA == NULL)||(vsB == NULL)) {
    return(FALSE);
  }
  for (j=0;j<iVARSTATELen;j++) {
    if (*pA++ != *pB++) {
      return(FALSE);
    }
  }
  return(TRUE);
}

BOOL IsVarStateInList(VARSTATELIST *vsList, VARSTATE vsIn) {
  VARSTATELIST *vsNext = vsList;

  while (vsNext) {
    if (vsNext->vsState) {
      if (IsVarStateEqual(vsNext->vsState,vsIn)) {
        return(TRUE);
      }
    }
    vsNext = vsNext->pNext;
  }
  return(FALSE);
}


UINT32 MinHammingVarStateList(VARSTATELIST *vsList, VARSTATE vsIn) {
  VARSTATELIST *vsNext = vsList;
  UINT32 iBest = iNumVars;
  UINT32 iDist;


  while (vsNext) {
    if (vsNext->vsState) {
      iDist = HammingDistVarState(vsNext->vsState,vsIn);
      if (iDist < iBest) {
        iBest = iDist;
      }
    }
    vsNext = vsNext->pNext;
  }
  return(iBest);
}


void AddToVarStateList(VARSTATELIST *vsList, VARSTATE vsAdd) {
  VARSTATELIST *pNext;

  if (vsList==NULL) {
    ReportPrint(pRepErr,"Unexpected Error: invalid VARSTATELIST\n");
    AbnormalExit();
  }
  if (vsList->vsState) {
    pNext = vsList->pNext;
    vsList->pNext = AllocateRAM(sizeof(VARSTATELIST));
    vsList->pNext->pNext = pNext;
    vsList->pNext->vsState = NewCopyVarState(vsAdd);
  } else {
    vsList->vsState = NewCopyVarState(vsAdd);
  }
}

BOOL AddUniqueToVarStateList(VARSTATELIST *vsList, VARSTATE vsAdd) {
  if (!IsVarStateInList(vsList,vsAdd)) {
    AddToVarStateList(vsList,vsAdd);
    return(TRUE);
  } else {
    return(FALSE);
  }
}

