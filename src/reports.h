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

extern REPORT *pRepErr;

extern REPORT *pRepOut;
extern REPORT *pRepStats;
extern REPORT *pRepRTD;
extern REPORT *pRepState;
extern REPORT *pRepModel;
extern REPORT *pRepSolution;
extern REPORT *pRepBest;
extern REPORT *pRepOptClauses;
extern REPORT *pRepCNFStats;
extern REPORT *pRepFlipCounts;
extern REPORT *pRepUnsatCounts;
extern REPORT *pRepVarLast;
extern REPORT *pRepClauseLast;
extern REPORT *pRepSATComp;
extern REPORT *pRepPenalty;
extern REPORT *pRepPenSol;
extern REPORT *pRepPenMean;
extern REPORT *pRepPenCV;
extern REPORT *pRepSQGrid;
