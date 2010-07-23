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
extern REPORT *pRepMobility;
extern REPORT *pRepMobFixed;
extern REPORT *pRepMobFixedFreq;
extern REPORT *pRepAutoCorr;
extern REPORT *pRepTriggers;
extern REPORT *pRepSATComp;

