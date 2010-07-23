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

REPORT *pRepHelp;
REPORT *pRepErr;

REPORT *pRepOut;
REPORT *pRepRTD;
REPORT *pRepStats;
REPORT *pRepState;
REPORT *pRepModel;
REPORT *pRepSolution;
REPORT *pRepUniqueSol;
REPORT *pRepBestSol;
REPORT *pRepBestStep;
REPORT *pRepTrajBestLM;
REPORT *pRepOptClauses;
REPORT *pRepFalseHist;
REPORT *pRepDistance;
REPORT *pRepDistHist;
REPORT *pRepCNFStats;
REPORT *pRepFlipCounts;
REPORT *pRepBiasCounts;
REPORT *pRepUnsatCounts;
REPORT *pRepVarLast;
REPORT *pRepClauseLast;
REPORT *pRepSQGrid;
REPORT *pRepPenalty;
REPORT *pRepPenMean;
REPORT *pRepPenStddev;
REPORT *pRepPenCV;
REPORT *pRepMobility;
REPORT *pRepMobFixed;
REPORT *pRepMobFixedFreq;
REPORT *pRepAutoCorr;
REPORT *pRepTriggers;
REPORT *pRepSATComp;

void AddReports() {

  pRepOut = CreateReport("out", "Standard Output with Columns","Prints an array of data, with each row representing an independent run~you can customize the report to include a variety of columns","stdout","ReportOut");
  AddReportParmString(pRepOut,"Columns to Display (ubcsat -hc for info)","default");
  AddReportParmUInt(pRepOut,"Delay: Set to 1 to print after all runs are complete",&bReportOutputSuppress,0);
  pRepOut->bActive = TRUE;

  pRepStats = CreateReport("stats","Statistics Report","Shows summary statistics from all runs~you can customize the report to include a variety of statistics","stdout","ReportStats");
  pRepStats->bActive = TRUE;
  AddReportParmString(pRepStats,"Statistics to Display (ubcsat -hs for info)","default");

  pRepRTD = CreateReport("rtd","Run-Length and Run-Time Distribution","Similar to (-r out), except results are sorted by search steps,~and include successful runs~you can customize the report to include a variety of columns", "stdout","ReportRTD,SortByStepPerformance");
  AddReportParmString(pRepRTD,"Columns to Display (ubcsat -hc for info)","rtd");

  pRepModel = CreateReport("model","Solution Model","Prints the solution model found from the last run executed~is turned on automatically with the -solve parameter~output format example is: -1 2 3 -4 ... ~meaning variables (1,4) are true, and variables (2,3) are false","stdout","ReportModelPrint");

  pRepCNFStats = CreateReport("cnfstats","Instance Statistics","Provides additional information about the properties of the instance","stdout","ReportCNFStatsPrint");
  
  pRepState = CreateReport("state","State Information Trace","Provides detailed state information for each search step, including:~the solution quality, the variable flipped, and the state of all variables","stdout","ReportStatePrint");
  AddReportParmUInt(pRepState,"Only print when in local minima",&bReportStateLMOnly,0);
  AddReportParmFloat(pRepState,"Only print at a specific Solution Quality (all if -1)",&fReportStateQuality,-1.0f);

  pRepSolution = CreateReport("solution","Solutions","Prints solutions for every successful run in the format:~1001001101...~where variable 1 is true, 2 is false, 3 is false, etc...","stdout","ReportSolutionPrint");

  pRepUniqueSol = CreateReport("uniquesol","Unique Solutions","Prints the same as (-r solution), except only unique solutions (no repeats)","stdout","ReportUniqueSolPrint");

  pRepBestSol = CreateReport("bestsol","Best Candidate Solutions","Similar to (-r solution), but includes variable assignments for the~best solution quality encountered when no solution is found","stdout","ReportBestSolPrint");

  pRepBestStep = CreateReport("beststep","Steps of Each New Best Solution Quality","Print step information for each new best solution encountered","stdout","ReportBestStepPrint");
  AddReportParmUInt(pRepBestStep,"Include the variable assignment for each new best",&bReportBestStepVars,0);

  pRepTrajBestLM = CreateReport("tbestlm","Trajectory Best Local Minima","Very similar to (-r beststep), except it ignores best solutions that~are improved upon in the next step","stdout","ReportTrajBestLM");
  
  pRepOptClauses = CreateReport("unsatclauses","Unsatisfied Clauses","Print unsatisfied clauses when the run is complete~mostly useful when -target > 0 -- the format is:~1101111...~where clause 3 was unsatisfied when the run terminated","stdout","ReportUnsatClausesPrint");
  AddReportParmUInt(pRepOptClauses,"Only Include runs where -target solution was reached",&bReportOptClausesSol,0);

  pRepFalseHist = CreateReport("falsehist","Number of False Clauses Histogram","Prints a count (histogram) that shows the distribution~of the number of false clauses throughout the search","stdout","ReportFalseHistPrint");
  AddReportParmUInt(pRepFalseHist,"Only include data from the last N steps",&iReportFalseHistCount,0);

  pRepDistance = CreateReport("distance","Hamming Distance Information","Detailed information for each step of the Hamming Distance~to the closest known solution~Note: Requires solution file (-filesol)","stdout","ReportDistancePrint");
  AddReportParmUInt(pRepDistance,"Only print when in local minima",&bReportDistanceLMOnly,0);

  pRepDistHist = CreateReport("disthist","Hamming Distance Histogram","Prints a count (histogram) that shows the distribution~of the Hamming distance from the solution(s) throughout the search~Note: Requires solution file (-filesol)","stdout","ReportDistHistPrint");
  AddReportParmUInt(pRepDistHist,"Only include data from the last N steps",&iReportDistHistCount,0);

  pRepFlipCounts = CreateReport("flipcount","Variable Flip Count","Prints the number of times each variable was flipped~restarts are counted in the first column (nullflips)","stdout","ReportFlipCountsPrint");

  pRepBiasCounts = CreateReport("biascount","Variable Bias Count","Prints the number of steps each variable was true and false","stdout","ReportBiasCountsPrint");

  pRepUnsatCounts = CreateReport("unsatcount","Clause Unsatisfied Count","Prints the number of steps that each clause was unsatisfied","stdout","ReportUnsatCountsPrint");

  pRepVarLast = CreateReport("varlastflip","Step of Last Variable Flip","Prints the step of the last time each variable was flipped","stdout","ReportVarLastPrint");

  pRepClauseLast = CreateReport("clauselast","Step of Last Clause Unsatisfied","Prints the last step each clause was unsatisfied","stdout","ReportClauseLastPrint");

  pRepSQGrid = CreateReport("sqgrid","Solution Quality Grid","Prints the solution quality at key step values","stdout","ReportSQGridPrint");
  AddReportParmUInt(pRepSQGrid,"# of key step points per decade",&iLogDistStepsPerDecade,10);

  pRepPenalty = CreateReport("penalty","Clause Penalties","Prints the Clause penalties (weights) from DLS algorithms (i.e.: SAPS,PAWS)~Note: the renormalize to total overrides renormalize to base","stdout","ReportPenaltyPrint");
  AddReportParmUInt(pRepPenalty,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmUInt(pRepPenalty,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);
  AddReportParmUInt(pRepPenalty,"Print penalties every local min (0=end of run only)",&bReportPenaltyEveryLM,0);

  pRepPenMean = CreateReport("penmean","Clause Penalties (Mean)","Mean of clause penalties throughout the run","stdout","ReportPenMeanPrint");
  AddReportParmUInt(pRepPenMean,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmUInt(pRepPenMean,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);
  pRepPenStddev = CreateReport("penstddev","Clause Penalties (Stddev)","StdDev of clause penalties throughout the run","stdout","ReportPenStddevPrint");
  AddReportParmUInt(pRepPenStddev,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmUInt(pRepPenStddev,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);
  pRepPenCV = CreateReport("pencv","Clause Penalties (CV)","CV of clause penalties throughout the run","stdout","ReportPenCVPrint");
  AddReportParmUInt(pRepPenCV,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmUInt(pRepPenCV,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);

  pRepMobility = CreateReport("mobility","Mobility Report","Mean Hamming distances for windows of sizes 1..length~Parameter 2 will normalize output so 1 is maximum value","stdout","ReportMobilityPrint");
  AddReportParmUInt(pRepMobility,"Largest mobility window length to display [default = n]",&iReportMobilityDisplay,0);
  AddReportParmUInt(pRepMobility,"Normalize Mobility values: Mobility/min(n,windowsize)",&bReportMobilityNormalized,0);

  pRepMobFixed = CreateReport("mobfixed","Mobility For a Fixed Window Size","Provides step-by-step Mobility data for a fixed window size~1st parameter sets the size of the window,~2nd parameter includes or ignores(default) the first 1..size steps~both parameters affect column results (mobx, mobxcv, etc.)~to set parameters without running report, use~-r mobfixed null INT BOOL","stdout","ReportMobFixedPrint");
  AddReportParmUInt(pRepMobFixed,"Mobility window size [default = n]",&iMobFixedWindow,0);
  AddReportParmUInt(pRepMobFixed,"Include first (1..windowsize) steps",&bMobilityFixedIncludeStart,0);

  pRepMobFixedFreq = CreateReport("mobfixedfreq","Mobility Fixed Window Histogram","Mobility frequency counts for a fixed window size~must use the same parameter settings as (-r mobfixed)","stdout","ReportMobFixedFreqPrint");
  AddReportParmUInt(pRepMobFixedFreq,"Mobility window size [default = n]",&iMobFixedWindow,0);
  AddReportParmUInt(pRepMobFixedFreq,"Include first (1..windowsize) steps ",&bMobilityFixedIncludeStart,0);

  pRepAutoCorr = CreateReport("autocorr","Autocorrelation Report","Autocorrelation Length (ACL) and autocorrelation values for lengths 1..max~if the ACL is greater than max, it returns an ACL value of max~parameters affect column (acl) results~to set parameters without running report, use~-r autocorr null INT FL","stdout","ReportAutoCorrPrint");
  AddReportParmUInt(pRepAutoCorr,"Maximum window length [default = n]",&iAutoCorrMaxLen,0);
  AddReportParmFloat(pRepAutoCorr,"Cutoff Value for finding ACL [default = 1/e]",&fAutoCorrCutoff,0.3678794f);
  
  pRepTriggers = CreateReport("triggers","Trigger Report","Prints Out All Active Triggers","stdout","ReportTriggersPrint");
  AddReportParmUInt(pRepTriggers,"Show All Triggers (not just active) [default = 0]",&bReportTriggersAll,0);

  pRepSATComp = CreateReport("satcomp","SAT Competition","Prints required output for 2005 SAT Competition (use -solve)","stdout","ReportSatCompetitionPrint");

  /***************************************************************************/

  AddColumnComposite("default","run,found,best,beststep,steps");
  AddColumnComposite("default_w","run,found,best_w,beststep_w,steps");
  AddColumnComposite("rtd","prob,steps,timesteps");

  AddColumnUInt("run","Run Number",
    "     ",
    "  Run",
    "  No.",
    "%5u",
    &iRun,"",ColTypeFinal);


  AddColumnUInt("found","Target Solution Quality Found? (1 => yes)",
    "F",
    "N",
    "D",
    "%1u"
    ,&bSolutionFound,"",ColTypeFinal);


  AddColumnUInt("steps","Total Number of Search Steps",
    "     Total",
    "    Search",
    "     Steps",
    "%10u",
    &iStep,"",ColTypeFinal);

  AddStatCol("steps","Steps","mean+cv+median",TRUE);

  
  AddColumnFloat("time","Time in seconds, measured (not accurate for short runs)",
    "  CPU Time",
    "in seconds",
    "(measured)",
    "%10.6f",
    &fRunTime,"",ColTypeFinal);

  AddStatCol("time","MeasuredCPUTime","mean",FALSE);


  AddColumnUInt("best","Best (Lowest) # of False Clauses Found",
    " Best",
    "Sol'n",
    "Found",
    "%5u",
    &iNumFalse,"",ColTypeMin);

  AddStatCol("best","BestSolution","mean",FALSE);

  AddColumnFloat("best_w","Best Weighted Solution Quality Found",
    "        Best",
    "    Solution",
    "     Quality",
    "%12.8f",
    &fBestSumFalseW,"",ColTypeMin);

  AddStatCol("best_w","BestWeightedSolution","mean",FALSE);


  AddColumnUInt("worst","Worst (Highest) # of False Clauses Found",
    "Worst",
    "Sol'n",
    "Found",
    "%5u",
    &iNumFalse,"",ColTypeMax);

  AddStatCol("worst","WorstSolution","mean",FALSE);

  AddColumnFloat("worst_w","Worst Weighted Solution Quality Found",
    "       Worst",
    "    Solution",
    "     Quality",
    "%12.8f",
    &fSumFalseW,"",ColTypeMax);

  AddStatCol("worst_w","WorstWeightedSolution","mean",FALSE);


  AddColumnUInt("last","Last (on final step) # of False Clauses",
    " Last",
    "Sol'n",
    "Found",
    "%5u",
    &iNumFalse,"",ColTypeFinal);

  AddStatCol("last","LastSolution","mean",FALSE);

  AddColumnFloat("last_w","Last (on final step) Weighted Solution Quality",
    "        Last",
    "    Solution",
    "     Quality",
    "%12.8f",
    &fSumFalseW,"",ColTypeFinal);

  AddStatCol("last_w","LastWeightedSolution","mean",FALSE);


  AddColumnUInt("start","Start (on first step) # of False Clauses",
    "Start",
    "Sol'n",
    "Found",
    "%5u",
    &iStartNumFalse,"StartFalse",ColTypeFinal);

  AddStatCol("start","StartSolution","mean",FALSE);

  AddColumnFloat("start_w","Start (on first step) Weighted Solution Quality",
    "       Start",
    "    Solution",
    "     Quality",
    "%12.8f",
    &fStartSumFalseW,"StartFalse",ColTypeFinal);

  AddStatCol("start_w","StartWeightedSolution","mean",FALSE);


  AddColumnUInt("beststep","Step of Best (Lowest) # of False Clauses Found",
    "      Step",
    "        of",
    "      Best",
    "%10u",
    &iBestStepNumFalse,"BestFalse",ColTypeFinal);

  AddStatCol("beststep","BestStep","mean",FALSE);
  
  AddColumnUInt("beststep_w","Step of Best Weighted Solution Quality Found",
    "      Step",
    "        of",
    "    W Best",
    "%10u",
    &iBestStepSumFalseW,"BestFalse",ColTypeFinal);

  AddStatCol("beststep_w","BestWeightedStep","mean",FALSE);


  AddColumnFloat("bestavgimpr","Mean Improvement per Step to Best Solution",
    "  Average",
    " Improve.",
    "  To Best",
    "%9.7f",
    &fImproveMean,"ImproveMean",ColTypeFinal);

  AddStatCol("bestavgimpr","BestAvgImprovement","mean",FALSE);
  
  AddColumnFloat("bestavgimpr_w","Mean Improvement per Step to Best Solution Quality",
    "  Average",
    " Improve.",
    "To W Best",
    "%9.7f",
    &fImproveMeanW,"ImproveMean",ColTypeFinal);

  AddStatCol("bestavgimpr_w","BestWeightedAvgImprovement","mean",FALSE);


  AddColumnUInt("firstlm","First Local Minimum # of False Clauses",
    "False",
    "@ 1st",
    "L.Min",
    "%5u",
    &iFirstLM,"FirstLM",ColTypeFinal);

  AddStatCol("firstlm","FirstLocalMin","mean",FALSE);

  AddColumnFloat("firstlm_w","First Weighted Local Minimum Solution Quality",
    "    Solution",
    "     Quality",
    " @ 1st WLMin",
    "%12.8f",
    &fFirstLMW,"FirstLM",ColTypeFinal);

  AddStatCol("firstlm_w","FirstWeightedLocalMin","mean",FALSE);


  AddColumnUInt("firstlmstep","Step of the First Local Minimum Encountered",
    " Step",
    "of1st",
    "L.Min",
    "%5u",
    &iFirstLMStep,"FirstLM",ColTypeFinal);

  AddStatCol("firstlmstep","FirstLocalMinStep","mean",FALSE);

  AddColumnUInt("firstlmstep_w","Step of the First Weighted Local Minimum Encountered",
    " Step",
    "of1st",
    "WLMin",
    "%5u",
    &iFirstLMStepW,"FirstLM",ColTypeFinal);

  AddStatCol("firstlmstep_w","FirstWeightedLocalMinStep","mean",FALSE);


  AddColumnFloat("firstlmratio","Improvement from 1st LM: (start-firstlm)/(start-best)",
    "    First",
    "Local Min",
    "    Ratio",
    "%9.7f",
    &fFirstLMRatio,"FirstLMRatio",ColTypeFinal);

  AddStatCol("firstlmratio","FirstLocalMinRatio","mean",FALSE);

  AddColumnFloat("firstlmratio_w","Improve from 1st WLM: (startw - firstlmw)/(startw-bestw)",
    " First W.",
    "Local Min",
    "    Ratio",
    "%9.6f",
    &fFirstLMRatioW,"FirstLMRatio",ColTypeFinal);

  AddStatCol("firstlmratio_w","FirstWeightedLocalMinRatio","mean",FALSE);


  AddColumnFloat("tbestlmmean","Mean of the Trajectory Best LM (# False)",
    "  Traj.",
    "Best LM",
    "   Mean",
    "%7.3f",
    &fTrajBestLMMean,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmmean","TrajBestLocalMinMean","mean",FALSE);

  AddColumnFloat("tbestlmmean_w","Mean of the Trajectory Best LM (W.Sol'n Quality)",
    "  Traj.",
    "Best LM",
    "   Mean",
    "%7.3f",
    &fTrajBestLMMeanW,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmmean_w","TrajBestLocalMinWeightedMean","mean",FALSE);

  AddColumnFloat("tbestlmcv","C.V. of the Trajectory Best LM  (# False)",
    "  Traj.",
    "Best LM",
    "   C.V.",
    "%7.5f",
    &fTrajBestLMCV,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmcv","TrajBestLocalMinCV","mean",FALSE);
  
  AddColumnFloat("tbestlmcv_w","C.V. of the Trajectory Best LM (W.Sol'n Quality)",
    "  Traj.",
    "Best LM",
    "   C.V.",
    "%7.5f",
    &fTrajBestLMCVW,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmcv_w","TrajBestLocalMinWeightedCV","mean",FALSE);

  AddColumnUInt("qualmean","Average (Mean) # of False Clauses",
    "Average",
    "   # of",
    "  False",
    "%7.3f",
    &iNumFalse,"",ColTypeMean);

  AddStatCol("qualmean","SolutionQualityMean","mean",FALSE);
  
  AddColumnFloat("qualmean_w","Average (Mean) of Weighted Solution Quality",
    "Average",
    "W.Sol'n",
    "Quality",
    "%7.3f",
    &fSumFalseW,"",ColTypeMean);

  AddStatCol("qualmean_w","WeightedSolutionQualityMean","mean",FALSE);
  
  
  AddColumnUInt("qualstddev","Std.Dev. # of False Clauses",
    "Std.Dev",
    "   # of",
    "  False",
    "%7.3f",
    &iNumFalse,"",ColTypeStddev);

  AddStatCol("qualstddev","SolutionQualityStdDev","mean",FALSE);
  
  AddColumnFloat("qualstddev_w","Std.Dev. of Weighted Solution Quality",
    "Std.Dev",
    "W.Sol'n",
    "Quality",
    "%7.3f",
    &fSumFalseW,"",ColTypeStddev);

  AddStatCol("qualstddev_w","WeightedSolutionQualityStdDev","mean",FALSE);

  AddColumnUInt("qualcv","Coeff. of Var. # of False Clauses",
    "   C.V.",
    "   # of",
    "  False",
    "%7.3f",
    &iNumFalse,"",ColTypeCV);

  AddStatCol("qualcv","SolutionQualityCV","mean",FALSE);
  
  AddColumnFloat("qualcv_w","Coeff. of Var. of Weighted Solution Quality",
    "   C.V.",
    "W.Sol'n",
    "Quality",
    "%7.3f",
    &fSumFalseW,"",ColTypeCV);

  AddStatCol("qualcv_w","WeightedSolutionQualityCV","mean",FALSE);

  
  AddColumnUInt("seed","Starting Seed For the Run",
    "          ",
    "  Starting",
    "      Seed",
    "%10u",
    &iStartSeed,"StartSeed",ColTypeFinal);

  
  AddColumnUInt("nullflips","Number of Null Flips",
    "    Number",
    "   of Null",
    "     Flips",
    "%10u",
    &iNumNullFlips,"NullFlips",ColTypeFinal);

  AddStatCol("nullflips","NullFlips","mean",FALSE);
  
  AddColumnUInt("percentnull","Percent of Null Flips",
    "Percent",
    "   Null",
    "  Flips",
    "%7.4f",
    &iNumNullFlips,"NullFlips",ColTypeFinalDivStep100);

  AddStatCol("percentnull","PercentNull","mean",FALSE);


  AddColumnUInt("restarts","Number of Restarts",
    "  Number",
    "      of",
    "Restarts",
    "%8u",
    &iNumRestarts,"NumRestarts",ColTypeFinal);

  AddStatCol("restarts","NumRestarts","mean",FALSE);


  AddColumnUInt("rand","Number of Random Decisions",
    "    Number",
    " of Random",
    " Decisions",
    "%10u",
    &iNumRandomCalls,"CountRandom",ColTypeFinal);

  AddStatCol("rand","NumRandomDecisions","mean",FALSE);

  AddColumnUInt("randstep","Mean Number of Random Decisions Per Step",
    "Avg. Rand",
    "Decisions",
    " per step",
    "%9.3f",
    &iNumRandomCalls,"CountRandom",ColTypeFinalDivStep);

  AddStatCol("randstep","RandomDecisionsPerStep","mean",FALSE);


  AddColumnUInt("candidates","Mean Number of Flip Candidates (not all algs support it)",
    " Mean #",
    "of Flip",
    " Candid",
    "%7.4f",
    &iNumCandidates,"",ColTypeMean);

  AddStatCol("candidates","FlipCandidatesMean","mean",FALSE);


  AddColumnUInt("localmins","Number of Local Minima Encountered",
    "    Number",
    "  of Local",
    "    Minima",
    "%10u",
    &iNumLocalMins,"LocalMins",ColTypeFinal);

  AddStatCol("localmins","LocalMins","mean",FALSE);

  AddColumnUInt("percentlocal","Percent of Steps in Local Minima",
    "Percent",
    "  Local",
    " Minima",
    "%7.4f",
    &iNumLocalMins,"LocalMins",ColTypeFinalDivStep100);

  AddStatCol("percentlocal","PercentLocal","mean",FALSE);

  AddColumnFloat("flipcountcv","CV of the Variable Flip Count Distribution",
    "  CV of",
    "   Flip",
    "  Count",
    "%7.5f",
    &fFlipCountsCV,"FlipCountStats",ColTypeFinal);

  AddStatCol("flipcountcv","FlipCountCV","mean",FALSE);

  AddColumnFloat("biasmax","Mean Frac. # steps vars spent same state as their most freq.",
    "   Max",
    "  Pol-",
    " arity",
    "%5.4f",
    &fMeanMaxBias,"BiasStats",ColTypeFinal);

  AddStatCol("biasmax","BiasMaxMean","mean",FALSE);

  AddColumnFloat("biasfinal","Mean Frac. # steps vars spent same as their final state",
    " Final",
    "  Pol-",
    " arity",
    "%5.4f",
    &fMeanFinalBias,"BiasStats",ColTypeFinal);

  AddStatCol("biasfinal","BiasFinalMean","mean",FALSE);

  AddColumnFloat("unsatcountcv","CV of the Clause Unsat Count Distribution",
    "  CV of",
    "  Unsat",
    "  Count",
    "%7.5f",
    &fUnsatCountsCV,"UnsatCountStats",ColTypeFinal);

  AddStatCol("unsatcountcv","UnsatCountCV","mean",FALSE);


  AddColumnUInt("soldistance","Mean Hamming Distance from Closest Known Solution(s)",
    "   Mean",
    "Dist To",
    "kn.soln",
    "%7.2f",
    &iSolutionDistance,"SolutionDistance",ColTypeMean);

  AddStatCol("soldistance","SolutionDistanceMean","mean",FALSE);


  AddColumnFloat("fdc","Fitness-Distance Correlation Factor (calc. in LM)",
    "Fitness",
    "Distnce",
    "   Calc",
    "%7.3f",
    &fFDCRun,"FDCRun",ColTypeFinal);

  AddStatCol("fdc","FDC","mean",FALSE);

  
  AddColumnUInt("acl","Auto-Correlation Length",
    "Auto",
    "Corr",
    "Len.",
    "%4u",
    &iAutoCorrLen,"AutoCorr",ColTypeFinal);

  AddStatCol("acl","ACL","mean",FALSE);


  AddColumnFloat("acone","Auto-Correlation of distance one",
    "  Auto-",
    "  Corr.",
    "  (one)",
    "%7.5f",
    &fAutoCorrOneVal,"AutoCorrOne",ColTypeFinal);

  AddStatCol("acone","ACOne","mean",FALSE);


  AddColumnFloat("estacl","Estimated Auto-Correlation Length from AC of 1",
    "       ",
    "   Est.",
    "    ACL",
    "%7.2f",
    &fAutoCorrOneEst,"AutoCorrOne",ColTypeFinal);

  AddStatCol("estacl","EstACL","mean",FALSE);


  AddColumnFloat("branchfact","Mean Branching Factor (# possible sidesteps / #vars)",
    "  Mean",
    "Branch",
    "  Fact",
    "%6.4f",
    &fBranchFactor, "BranchFactor", ColTypeMean);

  AddStatCol("branchfact","BranchFactorMean","mean",FALSE);

  AddColumnFloat("branchfact_w","Mean Weighted Branching Factor",
    "W.Mean",
    "Branch",
    "  Fact",
    "%6.4f",
    &fBranchFactorW, "BranchFactorW", ColTypeMean);

  AddStatCol("branchfact_w","WeightedBranchFactorMean","mean",FALSE);


  AddColumnFloat("mobn","Mean Mobility for window size n (# of vars)",
    "Mobility",
    "Window N",
    "    Mean",
    "%8.3f",
    &fMobilityColNMean,"MobilityColumn,InitMobilityColumnN",ColTypeFinal);

  AddStatCol("mobn","MobilitySizeNMean","mean",FALSE);


  AddColumnFloat("mobx","Mean Mobility for window size x (see -r mobfixed)",
    "Mobility",
    "Window X",
    "    Mean",
    "%8.3f",
    &fMobilityColXMean,"MobilityColumn,InitMobilityColumnX",ColTypeFinal);

  AddStatCol("mobx","MobilitySizeXMean","mean",FALSE);


  AddColumnFloat("normmobn","Normalized Mean Mobility for window size n (# of vars) ",
    "Norm Mob",
    "Window N",
    "    Mean",
    "%8.6f",
    &fMobilityColNMeanNorm,"MobilityColumn,InitMobilityColumnN",ColTypeFinal);

  AddStatCol("normmobn","NormalizedMobilitySizeN","mean",FALSE);


  AddColumnFloat("normmobx","Normalized Mean Mobility for window size x (see -r mobfixed)",
    "Norm Mob",
    "Window X",
    "    Mean",
    "%8.6f",
    &fMobilityColXMeanNorm,"MobilityColumn,InitMobilityColumnX",ColTypeFinal);

  AddStatCol("normmobx","NormalizedMobilitySizeXMean","mean",FALSE);

  
  AddColumnFloat("mobncv","Mobility C.V. for window size n (# of vars)",
    "Mobility",
    "Window N",
    "    C.V.",
    "%8.6f",
    &fMobilityColNCV,"MobilityColumn,InitMobilityColumnN",ColTypeFinal);

  AddStatCol("mobncv","MobilitySizeNMean","mean",FALSE);


  AddColumnFloat("mobxcv","Mobility C.V. for window size x (see -r mobfixed)",
    "Mobility",
    "Window X",
    "    C.V.",
    "%8.5f",
    &fMobilityColXCV,"MobilityColumn,InitMobilityColumnX",ColTypeFinal);

  AddStatCol("mobxcv","MobilitySizeXMean","mean",FALSE);


  AddColumnUInt("upsteps","Number of Up (Backward) Steps",
    "    Number",
    "     of Up",
    "     Steps",
    "%10u",
    &iNumUpSteps,"StepsUpDownSide",ColTypeFinal);

  AddColumnUInt("upsteps_w","Number of Up (Backward) Weighted Steps",
    "    Number",
    "     of Up",
    "  W. Steps",
    "%10u",
    &iNumUpStepsW,"StepsUpDownSide",ColTypeFinal);

  AddColumnUInt("percentup","Percent of Up (Backward) Steps",
    "Percent",
    "  of Up",
    "  Steps",
    "%7.4f",
    &iNumUpSteps,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentup","PercentUpSteps","mean",FALSE);

  AddColumnUInt("percentup_w","Percent of Up (Backward) Weighted Steps",
    "Percent",
    "  of Up",
    "W.Steps",
    "%7.4f",
    &iNumUpStepsW,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentup_w","PercentWeightedUpSteps","mean",FALSE);


  AddColumnUInt("downsteps","Number of Down (Improving) Steps",
    "    Number",
    "   of Down",
    "     Steps",
    "%10u",
    &iNumDownSteps,"StepsUpDownSide",ColTypeFinal);

  AddColumnUInt("downsteps_w","Number of Down (Improving) Weighted Steps",
    "    Number",
    "   of Down",
    "   W.Steps",
    "%10u",
    &iNumDownStepsW,"StepsUpDownSide",ColTypeFinal);

  AddColumnUInt("percentdown","Percent of Down (Improving) Steps",
    "Percent",
    "of Down",
    "  Steps",
    "%7.4f",
    &iNumDownSteps,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentdown","PercentUpSteps","mean",FALSE);
  
  AddColumnUInt("percentdown_w","Percent of Down (Improving) Weighted Steps",
    "Percent",
    "of Down",
    "W.Steps",
    "%7.4f",
    &iNumDownStepsW,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentdown_w","PercentWeightedUpSteps","mean",FALSE);


  AddColumnUInt("sidesteps","Number of Sideways (Plateau) Steps",
    " Number of",
    "  Sideways",
    "     Steps",
    "%10u",
    &iNumSideSteps,"StepsUpDownSide",ColTypeFinal);

  AddColumnUInt("sidesteps_w","Number of Sideways (Plateau) Weighted Steps",
    " Number of",
    "  Sideways",
    "  W. Steps",
    "%10u",
    &iNumSideStepsW,"StepsUpDownSide",ColTypeFinal);

  AddColumnUInt("percentside","Percent of Sideways (Plateau) Steps",
    "Percent",
    "Sideway",
    "  Steps",
    "%7.4f",
    &iNumSideSteps,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentside","PercentSideSteps","mean",FALSE);

  AddColumnUInt("percentside_w","Percent of Sideways (Plateau) Weighted Steps",
    "Percent",
    "Sideway",
    "W.Steps",
    "%7.4f",
    &iNumSideStepsW,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentside_w","PercentWeightedSideSteps","mean",FALSE);

  
  
  AddColumnFloat("timesteps","Time in seconds, calculated based on steps (RTD Only)",
    "  CPU Time",
    "in seconds",
    "(by steps)",
    "%10.6f",
    &fDummy,"UpdateTimes,ActivateStepsFoundColumns",ColTypeFinal);

  AddStatCol("timesteps","CPUTime","mean+cv+median",TRUE);

  AddColumnFloat("prob","Probability of Success (RTD Only)",
    "   Prob",
    "     of",
    "Success",
    "%7.5f",
    &fDummy,"UpdatePercents",ColTypeFinal);


  /***************************************************************************/
    
  AddContainerStat("default","instinfo,totaltime,fps,runs,numsolve,percentsolve,steps,timesteps");

  AddStatCustom("numclauses","Number of Clauses","Clauses","%u",(void *)&iNumClauses,DTypeUInt,"","");
  AddStatCustom("numvars","Number of Variables","Variables","%u",(void *)&iNumVars,DTypeUInt,"","");
  AddStatCustom("numlits","Number of Total Literals","TotalLiterals","%u",(void *)&iNumLits,DTypeUInt,"","");
  AddContainerStat("instinfo","numvars,numclauses,numlits");

  AddStatCustom("runs","Runs Executed","RunsExecuted","%u",(void *)&iRun,DTypeUInt,"","");

  AddStatCustom("numsolve","Number of Successful Runs","SuccessfulRuns","%u",(void *)&iNumSolutionsFound,DTypeUInt,"","");

  AddStatCustom("percentsolve","Percent of Successful Runs","PercentSuccess","%-6.2f",(void *)&fPercentSuccess,DTypeFloat,"","CalcPercentSolve");
  
  AddStatCustom("totaltime","Total Time Elapsed","TotalCPUTimeElapsed","%-10.3f",(void *)&fTotalTime,DTypeFloat,"","");

  AddStatCustom("fps","Flips Per Second","FlipsPerSecond","%-7.f",(void *)&fFlipsPerSecond,DTypeFloat,"","CalcFPS");

  AddStatCustom("numunique","Number of Unique Solutions Found","UniqueSolutions","%u",(void *)&iNumUniqueSolutions,DTypeUInt,"","UniqueSolutions");

  AddStatCustom("instname","File Name of the Instance","InstFileName","%s",(void *)&sFilenameIn,DTypeString,"","");

  AddStatCustom("alginfo","Algorithm Parameters","AlgParameters","%s",(void *)&sStringAlgParms,DTypeString,"","StringAlgParms");

  AddStatCustom("version","UBCSAT Version Information","UBCSATVersion","%s",(void *)&pVersion,DTypeString,"","");

}

