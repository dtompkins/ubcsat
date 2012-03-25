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

#ifndef UBCSAT_TRIGGERS_H
#define UBCSAT_TRIGGERS_H

#include "ubcsat-types.h"
#include "ubcsat-lit.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif


void AddDataTriggers();

/***** Trigger ReadCNF *****/
/* 
    iNumVars              # of variables
    iNumClauses           # of clauses
    iNumLits              # of literals

    aClauseLen[j]         length of clause[j]
    aClauseLits[j][k]     literal [k] of clause[j]           

    aClauseWeight         (if weighted algorithm) weight of clause[j]
    fTotalWeight          sum of all clause weights
*/

extern UINT32 iNumVars;
extern UINT32 iNumClauses;
extern UINT32 iNumLits;

extern UINT32 *aClauseLen;
extern LITTYPE **pClauseLits;
extern UINT32 iMaxClauseLen;

extern UBIGINT *aClauseWeight;
extern UBIGINT iTotalClauseWeight;

extern UINT32 iVARSTATELen;


/***** Trigger LitOccurence *****/
/* 
    aNumLitOcc[j]         # of times literal j occurs
    pLitClause[j]         pointer to list of clauses literal j appears in
*/

extern UINT32 *aNumLitOcc;
extern UINT32 *aLitOccData;
extern UINT32 **pLitClause;


/***** Trigger CandidateList *****/
/* 
    aCandidateList        array used by algorithms to store 'best' flip candidates
    iNumCandidates        current number of 'best' flip candidates
    iMaxCandidates        maximum size of the array
*/

extern UINT32 *aCandidateList;
extern UINT32 iNumCandidates;
extern UINT32 iMaxCandidates;


/***** Trigger InitVarsFromFile *****/
/***** Trigger DefaultInitVars *****/
/*
    aVarInit[j]           how variable[j] is initialized each run (0,1,2 => False,True,Random)
    iInitVarFlip          # of variables to be 're-flipped' after initialization
    bVarInitGreedy        if true, then greedily init vars that have a biased # of pos/neg literals
*/
  
extern UINT32 *aVarInit;
extern UINT32 iInitVarFlip;
extern BOOL bVarInitGreedy;


/***** Trigger DefaultStateInfo *****/
/*
    iNumFalse             # of currently unsatisfied (false) clauses
    iSumFalseWeight       sum of the weights of currently unsatisfied (false) clauses
    aNumTrueLit[j]        # of true literals occuring in clause[j] (i.e.: 0 -> false clause)
    aVarValue[j]          current value of variable[j] (0,1 => False,True)
*/

extern UINT32 *aNumTrueLit;
extern BOOL *aVarValue;
extern UINT32 iNumFalse;
extern UBIGINT iSumFalseWeight;


/***** Trigger DefaultFlip[W] *****/


/***** Trigger CheckTermination *****/


/***** Trigger FalseClauseList *****/
/***** Trigger Flip+FalseClauseList[W] *****/
/*
    aFalseList[j]         clause # for the jth false clause
    aFalseListPos[j]      for clause[j], position it occurs in aFalseList
    iNumFalseList         # entries in aFalseList (the same as iNumFalse, but updated separately)
*/

extern UINT32 *aFalseList;
extern UINT32 *aFalseListPos;
extern UINT32 iNumFalseList;


/***** Trigger VarScore[W] *****/
/***** Trigger Flip+VarScore[W] *****/
/*
    aVarScore[j]          score for variable[j]... Change in # false clauses if variable[j] is flipped
    aVarScoreWeight[j]    weighted score for variable[j]... Change in sum of false clause weights if variable[j] is flipped
*/

extern SINT32 *aVarScore;
extern SBIGINT *aVarScoreWeight;


/***** Trigger MakeBreak[W] *****/
/***** Trigger Flip+MakeBreak[W] *****/
/*
    aMakeCount[k]         # of clauses that flipping variable[j] will 'make'
    aBreakCount[k]        # of clauses that flipping variable[j] will 'break'
    aCritSat[k]           critical variable for clause[k] if clause has only one true literal
    amakeCountWeight[k]   (same as aMakeCount, but as a sum of clause weights)
    aBreakCountWeight[k]  (same as aBreakCount, but as a sum of clause weights)

*/

extern UINT32 *aBreakCount;
extern UINT32 *aMakeCount;
extern UINT32 *aCritSat;
extern UBIGINT *aBreakCountWeight;
extern UBIGINT *amakeCountWeight;


/***** Trigger VarInFalse *****/
/***** Trigger Flip+VarInFalse *****/
/*
    aVarInFalseList[j]    variable # for the jth variable that appears in false clauses
    aVarInFalseListPos[j] for variable[j], position it occurs in aVarInFalseList
    iNumVarsInFalseList   # variables that appear in false clauses 
*/

extern UINT32 iNumVarsInFalseList;
extern UINT32 *aVarInFalseList;
extern UINT32 *aVarInFalseListPos;


/***** Trigger VarLastChange *****/
/*
    aVarLastChange[j]     the step # of the most recent time variable[j] was flipped
    iVarLastChangeReset   the step # of the last time all aVarLastChange values were reset
*/

void UpdateVarLastChange();
extern UBIGINT *aVarLastChange;
extern UBIGINT iVarLastChangeReset;


/***** Trigger TrackChanges *****/
/*
    iNumChanges           # of changes to aVarScore[] values this step
    aChangeList[j]        variable # of the jth variable changed this step
    aChangeOldScore[j]    the previous score of variable[j]
    aChangeLastStep[j]    the step of the last change for variable[j]
*/

extern UINT32 iNumChanges;
extern UINT32 *aChangeList;
extern SINT32 *aChangeOldScore;
extern UBIGINT *aChangeLastStep;

extern UINT32 iNumChangesW;
extern UINT32 *aChangeListW;
extern SBIGINT *aChangeOldScoreWeight;
extern UBIGINT *aChangeLastStepWeight;

#define UpdateChange(var) {if(aChangeLastStep[var]!=iStep) {aChangeOldScore[var] = aVarScore[var]; aChangeLastStep[var]=iStep; aChangeList[iNumChanges++]=var;}}


/***** Trigger DecPromVars *****/
/*
    iNumDecPromVars       # Decreasing Promising Variables
    aDecPromVarsList[j]   variable # of the jth decreasing promising Variable list
*/

extern UINT32 *aDecPromVarsList;
extern UINT32 iNumDecPromVars;

extern UINT32 *aDecPromVarsListW;
extern UINT32 iNumDecPromVarsW;


/***** Trigger BestScoreList *****/
/*
    iNumBestScoreList       # of variables tied for the 'best' aVarScore
    *aBestScoreList[j]      variable # of the jth variable with the best score
    *aBestScoreListPos[j]   the location of variable[j] in aBestScoreList
*/

extern UINT32 iNumBestScoreList;
extern UINT32 *aBestScoreList;
extern UINT32 *aBestScoreListPos;


/***** Trigger ClausePenaltyFL *****/
/*
    aClausePenaltyFL[j]     dynamic clause penalty for clause[j]
    bClausePenaltyCreated   boolean to indicate clause penalties exist
    bClausePenaltyFLOAT     boolean to indicate clause penalties are floating point values
    fBasePenaltyFL          value of a "never been modified" clause penalty
    fTotalPenaltyFL         sum of all penalties
*/

extern FLOAT *aClausePenaltyFL;
extern BOOL bClausePenaltyCreated;
extern BOOL bClausePenaltyFLOAT;
extern FLOAT fBasePenaltyFL;
extern FLOAT fTotalPenaltyFL;


/***** Trigger MakeBreakPenaltyFL *****/
/*
    aMakePenaltyFL[j]       (same as aMakeCount, but as a sum of dynaimc clause penalties)  
    aBreakPenaltyFL[j]      (same as aBreakCount, but as a sum of dynaimc clause penalties)  
*/

extern FLOAT *aMakePenaltyFL;
extern FLOAT *aBreakPenaltyFL;


/***** Trigger ClausePenaltyINT *****/
/*
    aClausePenaltyINT[j]     dynamic clause penalty for clause[j]
    bClausePenaltyCreated    boolean to indicate clause penalties exist
    bClausePenaltyFLOAT      boolean to indicate clause penalties are floating point values
    iInitPenaltyINT          initialization value for penalties
    iBasePenaltyINT          value of a "never been modified" clause penalty
    iTotalPenaltyINT         sum of all penalties
*/

extern UINT32 *aClausePenaltyINT;
extern UINT32 iInitPenaltyINT;
extern UINT32 iBasePenaltyINT;
extern UINT32 iTotalPenaltyINT;


/***** Trigger MakeBreakPenaltyINT *****/
/*
    aMakePenaltyINT[j]       (same as aMakeCount, but as a sum of dynaimc clause penalties)  
    aBreakPenaltyINT[j]      (same as aBreakCount, but as a sum of dynaimc clause penalties)  
*/

extern UINT32 *aMakePenaltyINT;
extern UINT32 *aBreakPenaltyINT;


/***** Trigger NullFlips *****/
/*
    iNumNullFlips         number of null flips (steps where iFlipCandidate == 0)
*/

extern UBIGINT iNumNullFlips;

 
/***** Trigger LocalMins *****/
/*
    LocalMins              number of local minima encountered this run
*/

extern UBIGINT iNumLocalMins;


/***** Trigger LogDist *****/
/*
    aLogDistValues[j]         array from 10...iCutoff of log-distributed values
    iNumLogDistValues         size of aLogDistValues[]
    iLogDistStepsPerDecade    # of elements in aLogDistValues per decade
*/

extern UBIGINT *aLogDistValues;
extern UINT32 iNumLogDistValues;
extern UINT32 iLogDistStepsPerDecade;


/***** Trigger BestFalse *****/
/*
    iBestNumFalse             best value of iNumFalse this run
    iBestStepNumFalse         step where iBestNumFalse occured
    iBestSumFalseWeight       best weighted sum of false clauses seen this run
    iBestStepSumFalseWeight   step where iBestSumFalseWeight occured
*/

extern UINT32 iBestNumFalse;
extern UBIGINT iBestStepNumFalse;
extern UBIGINT iBestSumFalseWeight;
extern UBIGINT iBestStepSumFalseWeight;


/***** Trigger SaveBest *****/
/*
    vsBest                the candidate solution for the best solution quality seen this run   
*/
extern VARSTATE vsBest;


/***** Trigger StartFalse *****/
/*
    iStartNumFalse            value of iNumFalse on Step 1
    iStartSumFalseWeight      value of iSumFalseWeight on Step 1
*/

extern UINT32 iStartNumFalse;
extern UBIGINT iStartSumFalseWeight;


/***** Trigger ImproveMean *****/
/*
    fImproveMean              Mean Improvement per Step to Best Solution
    fImproveMeanW             Mean Improvement per Step to Best Solution Quality
*/

extern FLOAT fImproveMean;
extern FLOAT fImproveMeanW;


/***** Trigger FirstLM *****/

/*
    iFirstLM                  # of false clauses @ first local minimum
    iFirstLMStep              step of the first local minimum encountered
    iFirstLMWeight            Solution quality @ first weighted local minimum
    iFirstLMStepW             step of the first weighted local minimum encountered
*/

extern UINT32 iFirstLM;
extern UBIGINT iFirstLMStep;
extern UBIGINT iFirstLMWeight;
extern UBIGINT iFirstLMStepW;


/***** Trigger FirstLMRatio *****/

/*
    fFirstLMRatio             (iStartNumFalse - iFirstLM) / (iStartNumFalse - iBestNumFalse)
    fFirstLMRatioW            (iStartSumFalseWeight - iFirstLMWeight) / (iStartSumFalseWeight - iBestSumFalseWeight)
*/

extern FLOAT fFirstLMRatio;
extern FLOAT fFirstLMRatioW;


/***** Trigger TrajBestLM *****/

/*
    fTrajBestLMMean           Mean of # False @ every new best encountered that's a LM on the trajectory
    fTrajBestLMMeanW          Weighted Mean of Sol. Quality @ ... 
    fTrajBestLMMeanCV         CV of # False @ ... 
    fTrajBestLMMeanCVW        Weighted CV of Sol. Quality @ ... 
*/

extern FLOAT fTrajBestLMMean;
extern FLOAT fTrajBestLMMeanW;
extern FLOAT fTrajBestLMCV;
extern FLOAT fTrajBestLMCVW;


/***** Trigger NoImprove *****/
/*
    iNoImprove            terminate this run if no improvement in iNoImprove steps
*/

extern UBIGINT iNoImprove;


/***** Trigger EarlyTerm *****/
/*
    iEarlyTermSteps       terminate this run if after iEarlyTermSteps the quality is greater than iEarlyTermQual
    iEarlyTermQual
    iEarlyTermQualWeight
*/

extern UBIGINT iEarlyTermSteps;
extern UINT32 iEarlyTermQual;
extern UBIGINT iEarlyTermQualWeight;


/***** Trigger Strikes *****/
/*
    iStrikes              terminate all runs after iStrikes runes without reaching target
*/

extern UINT32 iStrikes;


/***** Trigger StartSeed *****/
/*
    iStartSeed            starting seed required to duplicate the current run
*/

extern UINT32 iStartSeed;


/***** Trigger CountRandom *****/
/*
    iNumRandomCalls       # of calls to random number generator this run

    (note that code is in ubcsat-io.c)
*/


/***** Trigger CheckTimeout *****/

extern UINT32 iTimeResolution;


/***** Trigger CheckForRestarts *****/


/***** Trigger FlipCounts *****/
/*
    aFlipCounts[j]        # of times that variable[j] has been flipped this run
*/

extern UBIGINT *aFlipCounts;


/***** Trigger FlipCountStats *****/
/*
    fFlipCountsMean       Mean of aFlipCounts[] excluding [0] (NullFlips)
    fFlipCountsStdDev     StdDev of aFlipCounts[] excluding [0] (NullFlips)
    fFlipCountsCV         CV of aFlipCounts[] excluding [0] (NullFlips)
*/

extern FLOAT fFlipCountsMean;
extern FLOAT fFlipCountsCV;
extern FLOAT fFlipCountsStdDev;


/***** Trigger BiasCounts *****/
/*
    aBiasTrueCounts[j]  # of steps that variable[j] has been True
    aBiasFalseCounts[j] # of steps that variable[j] has been False
*/

extern UBIGINT *aBiasTrueCounts;
extern UBIGINT *aBiasFalseCounts;


/***** Trigger BiasStats *****/
/*
    fMeanFinalBias    Mean Fraction of # steps each variable was in its final state
    fMeanMaxBias      Mean Fraction of # steps each variable was in its most frequent state
*/

extern FLOAT fMeanFinalBias;
extern FLOAT fMeanMaxBias;


/***** Trigger UnsatCounts *****/
/*
    aUnsatCounts[j]       # of steps that clause[j] has been unsatisfied
*/

extern UBIGINT *aUnsatCounts;


/***** Trigger UnsatCountStats *****/
/*
    fUnsatCountsMean      Mean of aUnsatCounts[]
    fUnsatCountsStdDev    StdDev of aUnsatCounts[]
    fUnsatCountsCV        CV of aUnsatCounts[]
*/

extern FLOAT fUnsatCountsMean;
extern FLOAT fUnsatCountsCV;
extern FLOAT fUnsatCountsStdDev;


/***** Trigger NumFalseCounts *****/
/*
    aNumFalseCounts[j]        # of steps that j clauses have been unsatisfied
    aNumFalseCountsWindow[j]  NumFalse values for the last iReportFalseHistCount steps
*/

extern UBIGINT *aNumFalseCounts;
extern UBIGINT *aNumFalseCountsWindow;


/***** Trigger DistanceCounts *****/
/*
    aDistanceCounts[j]        # of steps that search has been Hamming distance j from the solution
    aDistanceCountsWindow[j]  Hamming Distance values for the last iReportDistHistCount steps
*/

extern UBIGINT *aDistanceCounts;
extern UBIGINT *aDistanceCountsWindow;


/***** Trigger ClauseLast *****/
/*
    aClauseLast[j]        last step # for which clause[j] was unsatisfied
*/

extern UBIGINT *aClauseLast;


/***** Trigger ClauseLast *****/
/*
    aSQGrid[j][k]         for run [k], solution quality at step aLogDistValues[j]
    aSQGridWeight[j][k]   for run [k], solution quality at step aLogDistValues[j]
*/

extern UBIGINT *aSQGridWeight;
extern UINT32 *aSQGrid;

/***** Trigger PenaltyStats *****/
/*
    aPenaltyStatsMean[j]    for current run, clause j, Clause Penalty Mean
    aPenaltyStatsStddev[j]  for current run, clause j, Clause Penalty Stddev
    aPenaltyStatsCV[j]      for current run, clause j, Clause Penalty CV
*/

extern FLOAT *aPenaltyStatsMean;
extern FLOAT *aPenaltyStatsStddev;
extern FLOAT *aPenaltyStatsCV;

extern FLOAT *aPenaltyStatsSum;
extern FLOAT *aPenaltyStatsSum2;

extern FLOAT *aPenaltyStatsMeanSum;
extern FLOAT *aPenaltyStatsMeanSum2;
extern FLOAT *aPenaltyStatsStddevSum;
extern FLOAT *aPenaltyStatsStddevSum2;
extern FLOAT *aPenaltyStatsCVSum;
extern FLOAT *aPenaltyStatsCVSum2;


/***** Trigger VarFlipHistory *****/
/* 
    aVarFlipHistory[j]      circular array: last iVarFlipHistoryLen variables flipped
    iVarFlipHistoryLen      size of circular array aVarFlipHistory [default = 2 * #Vars]
*/

extern UINT32 *aVarFlipHistory;
extern UINT32 iVarFlipHistoryLen;



/***** Trigger MobilityWindow *****/
/*
    aMobilityWindow[j]      Current hamming distance for mobility window length of j
    aMobilityWindowSum[j]   Cumulative sum of aMobilityWindow[j]
    aMobilityWindowSum2[j]  Cumulative sum of aMobilityWindow[j] ^2
*/

extern UINT32 *aMobilityWindow;
extern FLOAT *aMobilityWindowSum;
extern FLOAT *aMobilityWindowSum2;


/***** Trigger MobilityFixedFrequencies *****/
/*
    aMobilityFixedFrequencies[j]  # of times value j has occured for aMobilityWindow[iMobFixedWindow]
*/

extern UINT32 *aMobilityFixedFrequencies;



/***** Trigger VarAgeFrequencies *****/
/*
    aVarAgeFrequency[j]  # of times variable had age j when flipped
*/

extern UINT32 iMaxVarAgeFrequency;
extern UBIGINT *aVarAgeFrequency;



/***** Trigger AutoCorr *****/
/*
    iAutoCorrMaxLen     Length of auto-correlation window (Maximum value of ACL)
    fAutoCorrCutoff     Value at which the auto-correlation value drops below to set ACL
    iAutoCorrLen        Auto-correlation length (calculated at the end of each run)
    aAutoCorrValues     Auto-correlation values (calculated at the end of each run)
*/

extern UINT32 iAutoCorrMaxLen;
extern FLOAT fAutoCorrCutoff;
extern UINT32 iAutoCorrLen;
extern FLOAT *aAutoCorrValues;



/***** Trigger AutoCorrOne *****/
/*
    fAutoCorrOneVal     Auto-correlation of distance one
    fAutoCorrOneEst     Estimated ACL from fAutoCorrOneVal = -1/ln(|.|)
*/

extern FLOAT fAutoCorrOneVal;
extern FLOAT fAutoCorrOneEst;



/***** Trigger BranchFactor *****/
/*
    fBranchFactor       Branching Factor: # "sideways" step vars / # vars
    fBranchFactorW      Weighted Branching Factor: # "sideways" step vars / # vars
*/

extern FLOAT fBranchFactor;
extern FLOAT fBranchFactorW;



/***** Trigger StepsUpDownSide *****/
/*  
    iNumUpSteps         Number of "up" steps this run
    iNumDownSteps       Number of "down" steps this run
    iNumSideSteps       Number of "sideways" steps this run
*/
  
extern UBIGINT iNumUpSteps;
extern UBIGINT iNumDownSteps;
extern UBIGINT iNumSideSteps;
extern UBIGINT iNumUpStepsW;
extern UBIGINT iNumDownStepsW;
extern UBIGINT iNumSideStepsW;


/***** Trigger NumRestarts *****/
/*  
    iNumRestarts        Number of Restarts (excl. initialization)
*/
  
extern UINT32 iNumRestarts;


/***** Trigger LoadKnownSolutions *****/
/*
    bKnownSolutions         TRUE if Known Solutions > 0
    vslKnownSoln            VARSTATELIST of Known Solutions
*/

extern VARSTATELIST vslKnownSoln;
extern BOOL bKnownSolutions;


/***** Trigger SolutionDistance *****/
/*
    iSolutionDistance           Current Distance from Known Solution(s)
    vsSolutionDistanceClosest   The closest known solution
*/

extern VARSTATE vsSolutionDistanceClosest;
extern UINT32 iSolutionDistance;


/***** Trigger FDCRun *****/
/*
    fFDCRun                 FDC for the current run (updated at end), using LM points only
*/

extern FLOAT fFDCRun;


/***** Trigger DynamicParms *****/


/***** Trigger FlushBuffers *****/


/***** Trigger CheckWeighted *****/


/***** Trigger UniqueSolutions *****/
/*
    vslUnique               linked list of all unique solutions
    iNumUniqueSolutions     number of unique solutions found so far
    iLastUnique             Run of the last unique solution found
*/

extern VARSTATELIST vslUnique;
extern UINT32 iNumUniqueSolutions;
extern UINT32 iLastUnique;


/***** Trigger VarsShareClauses *****/
/*
    aNumVarsShareClause[j]  number of vars that share a clause with var j
    pVarsShareClause[j][k]  kth var that shares a clause with var j
*/

extern UINT32 *aNumVarsShareClause;
extern UINT32 **pVarsShareClause;


#ifdef __cplusplus
}
#endif
#endif
