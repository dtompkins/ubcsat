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

#ifndef UBCSAT_REPORTS_H
#define UBCSAT_REPORTS_H

#include "ubcsat-types.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif


void AddReports();

extern REPORT *pRepHelp;
extern REPORT *pRepErr;

extern REPORT *pRepOut;
extern REPORT *pRepRTD;
extern REPORT *pRepStats;
extern REPORT *pRepState;
extern REPORT *pRepModel;
extern REPORT *pRepSolution;
extern REPORT *pRepUniqueSol;
extern REPORT *pRepBestSol;
extern REPORT *pRepBestStep;
extern REPORT *pRepTrajBestLM;
extern REPORT *pRepOptClauses;
extern REPORT *pRepFalseHist;
extern REPORT *pRepDistance;
extern REPORT *pRepDistHist;
extern REPORT *pRepCNFStats;
extern REPORT *pRepFlipCounts;
extern REPORT *pRepBiasCounts;
extern REPORT *pRepUnsatCounts;
extern REPORT *pRepVarLast;
extern REPORT *pRepClauseLast;
extern REPORT *pRepSQGrid;
extern REPORT *pRepPenalty;
extern REPORT *pRepPenMean;
extern REPORT *pRepPenStddev;
extern REPORT *pRepPenCV;
extern REPORT *pRepVW2Weights;
extern REPORT *pRepMobility;
extern REPORT *pRepMobFixed;
extern REPORT *pRepMobFixedFreq;
extern REPORT *pRepVarAgeFreq;
extern REPORT *pRepAutoCorr;
extern REPORT *pRepTriggers;
extern REPORT *pRepParamILS;
extern REPORT *pRepSATComp;
extern REPORT *pRepMAXSATComp;

#ifdef __cplusplus
}
#endif
#endif
