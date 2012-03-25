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
REPORT *pRepVW2Weights;
REPORT *pRepMobility;
REPORT *pRepMobFixed;
REPORT *pRepMobFixedFreq;
REPORT *pRepVarAgeFreq;
REPORT *pRepAutoCorr;
REPORT *pRepTriggers;
REPORT *pRepParamILS;
REPORT *pRepSATComp;
REPORT *pRepMAXSATComp;

void AddReports() {

  pRepOut = CreateReport("out", "Standard Output with Columns","Prints an array of data, with each row representing an independent run~you can customize the report to include a variety of columns","stdout","ReportOut");
  AddReportParmString(pRepOut,"Columns to Display (ubcsat -hc for info)","default");
  AddReportParmBool(pRepOut,"Delay: Set to 1 to print after all runs are complete",&bReportOutputSuppress,0);
  pRepOut->bDefaultActivate = 1;

  pRepStats = CreateReport("stats","Statistics Report","Shows summary statistics from all runs~you can customize the report to include a variety of statistics","stdout","ReportStats");
  AddReportParmString(pRepStats,"Statistics to Display (ubcsat -hs for info)","default");
  pRepStats->bDefaultActivate = 1;

  pRepRTD = CreateReport("rtd","Run-Length and Run-Time Distribution","Similar to (-r out), except results are sorted by search steps,~and include successful runs~you can customize the report to include a variety of columns", "stdout","ReportRTD,SortByStepPerformance");
  AddReportParmString(pRepRTD,"Columns to Display (ubcsat -hc for info)","rtd");

  pRepModel = CreateReport("model","Solution Model","Prints the solution model found from the last run executed~is turned on automatically with the -solve parameter~output format example is: -1 2 3 -4 ... ~meaning variables (1,4) are true, and variables (2,3) are false","stdout","ReportModelPrint");

  pRepCNFStats = CreateReport("cnfstats","Instance Statistics","Provides additional information about the properties of the instance","stdout","ReportCNFStatsPrint");
  
  pRepState = CreateReport("state","State Information Trace","Provides detailed state information for each search step, including:~the solution quality, the variable flipped, and the state of all variables","stdout","ReportStatePrint");
  AddReportParmBool(pRepState,"Only print when in local minima",&bReportStateLMOnly,0);

  pRepSolution = CreateReport("solution","Solutions","Prints solutions for every successful run in the format:~1001001101...~where variable 1 is true, 2 is false, 3 is false, etc...","stdout","ReportSolutionPrint");

  pRepUniqueSol = CreateReport("uniquesol","Unique Solutions","Prints similar to (-r solution), except only unique solutions (no repeats)","stdout","ReportUniqueSolPrint");

  pRepBestSol = CreateReport("bestsol","Best Candidate Solutions","Similar to (-r solution), but includes variable assignments for the~best solution quality encountered when no solution is found","stdout","ReportBestSolPrint");

  pRepBestStep = CreateReport("beststep","Steps of Each New Best Solution Quality","Print step information for each new best solution encountered","stdout","ReportBestStepPrint");
  AddReportParmBool(pRepBestStep,"Include the variable assignment for each new best",&bReportBestStepVars,0);

  pRepTrajBestLM = CreateReport("tbestlm","Trajectory Best Local Minima","Very similar to (-r beststep), except it ignores best solutions that~are improved upon in the next step","stdout","ReportTrajBestLM");
  
  pRepOptClauses = CreateReport("unsatclauses","Unsatisfied Clauses","Print unsatisfied clauses when the run is complete~mostly useful when -target > 0 -- the format is:~1101111...~where clause 3 was unsatisfied when the run terminated","stdout","ReportUnsatClausesPrint");
  AddReportParmBool(pRepOptClauses,"Only Include runs where -target solution was reached",&bReportOptClausesSol,0);

  pRepFalseHist = CreateReport("falsehist","Number of False Clauses Histogram","Prints a count (histogram) that shows the distribution~of the number of false clauses throughout the search","stdout","ReportFalseHistPrint");
  AddReportParmUInt(pRepFalseHist,"Only include data from the last N steps",&iReportFalseHistCount,0);

  pRepDistance = CreateReport("distance","Hamming Distance Information","Detailed information for each step of the Hamming Distance~to the closest known solution~Note: Requires solution file (-filesol)","stdout","ReportDistancePrint");
  AddReportParmBool(pRepDistance,"Only print when in local minima",&bReportDistanceLMOnly,0);

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
  AddReportParmBool(pRepPenalty,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmBool(pRepPenalty,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);
  AddReportParmBool(pRepPenalty,"Print penalties every local min (0=end of run only)",&bReportPenaltyEveryLM,0);

  pRepPenMean = CreateReport("penmean","Clause Penalties (Mean)","Mean of clause penalties throughout the run","stdout","ReportPenMeanPrint");
  AddReportParmBool(pRepPenMean,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmBool(pRepPenMean,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);
  pRepPenStddev = CreateReport("penstddev","Clause Penalties (Stddev)","StdDev of clause penalties throughout the run","stdout","ReportPenStddevPrint");
  AddReportParmBool(pRepPenStddev,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmBool(pRepPenStddev,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);
  pRepPenCV = CreateReport("pencv","Clause Penalties (CV)","CV of clause penalties throughout the run","stdout","ReportPenCVPrint");
  AddReportParmBool(pRepPenCV,"Re-normalise to base so an untouched clause is equal to 1",&bReportPenaltyReNormBase,1);
  AddReportParmBool(pRepPenCV,"Re-normalise to total so sum of all penalties equals 1",&bReportPenaltyReNormFraction,0);

  pRepVW2Weights = CreateReport("vw2weights","VW2 Weights","Prints the VW2 weights at the end of the run","stdout","ReportVW2WeightsPrint");

  pRepMobility = CreateReport("mobility","Mobility Report","Mean Hamming distances for windows of sizes 1..length~Parameter 2 will normalize output so 1 is maximum value","stdout","ReportMobilityPrint");
  AddReportParmUInt(pRepMobility,"Largest mobility window length to display [default = n]",&iReportMobilityDisplay,0);
  AddReportParmBool(pRepMobility,"Normalize Mobility values: Mobility/min(n,windowsize)",&bReportMobilityNormalized,0);

  pRepMobFixed = CreateReport("mobfixed","Mobility For a Fixed Window Size","Provides step-by-step Mobility data for a fixed window size~1st parameter sets the size of the window,~2nd parameter includes or ignores(default) the first 1..size steps~both parameters affect column results (mobx, mobxcv, etc.)~to set parameters without running report, use~-r mobfixed null INT BOOL","stdout","ReportMobFixedPrint");
  AddReportParmUInt(pRepMobFixed,"Mobility window size [default = n]",&iMobFixedWindow,0);
  AddReportParmBool(pRepMobFixed,"Include first (1..windowsize) steps",&bMobilityFixedIncludeStart,0);

  pRepMobFixedFreq = CreateReport("mobfixedfreq","Mobility Fixed Window Histogram","Mobility frequency counts for a fixed window size~must use the same parameter settings as (-r mobfixed)","stdout","ReportMobFixedFreqPrint");
  AddReportParmUInt(pRepMobFixedFreq,"Mobility window size [default = n]",&iMobFixedWindow,0);
  AddReportParmBool(pRepMobFixedFreq,"Include first (1..windowsize) steps ",&bMobilityFixedIncludeStart,0);

  pRepVarAgeFreq = CreateReport("varagefreq","Variable Flip Age Histogram","Number of times each variable age occured while flipping","stdout","ReportVarAgeFreqPrint");
  AddReportParmUInt(pRepVarAgeFreq,"Format: 0=counts, 1=percent, 2=CDF",&iRepVarAgeFreqFormat,0);
  AddReportParmUInt(pRepVarAgeFreq,"Mobility window size [default = n]",&iMaxVarAgeFrequency,0);

  pRepAutoCorr = CreateReport("autocorr","Autocorrelation Report","Autocorrelation Length (ACL) and autocorrelation values for lengths 1..max~if the ACL is greater than max, it returns an ACL value of max~parameters affect column (acl) results~to set parameters without running report, use~-r autocorr null INT FL","stdout","ReportAutoCorrPrint");
  AddReportParmUInt(pRepAutoCorr,"Maximum window length [default = n]",&iAutoCorrMaxLen,0);
  AddReportParmFloat(pRepAutoCorr,"Cutoff Value for finding ACL [default = 1/e]",&fAutoCorrCutoff,0.3678794f);
  
  pRepTriggers = CreateReport("triggers","Trigger Report","Prints Out All Active Triggers","stdout","ReportTriggersPrint");
  AddReportParmBool(pRepTriggers,"Show All Triggers (not just active) [default = 0]",&bReportTriggersAll,0);

  pRepParamILS = CreateReport("paramils","Output for ParamILS","Prints required output ParamILS~(use -r out null and -r stats null)~see http://www.cs.ubc.ca/labs/beta/Projects/ParamILS/","stdout","ReportParamILSPrint");

  pRepSATComp = CreateReport("satcomp","SAT Competition","Prints required output for 2005 SAT Competition (use -solve)","stdout","ReportSatCompetitionPrint,ReportCompetitionComment");

  pRepMAXSATComp = CreateReport("maxsatcomp","MAXSAT Competition","Prints required output for 2012 SAT Competition","stdout","ReportMaxSatCompetitionPrint,ReportCompetitionComment");
  AddReportParmFloat(pRepMAXSATComp,"if > 0, print sol'n (if new) every n seconds [default = 90]",&fMaxSatPrintInterval,90.0f);

  /***************************************************************************/

  AddColumnComposite("default","run,found,best,beststep,steps");
  AddColumnComposite("default_w","run,found,best_w,beststep_w,steps");
  AddColumnComposite("rtd","prob,steps,timesteps");

  AddColumnUInt("run","Run Number",
    "     ",
    "  Run",
    "  No.",
    "%5"P32,
    &iRun,"",ColTypeFinal);


  AddColumnUInt("found","Target Solution Quality Found? (1 => yes)",
    "F",
    "N",
    "D",
    "%1"P32,
    &iColSolutionFound,"UpdateColSolutionFound",ColTypeFinal);


  AddColumnUBigInt("steps","Total Number of Search Steps",
    "               Total",
    "              Search",
    "               Steps",
    "%20"P64,
    &iStep,"",ColTypeFinal);

  AddStatCol("steps","Steps","mean+cv+median",1);

  
  AddColumnFloat("time","Time in seconds, measured (not accurate for short runs)",
    "  CPU Time",
    "in seconds",
    "(measured)",
    "%10.6f",
    &fRunTime,"",ColTypeFinal);

  AddStatCol("time","MeasuredCPUTime","mean",0);


  AddColumnUInt("best","Best (Lowest) # of False Clauses Found",
    " Best",
    "Sol'n",
    "Found",
    "%5"P32,
    &iNumFalse,"",ColTypeMin);

  AddStatCol("best","BestSolution","mean",0);

  AddColumnUBigInt("best_w","Best Weighted Solution Quality Found",
    "                Best",
    "            Solution",
    "             Quality",
    "%20"P64,
    &iBestSumFalseWeight,"",ColTypeMin);

  AddStatCol("best_w","BestWeightedSolution","mean",0);


  AddColumnUInt("worst","Worst (Highest) # of False Clauses Found",
    "Worst",
    "Sol'n",
    "Found",
    "%5"P32,
    &iNumFalse,"",ColTypeMax);

  AddStatCol("worst","WorstSolution","mean",0);

  AddColumnUBigInt("worst_w","Worst Weighted Solution Quality Found",
    "               Worst",
    "            Solution",
    "             Quality",
    "%20"P64,
    &iSumFalseWeight,"",ColTypeMax);

  AddStatCol("worst_w","WorstWeightedSolution","mean",0);


  AddColumnUInt("last","Last (on final step) # of False Clauses",
    " Last",
    "Sol'n",
    "Found",
    "%5"P32,
    &iNumFalse,"",ColTypeFinal);

  AddStatCol("last","LastSolution","mean",0);

  AddColumnUBigInt("last_w","Last (on final step) Weighted Solution Quality",
    "               Last",
    "            Solution",
    "             Quality",
    "%20"P64,
    &iSumFalseWeight,"",ColTypeFinal);

  AddStatCol("last_w","LastWeightedSolution","mean",0);


  AddColumnUInt("start","Start (on first step) # of False Clauses",
    "Start",
    "Sol'n",
    "Found",
    "%5"P32,
    &iStartNumFalse,"StartFalse",ColTypeFinal);

  AddStatCol("start","StartSolution","mean",0);

  AddColumnUBigInt("start_w","Start (on first step) Weighted Solution Quality",
    "               Start",
    "            Solution",
    "             Quality",
    "%20"P64,
    &iStartSumFalseWeight,"StartFalse",ColTypeFinal);

  AddStatCol("start_w","StartWeightedSolution","mean",0);


  AddColumnUBigInt("beststep","Step of Best (Lowest) # of False Clauses Found",
    "                Step",
    "                  of",
    "                Best",
    "%20"P64,
    &iBestStepNumFalse,"BestFalse",ColTypeFinal);

  AddStatCol("beststep","BestStep","mean",0);
  
  AddColumnUBigInt("beststep_w","Step of Best Weighted Solution Quality Found",
    "                Step",
    "                  of",
    "              W Best",
    "%20"P64,
    &iBestStepSumFalseWeight,"BestFalse",ColTypeFinal);

  AddStatCol("beststep_w","BestWeightedStep","mean",0);


  AddColumnFloat("bestavgimpr","Mean Improvement per Step to Best Solution",
    "  Average",
    " Improve.",
    "  To Best",
    "%9.7f",
    &fImproveMean,"ImproveMean",ColTypeFinal);

  AddStatCol("bestavgimpr","BestAvgImprovement","mean",0);
  
  AddColumnFloat("bestavgimpr_w","Mean Improvement per Step to Best Solution Quality",
    "  Average",
    " Improve.",
    "To W Best",
    "%9.7f",
    &fImproveMeanW,"ImproveMean",ColTypeFinal);

  AddStatCol("bestavgimpr_w","BestWeightedAvgImprovement","mean",0);


  AddColumnUInt("firstlm","First Local Minimum # of False Clauses",
    "False",
    "@ 1st",
    "L.Min",
    "%5"P32,
    &iFirstLM,"FirstLM",ColTypeFinal);

  AddStatCol("firstlm","FirstLocalMin","mean",0);

  AddColumnUBigInt("firstlm_w","First Weighted Local Minimum Solution Quality",
    "            Solution",
    "             Quality",
    " @ 1st Weighted LMin",
    "%20"P64,
    &iFirstLMWeight,"FirstLM",ColTypeFinal);

  AddStatCol("firstlm_w","FirstWeightedLocalMin","mean",0);


  AddColumnUBigInt("firstlmstep","Step of the First Local Minimum Encountered",
    " Step",
    "of1st",
    "L.Min",
    "%5"P64,
    &iFirstLMStep,"FirstLM",ColTypeFinal);

  AddStatCol("firstlmstep","FirstLocalMinStep","mean",0);

  AddColumnUBigInt("firstlmstep_w","Step of the First Weighted Local Minimum Encountered",
    " Step",
    "of1st",
    "WLMin",
    "%5"P64,
    &iFirstLMStepW,"FirstLM",ColTypeFinal);

  AddStatCol("firstlmstep_w","FirstWeightedLocalMinStep","mean",0);


  AddColumnFloat("firstlmratio","Improvement from 1st LM: (start-firstlm)/(start-best)",
    "    First",
    "Local Min",
    "    Ratio",
    "%9.7f",
    &fFirstLMRatio,"FirstLMRatio",ColTypeFinal);

  AddStatCol("firstlmratio","FirstLocalMinRatio","mean",0);

  AddColumnFloat("firstlmratio_w","Improve from 1st WLM: (startw - firstlmw)/(startw-bestw)",
    " First W.",
    "Local Min",
    "    Ratio",
    "%9.6f",
    &fFirstLMRatioW,"FirstLMRatio",ColTypeFinal);

  AddStatCol("firstlmratio_w","FirstWeightedLocalMinRatio","mean",0);


  AddColumnFloat("tbestlmmean","Mean of the Trajectory Best LM (# False)",
    "  Traj.",
    "Best LM",
    "   Mean",
    "%7.3f",
    &fTrajBestLMMean,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmmean","TrajBestLocalMinMean","mean",0);

  AddColumnFloat("tbestlmmean_w","Mean of the Trajectory Best LM (W.Sol'n Quality)",
    "  Traj.",
    "Best LM",
    "   Mean",
    "%7.3f",
    &fTrajBestLMMeanW,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmmean_w","TrajBestLocalMinWeightedMean","mean",0);

  AddColumnFloat("tbestlmcv","C.V. of the Trajectory Best LM  (# False)",
    "  Traj.",
    "Best LM",
    "   C.V.",
    "%7.5f",
    &fTrajBestLMCV,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmcv","TrajBestLocalMinCV","mean",0);
  
  AddColumnFloat("tbestlmcv_w","C.V. of the Trajectory Best LM (W.Sol'n Quality)",
    "  Traj.",
    "Best LM",
    "   C.V.",
    "%7.5f",
    &fTrajBestLMCVW,"TrajBestLM",ColTypeFinal);

  AddStatCol("tbestlmcv_w","TrajBestLocalMinWeightedCV","mean",0);

  AddColumnUInt("qualmean","Average (Mean) # of False Clauses",
    "Average",
    "   # of",
    "  False",
    "%7.3f",
    &iNumFalse,"",ColTypeMean);

  AddStatCol("qualmean","SolutionQualityMean","mean",0);
  
  AddColumnUBigInt("qualmean_w","Average (Mean) of Weighted Solution Quality",
    "Average",
    "W.Sol'n",
    "Quality",
    "%7.1f",
    &iSumFalseWeight,"",ColTypeMean);

  AddStatCol("qualmean_w","WeightedSolutionQualityMean","mean",0);
  
  
  AddColumnUInt("qualstddev","Std.Dev. # of False Clauses",
    "Std.Dev",
    "   # of",
    "  False",
    "%7.3f",
    &iNumFalse,"",ColTypeStddev);

  AddStatCol("qualstddev","SolutionQualityStdDev","mean",0);
  
  AddColumnUBigInt("qualstddev_w","Std.Dev. of Weighted Solution Quality",
    "Std.Dev",
    "W.Sol'n",
    "Quality",
    "%7.3f",
    &iSumFalseWeight,"",ColTypeStddev);

  AddStatCol("qualstddev_w","WeightedSolutionQualityStdDev","mean",0);

  AddColumnUInt("qualcv","Coeff. of Var. # of False Clauses",
    "   C.V.",
    "   # of",
    "  False",
    "%7.3f",
    &iNumFalse,"",ColTypeCV);

  AddStatCol("qualcv","SolutionQualityCV","mean",0);
  
  AddColumnUBigInt("qualcv_w","Coeff. of Var. of Weighted Solution Quality",
    "   C.V.",
    "W.Sol'n",
    "Quality",
    "%7.3f",
    &iSumFalseWeight,"",ColTypeCV);

  AddStatCol("qualcv_w","WeightedSolutionQualityCV","mean",0);

  
  AddColumnUInt("seed","Starting Seed For the Run",
    "          ",
    "  Starting",
    "      Seed",
    "%10"P32,
    &iStartSeed,"StartSeed",ColTypeFinal);

  
  AddColumnUBigInt("nullflips","Number of Null Flips",
    "              Number",
    "             of Null",
    "               Flips",
    "%20"P64,
    &iNumNullFlips,"NullFlips",ColTypeFinal);

  AddStatCol("nullflips","NullFlips","mean",0);
  
  AddColumnUBigInt("percentnull","Percent of Null Flips",
    "Percent",
    "   Null",
    "  Flips",
    "%7.4f",
    &iNumNullFlips,"NullFlips",ColTypeFinalDivStep100);

  AddStatCol("percentnull","PercentNull","mean",0);


  AddColumnUInt("restarts","Number of Restarts",
    "  Number",
    "      of",
    "Restarts",
    "%8"P32,
    &iNumRestarts,"NumRestarts",ColTypeFinal);

  AddStatCol("restarts","NumRestarts","mean",0);


  AddColumnUBigInt("rand","Number of Random Decisions",
    "              Number",
    "           of Random",
    "           Decisions",
    "%20"P64,
    &iNumRandomCalls,"CountRandom",ColTypeFinal);

  AddStatCol("rand","NumRandomDecisions","mean",0);

  AddColumnUBigInt("randstep","Mean Number of Random Decisions Per Step",
    "Avg. Rand",
    "Decisions",
    " per step",
    "%9.3f",
    &iNumRandomCalls,"CountRandom",ColTypeFinalDivStep);

  AddStatCol("randstep","RandomDecisionsPerStep","mean",0);


  AddColumnUInt("candidates","Mean Number of Flip Candidates (not all algs support it)",
    " Mean #",
    "of Flip",
    " Candid",
    "%7.4f",
    &iNumCandidates,"",ColTypeMean);

  AddStatCol("candidates","FlipCandidatesMean","mean",0);


  AddColumnUBigInt("localmins","Number of Local Minima Encountered",
    "              Number",
    "            of Local",
    "              Minima",
    "%20"P64,
    &iNumLocalMins,"LocalMins",ColTypeFinal);

  AddStatCol("localmins","LocalMins","mean",0);

  AddColumnUBigInt("percentlocal","Percent of Steps in Local Minima",
    "Percent",
    "  Local",
    " Minima",
    "%7.4f",
    &iNumLocalMins,"LocalMins",ColTypeFinalDivStep100);

  AddStatCol("percentlocal","PercentLocal","mean",0);

  AddColumnFloat("flipcountcv","CV of the Variable Flip Count Distribution",
    "  CV of",
    "   Flip",
    "  Count",
    "%7.5f",
    &fFlipCountsCV,"FlipCountStats",ColTypeFinal);

  AddStatCol("flipcountcv","FlipCountCV","mean",0);

  AddColumnFloat("biasmax","Mean Frac. # steps vars spent same state as their most freq.",
    "   Max",
    "  Pol-",
    " arity",
    "%5.4f",
    &fMeanMaxBias,"BiasStats",ColTypeFinal);

  AddStatCol("biasmax","BiasMaxMean","mean",0);

  AddColumnFloat("biasfinal","Mean Frac. # steps vars spent same as their final state",
    " Final",
    "  Pol-",
    " arity",
    "%5.4f",
    &fMeanFinalBias,"BiasStats",ColTypeFinal);

  AddStatCol("biasfinal","BiasFinalMean","mean",0);

  AddColumnFloat("unsatcountcv","CV of the Clause Unsat Count Distribution",
    "  CV of",
    "  Unsat",
    "  Count",
    "%7.5f",
    &fUnsatCountsCV,"UnsatCountStats",ColTypeFinal);

  AddStatCol("unsatcountcv","UnsatCountCV","mean",0);


  AddColumnUInt("soldistance","Mean Hamming Distance from Closest Known Solution(s)",
    "   Mean",
    "Dist To",
    "kn.soln",
    "%7.2f",
    &iSolutionDistance,"SolutionDistance",ColTypeMean);

  AddStatCol("soldistance","SolutionDistanceMean","mean",0);


  AddColumnFloat("fdc","Fitness-Distance Correlation Factor (calc. in LM)",
    "Fitness",
    "Distnce",
    "   Calc",
    "%7.3f",
    &fFDCRun,"FDCRun",ColTypeFinal);

  AddStatCol("fdc","FDC","mean",0);

  
  AddColumnUInt("acl","Auto-Correlation Length",
    "Auto",
    "Corr",
    "Len.",
    "%4"P32,
    &iAutoCorrLen,"AutoCorr",ColTypeFinal);

  AddStatCol("acl","ACL","mean",0);


  AddColumnFloat("acone","Auto-Correlation of distance one",
    "  Auto-",
    "  Corr.",
    "  (one)",
    "%7.5f",
    &fAutoCorrOneVal,"AutoCorrOne",ColTypeFinal);

  AddStatCol("acone","ACOne","mean",0);


  AddColumnFloat("estacl","Estimated Auto-Correlation Length from AC of 1",
    "       ",
    "   Est.",
    "    ACL",
    "%7.2f",
    &fAutoCorrOneEst,"AutoCorrOne",ColTypeFinal);

  AddStatCol("estacl","EstACL","mean",0);


  AddColumnFloat("branchfact","Mean Branching Factor (# possible sidesteps / #vars)",
    "  Mean",
    "Branch",
    "  Fact",
    "%6.4f",
    &fBranchFactor, "BranchFactor", ColTypeMean);

  AddStatCol("branchfact","BranchFactorMean","mean",0);

  AddColumnFloat("branchfact_w","Mean Weighted Branching Factor",
    "W.Mean",
    "Branch",
    "  Fact",
    "%6.4f",
    &fBranchFactorW, "BranchFactorW", ColTypeMean);

  AddStatCol("branchfact_w","WeightedBranchFactorMean","mean",0);


  AddColumnFloat("mobn","Mean Mobility for window size n (# of vars)",
    "Mobility",
    "Window N",
    "    Mean",
    "%8.3f",
    &fMobilityColNMean,"MobilityColumn,InitMobilityColumnN",ColTypeFinal);

  AddStatCol("mobn","MobilitySizeNMean","mean",0);


  AddColumnFloat("mobx","Mean Mobility for window size x (see -r mobfixed)",
    "Mobility",
    "Window X",
    "    Mean",
    "%8.3f",
    &fMobilityColXMean,"MobilityColumn,InitMobilityColumnX",ColTypeFinal);

  AddStatCol("mobx","MobilitySizeXMean","mean",0);


  AddColumnFloat("normmobn","Normalized Mean Mobility for window size n (# of vars) ",
    "Norm Mob",
    "Window N",
    "    Mean",
    "%8.6f",
    &fMobilityColNMeanNorm,"MobilityColumn,InitMobilityColumnN",ColTypeFinal);

  AddStatCol("normmobn","NormalizedMobilitySizeN","mean",0);


  AddColumnFloat("normmobx","Normalized Mean Mobility for window size x (see -r mobfixed)",
    "Norm Mob",
    "Window X",
    "    Mean",
    "%8.6f",
    &fMobilityColXMeanNorm,"MobilityColumn,InitMobilityColumnX",ColTypeFinal);

  AddStatCol("normmobx","NormalizedMobilitySizeXMean","mean",0);

  
  AddColumnFloat("mobncv","Mobility C.V. for window size n (# of vars)",
    "Mobility",
    "Window N",
    "    C.V.",
    "%8.6f",
    &fMobilityColNCV,"MobilityColumn,InitMobilityColumnN",ColTypeFinal);

  AddStatCol("mobncv","MobilitySizeNMean","mean",0);


  AddColumnFloat("mobxcv","Mobility C.V. for window size x (see -r mobfixed)",
    "Mobility",
    "Window X",
    "    C.V.",
    "%8.5f",
    &fMobilityColXCV,"MobilityColumn,InitMobilityColumnX",ColTypeFinal);

  AddStatCol("mobxcv","MobilitySizeXMean","mean",0);


  AddColumnUBigInt("upsteps","Number of Up (Backward) Steps",
    "              Number",
    "               of Up",
    "               Steps",
    "%20"P64,
    &iNumUpSteps,"StepsUpDownSide",ColTypeFinal);

  AddColumnUBigInt("upsteps_w","Number of Up (Backward) Weighted Steps",
    "              Number",
    "               of Up",
    "            W. Steps",
    "%20"P64,
    &iNumUpStepsW,"StepsUpDownSide",ColTypeFinal);

  AddColumnUBigInt("percentup","Percent of Up (Backward) Steps",
    "Percent",
    "  of Up",
    "  Steps",
    "%7.4f",
    &iNumUpSteps,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentup","PercentUpSteps","mean",0);

  AddColumnUBigInt("percentup_w","Percent of Up (Backward) Weighted Steps",
    "Percent",
    "  of Up",
    "W.Steps",
    "%7.4f",
    &iNumUpStepsW,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentup_w","PercentWeightedUpSteps","mean",0);


  AddColumnUBigInt("downsteps","Number of Down (Improving) Steps",
    "              Number",
    "             of Down",
    "               Steps",
    "%20"P64,
    &iNumDownSteps,"StepsUpDownSide",ColTypeFinal);

  AddColumnUBigInt("downsteps_w","Number of Down (Improving) Weighted Steps",
    "              Number",
    "             of Down",
    "             W.Steps",
    "%20"P64,
    &iNumDownStepsW,"StepsUpDownSide",ColTypeFinal);

  AddColumnUBigInt("percentdown","Percent of Down (Improving) Steps",
    "Percent",
    "of Down",
    "  Steps",
    "%7.4f",
    &iNumDownSteps,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentdown","PercentUpSteps","mean",0);
  
  AddColumnUBigInt("percentdown_w","Percent of Down (Improving) Weighted Steps",
    "Percent",
    "of Down",
    "W.Steps",
    "%7.4f",
    &iNumDownStepsW,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentdown_w","PercentWeightedUpSteps","mean",0);


  AddColumnUBigInt("sidesteps","Number of Sideways (Plateau) Steps",
    "           Number of",
    "            Sideways",
    "               Steps",
    "%20"P64,
    &iNumSideSteps,"StepsUpDownSide",ColTypeFinal);

  AddColumnUBigInt("sidesteps_w","Number of Sideways (Plateau) Weighted Steps",
    "           Number of",
    "            Sideways",
    "            W. Steps",
    "%20"P64,
    &iNumSideStepsW,"StepsUpDownSide",ColTypeFinal);

  AddColumnUBigInt("percentside","Percent of Sideways (Plateau) Steps",
    "Percent",
    "Sideway",
    "  Steps",
    "%7.4f",
    &iNumSideSteps,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentside","PercentSideSteps","mean",0);

  AddColumnUBigInt("percentside_w","Percent of Sideways (Plateau) Weighted Steps",
    "Percent",
    "Sideway",
    "W.Steps",
    "%7.4f",
    &iNumSideStepsW,"StepsUpDownSide",ColTypeFinalDivStep100);

  AddStatCol("percentside_w","PercentWeightedSideSteps","mean",0);

  
  
  AddColumnFloat("timesteps","Time in seconds, calculated based on steps (RTD Only)",
    "  CPU Time",
    "in seconds",
    "(by steps)",
    "%10.6f",
    &fDummy,"UpdateTimes,ActivateStepsFoundColumns",ColTypeFinal);

  AddStatCol("timesteps","CPUTime","mean+cv+median",1);

  AddColumnFloat("prob","Probability of Success (RTD Only)",
    "   Prob",
    "     of",
    "Success",
    "%7.5f",
    &fDummy,"UpdatePercents",ColTypeFinal);


  /***************************************************************************/
    
  AddContainerStat("default","instinfo,totaltime,fps,runs,numsolve,percentsolve,steps,timesteps");

  AddStatCustom("numclauses","Number of Clauses","Clauses","%"P32,(void *)&iNumClauses,DTypeUInt,"","");
  AddStatCustom("numvars","Number of Variables","Variables","%"P32,(void *)&iNumVars,DTypeUInt,"","");
  AddStatCustom("numlits","Number of Total Literals","TotalLiterals","%"P32,(void *)&iNumLits,DTypeUInt,"","");
  AddContainerStat("instinfo","numvars,numclauses,numlits");

  AddStatCustom("runs","Runs Executed","RunsExecuted","%"P32,(void *)&iRun,DTypeUInt,"","");

  AddStatCustom("numsolve","Number of Successful Runs","SuccessfulRuns","%"P32,(void *)&iNumSolutionsFound,DTypeUInt,"","");

  AddStatCustom("percentsolve","Percent of Successful Runs","PercentSuccess","%-6.2f",(void *)&fPercentSuccess,DTypeFloat,"","CalcPercentSolve");
  
  AddStatCustom("totaltime","Total Time Elapsed","TotalCPUTimeElapsed","%-10.3f",(void *)&fTotalTime,DTypeFloat,"","");

  AddStatCustom("fps","Flips Per Second","FlipsPerSecond","%-7.f",(void *)&fFlipsPerSecond,DTypeFloat,"","CalcFPS");

  AddStatCustom("numunique","Number of Unique Solutions Found","UniqueSolutions","%"P32,(void *)&iNumUniqueSolutions,DTypeUInt,"","UniqueSolutions");

  AddStatCustom("instname","File Name of the Instance","InstFileName","%s",(void *)&sFilenameIn,DTypeString,"","");

  AddStatCustom("alginfo","Algorithm Parameters","AlgParameters","%s",(void *)&sStringAlgParms,DTypeString,"","StringAlgParms");

  AddStatCustom("version","UBCSAT Version Information","UBCSATVersion","%s",(void *)&pVersion,DTypeString,"","");

}

#ifdef __cplusplus

}
#endif
