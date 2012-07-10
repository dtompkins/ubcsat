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
    This file contains the code to make the various reports work
*/

/***** Trigger ReportOut *****/

BOOL bReportOutputSuppress;

void ReportOutSetup();
void ReportOutSplash();
void ReportOutHeader();
void ReportOutRow();
void ReportOutSuppressed();

/***** Trigger ReportStats *****/
void ReportStatsSetup();
void ReportStatsPrint();

/***** Trigger ReportRTD *****/
void ReportRTDSetup();
void ReportRTDPrint();

/***** Trigger ReportModelPrint *****/
void ReportModelPrint();

/***** Trigger ReportCNFStatsPrint *****/
void ReportCNFStatsPrint();

/***** Trigger ReportStatePrint *****/
void ReportStatePrint();

/***** Trigger ReportSolutionPrint *****/
void ReportSolutionPrint();

/***** Trigger ReportUniqueSolPrint *****/
void ReportUniqueSolPrint();

/***** Trigger ReportBestSolPrint *****/
void ReportBestSolPrint();

/***** Trigger ReportBestStepPrint *****/
void ReportBestStepPrint();

/***** Trigger ReportTrajBestLM *****/
void ReportTrajBestLMPostStep();
void ReportTrajBestLMPostRun();

/***** Trigger ReportUnsatClausesPrint *****/
void ReportUnsatClausesPrint();

/***** Trigger ReportFalseHistPrint *****/
void ReportFalseHistPrint();

/***** Trigger ReportDistHistPrint *****/
void ReportDistancePrint();

/***** Trigger ReportDistHistPrint *****/
void ReportDistHistPrint();

/***** Trigger ReportFlipCountsPrint *****/
void ReportFlipCountsPrint();

/***** Trigger ReportBiasCountsPrint *****/
void ReportBiasCountsPrint();

/***** Trigger ReportUnsatCountsPrint *****/
void ReportUnsatCountsPrint();

/***** Trigger ReportVarLastPrint *****/
void ReportVarLastPrint();

/***** Trigger ReportClauseLastPrint *****/
void ReportClauseLastPrint();

/***** Trigger ReportSQGridPrint *****/
void ReportSQGridPrint();

/***** Trigger ReportPenaltyPrint *****/
void ReportPenaltyCreate();
void ReportPenaltyPrintStep();
void ReportPenaltyPrintRun();
BOOL bReportPenaltyReNormBase;
BOOL bReportPenaltyReNormFraction;
BOOL bReportPenaltyEveryLM;
FLOAT *aPenaltyStatsFinal;
FLOAT *aPenaltyStatsFinalSum;
FLOAT *aPenaltyStatsFinalSum2;

/***** Trigger ReportPenMeanPrint *****/
void ReportPenMeanPrint();

/***** Trigger ReportPenStddevPrint *****/
void ReportPenStddevPrint();

/***** Trigger ReportPenCVPrint *****/
void ReportPenCVPrint();

/***** Trigger ReportVW2WeightsPrint *****/
void ReportVW2WeightsPrint();

/***** Trigger ReportMobilityPrint *****/
void ReportMobilityPrint();
UINT32 iReportMobilityDisplay;
BOOL bReportMobilityNormalized;

/***** Trigger ReportMobFixedPrint *****/
void ReportMobFixedPrint();
UINT32 iMobFixedWindow;
BOOL bMobilityFixedIncludeStart;

/***** Trigger ReportMobFixedFreqPrint *****/
void ReportMobFixedFreqPrint();

/***** Trigger VarAgeFreqPrint *****/
void ReportVarAgeFreqPrint();
UINT32 iRepVarAgeFreqFormat;

/***** Trigger ReportAutoCorrPrint *****/
void ReportAutoCorrPrint();

/***** Trigger ReportTriggersPrint *****/
void ReportTriggersPrint();

/***** Trigger ReportParamILSPrint *****/
void ReportParamILSPrint();

/***** Trigger ReportSatCompetitionPrint *****/
void ReportSatCompetitionPrint();

/***** Trigger ReportCompetitionComment *****/
void ReportCompetitionComment();

/***** Trigger ReportMaxSatCompetitionPrint *****/
void ReportMaxSatCompetitionPrintStep();
void ReportMaxSatCompetitionPrintFinal();
void CreateReportMaxSatCompetitionSaveBest();
FLOAT fMaxSatPrintInterval;
FLOAT fMaxSatNextPrintTime;
UBIGINT iLastMaxSatPrintStep;
BOOL *aMaxSatSaveBest;

/***** Trigger SetupCSV *****/
void SetupCSV();

/***** Trigger ActivateStepsFoundColumns *****/
void ActivateStepsFoundColumns();

/***** Trigger AllocateColumnRAM *****/
void AllocateColumnRAM();

/***** Trigger CalcPercentSolve *****/
void CalcPercentSolve();
FLOAT fPercentSuccess;

/***** Trigger ColumnRunCalculation *****/
void ColumnRunCalculation();

/***** Trigger ColumnStepCalculation *****/
void ColumnStepCalculation();

/***** Trigger ColumnInit *****/
void ColumnInit();

/***** Trigger InitSolveModev *****/
void InitSolveMode();

/***** Trigger UpdatePercents *****/
void UpdatePercents();

/***** Trigger UpdateColSolutionFound *****/
void UpdateColSolutionFound();
UINT32 iColSolutionFound;

/***** Trigger MobilityColumn *****/
void InitMobilityColumnN();
void InitMobilityColumnX();
void UpdateMobilityColumn();
BOOL bMobilityColNActive;
BOOL bMobilityColXActive;
FLOAT fMobilityColNMean;
FLOAT fMobilityColXMean;
FLOAT fMobilityColNMeanNorm;
FLOAT fMobilityColXMeanNorm;
FLOAT fMobilityColNCV;
FLOAT fMobilityColXCV;

/***** Trigger UpdateTimes *****/
void UpdateTimes();

/***** Trigger SortByStepPerformance *****/
UINT32 *aSortedBySteps;
BOOL bSortedByStepsValid;
UINT32 *aSortedByCurrent;
UINT32 *aSortedByStepsAndFound;
void SortByCurrentColData(REPORTCOL *pCol);
void SortByCurrentColDataAndFound(REPORTCOL *pCol);
void SortByStepPerformance();

/***** Trigger CalcFPS *****/
void CalcFPS();

/***** Trigger StringAlgParms *****/
char *sStringAlgParms;
void StringAlgParms();

void AddReportTriggers() {

  CreateTrigger("ReportOutSetup",PostParameters,ReportOutSetup,"","");
  CreateTrigger("ReportOutSplash",PostParameters,ReportOutSplash,"","");
  CreateTrigger("ReportOutHeader",PreStart,ReportOutHeader,"","");
  CreateTrigger("ReportOutRow",PostRun,ReportOutRow,"ColumnRunCalculation","");
  CreateTrigger("ReportOutSuppressed",FinalReports,ReportOutSuppressed,"","");
  CreateContainerTrigger("ReportOut","ReportOutSetup,ReportOutSplash,ReportOutHeader,ReportOutRow,ReportOutSuppressed");

  CreateTrigger("ReportStatsSetup",PostParameters,ReportStatsSetup,"","");
  CreateTrigger("ReportStatsPrint",FinalReports,ReportStatsPrint,"","");
  CreateContainerTrigger("ReportStats","ReportStatsSetup,ReportStatsPrint");

  CreateTrigger("ReportRTDSetup",PostParameters,ReportRTDSetup,"","");
  CreateTrigger("ReportRTDPrint",FinalReports,ReportRTDPrint,"","");
  CreateContainerTrigger("ReportRTD","ReportRTDSetup,ReportRTDPrint");

  CreateTrigger("ReportModelPrint",FinalReports,ReportModelPrint,"","");

  CreateTrigger("ReportCNFStatsPrint",FinalReports,ReportCNFStatsPrint,"","");

  CreateTrigger("ReportStatePrint",PostStep,ReportStatePrint,"","");
  
  CreateTrigger("ReportSolutionPrint",PostRun,ReportSolutionPrint,"","");

  CreateTrigger("ReportUniqueSolPrint",PostRun,ReportUniqueSolPrint,"UniqueSolutions","");

  CreateTrigger("ReportBestSolPrint",PostRun,ReportBestSolPrint,"SaveBest","");

  CreateTrigger("ReportBestStepPrint",StepCalculations,ReportBestStepPrint,"BestFalse","");

  CreateTrigger("ReportTrajBestLMPostStep",StepCalculations,ReportTrajBestLMPostStep,"BestFalse","");
  CreateTrigger("ReportTrajBestLMPostRun",PostRun,ReportTrajBestLMPostRun,"BestFalse","");
  CreateContainerTrigger("ReportTrajBestLM","ReportTrajBestLMPostStep,ReportTrajBestLMPostRun");

  CreateTrigger("ReportUnsatClausesPrint",PostRun,ReportUnsatClausesPrint,"","");

  CreateTrigger("ReportFalseHistPrint",PostRun,ReportFalseHistPrint,"NumFalseCounts","");

  CreateTrigger("ReportDistancePrint",PostStep,ReportDistancePrint,"SolutionDistance","");

  CreateTrigger("ReportDistHistPrint",PostRun,ReportDistHistPrint,"DistanceCounts","");

  CreateTrigger("ReportFlipCountsPrint",PostRun,ReportFlipCountsPrint,"FlipCounts","");

  CreateTrigger("ReportBiasCountsPrint",PostRun,ReportBiasCountsPrint,"BiasCounts","");

  CreateTrigger("ReportUnsatCountsPrint",PostRun,ReportUnsatCountsPrint,"UnsatCounts","");

  CreateTrigger("ReportVarLastPrint",PostRun,ReportVarLastPrint,"VarLastChange","");

  CreateTrigger("ReportClauseLastPrint",PostRun,ReportClauseLastPrint,"ClauseLast","");

  CreateTrigger("ReportSQGridPrint",PostRun,ReportSQGridPrint,"SQGrid","");

  CreateTrigger("ReportPenaltyCreate",CreateStateInfo,ReportPenaltyCreate,"","");
  CreateTrigger("ReportPenaltyPrintStep",PostStep,ReportPenaltyPrintStep,"NullFlips","");
  CreateTrigger("ReportPenaltyPrintRun",PostRun,ReportPenaltyPrintRun,"NullFlips","");
  CreateContainerTrigger("ReportPenaltyPrint","ReportPenaltyCreate,ReportPenaltyPrintStep,ReportPenaltyPrintRun");

  CreateTrigger("ReportPenMeanPrint",PostRun,ReportPenMeanPrint,"PenaltyStats","");
  CreateTrigger("ReportPenStddevPrint",PostRun,ReportPenStddevPrint,"PenaltyStats","");
  CreateTrigger("ReportPenCVPrint",PostRun,ReportPenCVPrint,"PenaltyStats","");

  CreateTrigger("ReportMobilityPrint",PostRun,ReportMobilityPrint,"MobilityWindow","");
  CreateTrigger("ReportMobFixedPrint",PostStep,ReportMobFixedPrint,"MobilityWindow","");
  CreateTrigger("ReportMobFixedFreqPrint",PostRun,ReportMobFixedFreqPrint,"MobilityFixedFrequencies","");

  CreateTrigger("ReportVarAgeFreqPrint",PostRun,ReportVarAgeFreqPrint,"VarAgeFrequencies","");

  CreateTrigger("ReportVW2WeightsPrint",PostRun,ReportVW2WeightsPrint,"VW2Weights","");

  CreateTrigger("ReportAutoCorrPrint",PostRun,ReportAutoCorrPrint,"AutoCorr","");

  CreateTrigger("ReportTriggersPrint",PreStart,ReportTriggersPrint,"","");

  CreateTrigger("ReportParamILSPrint",PostRun,ReportParamILSPrint,"BestFalse","");

  CreateTrigger("ReportSatCompetitionPrint",FinalReports,ReportSatCompetitionPrint,"","");

  CreateTrigger("ReportCompetitionComment",PostParameters,ReportCompetitionComment,"","");

  CreateTrigger("ReportMaxSatCompetitionPrintStep",PostStep,ReportMaxSatCompetitionPrintStep,"","");
  CreateTrigger("ReportMaxSatCompetitionPrintFinal",FinalReports,ReportMaxSatCompetitionPrintFinal,"","");
  CreateTrigger("CreateReportMaxSatCompetitionSaveBest",CreateStateInfo,CreateReportMaxSatCompetitionSaveBest,"","");
  CreateContainerTrigger("ReportMaxSatCompetitionPrint","ReportMaxSatCompetitionPrintStep,ReportMaxSatCompetitionPrintFinal,CreateReportMaxSatCompetitionSaveBest,BestFalse");

  CreateTrigger("SetupCSV",PostParameters,SetupCSV,"","");

  CreateTrigger("ActivateStepsFoundColumns",PostParameters,ActivateStepsFoundColumns,"","");

  CreateTrigger("AllocateColumnRAM",PostRead,AllocateColumnRAM,"","");

  CreateTrigger("CalcPercentSolve",FinalCalculations,CalcPercentSolve,"","");
  CreateTrigger("CalcFPS",FinalCalculations,CalcFPS,"","");
  CreateTrigger("StringAlgParms",FinalCalculations,StringAlgParms,"","");
  CreateTrigger("ColumnRunCalculation",PostRun,ColumnRunCalculation,"","");
  CreateTrigger("ColumnStepCalculation",StepCalculations,ColumnStepCalculation,"ColumnInit","");
  CreateTrigger("ColumnInit",PreRun,ColumnInit,"","");
  CreateTrigger("SolveMode",PreStart,InitSolveMode,"ReportModelPrint","");
  CreateTrigger("UpdatePercents",FinalCalculations,UpdatePercents,"SortByStepPerformance","");
  CreateTrigger("UpdateColSolutionFound",RunCalculations,UpdateColSolutionFound,"","");

  CreateTrigger("InitMobilityColumnN",PostParameters,InitMobilityColumnN,"","");
  CreateTrigger("InitMobilityColumnX",PostParameters,InitMobilityColumnX,"","");
  CreateTrigger("MobilityColumn",RunCalculations,UpdateMobilityColumn,"MobilityWindow","");

  CreateTrigger("UpdateTimes",FinalCalculations,UpdateTimes,"SortByStepPerformance","");  
  CreateTrigger("SortByStepPerformance",FinalCalculations,SortByStepPerformance,"ActivateStepsFoundColumns","");

}

/***************************************************************************/

/***** Report -r out *****/

void ReportOutSetup() {
  iNumOutputColumns = 0;
  ParseItemList(&listColumns,(char *)pRepOut->aParameters[0],ActivateColumnID);
  ParseItemList(&listColumns,(char *)pRepOut->aParameters[0],AddOutputColumnID);
}

void ReportOutSplash() {
  PrintUBCSATHeader(pRepOut);
  if (strcmp(sFilenameIn,"")==0) {
    ReportHdrPrefix(pRepOut);
    ReportHdrPrint(pRepOut,"no -inst file specified: reading instance from console (stdin)\n");
    ReportHdrPrefix(pRepOut);
    ReportHdrPrint(pRepOut,"  -- e.g.: ubcsat < myfile.cnf\n");
    ReportHdrPrefix(pRepOut);
    ReportHdrPrint(pRepOut,"\n");
  }
}

void PrintColHeaders(REPORT *pRep,UINT32 iNumCols, UINT32 *aCols) {

  UINT32 j;
  if (!bReportClean) {

    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"Output Columns: ");
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint1(pRep,"|%s",listColumns.aItems[aCols[j]].sID);
    }
    ReportHdrPrint(pRep,"|\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrefix(pRep);
      ReportHdrPrint2(pRep,"%s: %s\n",listColumns.aItems[aCols[j]].sID,aColumns[aCols[j]].sDescription);
    }
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint1(pRep,"%s",aColumns[aCols[j]].sHeader1);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint1(pRep,"%s",aColumns[aCols[j]].sHeader2);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {      
      ReportHdrPrint1(pRep,"%s",aColumns[aCols[j]].sHeader3);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
  }

  if (bReportCSV) {
    for (j=0;j<iNumCols;j++) {
      ReportPrint1(pRep,"%s",listColumns.aItems[aCols[j]].sID);
      if (j < (iNumCols-1)) {
        ReportPrint1(pRep,"%s",sColSepString);
      }
    }
    ReportPrint(pRep,"\n");
  }

}

void ReportOutHeader() {
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint1(pRepOut," -alg %s",pActiveAlgorithm->sName);
  if (*(pActiveAlgorithm->sVariant)) {
    ReportHdrPrint1(pRepOut," -v %s",pActiveAlgorithm->sVariant);
  }
  if (pActiveAlgorithm->bWeighted) {
    ReportHdrPrint(pRepOut," -w");
  }
  
  ReportHdrPrint(pRepOut,"\n");

  PrintAlgParmSettings(pRepOut,&parmUBCSAT);
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"\n");
  PrintAlgParmSettings(pRepOut,&pActiveAlgorithm->parmList);
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"\n");
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"UBCSAT default output: \n");
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"   'ubcsat -r out null' to suppress, 'ubcsat -hc' for customization help\n");
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint(pRepOut,"\n");
  PrintColHeaders(pRepOut,iNumOutputColumns,aOutputColumns);
}

void PrintRow(REPORT *pRep, UINT32 iRow, UINT32 iNumCols, UINT32 *aCols) {

  UINT32 j;
  REPORTCOL *pCol;
  if (!bReportCSV) {
    ReportPrint(pRep,"  ");
  }
  for (j=0;j<iNumCols;j++) {
    pCol = &aColumns[aCols[j]];
    if (pCol->bAllocateColumnRAM) {
      if (pCol->eFinalDataType == DTypeUInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->puiColumnData[iRow]);
      } else if (pCol->eFinalDataType == DTypeSInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->psiColumnData[iRow]);
      } else if (pCol->eFinalDataType == DTypeUBigInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->pubiColumnData[iRow]);
      } else if (pCol->eFinalDataType == DTypeSBigInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->psbiColumnData[iRow]);
      } else {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->pfColumnData[iRow]);
      }
      if (j < (iNumCols-1)) {
        ReportPrint1(pRep,"%s",sColSepString);
      }
    } else {
      if (pCol->eFinalDataType == DTypeUInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->uiCurRowValue);
      } else if (pCol->eFinalDataType == DTypeSInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->siCurRowValue);
      } else if (pCol->eFinalDataType == DTypeUBigInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->ubiCurRowValue);
      } else if (pCol->eFinalDataType == DTypeSBigInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->sbiCurRowValue);
      } else {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->fCurRowValue);
      }
      if (j < (iNumCols-1)) {
        ReportPrint1(pRep,"%s",sColSepString);
      }
    }
  }
  ReportPrint(pRep,"\n");

}

void ReportOutRow() {
  if (!bReportOutputSuppress) {
    PrintRow(pRepOut,iRun-1,iNumOutputColumns,aOutputColumns);
  }
}

void ReportOutSuppressed() {
  UINT32 j;
  if (bReportOutputSuppress) {
    for (j=0;j<iRun;j++) {
      PrintRow(pRepOut,j,iNumOutputColumns,aOutputColumns);
    }
  }
}



/***** Report -r stats *****/

void ReportStatsSetup() {
  ParseItemList(&listStats,(char *)pRepStats->aParameters[0],ActivateStatID);
}

FLOAT GetRowElement(REPORTCOL *pCol,UINT32 iRowRequested, BOOL bSorted, BOOL bSortByStep) {

  UINT32 iRow;

  if (!pCol->bAllocateColumnRAM) {
    ReportPrint1(pRepErr,"Unexpected Error: Column Data for [%s] was not allocated properly\n",pCol->sDescription);
    AbnormalExit();
    exit(1);
  }

  if (bSorted) {
    if (bSortByStep) {
      if (!bSortedByStepsValid) {
        SortByStepPerformance();
      }
      iRow = aSortedBySteps[iRowRequested];
    } else {
      iRow = aSortedByCurrent[iRowRequested];
    }
  } else {
    iRow = iRowRequested;
  }

  if (pCol->eFinalDataType == DTypeUInt) {
    return ((FLOAT)pCol->puiColumnData[iRow]);
  } else if (pCol->eFinalDataType == DTypeSInt) {
    return ((FLOAT)pCol->psiColumnData[iRow]);
  } else if (pCol->eFinalDataType == DTypeUBigInt) {
    return ((FLOAT)pCol->pubiColumnData[iRow]);
  } else if (pCol->eFinalDataType == DTypeSBigInt) {
    return ((FLOAT)pCol->psbiColumnData[iRow]);
  } else {
    return (pCol->pfColumnData[iRow]);
  }
}

void ReportStatsPrintSep() {
  if (bReportCSV) {
    ReportPrint(pRepStats,",");
  } else {
    ReportPrint(pRepStats," = ");
  }
}

void ReportStatsPrint() {
  
  UINT32 j,k,l;
  REPORTCOL *pCol;
  REPORTCOL *pColSteps;
  REPORTSTAT *pStat;

  FLOAT fMean;
  FLOAT fStdDev;
  FLOAT fCV;
  FLOAT fVar;
  FLOAT fStdErr;
  FLOAT fVMR;
  FLOAT fMedian;

  FLOAT fVal;
  FLOAT fVal2;
  UINT32 iPos;

  UINT32 iNumFail;

  char **pString;

  ReportPrint(pRepStats,"\n\n");

  for (k=0;k<iNumStatsActive;k++) {
    for (j=0;j<listStats.iNumItems;j++) {
      pStat = &aStats[j];
      if ((pStat->bActive)&&(pStat->iActiveID==k)) {
        if (pStat->bCustomField) {

          ReportPrint1(pRepStats,"%s",pStat->sBaseDescription);
          ReportStatsPrintSep();

          switch (pStat->eCustomType) {
            case DTypeUInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(UINT32 *) pStat->pCustomValue);
              break;
            case DTypeSInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(SINT32 *) pStat->pCustomValue);
              break;
            case DTypeUBigInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(UBIGINT *) pStat->pCustomValue);
              break;
            case DTypeSBigInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(SBIGINT *) pStat->pCustomValue);
              break;
            case DTypeFloat:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(FLOAT *) pStat->pCustomValue);
              break;
            case DTypeString:
              pString = (char **) pStat->pCustomValue;
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*pString);
              break;
            default:
              break;
          }

          ReportPrint(pRepStats,"\n");

        } else {

          if (iRun) {

            pCol = &aColumns[FindItem(&listColumns,pStat->sDataColumn)];

            if (pStat->iStatFlags & STATCODE_CALCMASK) {
              CalculateStats(&fMean, &fStdDev, &fCV, pCol->fColSum, pCol->fColSum2, iRun);

              fVar = fStdDev * fStdDev;
              fStdErr = fStdDev / (sqrt((FLOAT) iRun));            

              if (fMean != FLOATZERO) {
                fVMR = fVar / fMean;
              } else {
                fVMR = FLOATZERO;
              }

              if (pStat->iStatFlags & STATCODE_mean) {
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Mean");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fMean);
              }

              if (pStat->iStatFlags & STATCODE_stddev) {
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"StdDev");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fStdDev);
              }
              
              if (pStat->iStatFlags & STATCODE_cv) {
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"CoeffVariance");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fCV);
              }

              if (pStat->iStatFlags & STATCODE_var) {
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Variance");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVar);
              }

              if (pStat->iStatFlags & STATCODE_stderr) {
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"StdErr");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fStdErr);
              }

              if (pStat->iStatFlags & STATCODE_vmr) {
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"VarMeanRatio");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVMR);
              }
            }

            if (pStat->iStatFlags & STATCODE_sum) {
              ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Sum");
              ReportStatsPrintSep();
              ReportPrint1(pRepStats,"%.12g\n",pCol->fColSum);
            }

            if (pStat->iStatFlags & STATCODE_SORTMASK) {
              if (!pStat->bSortByStep) {
                SortByCurrentColData(pCol);
              }

              if (pStat->iStatFlags & STATCODE_median) {
                fMedian = GetRowElement(pCol,(iRun-1)>>1,1,pStat->bSortByStep);
                if (iRun % 2 == 0) {
                  fMedian += GetRowElement(pCol,(iRun)>>1,1,pStat->bSortByStep);
                  fMedian /= 2.0;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Median");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fMedian);
              }

              if (pStat->iStatFlags & STATCODE_min) {
                iPos = 0;
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Min");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_max) {
                iPos = iRun - 1;
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Max");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q05) {
                iPos = (UINT32)(floor(0.05 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.05");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q10) {
                iPos = (UINT32)(floor(0.10 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.10");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q25) {
                iPos = (UINT32)(floor(0.25 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.25");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q75) {
                iPos = (UINT32)(floor(0.75 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.75");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q90) {
                iPos = (UINT32)(floor(0.90 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.90");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q95) {
                iPos = (UINT32)(floor(0.95 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.95");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q98) {
                iPos = (UINT32)(floor(0.98 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.98");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
              if (pStat->iStatFlags & STATCODE_qr7525) {
                iPos = (UINT32)(floor(0.75 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                iPos = (UINT32)(floor(0.25 * (FLOAT) (iRun-1)));
                fVal2 = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                if (fVal2 != FLOATZERO) {
                  fVal2 = fVal / fVal2;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.75/25");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal2);
              }
              
              if (pStat->iStatFlags & STATCODE_qr9010) {
                iPos = (UINT32)(floor(0.90 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                iPos = (UINT32)(floor(0.10 * (FLOAT) (iRun-1)));
                fVal2 = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                if (fVal2 != FLOATZERO) {
                  fVal2 = fVal / fVal2;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.90/10");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal2);
              }

              if (pStat->iStatFlags & STATCODE_qr9505) {
                iPos = (UINT32)(floor(0.95 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                iPos = (UINT32)(floor(0.05 * (FLOAT) (iRun-1)));
                fVal2 = GetRowElement(pCol,iPos,1,pStat->bSortByStep);
                if (fVal2 != FLOATZERO) {
                  fVal2 = fVal / fVal2;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"Q.95/05");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal2);
              } 
            }
            if (pStat->iStatFlags & STATCODE_SFMASK) {
              SortByCurrentColDataAndFound(pCol);
              iNumFail = iRun - iNumSolutionsFound;

              if (pStat->iStatFlags & STATCODE_stepmean) {
                pColSteps = &aColumns[FindItem(&listColumns,"steps")];
                fVal = FLOATZERO;
                for (l=0;l<iRun;l++) {
                  fVal += (GetRowElement(pColSteps,l,0,0) * GetRowElement(pCol,l,0,0));
                }
                if (pColSteps->fColSum != FLOATZERO) {
                  fVal /= pColSteps->fColSum;
                } else {
                  fVal = FLOATZERO;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"StepMean");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
            
              if (pStat->iStatFlags & STATCODE_solvemean) {
                fVal = FLOATZERO;
                if (iNumSolutionsFound != 0) {
                  for (l=0;l<iNumSolutionsFound;l++) {
                    fVal += GetRowElement(pCol,l,1,0);
                  }
                  fVal /= iNumSolutionsFound;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"SuccessMean");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_failmean) {
                fVal = FLOATZERO;
                if (iNumFail != 0) {
                  for (l=iNumSolutionsFound;l<iRun;l++) {
                    fVal += GetRowElement(pCol,l,1,0);
                  }
                  fVal /= iNumFail;
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"FailureMean");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_solvemedian) {
                if (iNumSolutionsFound == 0) {
                  fVal = FLOATZERO;
                } else {
                  fVal = GetRowElement(pCol,(iNumSolutionsFound-1)>>1,1,0);
                  if (iNumSolutionsFound % 2 == 0) {
                    fVal += GetRowElement(pCol,(iNumSolutionsFound)>>1,1,0);
                    fVal /= 2.0;
                  }
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"SuccessMedian");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_failmedian) {
                if (iNumFail == 0) {
                  fVal = FLOATZERO;
                } else {
                  fVal = GetRowElement(pCol,((iNumFail-1)>>1) + iNumSolutionsFound,1,0);
                  if (iNumFail % 2 == 0) {
                    fVal += GetRowElement(pCol,((iNumFail)>>1) + iNumSolutionsFound,1,0);
                    fVal /= 2.0;
                  }
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"FailureMedian");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_solvemin) {
                if (iNumSolutionsFound == 0) {
                  fVal = FLOATZERO;
                } else {
                  fVal = GetRowElement(pCol,0,1,0);
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"SuccessMin");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_failmin) {
                if (iNumFail == 0) {
                  fVal = FLOATZERO;
                } else {
                  fVal = GetRowElement(pCol,iNumSolutionsFound,1,0);
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"FailureMin");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_solvemax) {
                if (iNumSolutionsFound == 0) {
                  fVal = FLOATZERO;
                } else {
                  fVal = GetRowElement(pCol,iNumSolutionsFound-1,1,0);
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"SuccessMax");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }

              if (pStat->iStatFlags & STATCODE_failmax) {
                if (iNumFail == 0) {
                  fVal = FLOATZERO;
                } else {
                  fVal = GetRowElement(pCol,iRun-1,1,0);
                }
                ReportPrint2(pRepStats,"%s_%s",pStat->sBaseDescription,"FailureMax");
                ReportStatsPrintSep();
                ReportPrint1(pRepStats,"%.12g\n",fVal);
              }
            }
          }
        }
      }
    }
  }
  ReportPrint(pRepStats,"\n\n");
}



/***** Report -r rtd *****/

void ReportRTDSetup() {
  iNumRTDColumns = 0;
  ParseItemList(&listColumns,(char *)pRepRTD->aParameters[0],ActivateColumnID);
  ParseItemList(&listColumns,(char *)pRepRTD->aParameters[0],AddRTDColumnID);
}

void PrintRTDRow(UINT32 iRow) {
  PrintRow(pRepRTD,aSortedByStepsAndFound[iRow],iNumRTDColumns,aRTDColumns);
}

void ReportRTDPrint() {
  UINT32 j;
  PrintColHeaders(pRepRTD,iNumRTDColumns,aRTDColumns);
  for (j=0;j<iNumSolutionsFound;j++) {
    PrintRTDRow(j);
  }
}



/***** Report -r model *****/

void InitSolveMode() {
  if (bSolveMode) {
    if (iFind==0) {
      iFind = 1;
    }
    pRepModel->bActive = 1;
  }
}

void ReportModelPrint() {
  UINT32 j;
  if ((bSolveMode)||(pRepModel->bActive)) {
    if (bSolutionFound) {
      ReportHdrPrefix(pRepModel);
      ReportHdrPrint(pRepModel,"\n");
      ReportHdrPrefix(pRepModel);
      if (bWeighted) {
        ReportHdrPrint1(pRepModel,"Solution found for -wtarget %"P64"\n\n", iTargetWeight);
      } else {
        ReportHdrPrint1(pRepModel,"Solution found for -target %"P32"\n\n", iTarget);
      }
      for (j=1;j<=iNumVars;j++) {
        if (!aVarValue[j]) {
          ReportPrint1(pRepModel," -%"P32,j);
        } else {
          ReportPrint1(pRepModel," %"P32,j);
        }
        if (j % 10 == 0) {
          ReportPrint(pRepModel,"\n");
        }
      }
      if ((j-1) % 10 != 0) {
        ReportPrint(pRepModel,"\n");
      }
    } else {
      ReportHdrPrefix(pRepModel);
      if (bWeighted) {
        ReportHdrPrint1(pRepModel,"No Solution found for -wtarget %"P64"\n\n", iTargetWeight);
      } else {
        ReportHdrPrint1(pRepModel,"No Solution found for -target %"P32"\n", iTarget);
      }

      
    }
  }
}



/***** Report -r cnfstats *****/

void ReportCNFStatsPrint() {

  UINT32 j;
  UINT32 iMaxClauseLen;
  FLOAT fAvgLitClause = FLOATZERO;
  FLOAT fAvgVarOccur = FLOATZERO;
  FLOAT fStdDevVarOccur = FLOATZERO;
  FLOAT fTemp;
  UINT32 iNumPos;
  UINT32 iNumNeg;
  FLOAT fPosNegRatio;

  UINT32 *aClauseBins;
  UINT32 *aVarBins;
  

  ReportPrint1(pRepCNFStats,"Clauses = %"P32"\n",iNumClauses);
  ReportPrint1(pRepCNFStats,"Variables = %"P32" \n",iNumVars);
  ReportPrint1(pRepCNFStats,"TotalLiterals = %"P32"\n",iNumLits);

  if ((iNumVars > 0) && (iNumClauses > 0)) {
    ReportPrint1(pRepCNFStats,"ClauseVarRatio = %.12g \n",(FLOAT) iNumClauses / (FLOAT) iNumVars);
    iMaxClauseLen = 0;
    for (j=0;j<iNumClauses;j++) {
      if (aClauseLen[j] > iMaxClauseLen) {
        iMaxClauseLen = aClauseLen[j];
      }
    }
    ReportPrint1(pRepCNFStats,"MaxClauseLen = %"P32"\n",iMaxClauseLen);

    aClauseBins = (UINT32 *) AllocateRAM((iMaxClauseLen + 3) * sizeof(UINT32), HeapReports);
    
    for (j=0;j<iMaxClauseLen + 3;j++) {
      aClauseBins[j] = 0;
    }

    for (j=0;j<iNumClauses;j++) {
      aClauseBins[aClauseLen[j]]++;
    }

    ReportPrint1(pRepCNFStats,"NumClauseLen1 = %"P32" \n",aClauseBins[1]);
    ReportPrint1(pRepCNFStats,"NumClauseLen2 =  %"P32" \n",aClauseBins[2]);
    ReportPrint1(pRepCNFStats,"NumClauseLen3+ = %"P32" \n",iNumClauses - aClauseBins[1] - aClauseBins[2]);

    ReportPrint(pRepCNFStats,"FullClauseDistribution =");
    for (j=0;j<=iMaxClauseLen;j++) {
      if (aClauseBins[j] > 0) {
        ReportPrint1(pRepCNFStats," %"P32,j);
        ReportPrint1(pRepCNFStats,":%"P32,aClauseBins[j]);
      }
    }
    ReportPrint(pRepCNFStats,"\n");

    fAvgLitClause = (FLOAT)iNumLits / (FLOAT) iNumClauses;
    ReportPrint1(pRepCNFStats,"MeanClauseLen = %.12g \n",fAvgLitClause);
    
    fAvgVarOccur = (FLOAT) iNumLits / (FLOAT) iNumVars;
    ReportPrint1(pRepCNFStats,"MeanVariableOcc = %.12g \n",fAvgVarOccur);
    
    UINT32 iVarOcc;
    UINT32 iMaxVarOcc;

    iMaxVarOcc = 0;
    fStdDevVarOccur = FLOATZERO; 
    for (j=1;j<=iNumVars;j++) {
      iVarOcc = aNumLitOcc[GetPosLit(j)] + aNumLitOcc[GetNegLit(j)];
      if (iVarOcc > iMaxVarOcc) {
        iMaxVarOcc = iVarOcc;
      }
      fTemp = ((FLOAT) iVarOcc) - fAvgVarOccur;
      fStdDevVarOccur += (fTemp*fTemp);
    }
    fStdDevVarOccur /= (FLOAT) (iNumVars-1);
    fStdDevVarOccur = sqrt(fStdDevVarOccur);
    ReportPrint1(pRepCNFStats,"StdDevVariableOcc = %.12g \n",fStdDevVarOccur);

    aVarBins = (UINT32 *) AllocateRAM((iMaxVarOcc + 1) * sizeof(UINT32), HeapReports);

    for (j=0;j<iMaxVarOcc + 1;j++) {
      aVarBins[j] = 0;
    }
    for (j=1;j<=iNumVars;j++) {
      iVarOcc = aNumLitOcc[GetPosLit(j)] + aNumLitOcc[GetNegLit(j)];
      aVarBins[iVarOcc]++;
    }
    ReportPrint(pRepCNFStats,"FullVarDistribution =");
    for (j=0;j<=iMaxVarOcc;j++) {
      if (aVarBins[j] > 0) {
        ReportPrint1(pRepCNFStats," %"P32,j);
        ReportPrint1(pRepCNFStats,":%"P32,aVarBins[j]);
      }
    }
    ReportPrint(pRepCNFStats,"\n");


    iNumPos = 0;
    iNumNeg = 0;
    for (j=1;j<=iNumVars;j++) {
      iNumPos += aNumLitOcc[GetPosLit(j)];
      iNumNeg += aNumLitOcc[GetNegLit(j)];
    }
    fPosNegRatio = (FLOAT) iNumPos / (FLOAT) iNumNeg;
    ReportPrint1(pRepCNFStats,"NumPosLit = %"P32" \n",iNumPos);
    ReportPrint1(pRepCNFStats,"NumNegLit = %"P32" \n",iNumNeg);
    ReportPrint1(pRepCNFStats,"RatioPos:NegLit = %.12g \n",fPosNegRatio);
  }
}



/***** Report -r state *****/

void ReportStatePrint() {
  UINT32 j;
  BOOL bLocalMin = 0;
  UINT32 bPrint;
  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepState);
    ReportHdrPrint(pRepState," Run ID | Step No. | Num False (or sum of false weights) | VarFlip | IsLocalMin | vararray\n");
  }

  bPrint = 1;

  if ((bPrint) && (bReportStateLMOnly)) {
    bLocalMin = IsLocalMinimum(bWeighted);
    if (!bLocalMin) {
      bPrint = 0;
    }
  }

  if (bPrint) {
    ReportPrint1(pRepState,"%"P32,iRun);
    ReportPrint1(pRepState,"%s",sColSepString);
    ReportPrint1(pRepState,"%"P64,iStep);
    ReportPrint1(pRepState,"%s",sColSepString);

    if (bWeighted) {
      ReportPrint1(pRepState,"%"P64,iSumFalseWeight);
      ReportPrint1(pRepState,"%s",sColSepString);
    } else {
      ReportPrint1(pRepState,"%"P32,iNumFalse);
      ReportPrint1(pRepState,"%s",sColSepString);
    }

    ReportPrint1(pRepState,"%"P32,iFlipCandidate);
    ReportPrint1(pRepState,"%s",sColSepString);
    if (bReportStateLMOnly) {
      ReportPrint1(pRepState,"%"P32,bLocalMin);
      ReportPrint1(pRepState,"%s",sColSepString);
    } else {
      ReportPrint1(pRepState,"%"P32,IsLocalMinimum(bWeighted));
      ReportPrint1(pRepState,"%s",sColSepString);
    }
    for (j=1;j<=iNumVars;j++) {
      if (aVarValue[j]) {
        ReportPrint(pRepState,"1");
      } else {
        ReportPrint(pRepState,"0");
      }
    }
    ReportPrint(pRepState,"\n");
  }
}



/***** Report -r bestsol  *****/

void ReportBestSolPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepBestSol);
    ReportHdrPrint(pRepBestSol," Run ID | Solution Found? | Best # false (or weighted best) | vararray\n");
  }
  ReportPrint1(pRepBestSol,"%"P32,iRun);
  ReportPrint1(pRepBestSol,"%s",sColSepString);
  ReportPrint1(pRepBestSol,"%"P32,bSolutionFound);
  ReportPrint1(pRepBestSol,"%s",sColSepString);
  if (bWeighted) {
    ReportPrint1(pRepBestSol,"%"P64,iBestSumFalseWeight);
    ReportPrint1(pRepBestSol,"%s",sColSepString);
  } else {
    ReportPrint1(pRepBestSol,"%"P32,iBestNumFalse);
    ReportPrint1(pRepBestSol,"%s",sColSepString);
  }
  for (j=1;j<=iNumVars;j++) {
    if (GetVarStateBit(vsBest,j-1)) {
      ReportPrint(pRepBestSol,"1");
    } else {
      ReportPrint(pRepBestSol,"0");
    }
  }
  ReportPrint(pRepBestSol,"\n");
}



/***** Report -r beststep  *****/

void ReportBestStepPrint() {

  UINT32 j;

  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepBestStep);
    ReportHdrPrint(pRepBestStep," Run ID | Step # | Best # false (or weighted best)");
    if (bReportBestStepVars) {
      ReportHdrPrint(pRepBestStep,"| vararray");
    }
    ReportHdrPrint(pRepBestStep,"\n");
  }

  if (bWeighted) {
    if (iBestStepSumFalseWeight == iStep) {
      ReportPrint1(pRepBestStep,"%"P32,iRun);
      ReportPrint1(pRepBestStep,"%s",sColSepString);
      ReportPrint1(pRepBestStep,"%"P64,iStep);
      ReportPrint1(pRepBestStep,"%s",sColSepString);
      ReportPrint1(pRepBestStep,"%"P64,iBestSumFalseWeight);
      if (bReportBestStepVars) {
        ReportPrint1(pRepBestStep,"%s",sColSepString);
        for (j=1;j<=iNumVars;j++) {
          if (aVarValue[j]) {
            ReportPrint(pRepBestStep,"1");
          } else {
            ReportPrint(pRepBestStep,"0");
          }
        }
      }
      ReportPrint(pRepBestStep,"\n");
    }
  } else {
    if (iBestStepNumFalse == iStep) {
      ReportPrint1(pRepBestStep,"%"P32,iRun);
      ReportPrint1(pRepBestStep,"%s",sColSepString);
      ReportPrint1(pRepBestStep,"%"P64,iStep);
      ReportPrint1(pRepBestStep,"%s",sColSepString);
      ReportPrint1(pRepBestStep,"%"P32,iBestNumFalse);
      if (bReportBestStepVars) {
        ReportPrint1(pRepBestStep,"%s",sColSepString);
        for (j=1;j<=iNumVars;j++) {
          if (aVarValue[j]) {
            ReportPrint(pRepBestStep,"1");
          } else {
            ReportPrint(pRepBestStep,"0");
          }
        }
      }
      ReportPrint(pRepBestStep,"\n");
    }
  }
}



/***** Report -r tbestlm  *****/

void ReportTrajBestLMPostStep() {

  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepTrajBestLM);
    ReportHdrPrint(pRepTrajBestLM," Run ID | Step # | Best # false (or weighted best)\n");
  }
  if (iStep > 1) {
    if (bWeighted) {
      if (iBestStepSumFalseWeight==(iStep-1)) {
        ReportPrint1(pRepTrajBestLM,"%"P32,iRun);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P64,iStep-1);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P64"\n",iBestSumFalseWeight);
      }
    } else {
      if (iBestStepNumFalse==(iStep-1)) {
        ReportPrint1(pRepTrajBestLM,"%"P32,iRun);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P64,iStep-1);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P32"\n",iBestNumFalse);
      }
    }
  }
}


void ReportTrajBestLMPostRun() {

  if (bWeighted) {
    if (iBestStepSumFalseWeight==(iStep)) {
        ReportPrint1(pRepTrajBestLM,"%"P32,iRun);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P64,iStep);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P64"\n",iBestSumFalseWeight);
    }
  } else {
    if (iBestStepNumFalse==(iStep)) {
        ReportPrint1(pRepTrajBestLM,"%"P32,iRun);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P64,iStep);
        ReportPrint1(pRepTrajBestLM,"%s",sColSepString);
        ReportPrint1(pRepTrajBestLM,"%"P32"\n",iBestNumFalse);
    }
  }
}


/***** Report -r solution  *****/

void ReportSolutionPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepSolution);
    ReportHdrPrint(pRepSolution," Run ID | vararray\n");
  }
  if (bSolutionFound) {
    ReportPrint1(pRepSolution,"%"P32,iRun);
    ReportPrint1(pRepSolution,"%s",sColSepString);
    for (j=1;j<=iNumVars;j++) {
      if (aVarValue[j]) {
        ReportPrint(pRepSolution,"1");
      } else {
        ReportPrint(pRepSolution,"0");
      }
    }
    ReportPrint(pRepSolution,"\n");
  }
}



/***** Report -r uniquesol  *****/

void ReportUniqueSolPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepUniqueSol);
    ReportHdrPrint(pRepUniqueSol,"vararrays\n");
  }
  if (bSolutionFound) {

    if (iLastUnique == iRun) {
      for (j=1;j<=iNumVars;j++) {
        if (aVarValue[j]) {
          ReportPrint(pRepUniqueSol,"1");
        } else {
          ReportPrint(pRepUniqueSol,"0");
        }
      }
      ReportPrint(pRepUniqueSol,"\n");
    }
  }
}



/***** Report -r unsatclauses *****/

void ReportUnsatClausesPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepOptClauses);
    ReportHdrPrint(pRepOptClauses," Run ID | clauses at end of search (1=satisfied) ...\n");
  }
  if ((bSolutionFound)||(!bReportOptClausesSol)) {
    ReportPrint1(pRepOptClauses,"%"P32,iRun);
    ReportPrint1(pRepOptClauses,"%s",sColSepString);
    for (j=0;j<iNumClauses;j++) {
      if (aNumTrueLit[j]==0) {
        ReportPrint(pRepOptClauses,"0");
      } else {
        ReportPrint(pRepOptClauses,"1");
      }
    }
    ReportPrint(pRepOptClauses,"\n");
  }
}



/***** Report -r falsehist *****/

void ReportFalseHistPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepFalseHist);
    ReportHdrPrint(pRepFalseHist," Run ID | # times where numfalse = 0 | 1 | 2 | ....\n");
    if (iReportFalseHistCount) {
      ReportHdrPrefix(pRepFalseHist);
      ReportHdrPrint1(pRepFalseHist,"Note: For the last %"P32" steps of the search\n",iReportFalseHistCount);
    }
  }
  ReportPrint1(pRepFalseHist,"%"P32,iRun);
  for (j=0;j<(iNumClauses+1);j++) {
    ReportPrint1(pRepFalseHist,"%s",sColSepString);
    ReportPrint1(pRepFalseHist,"%"P64,aNumFalseCounts[j]);
  }
  ReportPrint(pRepFalseHist,"\n");
}



/***** Report -r distance *****/

void ReportDistancePrint() {
  BOOL bLocalMin = 0;
  UINT32 bPrint;
  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepDistance);
    ReportHdrPrint(pRepDistance," Run ID | Step No. | Hamming Distance to Known Solution(s) | Num False (or sum of false weights)\n");
  }

  bPrint = 1;
  if (bReportDistanceLMOnly) {
    bLocalMin = IsLocalMinimum(bWeighted);
    if (!bLocalMin) {
      bPrint = 0;
    }
  }

  if (bPrint) {
    ReportPrint1(pRepDistance,"%"P32,iRun);
    ReportPrint1(pRepDistance,"%s",sColSepString);
    ReportPrint1(pRepDistance,"%"P64,iStep);
    ReportPrint1(pRepDistance,"%s",sColSepString);

    ReportPrint1(pRepDistance,"%"P32,iSolutionDistance);
    ReportPrint1(pRepDistance,"%s",sColSepString);

    if (bWeighted) {
      ReportPrint1(pRepDistance,"%"P64,iSumFalseWeight);
    } else {
      ReportPrint1(pRepDistance,"%"P32,iNumFalse);
    }
  }
  ReportPrint(pRepDistance,"\n");
}



/***** Report -r DistHistPrint *****/

void ReportDistHistPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepDistHist);
    ReportHdrPrint(pRepDistHist," Run ID | # times where hamming distance to solution(s) = 0 | 1 | 2 | ....\n");
    if (iReportDistHistCount) {
      ReportHdrPrefix(pRepDistHist);
      ReportHdrPrint1(pRepDistHist,"For the last %"P32" steps of the search\n",iReportDistHistCount);
    }
  }
  ReportPrint1(pRepDistHist,"%"P32,iRun);
  for (j=0;j<(iNumVars+1);j++) {
    ReportPrint1(pRepDistHist,"%s",sColSepString);
    ReportPrint1(pRepDistHist,"%"P64,aDistanceCounts[j]);
  }
  ReportPrint(pRepDistHist,"\n");
}



/***** Report -r flipcount *****/

void ReportFlipCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepFlipCounts);
    ReportHdrPrint(pRepFlipCounts," Run ID | FlipCount[0] (NullFlips) | FlipCount[1] | FlipCount[2]...\n");
  }
  ReportPrint1(pRepFlipCounts,"%"P32,iRun);
  for (j=0;j<=iNumVars;j++) {
    ReportPrint1(pRepFlipCounts,"%s",sColSepString);
    ReportPrint1(pRepFlipCounts,"%"P64,aFlipCounts[j]);
  }
  ReportPrint(pRepFlipCounts,"\n");
}



/***** Report -r biascount *****/

void ReportBiasCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepBiasCounts);
    ReportHdrPrint(pRepBiasCounts," Run ID | # Steps False Var[1] | # Steps True Var[1] | Fraction of Steps Same as at End Var[1] | .... Var[2]...\n");
  }
  ReportPrint1(pRepBiasCounts,"%"P32,iRun);
  for (j=1;j<(iNumVars+1);j++) {
    ReportPrint1(pRepBiasCounts,"%s",sColSepString);
    ReportPrint1(pRepBiasCounts,"%"P64,aBiasFalseCounts[j]);
    ReportPrint1(pRepBiasCounts,"%s",sColSepString);
    ReportPrint1(pRepBiasCounts,"%"P64,aBiasTrueCounts[j]);
    ReportPrint1(pRepBiasCounts,"%s",sColSepString);
    if (aBiasFalseCounts[j]+aBiasTrueCounts[j]>0) {
      if (aVarValue[j]) {
        ReportPrint1(pRepBiasCounts,"%5.4f",((FLOAT)aBiasTrueCounts[j])/((FLOAT)(aBiasTrueCounts[j]+aBiasFalseCounts[j])));
      } else {
        ReportPrint1(pRepBiasCounts,"%5.4f",((FLOAT)aBiasFalseCounts[j])/((FLOAT)(aBiasTrueCounts[j]+aBiasFalseCounts[j])));
      }
    } else {
        ReportPrint1(pRepBiasCounts,"%5.4f",FLOATZERO);
    }
  }
  ReportPrint(pRepBiasCounts,"\n");
}



/***** Report -r unsatcount *****/

void ReportUnsatCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepUnsatCounts);
    ReportHdrPrint(pRepUnsatCounts," Run ID | UnsatCount[1] | UnsatCount[2] ...\n");
    ReportHdrPrefix(pRepUnsatCounts);
    ReportHdrPrint(pRepUnsatCounts," ClauseLengths");
    for (j=0;j<iNumClauses;j++) {
      ReportHdrPrint1(pRepUnsatCounts," %"P32,aClauseLen[j]);
    }
    ReportHdrPrint(pRepUnsatCounts,"\n");
  }
  ReportPrint1(pRepUnsatCounts,"%"P32,iRun);
  for (j=0;j<iNumClauses;j++) {
    ReportPrint1(pRepUnsatCounts,"%s",sColSepString);
    ReportPrint1(pRepUnsatCounts,"%"P64,aUnsatCounts[j]);
  }
  ReportPrint(pRepUnsatCounts,"\n");
}



/***** Report -r varlastflip *****/

void ReportVarLastPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepVarLast);
    ReportHdrPrint(pRepVarLast," Run ID | StepLastFlip[1] | StepLastFlip[2] ...\n");
  }

  ReportPrint1(pRepVarLast,"%"P32,iRun);
  for (j=1;j<=iNumVars;j++) {
    ReportPrint1(pRepUnsatCounts,"%s",sColSepString);
    ReportPrint1(pRepVarLast,"%"P64,aVarLastChange[j]);
  }
  ReportPrint(pRepVarLast,"\n");
}



/***** Report -r clauselast *****/

void ReportClauseLastPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepClauseLast);
    ReportHdrPrint(pRepClauseLast," Run ID | StepLastUnsat[1] | StepLastUnsat[2] ...\n");
  }
  ReportPrint1(pRepClauseLast,"%"P32,iRun);
  for (j=0;j<iNumClauses;j++) {
    ReportPrint1(pRepClauseLast,"%s",sColSepString);
    ReportPrint1(pRepClauseLast,"%"P64,aClauseLast[j]);
  }
  ReportPrint(pRepClauseLast,"\n");
}



/***** Report -r sqgrid *****/

void ReportSQGridPrint() {

  UINT32 j;

  if (pRepSQGrid->bActive) {
    if (iRun==1) {
      ReportHdrPrefix(pRepSQGrid);
      ReportHdrPrint(pRepSQGrid," Run ID | Solution Quality at steps:");
      for (j=0;j<iNumLogDistValues;j++) {
        ReportHdrPrint1(pRepSQGrid," %"P64,aLogDistValues[j]);
      }
      ReportHdrPrint(pRepSQGrid,"\n");
    }

    ReportPrint1(pRepSQGrid,"%"P32,iRun);

    if (bWeighted) {
      for (j=0;j<iNumLogDistValues;j++) {
        ReportPrint1(pRepSQGrid,"%s",sColSepString);
        ReportPrint1(pRepSQGrid,"%"P64,aSQGridWeight[iNumLogDistValues * (iRun-1) + j]);
      }
    } else {
      for (j=0;j<iNumLogDistValues;j++) {
        ReportPrint1(pRepSQGrid,"%s",sColSepString);
        ReportPrint1(pRepSQGrid,"%"P32,aSQGrid[iNumLogDistValues * (iRun-1) + j]);
      }
    }
    ReportPrint(pRepSQGrid,"\n");
  }
}



/***** Report -r penalty *****/

void ReportPenaltyCreate() {

  aPenaltyStatsFinal = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT), HeapReports);
  aPenaltyStatsFinalSum = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT), HeapReports);
  aPenaltyStatsFinalSum2 = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT), HeapReports);
  memset(aPenaltyStatsFinalSum,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsFinalSum2,0,iNumClauses*sizeof(FLOAT));

}

void ReportPenaltyPrintStep() {
  UINT32 j;

  if (bReportPenaltyEveryLM) {
    if (iFlipCandidate) {
      return;
    }
    if (iStep==1) {
      return;
    }

    if ((iRun==1)&&(iNumNullFlips==1)) {
      if (bClausePenaltyCreated) {
        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"Run ID | Step # | Local Min # | Penalty[0] Penalty[1]...\n");
      } else {
        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"No Output... current algorithm does not have penalties\n");
      }
    }

    if (bClausePenaltyCreated) {
      ReportPrint1(pRepPenalty,"%"P32,iRun);
      ReportPrint1(pRepPenalty,"%s",sColSepString);
      ReportPrint1(pRepPenalty,"%"P64,iStep);
      ReportPrint1(pRepPenalty,"%s",sColSepString);
      ReportPrint1(pRepPenalty,"%"P64,iNumNullFlips);
      if (bClausePenaltyFLOAT) {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            ReportPrint1(pRepPenalty,"%s",sColSepString);
            ReportPrint1(pRepPenalty,"%.12g",aClausePenaltyFL[j] / fTotalPenaltyFL);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%.12g",aClausePenaltyFL[j] / fBasePenaltyFL);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%.12g",aClausePenaltyFL[j]);
            }
          }
        }
      } else {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            ReportPrint1(pRepPenalty,"%s",sColSepString);
            ReportPrint1(pRepPenalty,"%.12g",((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iTotalPenaltyINT);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%.12g",((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iBasePenaltyINT);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%"P32,aClausePenaltyINT[j]);
            }
          }
        }
      }
      ReportPrint(pRepPenalty,"\n");
    }
  }
}

void ReportPenaltyPrintRun() {
  
  UINT32 j;

  FLOAT fMean;
  FLOAT fStddev;
  FLOAT fCV;

  if (!bReportPenaltyEveryLM) {

    if (iRun==1) {
      if (bClausePenaltyCreated) {
        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"Run ID | Step # | Solution Found? | Best # false (or weighted best) | Penalty[0] Penalty[1]...\n");
      } else {
        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"No Output... current algorithm does not have penalties\n");
      }
    }

    if (bClausePenaltyCreated) {

      ReportPrint1(pRepPenalty,"%"P32,iRun);
      ReportPrint1(pRepPenalty,"%s",sColSepString);
      ReportPrint1(pRepPenalty,"%"P64,iStep);
      ReportPrint1(pRepPenalty,"%s",sColSepString);
      ReportPrint1(pRepPenalty,"%"P32,bSolutionFound);
      ReportPrint1(pRepPenalty,"%s",sColSepString);
      if (bWeighted) {
        ReportPrint1(pRepPenalty,"%"P64,iBestSumFalseWeight);
      } else {
        ReportPrint1(pRepPenalty,"%"P32,iBestNumFalse);
      }
      if (bClausePenaltyFLOAT) {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            aPenaltyStatsFinal[j] = aClausePenaltyFL[j] / fTotalPenaltyFL;
            ReportPrint1(pRepPenalty,"%s",sColSepString);
            ReportPrint1(pRepPenalty,"%.12g",aPenaltyStatsFinal[j]);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = aClausePenaltyFL[j] / fBasePenaltyFL;
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%.12g",aPenaltyStatsFinal[j]);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = aClausePenaltyFL[j];
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%.12g",aPenaltyStatsFinal[j]);
            }
          }
        }
      } else {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            aPenaltyStatsFinal[j] = ((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iTotalPenaltyINT;
            ReportPrint1(pRepPenalty,"%s",sColSepString);
            ReportPrint1(pRepPenalty,"%.12g",aPenaltyStatsFinal[j]);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = ((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iBasePenaltyINT;
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%.12g",aPenaltyStatsFinal[j]);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = (FLOAT) aClausePenaltyINT[j];
              ReportPrint1(pRepPenalty,"%s",sColSepString);
              ReportPrint1(pRepPenalty,"%"P32,aClausePenaltyINT[j]);
            }
          }
        }
      }
      ReportPrint(pRepPenalty,"\n");

      for (j=0;j<iNumClauses;j++) {
        aPenaltyStatsFinalSum[j] += aPenaltyStatsFinal[j];
        aPenaltyStatsFinalSum2[j] += (aPenaltyStatsFinal[j] * aPenaltyStatsFinal[j]);
      }

      if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"Mean");
        for (j=0;j<iNumClauses;j++) {
          CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsFinalSum[j],aPenaltyStatsFinalSum2[j],iRun);
          ReportPrint1(pRepPenalty,"%s",sColSepString);
          ReportPrint1(pRepPenalty,"%.12g",fMean);
        }
        ReportPrint(pRepPenalty,"\n");

        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"Stddev");
        for (j=0;j<iNumClauses;j++) {
          CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsFinalSum[j],aPenaltyStatsFinalSum2[j],iRun);
          ReportPrint1(pRepPenalty,"%s",sColSepString);
          ReportPrint1(pRepPenalty,"%.12g",fStddev);
        }
        ReportPrint(pRepPenalty,"\n");

        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"CV");
        for (j=0;j<iNumClauses;j++) {
          CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsFinalSum[j],aPenaltyStatsFinalSum2[j],iRun);
          ReportPrint1(pRepPenalty,"%s",sColSepString);
          ReportPrint1(pRepPenalty,"%.12g",fCV);
        }
        ReportPrint(pRepPenalty,"\n");
      }

    }
  }
}



/***** Report -r penmean *****/

void ReportPenMeanPrint() {
  UINT32 j;
  FLOAT fMean;
  FLOAT fStddev;
  FLOAT fCV;

  if (iRun==1) {
    if (bClausePenaltyCreated) {
      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"Run ID | Penalty[0] Penalty[1]...\n");
    } else {
      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"No Output... current algorithm does not have penalties\n");
    }
  }

  if (bClausePenaltyCreated) {
    ReportPrint1(pRepPenMean,"%"P32,iRun);
    for (j=0;j<iNumClauses;j++) {
      ReportPrint1(pRepPenMean,"%s",sColSepString);
      ReportPrint1(pRepPenMean,"%.12g",aPenaltyStatsMean[j]);
    }
    ReportPrint(pRepPenMean,"\n");

    if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"Mean");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsMeanSum[j],aPenaltyStatsMeanSum2[j],iRun);
        ReportPrint1(pRepPenMean,"%s",sColSepString);
        ReportPrint1(pRepPenMean,"%.12g",fMean);
      }
      ReportPrint(pRepPenMean,"\n");

      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"Stddev");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsMeanSum[j],aPenaltyStatsMeanSum2[j],iRun);
        ReportPrint1(pRepPenMean,"%s",sColSepString);
        ReportPrint1(pRepPenMean,"%.12g",fStddev);
      }
      ReportPrint(pRepPenMean,"\n");

      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"CV");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsMeanSum[j],aPenaltyStatsMeanSum2[j],iRun);
        ReportPrint1(pRepPenMean,"%s",sColSepString);
        ReportPrint1(pRepPenMean,"%.12g",fCV);
      }
      ReportPrint(pRepPenMean,"\n");
    }
  }
}



/***** Report -r penstddev *****/

void ReportPenStddevPrint() {
  UINT32 j;
  FLOAT fMean;
  FLOAT fStddev;
  FLOAT fCV;

  if (iRun==1) {
    if (bClausePenaltyCreated) {
      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"Run ID | Penalty[0] Penalty[1]...\n");
    } else {
      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"No Output... current algorithm does not have penalties\n");
    }
  }

  if (bClausePenaltyCreated) {
    ReportPrint1(pRepPenStddev,"%"P32,iRun);
    for (j=0;j<iNumClauses;j++) {
      ReportPrint1(pRepPenStddev,"%s",sColSepString);
      ReportPrint1(pRepPenStddev,"%.12g",aPenaltyStatsStddev[j]);
    }
    ReportPrint(pRepPenStddev,"\n");

    if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"Mean");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsStddevSum[j],aPenaltyStatsStddevSum2[j],iRun);
        ReportPrint1(pRepPenStddev,"%s",sColSepString);
        ReportPrint1(pRepPenStddev,"%.12g",fMean);
      }
      ReportPrint(pRepPenStddev,"\n");

      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"Stddev");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsStddevSum[j],aPenaltyStatsStddevSum2[j],iRun);
        ReportPrint1(pRepPenStddev,"%s",sColSepString);
        ReportPrint1(pRepPenStddev,"%.12g",fStddev);
      }
      ReportPrint(pRepPenStddev,"\n");

      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"CV");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsStddevSum[j],aPenaltyStatsStddevSum2[j],iRun);
        ReportPrint1(pRepPenStddev,"%s",sColSepString);
        ReportPrint1(pRepPenStddev,"%.12g",fCV);
      }
      ReportPrint(pRepPenStddev,"\n");
    }
  }
}



/***** Report -r pencv *****/

void ReportPenCVPrint() {
  UINT32 j;
  FLOAT fMean;
  FLOAT fStddev;
  FLOAT fCV;

  if (iRun==1) {
    if (bClausePenaltyCreated) {
      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"Run ID | Penalty[0] Penalty[1]...\n");
    } else {
      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"No Output... current algorithm does not have penalties\n");
    }
  }

  if (bClausePenaltyCreated) {
    ReportPrint1(pRepPenCV,"%"P32,iRun);
    for (j=0;j<iNumClauses;j++) {
      ReportPrint1(pRepPenCV,"%s",sColSepString);
      ReportPrint1(pRepPenCV,"%.12g",aPenaltyStatsCV[j]);
    }
    ReportPrint(pRepPenCV,"\n");

    if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"Mean");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsCVSum[j],aPenaltyStatsCVSum2[j],iRun);
        ReportPrint1(pRepPenCV,"%s",sColSepString);
        ReportPrint1(pRepPenCV,"%.12g",fMean);
      }
      ReportPrint(pRepPenCV,"\n");

      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"Stddev");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsCVSum[j],aPenaltyStatsCVSum2[j],iRun);
        ReportPrint1(pRepPenCV,"%s",sColSepString);
        ReportPrint1(pRepPenCV,"%.12g",fStddev);
      }
      ReportPrint(pRepPenCV,"\n");

      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"CV");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsCVSum[j],aPenaltyStatsCVSum2[j],iRun);
        ReportPrint1(pRepPenCV,"%s",sColSepString);
        ReportPrint1(pRepPenCV,"%.12g",fCV);
      }
      ReportPrint(pRepPenCV,"\n");
    }
  }
}



/***** Report -r vw2weights *****/

void ReportVW2WeightsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrint(pRepVW2Weights,"Run ID | Step # | Mean | VW2Weight[1] VW2Weight[2] ...\n");
  }
  ReportPrint1(pRepVW2Weights,"%"P32,iRun);
  ReportPrint1(pRepVW2Weights,"%s",sColSepString);
  ReportPrint1(pRepVW2Weights,"%"P64,iStep);
  ReportPrint1(pRepVW2Weights,"%s",sColSepString);
  ReportPrint1(pRepVW2Weights,"%g",fVW2WeightMean);
  for (j=1;j<=iNumVars;j++) {
    ReportPrint1(pRepVW2Weights,"%s",sColSepString);
    ReportPrint1(pRepVW2Weights,"%g",aVW2Weights[j]);
  }
  ReportPrint(pRepVW2Weights,"\n");
}

/***** report -r mobility *****/

void ReportMobilityPrint() {

  UINT32 j;
  FLOAT fMean;

  if (iRun==1) {
    ReportHdrPrefix(pRepMobility);
    ReportHdrPrint(pRepMobility," Run ID | AvgMobilityWindow[1] AvgMobilityWindow[2] ...\n");
  }

  ReportPrint1(pRepMobility,"%"P32,iRun);
  for (j=1;j<=iReportMobilityDisplay;j++) {
    fMean = aMobilityWindowSum[j] / (iStep - j);
    if (bReportMobilityNormalized) {
      if (j <= iNumVars) {
        fMean /= (FLOAT) j;
      } else {
        fMean /= (FLOAT) iNumVars;
      }
    }
    ReportPrint1(pRepMobility,"%s",sColSepString);
    ReportPrint1(pRepMobility,"%.12g",fMean);
  }
  ReportPrint(pRepMobility,"\n");
}



/***** report -r mobfixed *****/

void ReportMobFixedPrint() {

  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepMobFixed);
    ReportHdrPrint1(pRepMobFixed," Run ID | Step # | Mobility value window size %"P32"\n",iMobFixedWindow);
  }

  if (iStep <= iMobFixedWindow) {
    if (bMobilityFixedIncludeStart) {
      ReportPrint1(pRepMobFixed,"%"P32"\n",iRun);
      ReportPrint1(pRepMobFixed,"%s",sColSepString);
      ReportPrint1(pRepMobFixed,"%"P64"\n",iStep);
      ReportPrint1(pRepMobFixed,"%s",sColSepString);
      ReportPrint1(pRepMobFixed,"%"P32"\n",aMobilityWindow[iStep-1]);
    }
  } else {
    ReportPrint1(pRepMobFixed,"%"P32"\n",iRun);
    ReportPrint1(pRepMobFixed,"%s",sColSepString);
    ReportPrint1(pRepMobFixed,"%"P64"\n",iStep);
    ReportPrint1(pRepMobFixed,"%s",sColSepString);
    ReportPrint1(pRepMobFixed,"%"P32"\n",aMobilityWindow[iMobFixedWindow]);
  }
}


/***** report -r mobfixedfreq *****/

void ReportMobFixedFreqPrint() {

  UINT32 j;

  FLOAT fCurrent;
  FLOAT fCumulative = FLOATZERO;
  FLOAT iStepDiv;


  if (bMobilityFixedIncludeStart) {
    if (iRun==1) {
      ReportHdrPrefix(pRepMobFixedFreq);
      ReportHdrPrint1(pRepMobFixedFreq," Run ID | Count | Frequency of Count for mobility window size %"P32" | Normalized value ( / # steps) | Cumulative Normalized value \n",iMobFixedWindow);
    }
    
    iStepDiv = 1.0f / (FLOAT) iStep;      

    if (iStep > 0) {
      for (j=0;j<=iMobFixedWindow;j++) {

        fCurrent = ((FLOAT)aMobilityFixedFrequencies[j]) * iStepDiv;
        fCumulative += fCurrent;

        ReportPrint1(pRepMobFixedFreq,"%"P32,iRun);
        ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
        ReportPrint1(pRepMobFixedFreq,"%"P32,j);
        ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
        ReportPrint1(pRepMobFixedFreq,"%"P32,aMobilityFixedFrequencies[j]);
        ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
        ReportPrint1(pRepMobFixedFreq,"%.12g",fCurrent);
        ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
        ReportPrint1(pRepMobFixedFreq,"%.12g\n",fCumulative);
      }
    }
  } else {
    if (iRun==1) {
      ReportHdrPrefix(pRepMobFixedFreq);
      ReportHdrPrint1(pRepMobFixedFreq," Run ID | Count | Frequency of Count for mobility window size %"P32" | Normalized value [ / (# steps-window size)] | Cumulative Normalized value \n",iMobFixedWindow);
    }

    if (iStep > iMobFixedWindow) {
      iStepDiv = 1.0f / (FLOAT) (iStep - iMobFixedWindow);
    } else {
      iStepDiv = 1.0f / (FLOAT) iStep;      
    }

    for (j=0;j<=iMobFixedWindow;j++) {

      fCurrent = ((FLOAT)aMobilityFixedFrequencies[j]) * iStepDiv;
      fCumulative += fCurrent;

      ReportPrint1(pRepMobFixedFreq,"%"P32,iRun);
      ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
      ReportPrint1(pRepMobFixedFreq,"%"P32,j);
      ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
      ReportPrint1(pRepMobFixedFreq,"%"P32,aMobilityFixedFrequencies[j]);
      ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
      ReportPrint1(pRepMobFixedFreq,"%.12g",fCurrent);
      ReportPrint1(pRepMobFixedFreq,"%s",sColSepString);
      ReportPrint1(pRepMobFixedFreq,"%.12g\n",fCumulative);
    }
  }
}

/***** report -r varagefreq *****/

void ReportVarAgeFreqPrint() {

  UINT32 j;
  UBIGINT cdfVarAgeFreq = 0;

  if (iRun==1) {
    ReportHdrPrefix(pRepVarAgeFreq);
    ReportHdrPrint(pRepVarAgeFreq," Run ID | Frequency of Flips with Variable of Age 1,2,3... \n");
  }
  ReportPrint1(pRepVarAgeFreq,"%"P32,iRun);
  for (j=1; j < iMaxVarAgeFrequency + 1; j++) {
    ReportPrint1(pRepVarAgeFreq,"%s",sColSepString);
    if (iRepVarAgeFreqFormat == 0) {
      ReportPrint1(pRepVarAgeFreq,"%"P64,aVarAgeFrequency[j]);
    } else if (iRepVarAgeFreqFormat == 1) {
      ReportPrint1(pRepVarAgeFreq,"%g",aVarAgeFrequency[j] * 100.0 / iStep);
    } else {
      cdfVarAgeFreq += aVarAgeFrequency[j];
      ReportPrint1(pRepVarAgeFreq,"%"P64,cdfVarAgeFreq);
    }
  }
}

/***** report -r autocorr *****/

void ReportAutoCorrPrint() {

  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepAutoCorr);
    ReportHdrPrint(pRepAutoCorr," Run ID | AutoCorrelationLength | AutoCorrelation[1] AutoCorrelation[2] ...\n");
  }

  ReportPrint1(pRepAutoCorr,"%"P32,iRun);
  ReportPrint1(pRepAutoCorr,"%s",sColSepString);
  ReportPrint1(pRepAutoCorr,"%"P32,iAutoCorrLen);
  for (j=1;j<iAutoCorrMaxLen;j++) {
    ReportPrint1(pRepAutoCorr,"%s",sColSepString);
    ReportPrint1(pRepAutoCorr,"%.12g",aAutoCorrValues[j]);
  }
  ReportPrint(pRepAutoCorr,"\n");
}

/***** report -r triggers *****/

void ReportTriggersPrint() {

  UINT32 j,k,l;
  BOOL bFoundPos;
  TRIGGER *pTrig;
  ITEM *pItem;

  ReportHdrPrefix(pRepTriggers);
  ReportHdrPrint(pRepTriggers," Stage ID | Order | bActive? | bDisabled? | Trigger Name:\n");
  ReportPrint(pRepTriggers,"\n");

  if (bReportTriggersAll) {
    for (j=0;j<NUMEVENTPOINTS;j++) {
      for (k=0;k<listTriggers.iNumItems;k++) {
        pTrig = &aTriggers[k];
        pItem = &listTriggers.aItems[k];
        if (pTrig->eEventPoint== (enum EVENTPOINT) j) {
          bFoundPos = 0;
          for (l=0;l<aNumActiveProcedures[j];l++) {
            if (aActiveProcedures[j][l] == pTrig->pProcedure) {
              ReportPrint1(pRepTriggers,"%2"P32,j);
              ReportPrint1(pRepTriggers,"%s",sColSepString);
              ReportPrint1(pRepTriggers,"%"P32,l+1);
              ReportPrint1(pRepTriggers,"%s",sColSepString);
              ReportPrint1(pRepTriggers,"%"P32,pTrig->bActive);
              ReportPrint1(pRepTriggers,"%s",sColSepString);
              ReportPrint1(pRepTriggers,"%"P32,pTrig->bDisabled);
              ReportPrint1(pRepTriggers,"%s",sColSepString);
              ReportPrint1(pRepTriggers,"%s\n",pItem->sID);
              bFoundPos = 1;
              break;
            }
          }
          if (!bFoundPos) {
            ReportPrint1(pRepTriggers,"%2"P32,j);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint(pRepTriggers,"-");
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%"P32,pTrig->bActive);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%"P32,pTrig->bDisabled);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%s\n",pItem->sID);
          }
        }
      }
    }
  } else {
    for (j=0;j<NUMEVENTPOINTS;j++) {
      for (l=0;l<aNumActiveProcedures[j];l++) {
        for (k=0;k<listTriggers.iNumItems;k++) {
          pTrig = &aTriggers[k];
          pItem = &listTriggers.aItems[k];
          if ((pTrig->eEventPoint== (enum EVENTPOINT) j)&&(aActiveProcedures[j][l] == pTrig->pProcedure)) {
            ReportPrint1(pRepTriggers,"%2"P32,j);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%"P32,l+1);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%"P32,pTrig->bActive);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%"P32,pTrig->bDisabled);
            ReportPrint1(pRepTriggers,"%s",sColSepString);
            ReportPrint1(pRepTriggers,"%s\n",pItem->sID);
          }
        }
      }
    }
  }
}

/***** Report -r paramils *****/

void ReportParamILSPrint() {
  ReportPrint(pRepParamILS,"Result for ParamILS: ");
  if (bSolutionFound) {
    ReportPrint(pRepParamILS,"SAT");
  } else {
    ReportPrint(pRepParamILS,"TIMEOUT");
  }
  ReportPrint1(pRepParamILS,", %g",fRunTime);
  ReportPrint1(pRepParamILS,", %"P64,iStep);
  if (bWeighted) {
    ReportPrint1(pRepParamILS,", %"P64,iBestSumFalseWeight);
  } else {
    ReportPrint1(pRepParamILS,", %"P32,iBestNumFalse);
  }
  ReportPrint1(pRepParamILS,", %"P32"\n",iSeed);
  

}

/***** Report -r satcomp *****/

void ReportCompetitionComment() {
  SetString(&sCommentString,"c");
}

void ReportSatCompetitionPrint() {
  UINT32 j;

  PrintUBCSATHeader(pRepSATComp);
  PrintAlgParmSettings(pRepSATComp,&parmUBCSAT);
  ReportHdrPrefix(pRepSATComp);
  ReportHdrPrint(pRepSATComp,"\n");
  PrintAlgParmSettings(pRepSATComp,&pActiveAlgorithm->parmList);
  ReportHdrPrefix(pRepSATComp);
  ReportHdrPrint(pRepSATComp,"\n");

  if (bSolutionFound) {
    ReportPrint(pRepSATComp,"s SATISFIABLE\n");
    ReportPrint(pRepSATComp,"v ");
    for (j=1;j<=iNumVars;j++) {
      if (!aVarValue[j]) {
        ReportPrint1(pRepSATComp," -%"P32,j);
      } else {
        ReportPrint1(pRepSATComp," %"P32,j);
      }
      if (j % 10 == 0) {
        ReportPrint(pRepSATComp,"\nv ");
      }
    }
    ReportPrint(pRepSATComp," 0\n");
    exit(10);
  } else {
    ReportPrint(pRepSATComp,"s UNKNOWN\n");
    exit(0);
  }
}



/***** Report -r maxsatcomp *****/

void CreateReportMaxSatCompetitionSaveBest() {
  aMaxSatSaveBest = (BOOL *) AllocateRAM((iNumVars+1)*sizeof(BOOL), HeapReports);
}

void ReporMaxSatCompetitionPrintValues() {
  UINT32 j;
  if (iBestStepSumFalseWeight > iLastMaxSatPrintStep) {
    ReportPrint(pRepMAXSATComp,"v ");
    for (j=1;j<=iNumVars;j++) {
      if (!aMaxSatSaveBest[j]) {
        ReportPrint1(pRepMAXSATComp," -%"P32,j);
      } else {
        ReportPrint1(pRepMAXSATComp," %"P32,j);
      }
    }
    ReportPrint(pRepMAXSATComp,"\n");
    ReportFlush(pRepMAXSATComp);
    iLastMaxSatPrintStep = iStep;
  }
}

void ReportMaxSatCompetitionPrintStep() {
  double fTimeElapsed;
  UINT32 j;

  if ((iRun==1)&&(iStep==1)) {
    PrintUBCSATHeader(pRepMAXSATComp);
    fMaxSatNextPrintTime = fMaxSatPrintInterval;
    iLastMaxSatPrintStep = 0;
  }

  if (iStep == iBestStepSumFalseWeight) {
    ReportPrint1(pRepMAXSATComp,"o %"P64"\n",iBestSumFalseWeight);
    ReportFlush(pRepMAXSATComp);
    for (j=1;j<=iNumVars;j++) {
      aMaxSatSaveBest[j] = aVarValue[j];
    }
  }

  if (fMaxSatPrintInterval > FLOATZERO) {
    if ((iTimeResolution <= 1) || ((iTimeResolution % iTimeResolution) == 0)) {
      fTimeElapsed = TotalTimeElapsed();
      if (fTimeElapsed > fMaxSatNextPrintTime) {
        ReporMaxSatCompetitionPrintValues();
        fMaxSatNextPrintTime += fMaxSatPrintInterval;
      }
    }
  }
}

void ReportMaxSatCompetitionPrintFinal() {
  ReporMaxSatCompetitionPrintValues();
  ReportPrint(pRepMAXSATComp,"s UNKNOWN\n");
  ReportFlush(pRepMAXSATComp);
  exit(0);
}


/***** stat "percentsolve" *****/

void CalcPercentSolve() {
  fPercentSuccess = 100.0 * (FLOAT) iNumSolutionsFound / (FLOAT) iRun;
}


/***** Trigger SetupCSV *****/

void SetupCSV() {
  UINT32 j;
  REPORTCOL *pCol;
  if (bReportCSV) {

    SetString(&sColSepString,",");

    for (j=0;j<listColumns.iNumItems;j++) {
      if (!listColumns.aItems[j].bContainer) {
        pCol = &aColumns[j];
        switch (pCol->eFinalDataType) {
          case DTypeUInt:
            SetString(&pCol->sPrintFormat,"%"P32);
            break;
          case DTypeSInt:
            SetString(&pCol->sPrintFormat,"%ls");
            break;
          case DTypeUBigInt:
            SetString(&pCol->sPrintFormat,"%"P64);
            break;
          case DTypeSBigInt:
            SetString(&pCol->sPrintFormat,"%"PS64);
            break;
          case DTypeFloat:
            SetString(&pCol->sPrintFormat,"%f");
            break;
          case DTypeString:
            SetString(&pCol->sPrintFormat,"%s");
            break;
          default:
            break;
        }
      }
    }
  }
}


/***** Trigger ActivateStepsFoundColumns *****/

void ActivateStepsFoundColumns() {
  ActivateColumns("steps,found");
  AddAllocateRAMColumnID(FindItem(&listColumns,"steps"),"");
  AddAllocateRAMColumnID(FindItem(&listColumns,"found"),"");
}


/***** Trigger AllocateColumnRAM *****/

void AllocateColumnRAM() {

  UINT32 j;
  REPORTCOL *pCol;

  for (j=0;j<listColumns.iNumItems;j++) {
    pCol = &aColumns[j];
    if (pCol->bAllocateColumnRAM) {
      switch (pCol->eFinalDataType) {
        case DTypeUInt:
          pCol->puiColumnData = (UINT32 *) AllocateRAM(iNumRuns * sizeof(UINT32), HeapReports);
          memset(pCol->puiColumnData,0,(iNumRuns)*sizeof(UINT32));
          break;
        case DTypeSInt:
          pCol->psiColumnData = (SINT32 *) AllocateRAM(iNumRuns * sizeof(SINT32), HeapReports);
          memset(pCol->psiColumnData,0,(iNumRuns)*sizeof(SINT32));
          break;
        case DTypeUBigInt:
          pCol->pubiColumnData = (UBIGINT *) AllocateRAM(iNumRuns * sizeof(UBIGINT), HeapReports);
          memset(pCol->pubiColumnData,0,(iNumRuns)*sizeof(UBIGINT));
          break;
        case DTypeSBigInt:
          pCol->psbiColumnData = (SBIGINT *) AllocateRAM(iNumRuns * sizeof(SBIGINT), HeapReports);
          memset(pCol->psbiColumnData,0,(iNumRuns)*sizeof(SBIGINT));
          break;
        case DTypeFloat:
          pCol->pfColumnData = (FLOAT *) AllocateRAM(iNumRuns * sizeof(FLOAT), HeapReports);
          memset(pCol->pfColumnData,0,(iNumRuns)*sizeof(FLOAT));
          break;
        case DTypeString:
          ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
          AbnormalExit();
          exit(1);
        default:
          break;
      }
    }
  }
}


/***** Trigger ColumnInit *****/

void ColumnInit() {
  UINT32 j;
  REPORTCOL *pCol;
  
  for (j=0;j<iNumActiveCalcColumns;j++) {
    pCol = &aColumns[aActiveCalcColumns[j]];
    pCol->fRowSum = FLOATZERO;
    pCol->fRowSum2 = FLOATZERO;
    if (pCol->eColType == ColTypeMax) {
      pCol->fMinMaxVal = FLOATZERO;
      pCol->uiMinMaxVal = 0;
      pCol->siMinMaxVal = SINT32MIN;
      pCol->ubiMinMaxVal = 0;
      pCol->sbiMinMaxVal = SBIGINTMIN;
    } else {
      pCol->fMinMaxVal = FLOATMAX;
      pCol->uiMinMaxVal = UINT32MAX;
      pCol->siMinMaxVal = SINT32MAX;
      pCol->ubiMinMaxVal = UBIGINTMAX;
      pCol->sbiMinMaxVal = SBIGINTMAX;    }
  }
}



/***** Trigger ColumnRunCalculation *****/

void ColumnRunCalculation() {

  UINT32 j;
  FLOAT fStepMul;
  FLOAT fMean;
  FLOAT fStddev;
  FLOAT fCV;
  REPORTCOL *pCol;

  if (iStep == 0) {
    fStepMul = 1.0f;
  } else {
    fStepMul = 1.0f / ((FLOAT) iStep);
  }

  for (j=0;j<listColumns.iNumItems;j++) {
    pCol = &aColumns[j];
    if (pCol->bActive) {
      switch (pCol->eColType) {
        case ColTypeFinal:
          switch (pCol->eSourceDataType) {
          case DTypeUInt:
            pCol->uiCurRowValue = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            pCol->siCurRowValue = *pCol->psiCurValue;
            break;
          case DTypeUBigInt:
            pCol->ubiCurRowValue = *pCol->pubiCurValue;
            break;
          case DTypeSBigInt:
            pCol->sbiCurRowValue = *pCol->psbiCurValue;
            break;
          case DTypeFloat:
            pCol->fCurRowValue = *pCol->pfCurValue;
            break;
          case DTypeString:
            ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
            AbnormalExit();
            exit(1);
          default:
            break;
          }
          break;
        case ColTypeMin:
        case ColTypeMax:
          switch (pCol->eSourceDataType) {
          case DTypeUInt:
            pCol->uiCurRowValue = pCol->uiMinMaxVal;
            break;
          case DTypeSInt:
            pCol->siCurRowValue = pCol->siMinMaxVal;
            break;
          case DTypeUBigInt:
            pCol->ubiCurRowValue = pCol->ubiMinMaxVal;
            break;
          case DTypeSBigInt:
            pCol->sbiCurRowValue = pCol->sbiMinMaxVal;
            break;
          case DTypeFloat:
            pCol->fCurRowValue = pCol->fMinMaxVal;
            break;
          case DTypeString:
            ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
            AbnormalExit();
            exit(1);
          default:
            break;
          }
          break;
        case ColTypeFinalDivStep:
        case ColTypeFinalDivStep100:
          switch (pCol->eSourceDataType) {
          case DTypeUInt:
            pCol->fCurRowValue = (FLOAT) *pCol->puiCurValue;
            break;
          case DTypeSInt:
            pCol->fCurRowValue = (FLOAT) *pCol->psiCurValue;
            break;
          case DTypeUBigInt:
            pCol->fCurRowValue = (FLOAT) *pCol->pubiCurValue;
            break;
          case DTypeSBigInt:
            pCol->fCurRowValue = (FLOAT) *pCol->psbiCurValue;
            break;
          case DTypeFloat:
            pCol->fCurRowValue = *pCol->pfCurValue;
            break;
          case DTypeString:
            ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
            AbnormalExit();
            exit(1);
          default:
            break;
          }
          pCol->fCurRowValue *= fStepMul;
          if (pCol->eColType == ColTypeFinalDivStep100) {
            pCol->fCurRowValue *= 100.0f;
          }
          break;

        case ColTypeMean:
        case ColTypeStddev:
        case ColTypeCV:

          CalculateStats(&fMean,&fStddev,&fCV,pCol->fRowSum,pCol->fRowSum2,iStep);

          if (pCol->eColType == ColTypeMean) {
            pCol->fCurRowValue = fMean;
          } else {
            if (pCol->eColType == ColTypeStddev) {
              pCol->fCurRowValue = fStddev;
            } else {
              pCol->fCurRowValue = fCV;
            }
          }
          break;

        default:
          break;
      }
      if (pCol->bAllocateColumnRAM) {
        if (pCol->eFinalDataType == DTypeUInt) {
          pCol->puiColumnData[iRun-1] = pCol->uiCurRowValue;
        } else if (pCol->eFinalDataType == DTypeSInt) {
          pCol->psiColumnData[iRun-1] = pCol->siCurRowValue;
        } else if (pCol->eFinalDataType == DTypeUBigInt) {
          pCol->pubiColumnData[iRun-1] = pCol->ubiCurRowValue;
        } else if (pCol->eFinalDataType == DTypeSBigInt) {
          pCol->psbiColumnData[iRun-1] = pCol->sbiCurRowValue;
        } else {
          pCol->pfColumnData[iRun-1] = pCol->fCurRowValue;
        }
      }

      if (pCol->eFinalDataType == DTypeUInt) {
        pCol->fCurRowValue = (FLOAT) pCol->uiCurRowValue;
      } else if (pCol->eFinalDataType == DTypeSInt) {
        pCol->fCurRowValue = (FLOAT) pCol->siCurRowValue;
      } else if (pCol->eFinalDataType == DTypeUBigInt) {
        pCol->fCurRowValue = (FLOAT) pCol->ubiCurRowValue;
      } else if (pCol->eFinalDataType == DTypeSBigInt) {
        pCol->fCurRowValue = (FLOAT) pCol->sbiCurRowValue;
      }

      pCol->fColSum += pCol->fCurRowValue;
      pCol->fColSum2 += (pCol->fCurRowValue * pCol->fCurRowValue);

    }
  }
}



/***** Trigger ColumnStepCalculation *****/

void ColumnStepCalculation() {

  UINT32 j;
  REPORTCOL *pCol;
  
  for (j=0;j<iNumActiveCalcColumns;j++) {
    pCol = &aColumns[aActiveCalcColumns[j]];
    switch (pCol->eColType) {
      case ColTypeStddev:
      case ColTypeCV:
        switch (pCol->eSourceDataType) {
        case DTypeUInt:
          pCol->fRowSum2 += ((FLOAT) *pCol->puiCurValue) * ((FLOAT) *pCol->puiCurValue);
          break;
        case DTypeSInt:
          pCol->fRowSum2 += ((FLOAT) *pCol->psiCurValue) * ((FLOAT) *pCol->psiCurValue);
          break;
        case DTypeUBigInt:
          pCol->fRowSum2 += ((FLOAT) *pCol->pubiCurValue) * ((FLOAT) *pCol->pubiCurValue);
          break;
        case DTypeSBigInt:
          pCol->fRowSum2 += ((FLOAT) *pCol->psbiCurValue) * ((FLOAT) *pCol->psbiCurValue);
          break;
        case DTypeFloat:
          pCol->fRowSum2 += ((*pCol->pfCurValue) * (*pCol->pfCurValue));
          break;
        case DTypeString:
          ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
          AbnormalExit();
          exit(1);
        default:
          break;
        }
      case ColTypeMean:
        switch (pCol->eSourceDataType) {
        case DTypeUInt:
          pCol->fRowSum += (FLOAT) *pCol->puiCurValue;
          break;
        case DTypeSInt:
          pCol->fRowSum += (FLOAT) *pCol->psiCurValue;
          break;
        case DTypeUBigInt:
          pCol->fRowSum += (FLOAT) *pCol->pubiCurValue;
          break;
        case DTypeSBigInt:
          pCol->fRowSum += (FLOAT) *pCol->psbiCurValue;
          break;
        case DTypeFloat:
          pCol->fRowSum += *pCol->pfCurValue;
          break;
        case DTypeString:
          ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
          AbnormalExit();
          exit(1);
        default:
          break;
        }
        break;

      case ColTypeMin:
        switch (pCol->eSourceDataType) {
          case DTypeUInt:
            if (*pCol->puiCurValue < pCol->uiMinMaxVal) pCol->uiMinMaxVal = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            if (*pCol->psiCurValue < pCol->siMinMaxVal) pCol->siMinMaxVal = *pCol->psiCurValue;
            break;
          case DTypeUBigInt:
            if (*pCol->pubiCurValue < pCol->ubiMinMaxVal) pCol->ubiMinMaxVal = *pCol->pubiCurValue;
            break;
          case DTypeSBigInt:
            if (*pCol->psbiCurValue < pCol->sbiMinMaxVal) pCol->sbiMinMaxVal = *pCol->psbiCurValue;
            break;
          case DTypeFloat:
            if (*pCol->pfCurValue < pCol->fMinMaxVal) pCol->fMinMaxVal = *pCol->pfCurValue;
            break;
          case DTypeString:
            ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
            AbnormalExit();
            exit(1);
          default:
            break;
        }
        break;

      case ColTypeMax:
        switch (pCol->eSourceDataType) {
          case DTypeUInt:
            if (*pCol->puiCurValue > pCol->uiMinMaxVal) pCol->uiMinMaxVal = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            if (*pCol->psiCurValue > pCol->siMinMaxVal) pCol->siMinMaxVal = *pCol->psiCurValue;
            break;
          case DTypeUBigInt:
            if (*pCol->pubiCurValue > pCol->ubiMinMaxVal) pCol->ubiMinMaxVal = *pCol->pubiCurValue;
            break;
          case DTypeSBigInt:
            if (*pCol->psbiCurValue > pCol->sbiMinMaxVal) pCol->sbiMinMaxVal = *pCol->psbiCurValue;
            break;
          case DTypeFloat:
            if (*pCol->pfCurValue > pCol->fMinMaxVal) pCol->fMinMaxVal = *pCol->pfCurValue;
            break;
          case DTypeString:
            ReportPrint(pRepErr,"Unexpected Error: String Column Data\n");
            AbnormalExit();
            exit(1);
          default:
            break;
        }
        break;

      case ColTypeFinal:
      case ColTypeFinalDivStep:
      case ColTypeFinalDivStep100:
      default:
        break;
    }
  }
}



/***** Trigger CalcFPS *****/

void CalcFPS() {
  UINT32 j;
  REPORTCOL *pCol;

  pCol = &aColumns[FindItem(&listColumns,"steps")];
  fFlipsPerSecond = FLOATZERO;
  for (j=0;j< iRun;j++) {
    fFlipsPerSecond += GetRowElement(pCol,j,0,0);
  }
  fFlipsPerSecond /= fTotalTime;
}


/***** Trigger StringAlgParms *****/
void StringAlgParms() {

  UINT32 j;
  char * pNext;
  ALGPARM *pCurParm;
  
  sStringAlgParms = (char *) AllocateRAM(MAXPARMLINELEN, HeapString);
  
  pNext = sStringAlgParms;
  pNext += sprintf(pNext,"-alg %s",pActiveAlgorithm->sName);
  if (*(pActiveAlgorithm->sVariant)) {
    pNext += sprintf(pNext," -v %s",pActiveAlgorithm->sVariant);
  }
  if (pActiveAlgorithm->bWeighted) {
    pNext += sprintf(pNext," -w");
  }
  
  for (j=0;j<pActiveAlgorithm->parmList.iNumParms;j++) {
    pCurParm = &pActiveAlgorithm->parmList.aParms[j];
    pNext += sprintf(pNext," %s ",pCurParm->sSwitch);
    switch (pCurParm->eType) {
      case PTypeUInt:
        pNext += sprintf(pNext,"%"P32" ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeSInt:
        pNext += sprintf(pNext,"%"PS32" ", *(SINT32 *)pCurParm->pParmValue);
        break;
      case PTypeUBigInt:
        pNext += sprintf(pNext,"%"P64" ", *(UBIGINT *)pCurParm->pParmValue);
        break;
      case PTypeSBigInt:
        pNext += sprintf(pNext,"%"PS64" ", *(SBIGINT *)pCurParm->pParmValue);
        break;
      case PTypeProbability:
        pNext += sprintf(pNext,"%.4g ", ProbToFloat(*(PROBABILITY *)pCurParm->pParmValue));
        break;
      case PTypeString:
        if (**(char **)pCurParm->pParmValue ==0) {
          pNext += sprintf(pNext,"%s ","[null]");
        } else {
          pNext += sprintf(pNext,"%s ", *(char **)pCurParm->pParmValue);
        }
        break;
      case PTypeFloat:
        pNext += sprintf(pNext,"%.6g ", *(FLOAT *)pCurParm->pParmValue);
        break;
      case PTypeBool:
        pNext += sprintf(pNext,"%u", (unsigned int) *(BOOL *)pCurParm->pParmValue);
        break;
      case PTypeReport:
        break;
      default:
        break;
    }  
  }


}


/***** Trigger UpdatePercents *****/

void UpdatePercents() {
  
  UINT32 j;
  REPORTCOL *pCol;
  pCol = &aColumns[FindItem(&listColumns,"prob")];
  for (j=0;j<iRun;j++) {
    pCol->pfColumnData[aSortedByStepsAndFound[j]] = ((FLOAT) (j+1))/((FLOAT)iRun);
  }
}


/***** Trigger UpdateColSolutionFound *****/

void UpdateColSolutionFound() {
  
  if (bSolutionFound) {
    iColSolutionFound = 1;
  } else {
    iColSolutionFound = 0;
  }
}


/***** Trigger UpdateTimes *****/

void UpdateTimes() {
  
  UINT32 j;
  REPORTCOL *pColTimes;
  REPORTCOL *pColSteps;
  FLOAT *aTimes;
  UBIGINT *aSteps;

  FLOAT fTotalSteps = FLOATZERO;

  pColTimes = &aColumns[FindItem(&listColumns,"timesteps")];

  if (!pColTimes->bAllocateColumnRAM) {
    return; /* may not be an error if column requested in -r out but not -r rtd */
  }
  
  pColSteps = &aColumns[FindItem(&listColumns,"steps")];

  aSteps = pColSteps->pubiColumnData;
  for (j=0;j<iRun;j++) {
    fTotalSteps += (FLOAT) aSteps[j];
  }

  aTimes = pColTimes->pfColumnData;
  for (j=0;j<iRun;j++) {
    aTimes[j] = (FLOAT) aSteps[j];
    aTimes[j] /= fTotalSteps;
    aTimes[j] *= fTotalTime;

    pColTimes->fColSum += aTimes[j];
    pColTimes->fColSum2 += (aTimes[j] * aTimes[j]);
  }

}

/***** Trigger InitMobilityColumnN *****/

void InitMobilityColumnN() {
  bMobilityColNActive = 1;
}

/***** Trigger InitMobilityColumnX *****/

void InitMobilityColumnX() {
  bMobilityColXActive = 1;
}

void UpdateMobilityColumn() {

  UBIGINT iCount = (UBIGINT) iNumVars;
  FLOAT fStdDev;
  
  if (bMobilityColNActive) {

    if (iStep <= (UBIGINT) iNumVars) {
      iCount = iStep;
    } else {
      if (!bMobilityFixedIncludeStart) {
        iCount = iStep - iNumVars;
      } 
    }

    CalculateStats(&fMobilityColNMean,&fStdDev,&fMobilityColNCV,aMobilityWindowSum[iNumVars],aMobilityWindowSum2[iNumVars],iCount);
    fMobilityColNMeanNorm = fMobilityColNMean / (FLOAT) iNumVars;

  }

  if (bMobilityColXActive) {

    if (iStep <= iMobFixedWindow) {
      iCount = iStep;
    } else {
      if (!bMobilityFixedIncludeStart) {
        iCount = iStep - iMobFixedWindow;
      } 
    }

    CalculateStats(&fMobilityColXMean,&fStdDev,&fMobilityColXCV,aMobilityWindowSum[iMobFixedWindow],aMobilityWindowSum2[iMobFixedWindow],iCount);
    if (iMobFixedWindow < iNumVars) {
      fMobilityColXMeanNorm = fMobilityColXMean / (FLOAT) iMobFixedWindow;
    } else {
      fMobilityColXMeanNorm = fMobilityColXMean / (FLOAT) iNumVars;
    }
  }
}


/***** Trigger CompareSorted *****/

REPORTCOL *pSortCol;
UINT32 *aFoundData;
UINT32 *auiSortColData;
SINT32 *asiSortColData;
UBIGINT *aubiSortColData;
SBIGINT *asbiSortColData;
FLOAT *afSortColData;

int CompareSortedUInt(const void *a, const void *b) {
  UINT32 a1,b1;

  a1 = auiSortColData[*(UINT32 *)a];
  b1 = auiSortColData[*(UINT32 *)b];
  if (a1 < b1) {
    return(-1);
  } else if (a1 > b1) {
    return(1);
  };
  return(0);
}

int CompareSortedUBigInt(const void *a, const void *b) {
  UBIGINT a1,b1;

  a1 = aubiSortColData[*(UINT32 *)a];
  b1 = aubiSortColData[*(UINT32 *)b];
  if (a1 < b1) {
    return(-1);
  } else if (a1 > b1) {
    return(1);
  };
  return(0);
}

int CompareSortedSInt(const void *a, const void *b) {
  return (asiSortColData[*(UINT32 *)a] - asiSortColData[*(UINT32 *)b]);
}

int CompareSortedSBigInt(const void *a, const void *b) {
  SBIGINT a1,b1;

  a1 = asbiSortColData[*(UINT32 *)a];
  b1 = asbiSortColData[*(UINT32 *)b];
  if (a1 < b1) {
    return(-1);
  } else if (a1 > b1) {
    return(1);
  };
  return(0);
}


int CompareSortedFloat(const void *a, const void *b) {
  FLOAT fCompare;
  fCompare = afSortColData[*(UINT32 *)a] - afSortColData[*(UINT32 *)b];
  if (fCompare < FLOATZERO) {
    return(-1);
  } else if (fCompare > FLOATZERO) {
    return(1);
  }
  return(0);
}

void SortByCurrentColData(REPORTCOL *pCol) {
  
  UINT32 j;
  
  if (aSortedByCurrent == NULL) {
    aSortedByCurrent = (UINT32 *) AllocateRAM(sizeof(UINT32) * (iRun), HeapReports);
  }

  for (j=0;j<iRun;j++) {
    aSortedByCurrent[j] = j;
  }
  
  pSortCol = pCol;

  if (pSortCol->eFinalDataType == DTypeUInt) {
    auiSortColData = pSortCol->puiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSortedUInt);
  } else if (pSortCol->eFinalDataType == DTypeSInt) {
    asiSortColData = pSortCol->psiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSortedSInt);
  } else if (pSortCol->eFinalDataType == DTypeUBigInt) {
    aubiSortColData = pSortCol->pubiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSortedUBigInt);
  } else if (pSortCol->eFinalDataType == DTypeSBigInt) {
    asbiSortColData = pSortCol->psbiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSortedSBigInt);
  } else {
    afSortColData = pSortCol->pfColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSortedFloat);
  }

}

int CompareFoundSortedUInt(const void *a, const void *b) {
  UINT32 a1,b1;

  if (aFoundData[*(UINT32 *)a] != aFoundData[*(UINT32 *)b]) {
    if (aFoundData[*(UINT32 *)a]) {
      return(-1);
    } else {
      return(1);
    }
  }

  a1 = auiSortColData[*(UINT32 *)a];
  b1 = auiSortColData[*(UINT32 *)b];
  if (a1 < b1) {
    return(-1);
  } else if (a1 > b1) {
    return(1);
  };
  return(0);
}

int CompareFoundSortedSInt(const void *a, const void *b) {
  if (aFoundData[*(UINT32 *)a] != aFoundData[*(UINT32 *)b]) {
    if (aFoundData[*(UINT32 *)a]) {
      return(-1);
    } else {
      return(1);
    }
  }
  return (asiSortColData[*(UINT32 *)a] - asiSortColData[*(UINT32 *)b]);
}

int CompareFoundSortedUBigInt(const void *a, const void *b) {
  UBIGINT a1,b1;

  if (aFoundData[*(UINT32 *)a] != aFoundData[*(UINT32 *)b]) {
    if (aFoundData[*(UINT32 *)a]) {
      return(-1);
    } else {
      return(1);
    }
  }

  a1 = aubiSortColData[*(UINT32 *)a];
  b1 = aubiSortColData[*(UINT32 *)b];
  if (a1 < b1) {
    return(-1);
  } else if (a1 > b1) {
    return(1);
  };
  return(0);
}

int CompareFoundSortedSBigInt(const void *a, const void *b) {
  SBIGINT a1,b1;

  if (aFoundData[*(UINT32 *)a] != aFoundData[*(UINT32 *)b]) {
    if (aFoundData[*(UINT32 *)a]) {
      return(-1);
    } else {
      return(1);
    }
  }

  a1 = asbiSortColData[*(UINT32 *)a];
  b1 = asbiSortColData[*(UINT32 *)b];
  if (a1 < b1) {
    return(-1);
  } else if (a1 > b1) {
    return(1);
  };
  return(0);
}

int CompareFoundSortedFloat(const void *a, const void *b) {
  FLOAT fCompare;
  if (aFoundData[*(UINT32 *)a] != aFoundData[*(UINT32 *)b]) {
    if (aFoundData[*(UINT32 *)a]) {
      return(-1);
    } else {
      return(1);
    }
  }
  fCompare = afSortColData[*(UINT32 *)a] - afSortColData[*(UINT32 *)b];
  if (fCompare < FLOATZERO) {
    return(-1);
  } else if (fCompare > FLOATZERO) {
    return(1);
  }
  return(0);
}

void SortByCurrentColDataAndFound(REPORTCOL *pCol) {
  
  UINT32 j;
  REPORTCOL *pColFound;
  
  pColFound = &aColumns[FindItem(&listColumns,"found")];
  aFoundData = pColFound->puiColumnData;
  
  if (aSortedByCurrent == NULL) {
    aSortedByCurrent = (UINT32 *) AllocateRAM(sizeof(UINT32) * (iRun), HeapReports);
  }

  for (j=0;j<iRun;j++) {
    aSortedByCurrent[j] = j;
  }
  
  pSortCol = pCol;

  if (pSortCol->eFinalDataType == DTypeUInt) {
    auiSortColData = pSortCol->puiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareFoundSortedUInt);
  } else if (pSortCol->eFinalDataType == DTypeSInt) {
    asiSortColData = pSortCol->psiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareFoundSortedSInt);
  } else if (pSortCol->eFinalDataType == DTypeUBigInt) {
    aubiSortColData = pSortCol->pubiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareFoundSortedUBigInt);
  } else if (pSortCol->eFinalDataType == DTypeSBigInt) {
    asbiSortColData = pSortCol->psbiColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareFoundSortedSBigInt);
  } else {
    afSortColData = pSortCol->pfColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareFoundSortedFloat);
  }

}

void SortByStepPerformance() {

  UINT32 j;
  REPORTCOL *pCol;

  if (bSortedByStepsValid) {
    return;
  }

  pCol = &aColumns[FindItem(&listColumns,"steps")];

  SortByCurrentColData(pCol);
  aSortedBySteps = (UINT32 *) AllocateRAM(sizeof(UINT32) * (iRun), HeapReports);
  for (j=0;j<iRun;j++) {
    aSortedBySteps[j] = aSortedByCurrent[j];
  }

  SortByCurrentColDataAndFound(pCol);
  aSortedByStepsAndFound = (UINT32 *) AllocateRAM(sizeof(UINT32) * (iRun), HeapReports);
  for (j=0;j<iRun;j++) {
    aSortedByStepsAndFound[j] = aSortedByCurrent[j];
  }

  bSortedByStepsValid = 1;

}

#ifdef __cplusplus

}
#endif

