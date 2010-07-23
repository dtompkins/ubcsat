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

/***** Trigger ReportFlipCountsPrint *****/
void ReportFlipCountsPrint();

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
UINT32 bReportPenaltyEveryLM;
FLOAT *aPenaltyStatsFinal;
FLOAT *aPenaltyStatsFinalSum;
FLOAT *aPenaltyStatsFinalSum2;

/***** Trigger ReportPenMeanPrint *****/
void ReportPenMeanPrint();

/***** Trigger ReportPenStddevPrint *****/
void ReportPenStddevPrint();

/***** Trigger ReportPenCVPrint *****/
void ReportPenCVPrint();

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

/***** Trigger ReportAutoCorrPrint *****/
void ReportAutoCorrPrint();

/***** Trigger ReportSatCompetitionPrint *****/
void ReportSatCompetitionPrint();

/***** Trigger ActivateStepsColumn *****/
void ActivateStepsColumn();

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
void SortByCurrentColData(REPORTCOL *pCol);
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

  CreateTrigger("ReportFlipCountsPrint",PostRun,ReportFlipCountsPrint,"FlipCounts","");

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

  CreateTrigger("ReportAutoCorrPrint",PostRun,ReportAutoCorrPrint,"AutoCorr","");

  CreateTrigger("ReportSatCompetitionPrint",FinalReports,ReportSatCompetitionPrint,"","");

  CreateTrigger("ActivateStepsColumn",PostParameters,ActivateStepsColumn,"","");

  CreateTrigger("AllocateColumnRAM",PostRead,AllocateColumnRAM,"","");

  CreateTrigger("CalcPercentSolve",FinalCalculations,CalcPercentSolve,"","");
  CreateTrigger("CalcFPS",FinalCalculations,CalcFPS,"","");
  CreateTrigger("StringAlgParms",FinalCalculations,StringAlgParms,"","");
  CreateTrigger("ColumnRunCalculation",PostRun,ColumnRunCalculation,"","");
  CreateTrigger("ColumnStepCalculation",StepCalculations,ColumnStepCalculation,"ColumnInit","");
  CreateTrigger("ColumnInit",PreRun,ColumnInit,"","");
  CreateTrigger("SolveMode",PreStart,InitSolveMode,"ReportModelPrint","");
  CreateTrigger("UpdatePercents",FinalCalculations,UpdatePercents,"SortByStepPerformance","");

  CreateTrigger("InitMobilityColumnN",PostParameters,InitMobilityColumnN,"","");
  CreateTrigger("InitMobilityColumnX",PostParameters,InitMobilityColumnX,"","");
  CreateTrigger("MobilityColumn",RunCalculations,UpdateMobilityColumn,"MobilityWindow","");

  CreateTrigger("UpdateTimes",FinalCalculations,UpdateTimes,"SortByStepPerformance","");  
  CreateTrigger("SortByStepPerformance",FinalCalculations,SortByStepPerformance,"","");

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
      ReportHdrPrint(pRep,aColumns[aCols[j]].sHeader1);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {
      ReportHdrPrint(pRep,aColumns[aCols[j]].sHeader2);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    for (j=0;j<iNumCols;j++) {      
      ReportHdrPrint(pRep,aColumns[aCols[j]].sHeader3);
      ReportHdrPrint(pRep," ");
    }
    ReportHdrPrint(pRep,"\n");
    ReportHdrPrefix(pRep);
    ReportHdrPrint(pRep,"\n");
  }

}

void ReportOutHeader() {
  ReportHdrPrefix(pRepOut);
  ReportHdrPrint1(pRepOut," -alg %s",pActiveAlgorithm->sName);
  if (*(pActiveAlgorithm->sVariant))
    ReportHdrPrint1(pRepOut," -v %s",pActiveAlgorithm->sVariant);
  if (pActiveAlgorithm->bWeighted)
    ReportHdrPrint(pRepOut," -w");
  
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

  ReportPrint(pRep,"  ");

  for (j=0;j<iNumCols;j++) {
    pCol = &aColumns[aCols[j]];

    if (pCol->bAllocateColumnRAM) {
      if (pCol->eFinalDataType == DTypeUInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->puiColumnData[iRow]);
      } else if (pCol->eFinalDataType == DTypeSInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->psiColumnData[iRow]);
      } else {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->pfColumnData[iRow]);
      }
      ReportPrint(pRep," ");
    } else {
      if (pCol->eFinalDataType == DTypeUInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->uiCurRowValue);
      } else if (pCol->eFinalDataType == DTypeSInt) {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->siCurRowValue);
      } else {
        ReportPrint1(pRep,pCol->sPrintFormat,pCol->fCurRowValue);
      }
      ReportPrint(pRep," ");
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

  if (pCol->bAllocateColumnRAM == FALSE) {
    ReportPrint1(pRepErr,"Unexpected Error: Column Data for [%s] was not allocated properly\n",pCol->sDescription);
    AbnormalExit();
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
  } else {
    return (pCol->pfColumnData[iRow]);
  }
}

void ReportStatsPrint() {
  
  UINT32 j,k;
  REPORTCOL *pCol;
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

  char **pString;

  ReportPrint(pRepStats,"\n\n");

  for (k=0;k<iNumStatsActive;k++) {
    for (j=0;j<listStats.iNumItems;j++) {
      pStat = &aStats[j];
      if ((pStat->bActive)&&(pStat->iActiveID==k)) {
        if (pStat->bCustomField) {

          ReportPrint1(pRepStats,"%s = ",pStat->sBaseDescription);

          switch (pStat->eCustomType) {
            case DTypeUInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(UINT32 *) pStat->pCustomValue);
              break;
            case DTypeSInt:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(SINT32 *) pStat->pCustomValue);
              break;
            case DTypeFloat:
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*(FLOAT *) pStat->pCustomValue);
              break;
            case DTypeString:
              pString = (char **) pStat->pCustomValue;
              ReportPrint1(pRepStats,pStat->sPrintCustomFormat,*pString);
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
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Mean",fMean);
              }

              if (pStat->iStatFlags & STATCODE_stddev) {
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"StdDev",fStdDev);
              }

              if (pStat->iStatFlags & STATCODE_cv) {
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"CoeffVariance",fCV);
              }

              if (pStat->iStatFlags & STATCODE_var) {
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Variance",fVar);
              }

              if (pStat->iStatFlags & STATCODE_stderr) {
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"StdErr",fStdErr);
              }

              if (pStat->iStatFlags & STATCODE_vmr) {
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"VarMeanRatio",fVMR);
              }
            }

            if (pStat->iStatFlags & STATCODE_sum) {
              ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Sum",pCol->fColSum);
            }

            if (pStat->iStatFlags & STATCODE_SORTMASK) {
              if (!pStat->bSortByStep) {
                SortByCurrentColData(pCol);
              }

              if (pStat->iStatFlags & STATCODE_median) {
                fMedian = GetRowElement(pCol,(iRun-1)>>1,TRUE,pStat->bSortByStep);
                if (iRun % 2 == 0) {
                  fMedian += GetRowElement(pCol,(iRun)>>1,TRUE,pStat->bSortByStep);
                  fMedian /= 2.0;
                }
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Median",fMedian);
              }

              if (pStat->iStatFlags & STATCODE_min) {
                iPos = 0;
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Min",fVal);
              }

              if (pStat->iStatFlags & STATCODE_max) {
                iPos = iRun - 1;
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Max",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q05) {
                iPos = (UINT32)(floor(0.05 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.05",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q10) {
                iPos = (UINT32)(floor(0.10 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.10",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q25) {
                iPos = (UINT32)(floor(0.25 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.25",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q75) {
                iPos = (UINT32)(floor(0.75 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.75",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q90) {
                iPos = (UINT32)(floor(0.90 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.90",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q95) {
                iPos = (UINT32)(floor(0.95 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.95",fVal);
              }
              if (pStat->iStatFlags & STATCODE_q98) {
                iPos = (UINT32)(floor(0.98 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.98",fVal);
              }
              if (pStat->iStatFlags & STATCODE_qr7525) {
                iPos = (UINT32)(floor(0.75 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                iPos = (UINT32)(floor(0.25 * (FLOAT) (iRun-1)));
                fVal2 = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                if (fVal2 != FLOATZERO) {
                  fVal2 = fVal / fVal2;
                }
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.75/25",fVal2);
              }
              
              if (pStat->iStatFlags & STATCODE_qr9010) {
                iPos = (UINT32)(floor(0.90 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                iPos = (UINT32)(floor(0.10 * (FLOAT) (iRun-1)));
                fVal2 = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                if (fVal2 != FLOATZERO) {
                  fVal2 = fVal / fVal2;
                }
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.90/10",fVal2);
              }

              if (pStat->iStatFlags & STATCODE_qr9505) {
                iPos = (UINT32)(floor(0.95 * (FLOAT) (iRun-1)));
                fVal = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                iPos = (UINT32)(floor(0.05 * (FLOAT) (iRun-1)));
                fVal2 = GetRowElement(pCol,iPos,TRUE,pStat->bSortByStep);
                if (fVal2 != FLOATZERO) {
                  fVal2 = fVal / fVal2;
                }
                ReportPrint3(pRepStats,"%s_%s = %g\n",pStat->sBaseDescription,"Q.95/05",fVal2);
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
  PrintRow(pRepRTD,aSortedBySteps[iRow],iNumRTDColumns,aRTDColumns);
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
    pRepModel->bActive = TRUE;
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
        ReportHdrPrint1(pRepModel,"Solution found for -wtarget %f\n\n", fTargetW);
      } else {
        ReportHdrPrint1(pRepModel,"Solution found for -target %u\n\n", iTarget);
      }
      for (j=1;j<=iNumVars;j++) {
        if (!aVarValue[j]) {
          ReportPrint1(pRepModel," -%u",j);
        } else {
          ReportPrint1(pRepModel," %u",j);
        }
        if (j % 10 == 0)
          ReportPrint(pRepModel,"\n");
      }
      if ((j-1) % 10 != 0) 
        ReportPrint(pRepModel,"\n");
    } else {
      ReportHdrPrefix(pRepModel);
      if (bWeighted) {
        ReportHdrPrint1(pRepModel,"No Solution found for -wtarget %f\n\n", fTargetW);
      } else {
        ReportHdrPrint1(pRepModel,"No Solution found for -target %u\n", iTarget);
      }

      
    }
  }
}



/***** Report -r cnfstats *****/

void ReportCNFStatsPrint() {

  UINT32 j;
  UINT32 iMaxClauseLen;
  FLOAT fAvgLitClause = 0.0;
  FLOAT fAvgVarOccur = 0.0;
  FLOAT fStdDevVarOccur = 0.0;
  FLOAT fTemp;
  UINT32 iNumPos;
  UINT32 iNumNeg;
  FLOAT fPosNegRatio;

  UINT32 *aClauseBins;
  

  ReportPrint1(pRepCNFStats,"Clauses = %u\n",iNumClauses);
  ReportPrint1(pRepCNFStats,"Variables = %u \n",iNumVars);
  ReportPrint1(pRepCNFStats,"TotalLiterals = %u\n",iNumLits);

  iMaxClauseLen = 0;
  for (j=0;j<iNumClauses;j++) {
    if (aClauseLen[j] > iMaxClauseLen)
      iMaxClauseLen = aClauseLen[j];
  }
  ReportPrint1(pRepCNFStats,"MaxClauseLen = %u\n",iMaxClauseLen);

  if (iNumClauses > 0) {

    aClauseBins = AllocateRAM((iMaxClauseLen + 3) * sizeof(UINT32));
    
    for (j=0;j<iMaxClauseLen + 3;j++) {
      aClauseBins[j] = 0;
    }

    for (j=0;j<iNumClauses;j++) {
      aClauseBins[aClauseLen[j]]++;
    }

    ReportPrint1(pRepCNFStats,"NumClauseLen1 = %u \n",aClauseBins[1]);
    ReportPrint1(pRepCNFStats,"NumClauseLen2 =  %u \n",aClauseBins[2]);
    ReportPrint1(pRepCNFStats,"NumClauseLen3+ = %u \n",iNumClauses - aClauseBins[1] - aClauseBins[2]);

    ReportPrint(pRepCNFStats,"FullClauseDistribution = ");
    for (j=0;j<=iMaxClauseLen;j++) {
      if (aClauseBins[j] > 0) {
        ReportPrint1(pRepCNFStats," %u",j);
        ReportPrint1(pRepCNFStats,":%u",aClauseBins[j]);
      }
    }
    ReportPrint(pRepCNFStats,"\n");

    fAvgLitClause = (FLOAT)iNumLits / (FLOAT) iNumClauses;
    ReportPrint1(pRepCNFStats,"MeanClauseLen = %f \n",fAvgLitClause);
    
    fAvgVarOccur = (FLOAT) iNumLits / (FLOAT) iNumVars;
    ReportPrint1(pRepCNFStats,"MeanVariableOcc = %f \n",fAvgVarOccur);

    fStdDevVarOccur = 0.0; 
    for (j=1;j<=iNumVars;j++) {
      fTemp = (aNumLitOcc[GetPosLit(j)] + aNumLitOcc[GetNegLit(j)])-fAvgVarOccur;
      fStdDevVarOccur += (fTemp*fTemp);
    }
    fStdDevVarOccur /= (FLOAT) (iNumVars-1);
    fStdDevVarOccur = sqrt(fStdDevVarOccur);
    
    ReportPrint1(pRepCNFStats,"StdDevVariableOcc = %f \n",fStdDevVarOccur);

    iNumPos = 0;
    iNumNeg = 0;
    for (j=1;j<=iNumVars;j++) {
      iNumPos += aNumLitOcc[GetPosLit(j)];
      iNumNeg += aNumLitOcc[GetNegLit(j)];
    }
    fPosNegRatio = (FLOAT) iNumPos / (FLOAT) iNumNeg;
    ReportPrint1(pRepCNFStats,"NumPosLit = %u \n",iNumPos);
    ReportPrint1(pRepCNFStats,"NumNegLit = %u \n",iNumNeg);
    ReportPrint1(pRepCNFStats,"RatioPos:NegLit = %f \n",fPosNegRatio);
  }
}



/***** Report -r state *****/

void ReportStatePrint() {
  UINT32 j;
  UINT32 bLocalMin = FALSE;
  UINT32 bPrint;
  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepState);
    ReportHdrPrint(pRepState," Run ID | Step No. | Num False (or sum of false weights) | VarFlip | IsLocalMin | vararray\n");
    if (fReportStateQuality < 0) {
      bReportStateQuality = FALSE;
    } else {
      bReportStateQuality = TRUE;
      if (bWeighted==FALSE) {
        iReportStateQuality = (UINT32) fReportStateQuality;
      }
    }
  }

  bPrint = TRUE;
  if (bReportStateQuality) {
   if (bWeighted) {
     if (fSumFalseW != fReportStateQuality) {
       bPrint = FALSE;
     }
   } else {
     if (iNumFalse != iReportStateQuality) {
       bPrint = FALSE;
     }
   }
  }

  if ((bPrint) && (bReportStateLMOnly)) {
    bLocalMin = IsLocalMinimum(bWeighted);
    if (!bLocalMin) {
      bPrint = FALSE;
    }
  }

  if (bPrint) {
    ReportPrint1(pRepState,"%u ",iRun);
    ReportPrint1(pRepState,"%u ",iStep);

    if (bWeighted) {
      ReportPrint1(pRepState,"%f ",fSumFalseW);
    } else {
      ReportPrint1(pRepState,"%u ",iNumFalse);
    }

    ReportPrint1(pRepState,"%u ",iFlipCandidate);
    if (bReportStateLMOnly) {
      ReportPrint1(pRepState,"%u ",bLocalMin);
    } else {
      ReportPrint1(pRepState,"%u ",IsLocalMinimum(bWeighted));
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
  ReportPrint2(pRepBestSol,"%u %u ",iRun, bSolutionFound);
  if (bWeighted) {
    ReportPrint1(pRepBestSol,"%f ",fBestSumFalseW);
  } else {
    ReportPrint1(pRepBestSol,"%u ",iBestNumFalse);
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
    if (iBestStepSumFalseW == iStep) {
      ReportPrint3(pRepBestStep,"%u %u %f ",iRun, iStep, fBestSumFalseW);
      if (bReportBestStepVars) {
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
      ReportPrint3(pRepBestStep,"%u %u %u ",iRun, iStep, iBestNumFalse);
      if (bReportBestStepVars) {
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
      if (iBestStepSumFalseW==(iStep-1)) {
        ReportPrint3(pRepTrajBestLM,"%u %u %f\n",iRun, iStep-1, fBestSumFalseW);

      }
    } else {
      if (iBestStepNumFalse==(iStep-1)) {
        ReportPrint3(pRepTrajBestLM,"%u %u %u\n",iRun, iStep-1, iBestNumFalse);
      }
    }
  }
}


void ReportTrajBestLMPostRun() {

  if (bWeighted) {
    if (iBestStepSumFalseW==(iStep)) {
      ReportPrint3(pRepTrajBestLM,"%u %u %f\n",iRun, iStep, fBestSumFalseW);

    }
  } else {
    if (iBestStepNumFalse==(iStep)) {
      ReportPrint3(pRepTrajBestLM,"%u %u %u\n",iRun, iStep, iBestNumFalse);
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
    ReportPrint1(pRepSolution,"%u ",iRun);
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
  if ((bSolutionFound)||(bReportOptClausesSol==FALSE)) {
    ReportPrint1(pRepOptClauses,"%u ",iRun);
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



/***** Report -r flipcount *****/

void ReportFlipCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepFlipCounts);
    ReportHdrPrint(pRepFlipCounts," Run ID | FlipCount[0] (NullFlips) | FlipCount[1] | FlipCount[2]...\n");
  }
  ReportPrint1(pRepFlipCounts,"%u",iRun);
  for (j=0;j<=iNumVars;j++) {
    ReportPrint1(pRepFlipCounts," %u",aFlipCounts[j]);
  }
  ReportPrint(pRepFlipCounts,"\n");
}



/***** Report -r unsatcount *****/

void ReportUnsatCountsPrint() {
  UINT32 j;
  if (iRun==1) {
    ReportHdrPrefix(pRepUnsatCounts);
    ReportHdrPrint(pRepUnsatCounts," Run ID | UnsatCount[1] | UnsatCount[2] ...\n");
  }
  ReportPrint1(pRepUnsatCounts,"%u",iRun);
  for (j=0;j<iNumClauses;j++) {
    ReportPrint1(pRepUnsatCounts," %u",aUnsatCounts[j]);
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

  ReportPrint1(pRepVarLast,"%u",iRun);
  for (j=1;j<=iNumVars;j++) {
    ReportPrint1(pRepVarLast," %u",aVarLastChange[j]);
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
  ReportPrint1(pRepClauseLast,"%u",iRun);
  for (j=0;j<iNumClauses;j++) {
    ReportPrint1(pRepClauseLast," %u",aClauseLast[j]);
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
        ReportHdrPrint1(pRepSQGrid," %u",aLogDistValues[j]);
      }
      ReportHdrPrint(pRepSQGrid,"\n");
    }

    ReportPrint1(pRepSQGrid,"%u",iRun);

    if (bWeighted) {
      for (j=0;j<iNumLogDistValues;j++) {
        ReportPrint1(pRepSQGrid," %f",aSQGridW[iNumLogDistValues * (iRun-1) + j]);
      }
    } else {
      for (j=0;j<iNumLogDistValues;j++) {
        ReportPrint1(pRepSQGrid," %u",aSQGrid[iNumLogDistValues * (iRun-1) + j]);
      }
    }
    ReportPrint(pRepSQGrid,"\n");
  }
}



/***** Report -r penalty *****/

void ReportPenaltyCreate() {

  aPenaltyStatsFinal = AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsFinalSum = AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsFinalSum2 = AllocateRAM(iNumClauses*sizeof(FLOAT));
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
      ReportPrint3(pRepPenalty,"%u %u %u",iRun,iStep,iNumNullFlips);
      if (bClausePenaltyFLOAT) {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            ReportPrint1(pRepPenalty," %12.10f",aClausePenaltyFL[j] / fTotalPenaltyFL);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty," %f",aClausePenaltyFL[j] / fBasePenaltyFL);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty," %f",aClausePenaltyFL[j]);
            }
          }
        }
      } else {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            ReportPrint1(pRepPenalty," %12.10f",((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iTotalPenaltyINT);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty," %f",((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iBasePenaltyINT);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              ReportPrint1(pRepPenalty," %u",aClausePenaltyINT[j]);
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

      ReportPrint3(pRepPenalty,"%u %u %u ",iRun, iStep, bSolutionFound);
      if (bWeighted) {
        ReportPrint1(pRepPenalty,"%12f",fBestSumFalseW);
      } else {
        ReportPrint1(pRepPenalty,"%u",iBestNumFalse);
      }
      if (bClausePenaltyFLOAT) {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            aPenaltyStatsFinal[j] = aClausePenaltyFL[j] / fTotalPenaltyFL;
            ReportPrint1(pRepPenalty," %12.10f",aPenaltyStatsFinal[j]);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = aClausePenaltyFL[j] / fBasePenaltyFL;
              ReportPrint1(pRepPenalty," %f",aPenaltyStatsFinal[j]);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = aClausePenaltyFL[j];
              ReportPrint1(pRepPenalty," %f",aPenaltyStatsFinal[j]);
            }
          }
        }
      } else {
        if (bReportPenaltyReNormFraction) {
          for (j=0;j<iNumClauses;j++) {
            aPenaltyStatsFinal[j] = ((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iTotalPenaltyINT;
            ReportPrint1(pRepPenalty," %12.10f",aPenaltyStatsFinal[j]);
          }
        } else {
          if (bReportPenaltyReNormBase) {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = ((FLOAT) aClausePenaltyINT[j]) / (FLOAT) iBasePenaltyINT;
              ReportPrint1(pRepPenalty," %f",aPenaltyStatsFinal[j]);
            }
          } else {
            for (j=0;j<iNumClauses;j++) {
              aPenaltyStatsFinal[j] = (FLOAT) aClausePenaltyINT[j];
              ReportPrint1(pRepPenalty," %u",aClausePenaltyINT[j]);
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
          if (bReportPenaltyReNormFraction) {
            ReportPrint1(pRepPenalty," %12.10f",fMean);
          } else {
            ReportPrint1(pRepPenalty," %f",fMean);
          }
        }
        ReportPrint(pRepPenalty,"\n");

        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"Stddev");
        for (j=0;j<iNumClauses;j++) {
          CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsFinalSum[j],aPenaltyStatsFinalSum2[j],iRun);
          if (bReportPenaltyReNormFraction) {
            ReportPrint1(pRepPenalty," %12.10f",fStddev);
          } else {
            ReportPrint1(pRepPenalty," %f",fStddev);
          }
          ReportPrint1(pRepPenalty," %f",fStddev);
        }
        ReportPrint(pRepPenalty,"\n");

        ReportHdrPrefix(pRepPenalty);
        ReportHdrPrint(pRepPenalty,"CV");
        for (j=0;j<iNumClauses;j++) {
          CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsFinalSum[j],aPenaltyStatsFinalSum2[j],iRun);
          ReportPrint1(pRepPenalty," %f",fCV);
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
    ReportPrint1(pRepPenMean,"%u",iRun);
    for (j=0;j<iNumClauses;j++) {
      ReportPrint1(pRepPenMean," %f",aPenaltyStatsMean[j]);
    }
    ReportPrint(pRepPenMean,"\n");

    if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"Mean");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsMeanSum[j],aPenaltyStatsMeanSum2[j],iRun);
        ReportPrint1(pRepPenMean," %f",fMean);
      }
      ReportPrint(pRepPenMean,"\n");

      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"Stddev");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsMeanSum[j],aPenaltyStatsMeanSum2[j],iRun);
        ReportPrint1(pRepPenMean," %f",fStddev);
      }
      ReportPrint(pRepPenMean,"\n");

      ReportHdrPrefix(pRepPenMean);
      ReportHdrPrint(pRepPenMean,"CV");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsMeanSum[j],aPenaltyStatsMeanSum2[j],iRun);
        ReportPrint1(pRepPenMean," %f",fCV);
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
    ReportPrint1(pRepPenStddev,"%u",iRun);
    for (j=0;j<iNumClauses;j++) {
      ReportPrint1(pRepPenStddev," %f",aPenaltyStatsStddev[j]);
    }
    ReportPrint(pRepPenStddev,"\n");

    if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"Mean");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsStddevSum[j],aPenaltyStatsStddevSum2[j],iRun);
        ReportPrint1(pRepPenStddev," %f",fMean);
      }
      ReportPrint(pRepPenStddev,"\n");

      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"Stddev");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsStddevSum[j],aPenaltyStatsStddevSum2[j],iRun);
        ReportPrint1(pRepPenStddev," %f",fStddev);
      }
      ReportPrint(pRepPenStddev,"\n");

      ReportHdrPrefix(pRepPenStddev);
      ReportHdrPrint(pRepPenStddev,"CV");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsStddevSum[j],aPenaltyStatsStddevSum2[j],iRun);
        ReportPrint1(pRepPenStddev," %f",fCV);
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
    ReportPrint1(pRepPenCV,"%u",iRun);
    for (j=0;j<iNumClauses;j++) {
      ReportPrint1(pRepPenCV," %f",aPenaltyStatsCV[j]);
    }
    ReportPrint(pRepPenCV,"\n");

    if ((iRun == iNumRuns) || (bTerminateAllRuns)) {
      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"Mean");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsCVSum[j],aPenaltyStatsCVSum2[j],iRun);
        ReportPrint1(pRepPenCV," %f",fMean);
      }
      ReportPrint(pRepPenCV,"\n");

      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"Stddev");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsCVSum[j],aPenaltyStatsCVSum2[j],iRun);
        ReportPrint1(pRepPenCV," %f",fStddev);
      }
      ReportPrint(pRepPenCV,"\n");

      ReportHdrPrefix(pRepPenCV);
      ReportHdrPrint(pRepPenCV,"CV");
      for (j=0;j<iNumClauses;j++) {
        CalculateStats(&fMean,&fStddev,&fCV,aPenaltyStatsCVSum[j],aPenaltyStatsCVSum2[j],iRun);
        ReportPrint1(pRepPenCV," %f",fCV);
      }
      ReportPrint(pRepPenCV,"\n");
    }
  }
}



/***** report -r mobility *****/

void ReportMobilityPrint() {

  UINT32 j;
  FLOAT fMean;

  if (iRun==1) {
    ReportHdrPrefix(pRepMobility);
    ReportHdrPrint(pRepMobility," Run ID | AvgMobilityWindow[1] AvgMobilityWindow[2] ...\n");
  }

  ReportPrint1(pRepMobility,"%u",iRun);
  for (j=1;j<=iReportMobilityDisplay;j++) {
    fMean = aMobilityWindowSum[j] / (iStep - j);
    if (bReportMobilityNormalized) {
      if (j <= iNumVars) {
        fMean /= (FLOAT) j;
      } else {
        fMean /= (FLOAT) iNumVars;
      }
    }
    ReportPrint1(pRepMobility," %f",fMean);
  }
  ReportPrint(pRepMobility,"\n");
}



/***** report -r mobfixed *****/

void ReportMobFixedPrint() {

  if ((iRun==1)&&(iStep==1)) {
    ReportHdrPrefix(pRepMobFixed);
    ReportHdrPrint1(pRepMobFixed," Run ID | Step # | Mobility value window size %u\n",iMobFixedWindow);
  }

  if (iStep <= iMobFixedWindow) {
    if (bMobilityFixedIncludeStart) {
      ReportPrint3(pRepMobFixed,"%u %u %u\n",iRun,iStep,aMobilityWindow[iStep-1]);
    }
  } else {
    ReportPrint3(pRepMobFixed,"%u %u %u\n",iRun,iStep,aMobilityWindow[iMobFixedWindow]);
  }
}


/***** report -r mobfixedfreq *****/

void ReportMobFixedFreqPrint() {

  unsigned int j;

  FLOAT fCurrent;
  FLOAT fCumulative = FLOATZERO;
  FLOAT iStepDiv;


  if (bMobilityFixedIncludeStart) {
    if (iRun==1) {
      ReportHdrPrefix(pRepMobFixedFreq);
      ReportHdrPrint1(pRepMobFixedFreq," Run ID | Count | Frequency of Count for mobility window size %u | Normalized value ( / # steps) | Cumulative Normalized value \n",iMobFixedWindow);
    }
    
    iStepDiv = 1.0f / (FLOAT) iStep;      

    if (iStep > 0) {
      for (j=0;j<=iMobFixedWindow;j++) {

        fCurrent = ((FLOAT)aMobilityFixedFrequencies[j]) * iStepDiv;
        fCumulative += fCurrent;

        ReportPrint2(pRepMobFixedFreq,"%u %u",iRun,j);
        ReportPrint1(pRepMobFixedFreq," %u",aMobilityFixedFrequencies[j]);
        ReportPrint1(pRepMobFixedFreq," %g",fCurrent);
        ReportPrint1(pRepMobFixedFreq," %g\n",fCumulative);
      }
    }
  } else {
    if (iRun==1) {
      ReportHdrPrefix(pRepMobFixedFreq);
      ReportHdrPrint1(pRepMobFixedFreq," Run ID | Count | Frequency of Count for mobility window size %u | Normalized value [ / (# steps-window size)] | Cumulative Normalized value \n",iMobFixedWindow);
    }

    if (iStep > iMobFixedWindow) {
      iStepDiv = 1.0f / (FLOAT) (iStep - iMobFixedWindow);
    } else {
      iStepDiv = 1.0f / (FLOAT) iStep;      
    }

    for (j=0;j<=iMobFixedWindow;j++) {

      fCurrent = ((FLOAT)aMobilityFixedFrequencies[j]) * iStepDiv;
      fCumulative += fCurrent;

      ReportPrint2(pRepMobFixedFreq,"%u %u",iRun,j);
      ReportPrint1(pRepMobFixedFreq," %u",aMobilityFixedFrequencies[j]);
      ReportPrint1(pRepMobFixedFreq," %g",fCurrent);
      ReportPrint1(pRepMobFixedFreq," %g\n",fCumulative);
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

  ReportPrint1(pRepAutoCorr,"%u",iRun);
  ReportPrint1(pRepAutoCorr," %u",iAutoCorrLen);
  for (j=1;j<iAutoCorrMaxLen;j++) {
    ReportPrint1(pRepAutoCorr," %f",aAutoCorrValues[j]);
  }
  ReportPrint(pRepAutoCorr,"\n");
}

/***** Report -r satcomp *****/

void ReportSatCompetitionPrint() {
  UINT32 j;

  SetString(&sCommentString,"c");

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
        ReportPrint1(pRepSATComp," -%u",j);
      } else {
        ReportPrint1(pRepSATComp," %u",j);
      }
      if (j % 10 == 0)
        ReportPrint(pRepSATComp,"\nv ");
    }
    ReportPrint(pRepSATComp," 0\n");
    exit(10);
  } else {
    ReportPrint(pRepSATComp,"s UNKNOWN\n");
    exit(0);
  }
}



/***** stat "percentsolve" *****/

void CalcPercentSolve() {
  fPercentSuccess = 100.0 * (FLOAT) iNumSolutionsFound / (FLOAT) iRun;
}



/***** Trigger ActivateStepsColumn *****/

void ActivateStepsColumn() {
  ActivateColumns("steps");
  AddAllocateRAMColumnID(FindItem(&listColumns,"steps"),"");
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
          pCol->puiColumnData = AllocateRAM(iNumRuns * sizeof(UINT32));
          memset(pCol->puiColumnData,0,(iNumRuns)*sizeof(UINT32));
          break;
        case DTypeSInt:
          pCol->psiColumnData = AllocateRAM(iNumRuns * sizeof(SINT32));
          memset(pCol->psiColumnData,0,(iNumRuns)*sizeof(SINT32));
          break;
        case DTypeFloat:
          pCol->pfColumnData = AllocateRAM(iNumRuns * sizeof(FLOAT));
          memset(pCol->pfColumnData,0,(iNumRuns)*sizeof(FLOAT));
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
    } else {
      pCol->fMinMaxVal = FLOATMAX;
      pCol->uiMinMaxVal = UINT32MAX;
      pCol->siMinMaxVal = SINT32MAX;
    }
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
      switch(pCol->eColType)
      {
        case ColTypeFinal:
          switch(pCol->eSourceDataType)
          {
          case DTypeUInt:
            pCol->uiCurRowValue = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            pCol->siCurRowValue = *pCol->psiCurValue;
            break;
          case DTypeFloat:
            pCol->fCurRowValue = *pCol->pfCurValue;
            break;
          }
          break;
        case ColTypeMin:
        case ColTypeMax:
          switch(pCol->eSourceDataType)
          {
          case DTypeUInt:
            pCol->uiCurRowValue = pCol->uiMinMaxVal;
            break;
          case DTypeSInt:
            pCol->siCurRowValue = pCol->siMinMaxVal;
            break;
          case DTypeFloat:
            pCol->fCurRowValue = pCol->fMinMaxVal;
            break;
          }
          break;
        case ColTypeFinalDivStep:
        case ColTypeFinalDivStep100:
          switch(pCol->eSourceDataType)
          {
          case DTypeUInt:
            pCol->fCurRowValue = (FLOAT) *pCol->puiCurValue;
            break;
          case DTypeSInt:
            pCol->fCurRowValue = (FLOAT) *pCol->psiCurValue;
            break;
          case DTypeFloat:
            pCol->fCurRowValue = *pCol->pfCurValue;
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
      }
      if (pCol->bAllocateColumnRAM) {
        if (pCol->eFinalDataType == DTypeUInt) {
          pCol->puiColumnData[iRun-1] = pCol->uiCurRowValue;
        } else if (pCol->eFinalDataType == DTypeSInt) {
          pCol->psiColumnData[iRun-1] = pCol->siCurRowValue;
        } else {
          pCol->pfColumnData[iRun-1] = pCol->fCurRowValue;
        }
      }

      if (pCol->eFinalDataType == DTypeUInt) {
        pCol->fCurRowValue = (FLOAT) pCol->uiCurRowValue;
      } else if (pCol->eFinalDataType == DTypeSInt) {
        pCol->fCurRowValue = (FLOAT) pCol->siCurRowValue;
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
    switch(pCol->eColType)
    {
      case ColTypeStddev:
      case ColTypeCV:
        switch(pCol->eSourceDataType)
        {
        case DTypeUInt:
          pCol->fRowSum2 += ((FLOAT) *pCol->puiCurValue) * ((FLOAT) *pCol->puiCurValue);
          break;
        case DTypeSInt:
          pCol->fRowSum2 += ((FLOAT) *pCol->psiCurValue) * ((FLOAT) *pCol->psiCurValue);
          break;
        case DTypeFloat:
          pCol->fRowSum2 += ((*pCol->pfCurValue) * (*pCol->pfCurValue));
          break;
        }
      case ColTypeMean:
        switch(pCol->eSourceDataType)
        {
        case DTypeUInt:
          pCol->fRowSum += (FLOAT) *pCol->puiCurValue;
          break;
        case DTypeSInt:
          pCol->fRowSum += (FLOAT) *pCol->psiCurValue;
          break;
        case DTypeFloat:
          pCol->fRowSum += *pCol->pfCurValue;
          break;
        }
        break;

      case ColTypeMin:
        switch(pCol->eSourceDataType)
        {
          case DTypeUInt:
            if (*pCol->puiCurValue < pCol->uiMinMaxVal) pCol->uiMinMaxVal = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            if (*pCol->psiCurValue < pCol->siMinMaxVal) pCol->siMinMaxVal = *pCol->psiCurValue;
            break;
          case DTypeFloat:
            if (*pCol->pfCurValue < pCol->fMinMaxVal) pCol->fMinMaxVal = *pCol->pfCurValue;
            break;
        }
        break;

      case ColTypeMax:
        switch(pCol->eSourceDataType)
        {
          case DTypeUInt:
            if (*pCol->puiCurValue > pCol->uiMinMaxVal) pCol->uiMinMaxVal = *pCol->puiCurValue;
            break;
          case DTypeSInt:
            if (*pCol->psiCurValue > pCol->siMinMaxVal) pCol->siMinMaxVal = *pCol->psiCurValue;
            break;
          case DTypeFloat:
            if (*pCol->pfCurValue > pCol->fMinMaxVal) pCol->fMinMaxVal = *pCol->pfCurValue;
            break;
        }
        break;
    }
  }
}



/***** Trigger CalcFPS *****/

void CalcFPS() {
  UINT32 j;
  REPORTCOL *pCol;

  pCol = &aColumns[FindItem(&listColumns,"steps")];
  fFlipsPerSecond = 0.0;
  for (j=0;j< iRun;j++) {
    fFlipsPerSecond += GetRowElement(pCol,j,FALSE,FALSE);
  }
  fFlipsPerSecond /= fTotalTime;
}


/***** Trigger StringAlgParms *****/
void StringAlgParms() {

  UINT32 j;
  char * pNext;
  ALGPARM *pCurParm;
  
  sStringAlgParms = AllocateRAM(MAXPARMLINELEN);
  
  pNext = sStringAlgParms;
  pNext += sprintf(pNext,"-alg %s",pActiveAlgorithm->sName);
  if (*(pActiveAlgorithm->sVariant))
    pNext += sprintf(pNext," -v %s",pActiveAlgorithm->sVariant);
  if (pActiveAlgorithm->bWeighted)
    pNext += sprintf(pNext," -w");
  
  for (j=0;j<pActiveAlgorithm->parmList.iNumParms;j++) {
    pCurParm = &pActiveAlgorithm->parmList.aParms[j];
    pNext += sprintf(pNext," %s ",pCurParm->sSwitch);
    switch(pCurParm->eType)
    {
      case PTypeUInt:
        pNext += sprintf(pNext,"%u ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeSInt:
        pNext += sprintf(pNext,"%d ", *(int *)pCurParm->pParmValue);
        break;
      case PTypeProbability:
        pNext += sprintf(pNext,"%f ", ProbToFloat(*(PROBABILITY *)pCurParm->pParmValue));
        break;
      case PTypeString:
        if (**(char **)pCurParm->pParmValue ==0) {
          pNext += sprintf(pNext,"%s ","[null]");
        } else {
          pNext += sprintf(pNext,"%s ", *(char **)pCurParm->pParmValue);
        }
        break;
      case PTypeFloat:
        pNext += sprintf(pNext,"%f ", *(FLOAT *)pCurParm->pParmValue);
        break;
      case PTypeBool:
        pNext += sprintf(pNext,"%u ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeReport:
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
    pCol->pfColumnData[aSortedBySteps[j]] = ((FLOAT) (j+1))/((FLOAT)iRun);
  }

}



/***** Trigger UpdateTimes *****/

void UpdateTimes() {
  
  UINT32 j;
  REPORTCOL *pColTimes;
  REPORTCOL *pColSteps;
  FLOAT *aTimes;
  UINT32 *aSteps;

  FLOAT fTotalSteps = 0.0;

  pColTimes = &aColumns[FindItem(&listColumns,"timesteps")];

  if (pColTimes->bAllocateColumnRAM == FALSE) {
    return; /* may not be an error if column requested in -r out but not -r rtd */
  }
  
  pColSteps = &aColumns[FindItem(&listColumns,"steps")];

  aSteps = pColSteps->puiColumnData;
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
  bMobilityColNActive = TRUE;
}

/***** Trigger InitMobilityColumnX *****/

void InitMobilityColumnX() {
  bMobilityColXActive = TRUE;
}

void UpdateMobilityColumn() {

  UINT32 iCount = iNumVars;
  FLOAT fStdDev;
  
  if (bMobilityColNActive) {

    if (iStep <= iNumVars) {
      iCount = iStep;
    } else {
      if (bMobilityFixedIncludeStart==FALSE) {
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
      if (bMobilityFixedIncludeStart==FALSE) {
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
UINT32 *auiSortColData;
SINT32 *asiSortColData;
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

int CompareSortedSInt(const void *a, const void *b) {
  return (asiSortColData[*(UINT32 *)a] - asiSortColData[*(UINT32 *)b]);
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
    aSortedByCurrent = AllocateRAM(sizeof(UINT32) * (iRun));
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
  } else {
    afSortColData = pSortCol->pfColumnData;
    qsort((void *)aSortedByCurrent,iRun,sizeof(UINT32),CompareSortedFloat);
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

  aSortedBySteps = AllocateRAM(sizeof(UINT32) * (iRun));

  for (j=0;j<iRun;j++) {
    aSortedBySteps[j] = aSortedByCurrent[j];
  }

  bSortedByStepsValid = TRUE;

}
