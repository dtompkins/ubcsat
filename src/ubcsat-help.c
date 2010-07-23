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
  This file contains routines for displaying help 
*/

BOOL bShowHelp;
BOOL bShowHelpA;
BOOL bShowHelpW;
BOOL bShowHelpP;
BOOL bShowHelpV;
BOOL bShowHelpT;
BOOL bShowHelpR;
BOOL bShowHelpC;
BOOL bShowHelpS;

void HelpNoAlgorithm();
void HelpBadReport();
void HelpPrintAlgorithms();
void HelpPrintAlgorithmsW();
void HelpPrintParms();
void HelpShowBasic();
void HelpShowVerbose();
void HelpShowTerse();
void HelpPrintReports();
void HelpPrintColumns();
void HelpPrintStats();
void HelpPrintParameter(ALGPARM *pCurParm, BOOL bAlgOffset);
void HelpPrintSpecialParameters();
void HelpPrintAlgorithm(ALGORITHM *pAlg, BOOL bTerse, BOOL bVerbose);
void HelpPrintReport(REPORT *pRep);

void CheckPrintHelp() {
  if (bShowHelp)  HelpShowBasic();
  if (bShowHelpP) HelpPrintParms();
  if (bShowHelpA) HelpPrintAlgorithms();
  if (bShowHelpW) HelpPrintAlgorithmsW();
  if (bShowHelpR) HelpPrintReports();
  if (bShowHelpC) HelpPrintColumns();
  if (bShowHelpS) HelpPrintStats();
  if (bShowHelpV) HelpShowVerbose();
  if (bShowHelpT) HelpShowTerse();

  if ((bShowHelp)||(bShowHelpP)||(bShowHelpA)||(bShowHelpW)||(bShowHelpV)||(bShowHelpT)||(bShowHelpR)||(bShowHelpC)||(bShowHelpS)) {
    AbnormalExit();
  }

  if (!pActiveAlgorithm) HelpNoAlgorithm();
}

BOOL bHelpHeaderShown;

void HelpShowHeader() {
  if (bHelpHeaderShown) {
    return;
  } else {
    ReportPrint1(pRepHelp,"\nUBCSAT version %s\n\n",sVersion);
    ReportPrint(pRepHelp,"http://www.satlib.org/ubcsat\n\n");
    bHelpHeaderShown = TRUE;
  }
}

void HelpShowBasic() {

  HelpShowHeader();
  
  ReportPrint(pRepHelp,"GENERAL HELP\n");
  ReportPrint(pRepHelp,"============\n\n");

  ReportPrint(pRepHelp,"UBCSAT is a collection of Stochastic Local Search (SLS) algorithms for solving\n");
  ReportPrint(pRepHelp,"Satisfiability (SAT) instances, and a tool for analyzing the behaviour of those\n");
  ReportPrint(pRepHelp,"SLS algorithms with numerous reports and statistics.\n\n");

  ReportPrint(pRepHelp,"UBCSAT always requires two key command-line parameters:\n");
  ReportPrint(pRepHelp,"  1) an algorithm to use, specified with the [-alg] parameter, and\n");
  ReportPrint(pRepHelp,"  2) a SAT instance file to solve, specified with [-i] parameter\n\n");

  ReportPrint(pRepHelp,"For example, to use the SAPS algorithm to solve the instance file sample.cnf\n\n");
  ReportPrint(pRepHelp,"  >ubcsat -alg saps -i sample.cnf \n\n");

  ReportPrint(pRepHelp,"If your primary goal is to find a solution, use the [-solve] parameter\n\n");
  ReportPrint(pRepHelp,"  >ubcsat -alg saps -i sample.cnf -solve\n\n");
  
  ReportPrint(pRepHelp,"To analyze the behaviour of an SLS algorithm, you must run it several times.\n");  
  ReportPrint(pRepHelp,"To run the same algorithm 100 times with a maximum of a million steps per run:\n\n");
  ReportPrint(pRepHelp,"  >ubcsat -alg saps -i sample.cnf -runs 100 -cutoff 1000000\n\n");

  ReportPrint(pRepHelp,"For additional help, consult one of the following:\n\n");
  ReportPrint(pRepHelp,"  ubcsat -hp    list all of the [p]arameters\n");
  ReportPrint(pRepHelp,"  ubcsat -ha    list the available [a]lgorithms\n");
  ReportPrint(pRepHelp,"  ubcsat -hw    list the available [w]eighted algorithms\n");
  ReportPrint(pRepHelp,"  ubcsat -hr    list the available [r]eports\n");
  ReportPrint(pRepHelp,"  ubcsat -hc    For help with the [c]olumns of the default output report\n");
  ReportPrint(pRepHelp,"  ubcsat -hs    For help with the [s]tatistics report\n\n");

  ReportPrint(pRepHelp,"Consult the website and email ubcsat-help [@] cs.ubc.ca for support\n\n\n");

}

void HelpPrintParms() {

  HelpShowHeader();

  ReportPrint(pRepHelp,"UBCSAT PARAMETERS\n");
  ReportPrint(pRepHelp,"=================\n\n");

  ReportPrint(pRepHelp,"UBCSAT uses a command-line [-parameter value] interface for all parameters.\n");
  ReportPrint(pRepHelp,"If you prefer, you can use a parameter file. (see [-param,-fp] below)\n");
  ReportPrint(pRepHelp,"All parameters (and algorithms) are specified in lower-case.\n\n");

  ReportPrint(pRepHelp,"The syntax of the parameter depends on the type of the parameter:\n\n");

  ReportPrint(pRepHelp,"-parameter          BOOLEAN - turns a feature on (no value required)\n\n");

  ReportPrint(pRepHelp,"-parameter INT      INTEGER - integer value \n\n");

  ReportPrint(pRepHelp,"-parameter FL       FLOAT - float value\n\n");

  ReportPrint(pRepHelp,"-parameter STR      STRING - string (text) value\n\n");

  ReportPrint(pRepHelp,"-parameter PR       PROBABILITY - a float in the range 0.0 .. 1.0\n");
  ReportPrint(pRepHelp,"                                  or a float in the range 1.0 .. 100.0 (%%)\n");
  ReportPrint(pRepHelp,"                                  or 2 ints as a ratio (e.g.: 1 3 for 1/3)\n\n");

  ReportPrint(pRepHelp,"For INT and FL parameters, you can add a trailing \"n\", to specify the\n");
  ReportPrint(pRepHelp,"value as a multiple of the number of variables in the instance.\n");
  ReportPrint(pRepHelp,"  e.g.: -parameter 0.6n\n\n");
  
  HelpPrintSpecialParameters();
  
  if (pActiveAlgorithm) {
    HelpPrintAlgorithm(pActiveAlgorithm,FALSE,TRUE);
  }

}

void HelpPrintAlgorithms() {
  UINT32 j;

  HelpShowHeader();

  ReportPrint(pRepHelp,"SUPPORTED ALGORITHMS: (unweighted)\n");
  ReportPrint(pRepHelp,"==================================\n\n");
  ReportPrint(pRepHelp,"see http://www.satlib.org/ubcsat/algorithms for additional info\n\n");

  for (j=0;j<iNumAlg;j++) {
    if (aAlgorithms[j].bWeighted == FALSE) {
      HelpPrintAlgorithm(&aAlgorithms[j],FALSE,FALSE);
    }
  }
}


void HelpPrintAlgorithmsW() {
  UINT32 j;

  HelpShowHeader();

  ReportPrint(pRepHelp,"SUPPORTED ALGORITHMS: (weighted)\n");
  ReportPrint(pRepHelp,"================================\n\n");
  ReportPrint(pRepHelp,"see http://www.satlib.org/ubcsat/algorithms for additional info\n\n");

  for (j=0;j<iNumAlg;j++) {
    if (aAlgorithms[j].bWeighted) {
      HelpPrintAlgorithm(&aAlgorithms[j],FALSE,FALSE);
    }
  }
}


void HelpPrintReports() {
  UINT32 j;

  HelpShowHeader();

  ReportPrint(pRepHelp,"UBCSAT REPORTS:\n");
  ReportPrint(pRepHelp,"==============\n\n");

  ReportPrint(pRepHelp,"To activate a report use the following command-line syntax:\n\n");
  ReportPrint(pRepHelp,"  -r reportname filename parameter1 parameter2 ... \n\n");
  
  ReportPrint(pRepHelp,"Instead of printing to a file, you can use one of the special keywords:\n");
  ReportPrint(pRepHelp,"  stdout (default) - print report to standard output (console)\n");
  ReportPrint(pRepHelp,"  stderr           - print report to stderr (usually the console)\n");
  ReportPrint(pRepHelp,"  null             - do not print the report and deactivate the report\n\n");
  ReportPrint(pRepHelp,"Reports (out) and (stats) are activated by default: use \"null\" to deactivate\n\n");

  ReportPrint(pRepHelp,"AVAILABLE REPORTS:  [default parameters in square brackets]\n");
  ReportPrint(pRepHelp,"=================\n\n");
  
  for (j=0;j<iNumReports;j++) {

    if ((&aReports[j] != pRepHelp)) {
      HelpPrintReport(&aReports[j]);
    }
  }

  ReportPrint(pRepHelp,"\n");

}

void HelpPrintColumns() {
  UINT32 j;

  HelpShowHeader();

  ReportPrint(pRepHelp,"UBCSAT OUTPUT COLUMNS:\n");
  ReportPrint(pRepHelp,"=====================\n\n");

  ReportPrint(pRepHelp,"For Output & RTD reports, you can customize the report by specifying\n");
  ReportPrint(pRepHelp,"which columns you wish to display.  For example:\n\n");

  ReportPrint(pRepHelp,"  -r out stdout found,beststep\n\n");

  ReportPrint(pRepHelp,"displays only the output columns found & beststep to the console (stdout) \n");
  ReportPrint(pRepHelp,"if you wish to write the output to a file, specify a filename instead of stdout\n\n");

  ReportPrint(pRepHelp,"Output (-r out) & RTD (-r rtd) Columns available:\n\n");

  for (j=0;j<listColumns.iNumItems;j++) {
    if (!listColumns.aItems[j].bContainer) {
      ReportPrint1(pRepHelp,"%15s -  ",listColumns.aItems[j].sID);
      ReportPrint1(pRepHelp,"%s\n",aColumns[j].sDescription);
    }
  }

  ReportPrint(pRepHelp,"\nContainer Columns available:\n\n");

  for (j=0;j<listColumns.iNumItems;j++) {
    if (listColumns.aItems[j].bContainer) {
      ReportPrint1(pRepHelp,"%15s -  ",listColumns.aItems[j].sID);
      ReportPrint1(pRepHelp,"%s\n",listColumns.aItems[j].sContainerList);
    }
  }

  ReportPrint(pRepHelp,"\n");

}

void HelpPrintStats() {
  UINT32 j;

  HelpShowHeader();

  ReportPrint(pRepHelp,"UBCSAT STATISTICS:\n");
  ReportPrint(pRepHelp,"=================\n\n");

  ReportPrint(pRepHelp,"For the statistics report, you can customize the report by specifying\n");
  ReportPrint(pRepHelp,"which statistics you wish to display.  For example:\n\n");
  ReportPrint(pRepHelp,"  -r stats mystats.stats percentsolve,steps[mean+median+max],numlits\n\n");
  ReportPrint(pRepHelp,"writes to file mystats.stats the following lines:\n\n");
  
  ReportPrint(pRepHelp,"PercentSuccess = x.x\n");
  ReportPrint(pRepHelp,"Steps_Mean = x.x\n");
  ReportPrint(pRepHelp,"Steps_Median = x.x\n");
  ReportPrint(pRepHelp,"Steps_Max = x.x\n");
  ReportPrint(pRepHelp,"TotalLiterals = x\n\n");

  ReportPrint(pRepHelp,"For column statistics, you can specify which fields to display:\n");
  ReportPrint(pRepHelp,"  using the format column[field1+field2+field3...]\n\n");
  ReportPrint(pRepHelp,"Available Fields:   fieldname -  description <Suffix>\n\n");

  ReportPrint(pRepHelp,"         all -  all of the following fields\n");
  ReportPrint(pRepHelp,"        mean -  mean <Mean>\n");
  ReportPrint(pRepHelp,"      stddev -  standard deviation <StdDev>\n");
  ReportPrint(pRepHelp,"          cv -  coefficient of variance: stddev / mean <CoeffVariance>\n");
  ReportPrint(pRepHelp,"         var -  variance: stddev^2 <Variance>\n");
  ReportPrint(pRepHelp,"      stderr -  standard error: stddev / sqrt(count) <StdErr>\n");
  ReportPrint(pRepHelp,"         vmr -  variance to mean ratio: variance / mean <VarMeanRatio>\n");
  ReportPrint(pRepHelp,"         sum -  sum <Sum>\n");
  ReportPrint(pRepHelp,"      median -  median value <Median>\n");
  ReportPrint(pRepHelp,"         min -  minimum <Min>\n");
  ReportPrint(pRepHelp,"         max -  maximum <Max>\n");
  ReportPrint(pRepHelp,"         q05 -  corresponding quantile value <Q.05>\n");
  ReportPrint(pRepHelp,"         q10 -  corresponding quantile value <Q.10>\n");
  ReportPrint(pRepHelp,"         q25 -  corresponding quantile value <Q.25>\n");
  ReportPrint(pRepHelp,"         q75 -  corresponding quantile value <Q.75>\n");
  ReportPrint(pRepHelp,"         q90 -  corresponding quantile value <Q.90>\n");
  ReportPrint(pRepHelp,"         q95 -  corresponding quantile value <Q.95>\n");
  ReportPrint(pRepHelp,"         q98 -  corresponding quantile value <Q.98>\n");
  ReportPrint(pRepHelp,"     qr75/25 -  ratio of the two quantile values <Q.75/25>\n");
  ReportPrint(pRepHelp,"     qr90/10 -  ratio of the two quantile values <Q.90/10>\n");
  ReportPrint(pRepHelp,"     qr95/05 -  ratio of the two quantile values <Q.95/05>\n");
  ReportPrint(pRepHelp,"    stepmean -  mean weighted by the length of the run <StepMean>\n");
  ReportPrint(pRepHelp,"   solvemean -  mean from only successful runs <SuccessMean>\n");
  ReportPrint(pRepHelp,"    failmean -  mean from only unsuccessful runs <FailureMean>\n");
  ReportPrint(pRepHelp," solvemedian -  median from only successful runs <SuccessMedian>\n");
  ReportPrint(pRepHelp,"  failmedian -  median from only unsuccessful runs <FailureMedian>\n");
  ReportPrint(pRepHelp,"    solvemin -  minimum from only successful runs <SuccessMedian>\n");
  ReportPrint(pRepHelp,"     failmin -  minimum from only unsuccessful runs <FailureMedian>\n");
  ReportPrint(pRepHelp,"    solvemax -  maxiumum from only successful runs <SuccessMedian>\n");
  ReportPrint(pRepHelp,"     failmax -  maxiumum from only unsuccessful runs <FailureMedian>\n");
  ReportPrint(pRepHelp,"\n");

  ReportPrint(pRepHelp,"Column Statistics available:   colname - <Prefix> [default field(s)]\n");
  ReportPrint(pRepHelp,"  (see help -hc for specific column information)\n\n");
  for (j=0;j<listStats.iNumItems;j++) {
    if (!listStats.aItems[j].bContainer) {
      if (!aStats[j].bCustomField) {
        ReportPrint3(pRepHelp,"%15s -  <%s> [%s]\n",listStats.aItems[j].sID,aStats[j].sBaseDescription,aStats[j].sStatParms);
      }
    }
  }

  ReportPrint(pRepHelp,"\n\nSingle Statistics available:      statname -  description <Prefix>\n\n");

  for (j=0;j<listStats.iNumItems;j++) {
    if (!listStats.aItems[j].bContainer) {
      if (aStats[j].bCustomField) {
        ReportPrint3(pRepHelp,"%15s -  %s <%s>\n",listStats.aItems[j].sID,aStats[j].sCustomDescription,aStats[j].sBaseDescription);
      }
    }
  }


  ReportPrint(pRepHelp,"\nContainer Statistics available:\n\n");

  for (j=0;j<listStats.iNumItems;j++) {
    if (listStats.aItems[j].bContainer) {
      ReportPrint2(pRepHelp,"%15s -  %s\n",listStats.aItems[j].sID,listStats.aItems[j].sContainerList);
    }
  }

  ReportPrint(pRepHelp,"\n");

}

char sHelpString[HELPSTRINGLENGTH];

void SetHelpStringAlg(ALGORITHM *pAlg) {

  sHelpString[0] = 0;

  strcpy(sHelpString,"-alg ");
  strcat(sHelpString,pAlg->sName);

  if (strlen(pAlg->sVariant)) {
    strcat(sHelpString," -v ");
    strcat(sHelpString,pAlg->sVariant);
  }

  if (pAlg->bWeighted) {
    strcat(sHelpString," -w");
  }
}

void SetHelpStringRep(REPORT *pRep) {

  sHelpString[0] = 0;

  strcpy(sHelpString,"-r ");
  strcat(sHelpString,pRep->sID);
}

void PrintUBCSATHeader(REPORT *pRep) {
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint1(pRep,"UBCSAT version %s\n",sVersion);
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"http://www.satlib.org/ubcsat\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"ubcsat -h for help\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
}

void HelpPrintReport(REPORT *pRep) {
  UINT32 j;
  UINT32 k;
  char *pCur;
  char *pNext;

  SetHelpStringRep(pRep);

  ReportPrint1(pRepHelp,"%s\n",sHelpString);
  for (j=0;j<strlen(sHelpString);j++) {
    ReportPrint(pRepHelp,"=");
  }
  ReportPrint(pRepHelp,"\n");

  if (strlen(pRep->sDescription)) {
    ReportPrint1(pRepHelp,"    %s\n",pRep->sDescription);
  }

  if (strlen(pRep->sVerboseDescription)) {
    ReportPrint(pRepHelp,"\n");
    pCur = pRep->sVerboseDescription;
    pNext = strstr(pCur,"~");
    while (pNext) {
      *pNext = 0;
      ReportPrint(pRepHelp,"    ");
      ReportPrint1(pRepHelp,pCur,sHelpString);
      ReportPrint(pRepHelp,"\n");
      *pNext = '~';
      pCur = ++pNext;
      pNext = strstr(pCur,"~");
    }
    ReportPrint(pRepHelp,"    ");
    ReportPrint1(pRepHelp,pCur,sHelpString);
    ReportPrint(pRepHelp,"\n\n");
  }
  if (pRep->iNumParms) {

    for (k=0;k<pRep->iNumParms;k++) {
      ReportPrint2(pRepHelp,"    Param %2d: %s",k+1,pRep->aParmName[k]);
      switch (pRep->aParmTypes[k]) {
        {
        case PTypeUInt:
          ReportPrint1(pRepHelp," [%u] \n",*(int *)pRep->aParameters[k]);
          break;
        case PTypeFloat:
          ReportPrint1(pRepHelp," [%.6g] \n",*(FLOAT *)pRep->aParameters[k]);
          break;
        case PTypeString:
          ReportPrint1(pRepHelp," [%s] \n",(char *)pRep->aParameters[k]);          
          break;
        }
      }
    }
  }
  ReportPrint(pRepHelp,"\n");

}


void HelpPrintAlgorithm(ALGORITHM *pAlg, BOOL bTerse, BOOL bVerbose) {
  UINT32 j;

  SetHelpStringAlg(pAlg);

  if (bTerse) {
    ReportPrint1(pRepHelp,"%s",sHelpString);
    for (j=0;j<pAlg->parmList.iNumParms;j++) {
      ReportPrint1(pRepHelp," [%s]",pAlg->parmList.aParms[j].sSwitch);
    }
    ReportPrint(pRepHelp,"\n");
  } else {
    ReportPrint1(pRepHelp,"%s\n",sHelpString);
    for (j=0;j<strlen(sHelpString);j++) {
      ReportPrint(pRepHelp,"=");
    }
    ReportPrint(pRepHelp,"\n");

    if (strlen(pAlg->sDescription)) {
      ReportPrint1(pRepHelp,"    %s\n",pAlg->sDescription);
    }
    if (strlen(pAlg->sAuthors)) {
      ReportPrint1(pRepHelp,"    %s\n",pAlg->sAuthors);
    }
    ReportPrint(pRepHelp,"\n");

    if (bVerbose) {
      for (j=0;j<pAlg->parmList.iNumParms;j++) {
        HelpPrintParameter(&pAlg->parmList.aParms[j],TRUE);
      }
    }
  }
}



void HelpNoAlgorithm() {

  PrintUBCSATHeader(pRepErr);
  ReportPrint(pRepErr,"\n");
  ReportPrint(pRepErr,"FATAL ERROR ENCOUNTERED: \n");

  if (*sAlgName) {
    ReportPrint(pRepErr,"   Invalid algorithm specified\n");
  } else {
    ReportPrint(pRepErr,"   UBCSAT requires you to specify an algorithm with -alg\n");
  }
  ReportPrint(pRepErr,"   ubcsat -ha for a list of valid algorithms\n");
  AbnormalExit();
}



void HelpPrintParameter(ALGPARM *pCurParm, BOOL bAlgOffset) {
  
  char *pCur;
  char *pNext;
  UINT32 iNumSpaces;
  UINT32 j;

  switch(pCurParm->eType)
  {
    case PTypeBool:
      sprintf(sHelpString,"%u",pCurParm->defDefault.bBool);
      break;
    case PTypeUInt:
      sprintf(sHelpString,"%u",pCurParm->defDefault.iUInt);
      break;
    case PTypeSInt:
      sprintf(sHelpString,"%d",pCurParm->defDefault.iSInt);
      break;
    case PTypeProbability:
      sprintf(sHelpString,"%.4g",ProbToFloat(pCurParm->defDefault.iProb));
      break;
    case PTypeString:
      sprintf(sHelpString,"");
      break;
    case PTypeFloat:
      sprintf(sHelpString,"%.6g",pCurParm->defDefault.fFloat);
      break;
    case PTypeReport:
      sprintf(sHelpString,"");
      break;
  }  

  if (bAlgOffset) {
    if (pCurParm->eType == PTypeBool) {
      ReportPrint1(pRepHelp,"    %-15s ",pCurParm->sSwitch);
    } else {
      ReportPrint1(pRepHelp,"    %s ",pCurParm->sSwitch);
    }
  } else {
    if (pCurParm->eType == PTypeBool) {
      ReportPrint1(pRepHelp,"%-18s  ",pCurParm->sSwitch);
    } else {
      ReportPrint1(pRepHelp,"%s ",pCurParm->sSwitch);
    }
  }

  switch(pCurParm->eType)
  {
    case PTypeBool:
      break;
    case PTypeUInt:
      ReportPrint(pRepHelp,"INT ");
      break;
    case PTypeSInt:
      ReportPrint(pRepHelp,"INT ");
      break;
    case PTypeProbability:
      ReportPrint(pRepHelp,"PR  ");
      break;
    case PTypeString:
      ReportPrint(pRepHelp,"STR ");
      break;
    case PTypeFloat:
      ReportPrint(pRepHelp,"FL  ");
      break;
    case PTypeReport:
      ReportPrint(pRepHelp,"... ");
      break;
  }  
  if (pCurParm->eType != PTypeBool) {
    iNumSpaces = 15 - strlen(pCurParm->sSwitch);
    if (bAlgOffset) {
      iNumSpaces -= 4;
    }
    if (iNumSpaces > 20) {
      iNumSpaces = 0;
    }
    for (j=0;j<iNumSpaces;j++) {
      ReportPrint(pRepHelp," ");
    }
  } 

  ReportPrint1(pRepHelp,pCurParm->sTerseDescription,sHelpString);
  ReportPrint(pRepHelp,"\n\n");

  if (strlen(pCurParm->sVerboseDescription)) {
    pCur = pCurParm->sVerboseDescription;
    pNext = strstr(pCur,"~");
    while (pNext) {
      *pNext = 0;
      ReportPrint(pRepHelp,"                    ");
      ReportPrint1(pRepHelp,pCur,sHelpString);
      ReportPrint(pRepHelp,"\n");
      *pNext = '~';
      pCur = ++pNext;
      pNext = strstr(pCur,"~");
    }
    ReportPrint(pRepHelp,"                    ");
    ReportPrint1(pRepHelp,pCur,sHelpString);
    ReportPrint(pRepHelp,"\n\n");
  }
  ReportPrint(pRepHelp,"\n");
}


void HelpPrintParameters(ALGPARMLIST *pParmList) {
  
  UINT32 j;
  if (pParmList->iNumParms) {
    for (j=0;j<pParmList->iNumParms;j++) {
      HelpPrintParameter(&pParmList->aParms[j], FALSE);
    }
  }
}

void HelpPrintParametersTerse(ALGPARMLIST *pParmList) {
  
  UINT32 j;
  ALGPARM *pCurParm;

  for (j=0;j<pParmList->iNumParms;j++) {
    pCurParm = &pParmList->aParms[j];
    switch(pCurParm->eType)
    {
      case PTypeBool:
        sprintf(sHelpString,"%u",pCurParm->defDefault.bBool);
        break;
      case PTypeUInt:
        sprintf(sHelpString,"%u",pCurParm->defDefault.iUInt);
        break;
      case PTypeSInt:
        sprintf(sHelpString,"%d",pCurParm->defDefault.iSInt);
        break;
      case PTypeProbability:
        sprintf(sHelpString,"%.4g",ProbToFloat(pCurParm->defDefault.iProb));
        break;
      case PTypeString:
        sprintf(sHelpString,"");
        break;
      case PTypeFloat:
        sprintf(sHelpString,"%.6g",pCurParm->defDefault.fFloat);
        break;
      case PTypeReport:
        sprintf(sHelpString,"");
        break;
    }  

    ReportPrint1(pRepHelp," %-18s ",pCurParm->sSwitch);
    ReportPrint1(pRepHelp,pCurParm->sTerseDescription,sHelpString);
    ReportPrint(pRepHelp,"\n");
  }
  ReportPrint(pRepHelp,"\n");
}

void HelpPrintParametersTerse2(ALGPARMLIST *pParmList) {
  
  UINT32 j;
  ALGPARM *pCurParm;

  for (j=0;j<pParmList->iNumParms;j++) {
    pCurParm = &pParmList->aParms[j];
    ReportPrint1(pRepHelp,"%s\n",pCurParm->sSwitch);
  }
  ReportPrint(pRepHelp,"\n");
}


void HelpPrintAlgParameters(ALGORITHM *pCurAlg) {

  if (pCurAlg) {
    ReportPrint1(pRepHelp,"-alg %s",pCurAlg->sName);
    if (*pCurAlg->sVariant != 0) {
      ReportPrint1(pRepHelp," -v %s",pCurAlg->sVariant);
    }
    if (pCurAlg->bWeighted) {
      ReportPrint(pRepHelp," -w");
    }
    ReportPrint(pRepHelp,"\n");

    ReportPrint1(pRepHelp,"  %s\n",pCurAlg->sDescription);
    ReportPrint1(pRepHelp,"  %s\n\n",pCurAlg->sAuthors);
    HelpPrintParameters(&pCurAlg->parmList);
  }
}


void HelpPrintAlgParametersTerse(ALGORITHM *pCurAlg) {

  if (strcmp(pCurAlg->sName,"default")!=0) {
    ReportPrint1(pRepHelp,"-alg %s",pCurAlg->sName);
    if (*pCurAlg->sVariant != 0) {
      ReportPrint1(pRepHelp," -v %s",pCurAlg->sVariant);
    }
    if (pCurAlg->bWeighted) {
      ReportPrint(pRepHelp," -w");
    }
    ReportPrint(pRepHelp,"");
  }
  HelpPrintParametersTerse(&pCurAlg->parmList);
}

void HelpPrintSpecialParameters() {

  ReportPrint(pRepHelp,"\nHELP PARAMETERS:\n===============\n\n");
  HelpPrintParameters(&parmHelp);

  ReportPrint(pRepHelp,"ALGORITHM SPECIFICATION:\n=======================\n\n");
  HelpPrintParameters(&parmAlg);
  
  ReportPrint(pRepHelp,"RUNTIME PARAMETERS:\n==================\n\n");
  HelpPrintParameters(&parmUBCSAT);

  ReportPrint(pRepHelp,"REPORTING & FILE I/O PARAMETERS:\n===============================\n\n");
  HelpPrintParameters(&parmIO);  

}

void HelpPrintSpecialParametersTerse() {

  HelpPrintParametersTerse(&parmHelp);
  HelpPrintParametersTerse(&parmAlg);
  HelpPrintParametersTerse(&parmUBCSAT);
  HelpPrintParametersTerse(&parmIO);

}


void HelpShowVerbose() {
  UINT32 j;
  HelpPrintSpecialParameters();
  for (j=0;j<iNumAlg;j++)
    HelpPrintAlgorithm(&aAlgorithms[j],FALSE,TRUE);
}

void HelpShowTerse() {
  UINT32 j;
  HelpPrintSpecialParametersTerse();
  for (j=0;j<iNumAlg;j++)
    HelpPrintAlgorithm(&aAlgorithms[j],TRUE,FALSE);
}

void HelpBadParm(char *sParm) {
  PrintUBCSATHeader(pRepErr);
  ReportPrint(pRepErr,"\n");
  ReportPrint(pRepErr,"FATAL ERROR ENCOUNTERED: \n");
  ReportPrint1(pRepErr,"   Parameter [%s] is unrecognized or invalid\n",sParm);
  ReportPrint(pRepErr,"\n");
  AbnormalExit();
}


void HelpBadReport(char *sParm) {
  PrintUBCSATHeader(pRepErr);
  ReportPrint(pRepErr,"\n");
  ReportPrint(pRepErr,"FATAL ERROR ENCOUNTERED: \n");
  ReportPrint1(pRepErr,"   Report Parameter [%s] is unrecognized\n",sParm);
  ReportPrint(pRepErr,"   ubcsat -hr for help on report parameters  \n");
  ReportPrint(pRepErr,"\n");
  AbnormalExit();
}











/* Some Easter Eggs for Holger:

                  .-^-.
                .'=^=^='.
               /=^=^=^=^=\
       .-~-.  :^= HAPPY =^;
     .'~~*~~'.|^ EASTER! ^|
    /~~*~~~*~~\^=^=^=^=^=^:
   :~*~~~*~~~*~;\.-*))`*-,/
   |~~~*~~~*~~|/*  ((*   *'.
   :~*~~~*~~~*|   *))  *   *\
    \~~*~~~*~~| *  ((*   *  /
     `.~~*~~.' \  *))  *  .'
       `~~~`    '-.((*_.-'

*/





