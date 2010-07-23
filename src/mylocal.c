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

  AddLocal() is here for you to add your own algorithms, reports, etc... 
  
  this will help to avoid your work being 'clobbered' by future releases of ubcsat
  
  if you've added some features you'd like to see included in the ubcsat package,
  then please send us your files!

*/

void AddWalkSatTabuNoNull();
void PickWalkSatTabuNoNull();
void AddAgeStat();
void UpdateCurVarAge();

void AddLocal() {

  AddWalkSatTabuNoNull();
  AddAgeStat();

}

/***** EXAMPLE 1: Adding a new algorithm variant *****/

void AddWalkSatTabuNoNull() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm(
    "walksat-tabu","nonull",FALSE,                          /* algorithm name, variant, non-weighted */
    "WALKSAT-TABU-NoNull: WALKSAT-TABU without null flips", /* algorithm description */
    "McAllester, Selman, Kautz [AAAI 97] (modified)",       /* author information */
    "PickWalkSatTabuNoNull",                                /* Heuristic Triggers */
    "",                                                     /* Data Triggers (blank because we'll inherit */
    "default,agemean",                                      /* default parameter (columns) for output (-r out) -- will demo example 2*/
    "default");                                             /* default parameter (stats) for statistics (-r stats) */
  
  InheritDataTriggers(pCurAlg,"walksat-tabu","",FALSE);     /* Copy the data triggers from walksat-tabu */
  
  CopyParameters(pCurAlg,"walksat-tabu","",FALSE);          /* Copy the command line parameters from walksat-tabu */

  CreateTrigger("PickWalkSatTabuNoNull",                    /* Add the new trigger for the heuristic */
    ChooseCandidate,                                        /* Occurs at the event point for choosing flip candidates */
    PickWalkSatTabuNoNull,                                  /* the corresponding function name */
    "",                                                     /* no dependency triggers */
    "");                                                    /* no deactivation triggers */

}

void PickWalkSatTabuNoNull() {
  
  LITTYPE litPick;
  UINT32 iClauseLitNo;

  PickWalkSatTabu();                                              /* Perform regular walksat-tabu step */

  if (iFlipCandidate==0) {                                        /* if null flip then */
    iClauseLitNo = RandomInt(aClauseLen[iWalkSATTabuClause]);     /* choose a # from [0 .. clausesize-1] */
    litPick = pClauseLits[iWalkSATTabuClause][iClauseLitNo];      /* select the literal from the clause */
    iFlipCandidate = GetVarFromLit(litPick);                      /* set iFlipCandidate to the corresponding variable */
  }
}


/***** EXAMPLE 2: Adding a statistic *****/

UINT32 iCurVarAge;                                                /* variable to store current variable age */

void AddAgeStat() {
                                                                  /* note that UInt refers to the data type of the _source_ */
  AddColumnUInt("agemean",                                        /* name of the column... as in: -r out stdout default,agemean */
    "Mean age of variables when flipped",                         /* description */
    "   Mean",                                                    /* the next 3 columns are the column headers */
    " Age of",                                                    /* they should be the same width as each other */
    "   Vars",                                                    /* and the same width as the.... */
    "%7.1f",                                                      /* format string: Note %f (float) because it's a mean value */
    &iCurVarAge,                                                  /* pointer to location of statistic */
    "UpdateCurVarAge",                                            /* trigger to be activated */
    ColTypeMean);                                                 /* find the mean over all search steps */

  CreateTrigger("UpdateCurVarAge",                                /* add the trigger to update the value */
    PreFlip,
    UpdateCurVarAge,
    "VarLastChange",
    "");

  AddStatCol("agemean",                                           /* column statistic for the -r stats report */
    "MeanAge",                                                    /* prefix for the report */
    "mean+cv+median+min+max",                                     /* default stats to show */
    FALSE);                                                       /* specify TRUE to sort by the steps column (for median, etc.) instead of this column */

}

void UpdateCurVarAge() {
  iCurVarAge = iStep - aVarLastChange[iFlipCandidate];
}

