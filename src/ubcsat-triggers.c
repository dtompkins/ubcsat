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

#ifdef __cplusplus 
namespace ubcsat {
#endif

/*  
    This file contains the code to make the various data triggers work
*/

/***** Trigger ReadCNF *****/

void ReadCNF();

UINT32 iNumVars;
UINT32 iNumClauses;
UINT32 iNumLits;

UINT32 *aClauseLen;
LITTYPE **pClauseLits;
UINT32 iMaxClauseLen;

FLOAT *aClauseWeight;
FLOAT fTotalWeight;

UINT32 iVARSTATELen;


/***** Trigger LitOccurence *****/

void CreateLitOccurence();

UINT32 *aNumLitOcc;
UINT32 *aLitOccData;
UINT32 **pLitClause;


/***** Trigger CandidateList *****/

void CreateCandidateList();

UINT32 *aCandidateList;
UINT32 iNumCandidates;
UINT32 iMaxCandidates;


/***** Trigger InitVarsFromFile *****/
/***** Trigger DefaultInitVars *****/
  
void InitVarsFromFile();
void DefaultInitVars();

UINT32 *aVarInit;
UINT32 iInitVarFlip;
BOOL bVarInitGreedy;


/***** Trigger DefaultStateInfo *****/

void CreateDefaultStateInfo();
void InitDefaultStateInfo();

UINT32 *aNumTrueLit;
BOOL *aVarValue;
UINT32 iNumFalse;
FLOAT fSumFalseW;


/***** Trigger DefaultFlip[W] *****/

void DefaultFlip();
void DefaultFlipW();


/***** Trigger CheckTermination *****/

void CheckTermination();


/***** Trigger FalseClauseList *****/
/***** Trigger Flip+FalseClauseList[W] *****/

void CreateFalseClauseList();
void InitFalseClauseList();
void UpdateFalseClauseList();
void InitFalseClauseList();
void FlipFalseClauseList();
void FlipFalseClauseListW();

UINT32 *aFalseList;
UINT32 *aFalseListPos;
UINT32 iNumFalseList;


/***** Trigger VarScore[W] *****/
/***** Trigger Flip+VarScore[W] *****/

void CreateVarScore();
void InitVarScore();
void UpdateVarScore();
void FlipVarScore();
void CreateVarScoreW();
void InitVarScoreW();
void UpdateVarScoreW();
void FlipVarScoreW();

SINT32 *aVarScore;
FLOAT *aVarScoreW;


/***** Trigger MakeBreak[W] *****/
/***** Trigger Flip+MakeBreak[W] *****/

void CreateMakeBreak();
void InitMakeBreak();
void UpdateMakeBreak();
void FlipMakeBreak();
void CreateMakeBreakW();
void InitMakeBreakW();
void UpdateMakeBreakW();
void FlipMakeBreakW();

UINT32 *aBreakCount;
UINT32 *aMakeCount;
UINT32 *aCritSat;
FLOAT *aBreakCountW;
FLOAT *aMakeCountW;


/***** Trigger VarInFalse *****/
/***** Trigger Flip+VarInFalse *****/
/*
    aVarInFalseList[j]    variable # for the jth variable that appears in false clauses
    aVarInFalseListPos[j] for variable[j], position it occurs in aVarInFalseList
    iNumVarsInFalseList   # variables that appear in false clauses 
*/

void CreateVarInFalse();
void InitVarInFalse();
void UpdateVarInFalse();
void FlipVarInFalse();

UINT32 iNumVarsInFalseList;
UINT32 *aVarInFalseList;
UINT32 *aVarInFalseListPos;


/***** Trigger VarLastChange *****/
/*
    aVarLastChange[j]     the step # of the most recent time variable[j] was flipped
    iVarLastChangeReset   the step # of the last time all aVarLastChange values were reset
*/
void CreateVarLastChange();
void InitVarLastChange();
void UpdateVarLastChange();

UBIGINT *aVarLastChange;
UBIGINT iVarLastChangeReset;


/***** Trigger TrackChanges[W] *****/
/*
    iNumChanges           # of changes to aVarScore[] values this step
    aChangeList[j]        variable # of the jth variable changed this step
    aChangeOldScore[j]    the previous score of variable[j]
    aChangeLastStep[j]    the step of the last change for variable[j]
*/

void CreateTrackChanges();
void InitTrackChanges();
void UpdateTrackChanges();
void FlipTrackChanges();
void FlipTrackChangesFCL();

void CreateTrackChangesW();
void InitTrackChangesW();
void UpdateTrackChangesW();
void FlipTrackChangesW();
void FlipTrackChangesFCLW();

UINT32 iNumChanges;
UINT32 *aChangeList;
SINT32 *aChangeOldScore;
UBIGINT *aChangeLastStep;

UINT32 iNumChangesW;
UINT32 *aChangeListW;
FLOAT *aChangeOldScoreW;
UBIGINT *aChangeLastStepW;


/***** Trigger DecPromVars[W] *****/

void CreateDecPromVars();
void InitDecPromVars();
void UpdateDecPromVars();

void CreateDecPromVarsW();
void InitDecPromVarsW();
void UpdateDecPromVarsW();

UINT32 *aDecPromVarsList;
UINT32 iNumDecPromVars;

UINT32 *aDecPromVarsListW;
UINT32 iNumDecPromVarsW;

/***** Trigger Flip+DecPromVars+FCL *****/
void FlipDecPromVarsFCL();


/***** Trigger PenaltyDecPromVars *****/

void CreatePenaltyDecPromVars();
void InitPenaltyDecPromVars();
void UpdatePenaltyDecPromVars();

UINT32 *aPenaltyDecPromVarsList;
UINT32 iNumPenaltyDecPromVars;

/***** Trigger Flip+PenaltyDecPromVars+FCL *****/
void FlipPenaltyDecPromVarsFCL();

/***** Trigger BestScoreList *****/

void CreateBestScoreList();
void InitBestScoreList();
void UpdateBestScoreList();

UINT32 iNumBestScoreList;
UINT32 *aBestScoreList;
UINT32 *aBestScoreListPos;


/***** Trigger ClausePenaltyFL[W] *****/

void CreateClausePenaltyFL();
void InitClausePenaltyFL();
void InitClausePenaltyFLW();

FLOAT *aClausePenaltyFL;
BOOL bClausePenaltyCreated;
BOOL bClausePenaltyFLOAT;
FLOAT fBasePenaltyFL;
FLOAT fTotalPenaltyFL;



/***** Trigger MakeBreakPenaltyFL *****/

void CreateMakeBreakPenaltyFL();
void InitMakeBreakPenaltyFL();
void UpdateMakeBreakPenaltyFL();
void FlipMBPFLandFCLandVIF();
void FlipMBPFLandFCLandVIFandW();

FLOAT *aMakePenaltyFL;
FLOAT *aBreakPenaltyFL;


/***** Trigger ClausePenaltyINT *****/

void CreateClausePenaltyINT();
void InitClausePenaltyINT();
void InitClausePenaltyINTW();

UINT32 *aClausePenaltyINT;
UINT32 iInitPenaltyINT;
UINT32 iBasePenaltyINT;
UINT32 iTotalPenaltyINT;



/***** Trigger MakeBreakPenalty *****/

void CreateMakeBreakPenaltyINT();
void InitMakeBreakPenaltyINT();
void UpdateMakeBreakPenaltyINT();
void FlipMBPINTandFCLandVIF();
void FlipMBPINTandFCLandVIFandW();

UINT32 *aMakePenaltyINT;
UINT32 *aBreakPenaltyINT;


/***** Trigger NullFlips *****/

UBIGINT iNumNullFlips;

void InitNullFlips();
void UpdateNullFlips();


/***** Trigger LocalMins *****/

UBIGINT iNumLocalMins;

void InitLocalMins();
void UpdateLocalMins();


/***** Trigger LogDist *****/

void CreateLogDist();

UBIGINT *aLogDistValues;
UINT32 iNumLogDistValues;
UINT32 iLogDistStepsPerDecade;


/***** Trigger BestFalse *****/

void InitBestFalse();
void UpdateBestFalse();

UINT32 iBestNumFalse;
UBIGINT iBestStepNumFalse;
FLOAT fBestSumFalseW;
UBIGINT iBestStepSumFalseW;


/***** Trigger SaveBest *****/

void CreateSaveBest();
void UpdateSaveBest();

VARSTATE vsBest;


/***** Trigger StartFalse *****/

void UpdateStartFalse();

UINT32 iStartNumFalse;
FLOAT fStartSumFalseW;


/***** Trigger CalcImproveMean *****/

void CalcImproveMean();

FLOAT fImproveMean;
FLOAT fImproveMeanW;


/***** Trigger FirstLM *****/

void InitFirstLM();
void UpdateFirstLM();
void CalcFirstLM();

UINT32 iFirstLM;
UBIGINT iFirstLMStep;
FLOAT fFirstLMW;
UBIGINT iFirstLMStepW;


/***** Trigger FirstLMRatio *****/

void CalcFirstLMRatio();

FLOAT fFirstLMRatio;
FLOAT fFirstLMRatioW;


/***** Trigger TrajBestLM *****/

void UpdateTrajBestLM();
void CalcTrajBestLM();

UINT32 iTrajBestLMCount;
FLOAT fTrajBestLMSum;
FLOAT fTrajBestLMSum2;
UINT32 iTrajBestLMCountW;
FLOAT fTrajBestLMSumW;
FLOAT fTrajBestLMSum2W;

FLOAT fTrajBestLMMean;
FLOAT fTrajBestLMMeanW;
FLOAT fTrajBestLMCV;
FLOAT fTrajBestLMCVW;


/***** Trigger NoImprove *****/

void CheckNoImprove();

UBIGINT iNoImprove;


/***** Trigger EarlyTerm *****/

void CheckEarlyTerm();

UBIGINT iEarlyTermSteps;
UINT32 iEarlyTermQual;
FLOAT fEarlyTermQualW;


/***** Trigger Strikes *****/

void CheckStrikes();

UINT32 iStrikes;


/***** Trigger StartSeed *****/

void StartSeed();

UINT32 iStartSeed;


/***** Trigger CountRandom *****/


/***** Trigger CheckTimeout *****/

void CheckTimeout();


/***** Trigger CheckForRestarts *****/

void CheckForRestarts();


/***** Trigger FlipCounts *****/

void CreateFlipCounts();
void InitFlipCounts();
void UpdateFlipCounts();

UBIGINT *aFlipCounts;


/***** Trigger FlipCountStats *****/

void FlipCountStats();

FLOAT fFlipCountsMean;
FLOAT fFlipCountsCV;
FLOAT fFlipCountsStdDev;


/***** Trigger BiasCounts *****/

void CreateBiasCounts();
void PreInitBiasCounts();
void UpdateBiasCounts();
void FinalBiasCounts();

UBIGINT *aBiasTrueCounts;
UBIGINT *aBiasFalseCounts;


/***** Trigger BiasStats *****/

void BiasStats();

FLOAT fMeanFinalBias;
FLOAT fMeanMaxBias;


/***** Trigger UnsatCounts *****/

void CreateUnsatCounts();
void InitUnsatCounts();
void UpdateUnsatCounts();

UBIGINT *aUnsatCounts;


/***** Trigger UnsatCountStats *****/

void UnsatCountStats();

FLOAT fUnsatCountsMean;
FLOAT fUnsatCountsCV;
FLOAT fUnsatCountsStdDev;


/***** Trigger NumFalseCounts *****/

void CreateNumFalseCounts();
void InitNumFalseCounts();
void UpdateNumFalseCounts();

UBIGINT *aNumFalseCounts;
UBIGINT *aNumFalseCountsWindow;


/***** Trigger DistanceCounts *****/

void CreateDistanceCounts();
void InitDistanceCounts();
void UpdateDistanceCounts();

UBIGINT *aDistanceCounts;
UBIGINT *aDistanceCountsWindow;


/***** Trigger ClauseLast *****/

void CreateClauseLast();
void InitClauseLast();
void UpdateClauseLast();

UBIGINT *aClauseLast;


/***** Trigger SQGrid *****/

void CreateSQGrid();
void InitSQGrid();
void UpdateSQGrid();
void FinishSQGrid();

FLOAT *aSQGridW;
UINT32 *aSQGrid;


/***** Trigger PenaltyStats *****/

void CreatePenaltyStats();
void InitPenaltyStats();
void UpdatePenaltyStatsStep();
void UpdatePenaltyStatsRun();

FLOAT *aPenaltyStatsMean;
FLOAT *aPenaltyStatsStddev;
FLOAT *aPenaltyStatsCV;

FLOAT *aPenaltyStatsSum;
FLOAT *aPenaltyStatsSum2;

FLOAT *aPenaltyStatsMeanSum;
FLOAT *aPenaltyStatsMeanSum2;
FLOAT *aPenaltyStatsStddevSum;
FLOAT *aPenaltyStatsStddevSum2;
FLOAT *aPenaltyStatsCVSum;
FLOAT *aPenaltyStatsCVSum2;


/***** Trigger VarFlipHistory *****/

void CreateVarFlipHistory();
void UpdateVarFlipHistory();

UINT32 *aVarFlipHistory;
UINT32 iVarFlipHistoryLen;


/***** Trigger MobilityWindow *****/

void CreateMobilityWindow();
void InitMobilityWindow();
void UpdateMobilityWindow();

UINT32 *aMobilityWindowVarChange;
UINT32 *aMobilityWindow;
FLOAT *aMobilityWindowSum;
FLOAT *aMobilityWindowSum2;


/***** Trigger MobilityFixedFrequencies *****/

void CreateMobilityFixedFrequencies();
void InitMobilityFixedFrequencies();
void UpdateMobilityFixedFrequencies();

UINT32 *aMobilityFixedFrequencies;


/***** Trigger VarAgeFrequencies *****/

void CreateVarAgeFrequencies();
void InitVarAgeFrequencies();
void UpdateVarAgeFrequencies();

UINT32 iMaxVarAgeFrequency;
UBIGINT *aVarAgeFrequency;


/***** Trigger AutoCorr *****/

void CreateAutoCorr();
void InitAutoCorr();
void UpdateAutoCorr();
void CalcAutoCorr();

UINT32 iAutoCorrMaxLen;
FLOAT fAutoCorrCutoff;
UINT32 iAutoCorrLen;
FLOAT *aAutoCorrValues;
FLOAT *aAutoCorrStartBuffer;
FLOAT *aAutoCorrEndCircBuffer;
FLOAT fAutoCorrSum;
FLOAT fAutoCorrSum2;
FLOAT *aAutoCorrCrossSum;

/***** Trigger AutoCorrOne *****/

void InitAutoCorrOne();
void UpdateAutoCorrOne();
void CalcAutoCorrOne();

FLOAT fAutoCorrOneVal;
FLOAT fAutoCorrOneEst;
FLOAT fAutoCorrOneStart;
FLOAT fAutoCorrOneLast;
FLOAT fAutoCorrOneSum;
FLOAT fAutoCorrOneSum2;
FLOAT fAutoCorrOneCrossSum;


/***** Trigger BranchFactor *****/

void BranchFactor();
void BranchFactorW();

FLOAT fBranchFactor;
FLOAT fBranchFactorW;


/****** Trigger StepsUpDownSide *****/

void InitStepsUpDownSide();
void UpdateStepsUpDownSide();

UBIGINT iNumUpSteps;
UBIGINT iNumDownSteps;
UBIGINT iNumSideSteps;
UBIGINT iNumUpStepsW;
UBIGINT iNumDownStepsW;
UBIGINT iNumSideStepsW;

/****** Trigger NumRestarts *****/

void NumRestarts();

UINT32 iNumRestarts;

/***** Trigger LoadKnownSolutions *****/

void LoadKnownSolutions();

VARSTATELIST vslKnownSoln;
BOOL bKnownSolutions;


/***** Trigger SolutionDistance *****/

void CreateSolutionDistance();
void UpdateSolutionDistance();

VARSTATE vsSolutionDistanceCurrent;
VARSTATE vsSolutionDistanceClosest;
UINT32 iSolutionDistance;


/***** Trigger FDCRun *****/

void CreateFDCRun();
void InitFDCRun();
void UpdateFDCRun();
void CalcFDCRun();

FLOAT fFDCRun;

FLOAT fFDCRunHeightDistanceSum;
FLOAT fFDCRunHeightSum;
FLOAT fFDCRunHeightSum2;
FLOAT fFDCRunDistanceSum;
FLOAT fFDCRunDistanceSum2;
UINT32 iFDCRunCount;



/***** Trigger DynamicParms *****/

void DynamicParms();



/***** Trigger FlushBuffers *****/

void FlushBuffers();



/***** Trigger CheckWeighted *****/

void CheckWeighted();



/***** Trigger UniqueSolutions *****/

void CreateUniqueSolutions();
void UpdateUniqueSolutions();

VARSTATELIST vslUnique;
VARSTATE vsCheckUnique;
UINT32 iNumUniqueSolutions;
UINT32 iLastUnique;


/***** Trigger VarsShareClauses *****/

UINT32 *aNumVarsShareClause;
UINT32 **pVarsShareClause;

void CreateVarsShareClauses();



void AddDataTriggers() {

  CreateTrigger("ReadCNF",ReadInInstance,ReadCNF,"","");

  CreateTrigger("LitOccurence",CreateData,CreateLitOccurence,"","");
  
  CreateTrigger("CandidateList",CreateData,CreateCandidateList,"","");

  CreateTrigger("InitVarsFromFile",PreStart,InitVarsFromFile,"DefaultStateInfo","");
  CreateTrigger("DefaultInitVars",InitData,DefaultInitVars,"DefaultStateInfo","");

  CreateTrigger("CreateDefaultStateInfo",CreateStateInfo,CreateDefaultStateInfo,"","");
  
  CreateTrigger("InitDefaultStateInfo",InitStateInfo,InitDefaultStateInfo,"","");
  CreateContainerTrigger("DefaultStateInfo","CreateDefaultStateInfo,InitDefaultStateInfo");
 
  CreateTrigger("DefaultFlip",FlipCandidate,DefaultFlip,"","");
  CreateTrigger("DefaultFlipW",FlipCandidate,DefaultFlipW,"","");

  CreateTrigger("CheckTermination",CheckTerminate,CheckTermination,"","");
  
  CreateContainerTrigger("DefaultProcedures","ReadCNF,LitOccurence,CandidateList,InitVarsFromFile,DefaultStateInfo,DefaultInitVars,DefaultFlip,CheckTermination");
  CreateContainerTrigger("DefaultProceduresW","ReadCNF,LitOccurence,CandidateList,InitVarsFromFile,DefaultStateInfo,DefaultInitVars,DefaultFlipW,CheckTermination");

  CreateTrigger("CreateFalseClauseList",CreateStateInfo,CreateFalseClauseList,"","");
  CreateTrigger("InitFalseClauseList",InitStateInfo,InitFalseClauseList,"InitDefaultStateInfo","");
  CreateTrigger("UpdateFalseClauseList",UpdateStateInfo,UpdateFalseClauseList,"","");
  CreateContainerTrigger("FalseClauseList","CreateFalseClauseList,InitFalseClauseList,UpdateFalseClauseList");
  CreateTrigger("Flip+FalseClauseList",FlipCandidate,FlipFalseClauseList,"FalseClauseList","DefaultFlip,UpdateFalseClauseList");
  CreateTrigger("Flip+FalseClauseListW",FlipCandidate,FlipFalseClauseListW,"FalseClauseList","DefaultFlipW,UpdateFalseClauseList");

  CreateTrigger("CreateVarScore",CreateStateInfo,CreateVarScore,"","");
  CreateTrigger("InitVarScore",InitStateInfo,InitVarScore,"InitDefaultStateInfo","");
  CreateTrigger("UpdateVarScore",UpdateStateInfo,UpdateVarScore,"","");
  CreateContainerTrigger("VarScore","CreateVarScore,InitVarScore,UpdateVarScore");
  CreateTrigger("Flip+VarScore",FlipCandidate,FlipVarScore,"VarScore","DefaultFlip,UpdateVarScore");

  CreateTrigger("CreateVarScoreW",CreateStateInfo,CreateVarScoreW,"","");
  CreateTrigger("InitVarScoreW",InitStateInfo,InitVarScoreW,"InitDefaultStateInfo","");
  CreateTrigger("UpdateVarScoreW",UpdateStateInfo,UpdateVarScoreW,"","");
  CreateContainerTrigger("VarScoreW","CreateVarScoreW,InitVarScoreW,UpdateVarScoreW");
  CreateTrigger("Flip+VarScoreW",FlipCandidate,FlipVarScoreW,"VarScoreW","DefaultFlipW,UpdateVarScoreW");

  CreateTrigger("CreateMakeBreak",CreateStateInfo,CreateMakeBreak,"","");
  CreateTrigger("InitMakeBreak",InitStateInfo,InitMakeBreak,"InitDefaultStateInfo","");
  CreateTrigger("UpdateMakeBreak",UpdateStateInfo,UpdateMakeBreak,"","");
  CreateContainerTrigger("MakeBreak","CreateMakeBreak,InitMakeBreak,UpdateMakeBreak");
  CreateTrigger("Flip+MakeBreak",FlipCandidate,FlipMakeBreak,"MakeBreak","DefaultFlip,UpdateMakeBreak");

  CreateTrigger("CreateMakeBreakW",CreateStateInfo,CreateMakeBreakW,"","");
  CreateTrigger("InitMakeBreakW",InitStateInfo,InitMakeBreakW,"InitDefaultStateInfo","");
  CreateTrigger("UpdateMakeBreakW",UpdateStateInfo,UpdateMakeBreakW,"","");
  CreateContainerTrigger("MakeBreakW","CreateMakeBreakW,InitMakeBreakW,UpdateMakeBreakW");
  CreateTrigger("Flip+MakeBreakW",FlipCandidate,FlipMakeBreakW,"MakeBreakW","DefaultFlipW,UpdateMakeBreakW");

  CreateTrigger("CreateVarInFalse",CreateStateInfo,CreateVarInFalse,"CreateMakeBreak","");
  CreateTrigger("InitVarInFalse",InitStateInfo,InitVarInFalse,"InitMakeBreak","");
  CreateTrigger("UpdateVarInFalse",UpdateStateInfo,UpdateVarInFalse,"","UpdateMakeBreak");
  CreateContainerTrigger("VarInFalse","CreateVarInFalse,InitVarInFalse,UpdateVarInFalse");
  CreateTrigger("Flip+VarInFalse",FlipCandidate,FlipVarInFalse,"VarInFalse","DefaultFlip,UpdateVarInFalse");

  CreateTrigger("CreateVarLastChange",CreateStateInfo,CreateVarLastChange,"","");
  CreateTrigger("InitVarLastChange",InitStateInfo,InitVarLastChange,"","");
  CreateTrigger("UpdateVarLastChange",UpdateStateInfo,UpdateVarLastChange,"","");
  CreateContainerTrigger("VarLastChange","CreateVarLastChange,InitVarLastChange,UpdateVarLastChange");

  CreateTrigger("CreateTrackChanges",CreateStateInfo,CreateTrackChanges,"CreateVarScore","");
  CreateTrigger("InitTrackChanges",InitStateInfo,InitTrackChanges,"InitVarScore","");
  CreateTrigger("UpdateTrackChanges",UpdateStateInfo,UpdateTrackChanges,"","UpdateVarScore");
  CreateContainerTrigger("TrackChanges","CreateTrackChanges,InitTrackChanges,UpdateTrackChanges");
  CreateTrigger("Flip+TrackChanges",FlipCandidate,FlipTrackChanges,"TrackChanges","DefaultFlip,UpdateTrackChanges,UpdateVarScore");
  
  CreateTrigger("Flip+TrackChanges+FCL",FlipCandidate,FlipTrackChangesFCL,"TrackChanges,FalseClauseList","DefaultFlip,UpdateTrackChanges,UpdateVarScore,UpdateFalseClauseList");

  CreateTrigger("CreateTrackChangesW",CreateStateInfo,CreateTrackChangesW,"CreateVarScoreW","");
  CreateTrigger("InitTrackChangesW",InitStateInfo,InitTrackChangesW,"InitVarScoreW","");
  CreateTrigger("UpdateTrackChangesW",UpdateStateInfo,UpdateTrackChangesW,"","UpdateVarScoreW");
  CreateContainerTrigger("TrackChangesW","CreateTrackChangesW,InitTrackChangesW,UpdateTrackChangesW");

  CreateTrigger("Flip+TrackChangesW",FlipCandidate,FlipTrackChangesW,"TrackChangesW","DefaultFlipW,UpdateTrackChangesW,UpdateVarScoreW");
  CreateTrigger("Flip+TrackChanges+FCL+W",FlipCandidate,FlipTrackChangesFCLW,"TrackChangesW,FalseClauseList","DefaultFlipW,UpdateTrackChangesW,UpdateVarScoreW,UpdateFalseClauseList");

  CreateTrigger("CreateDecPromVars",CreateStateInfo,CreateDecPromVars,"CreateTrackChanges","");
  CreateTrigger("InitDecPromVars",InitStateInfo,InitDecPromVars,"InitTrackChanges","");
  CreateTrigger("UpdateDecPromVars",UpdateStateInfo,UpdateDecPromVars,"UpdateTrackChanges","");
  CreateContainerTrigger("DecPromVars","CreateDecPromVars,InitDecPromVars,UpdateDecPromVars");

  CreateTrigger("Flip+DecPromVars+FCL",FlipCandidate,FlipDecPromVarsFCL,"DecPromVars,FalseClauseList","DefaultFlip,UpdateTrackChanges,UpdateVarScore,UpdateFalseClauseList,CreateTrackChanges,InitTrackChanges,UpdateTrackChanges");

  CreateTrigger("CreatePenaltyDecPromVars",CreateStateInfo,CreatePenaltyDecPromVars,"CreateTrackChanges,CreateMakeBreakPenaltyINT","CreateVarScore");
  CreateTrigger("InitPenaltyDecPromVars",InitStateInfo,InitPenaltyDecPromVars,"InitTrackChanges,InitMakeBreakPenaltyINT","InitVarScore");
  CreateTrigger("UpdatePenaltyDecPromVars",UpdateStateInfo,UpdatePenaltyDecPromVars,"UpdateTrackChanges,UpdateMakeBreakPenaltyINT","UpdateVarScore");
  CreateContainerTrigger("PenaltyDecPromVars","CreatePenaltyDecPromVars,InitPenaltyDecPromVars,UpdatePenaltyDecPromVars");

  CreateTrigger("Flip+PenaltyDecPromVars+FCL",FlipCandidate,FlipPenaltyDecPromVarsFCL,"PenaltyDecPromVars,FalseClauseList","DefaultFlip,UpdateTrackChanges,UpdatePenaltyDecPromVars,UpdateFalseClauseList,CreateTrackChanges,InitTrackChanges,UpdateTrackChanges,UpdateMakeBreakPenaltyINT");

  CreateTrigger("CreateDecPromVarsW",CreateStateInfo,CreateDecPromVarsW,"CreateTrackChangesW","");
  CreateTrigger("InitDecPromVarsW",InitStateInfo,InitDecPromVarsW,"InitTrackChangesW","");
  CreateTrigger("UpdateDecPromVarsW",UpdateStateInfo,UpdateDecPromVarsW,"UpdateTrackChangesW","");
  CreateContainerTrigger("DecPromVarsW","CreateDecPromVarsW,InitDecPromVarsW,UpdateDecPromVarsW");

  CreateTrigger("CreateBestScoreList",CreateStateInfo,CreateBestScoreList,"","");
  CreateTrigger("InitBestScoreList",InitStateInfo,InitBestScoreList,"InitVarScore","");
  CreateTrigger("UpdateBestScoreList",UpdateStateInfo,UpdateBestScoreList,"","");
  CreateContainerTrigger("BestScoreList","TrackChanges,CreateBestScoreList,InitBestScoreList,UpdateBestScoreList");

  CreateTrigger("CreateClausePenaltyFL",CreateStateInfo,CreateClausePenaltyFL,"","");
  CreateTrigger("InitClausePenaltyFL",InitStateInfo,InitClausePenaltyFL,"","");
  CreateContainerTrigger("ClausePenaltyFL","CreateClausePenaltyFL,InitClausePenaltyFL");

  CreateTrigger("InitClausePenaltyFLW",InitStateInfo,InitClausePenaltyFLW,"","InitClausePenaltyFL");

  CreateTrigger("CreateMakeBreakPenaltyFL",CreateStateInfo,CreateMakeBreakPenaltyFL,"CreateClausePenaltyFL","");
  CreateTrigger("InitMakeBreakPenaltyFL",InitStateInfo,InitMakeBreakPenaltyFL,"InitClausePenaltyFL","");
  CreateTrigger("UpdateMakeBreakPenaltyFL",UpdateStateInfo,UpdateMakeBreakPenaltyFL,"","");
  CreateContainerTrigger("MakeBreakPenaltyFL","CreateMakeBreakPenaltyFL,InitMakeBreakPenaltyFL,UpdateMakeBreakPenaltyFL");
  CreateTrigger("Flip+MBPFL+FCL+VIF",FlipCandidate,FlipMBPFLandFCLandVIF,"MakeBreakPenaltyFL,FalseClauseList,VarInFalse","DefaultFlip,UpdateMakeBreakPenaltyFL,UpdateFalseClauseList,UpdateVarInFalse");
  CreateTrigger("Flip+MBPFL+FCL+VIF+W",FlipCandidate,FlipMBPFLandFCLandVIFandW,"MakeBreakPenaltyFL,FalseClauseList,VarInFalse","DefaultFlipW,UpdateMakeBreakPenaltyFL,UpdateFalseClauseList,UpdateVarInFalse");

  CreateTrigger("CreateClausePenaltyINT",CreateStateInfo,CreateClausePenaltyINT,"","");
  CreateTrigger("InitClausePenaltyINT",InitStateInfo,InitClausePenaltyINT,"","");
  CreateContainerTrigger("ClausePenaltyINT","CreateClausePenaltyINT,InitClausePenaltyINT");

  CreateTrigger("InitClausePenaltyINTW",InitStateInfo,InitClausePenaltyINTW,"","InitClausePenaltyINT");

  CreateTrigger("CreateMakeBreakPenaltyINT",CreateStateInfo,CreateMakeBreakPenaltyINT,"CreateClausePenaltyINT","");
  CreateTrigger("InitMakeBreakPenaltyINT",InitStateInfo,InitMakeBreakPenaltyINT,"InitClausePenaltyINT","");
  CreateTrigger("UpdateMakeBreakPenaltyINT",UpdateStateInfo,UpdateMakeBreakPenaltyINT,"","");
  CreateContainerTrigger("MakeBreakPenaltyINT","CreateMakeBreakPenaltyINT,InitMakeBreakPenaltyINT,UpdateMakeBreakPenaltyINT");
  CreateTrigger("Flip+MBPINT+FCL+VIF",FlipCandidate,FlipMBPINTandFCLandVIF,"MakeBreakPenaltyINT,FalseClauseList,VarInFalse","DefaultFlip,UpdateMakeBreakPenaltyINT,UpdateFalseClauseList,UpdateVarInFalse");
  CreateTrigger("Flip+MBPINT+FCL+VIF+W",FlipCandidate,FlipMBPINTandFCLandVIFandW,"MakeBreakPenaltyINT,FalseClauseList,VarInFalse","DefaultFlipW,UpdateMakeBreakPenaltyINT,UpdateFalseClauseList,UpdateVarInFalse");

  CreateTrigger("InitNullFlips",PreRun,InitNullFlips,"","");
  CreateTrigger("UpdateNullFlips",UpdateStateInfo,UpdateNullFlips,"","");
  CreateContainerTrigger("NullFlips","InitNullFlips,UpdateNullFlips");

  CreateTrigger("InitLocalMins",PreRun,InitLocalMins,"","");
  CreateTrigger("UpdateLocalMins",UpdateStateInfo,UpdateLocalMins,"","");
  CreateContainerTrigger("LocalMins","InitLocalMins,UpdateLocalMins");

  CreateTrigger("LogDist",CreateData,CreateLogDist,"","");

  CreateTrigger("InitBestFalse",PreRun,InitBestFalse,"","");
  CreateTrigger("UpdateBestFalse",PostStep,UpdateBestFalse,"","");
  CreateContainerTrigger("BestFalse","InitBestFalse,UpdateBestFalse");

  CreateTrigger("CreateSaveBest",CreateStateInfo,CreateSaveBest,"","");
  CreateTrigger("UpdateSaveBest",PostStep,UpdateSaveBest,"BestFalse","");
  CreateContainerTrigger("SaveBest","CreateSaveBest,UpdateSaveBest");

  CreateTrigger("StartFalse",PostStep,UpdateStartFalse,"","");

  CreateTrigger("ImproveMean",RunCalculations,CalcImproveMean,"StartFalse,BestFalse","");

  CreateTrigger("InitFirstLM",PreRun,InitFirstLM,"","");
  CreateTrigger("UpdateFirstLM",PostStep,UpdateFirstLM,"","");
  CreateContainerTrigger("FirstLM","InitFirstLM,UpdateFirstLM");

  CreateTrigger("FirstLMRatio",RunCalculations,CalcFirstLMRatio,"FirstLM,StartFalse,BestFalse","");

  CreateTrigger("UpdateTrajBestLM",PostStep,UpdateTrajBestLM,"BestFalse","");
  CreateTrigger("CalcTrajBestLM",RunCalculations,CalcTrajBestLM,"BestFalse","");
  CreateContainerTrigger("TrajBestLM","UpdateTrajBestLM,CalcTrajBestLM");

  CreateTrigger("NoImprove",CheckTerminate,CheckNoImprove,"BestFalse","");

  CreateTrigger("EarlyTerm",CheckTerminate,CheckEarlyTerm,"","");
  CreateTrigger("Strikes",PostRun,CheckStrikes,"","");

  CreateTrigger("StartSeed",PreRun,StartSeed,"","");

  CreateTrigger("SetupCountRandom",PreStart,SetupCountRandom,"","");
  CreateTrigger("InitCountRandom",PreRun,InitCountRandom,"","");
  CreateContainerTrigger("CountRandom","SetupCountRandom,InitCountRandom");

  CreateTrigger("CheckTimeout",CheckTerminate,CheckTimeout,"","");

  CreateTrigger("CheckForRestarts",CheckRestart,CheckForRestarts,"","");

  CreateTrigger("CreateFlipCounts",CreateStateInfo,CreateFlipCounts,"","");
  CreateTrigger("InitFlipCounts",InitStateInfo,InitFlipCounts,"","");
  CreateTrigger("UpdateFlipCounts",UpdateStateInfo,UpdateFlipCounts,"","");
  CreateContainerTrigger("FlipCounts","CreateFlipCounts,InitFlipCounts,UpdateFlipCounts");

  CreateTrigger("FlipCountStats",RunCalculations,FlipCountStats,"FlipCounts","");

  CreateTrigger("CreateBiasCounts",CreateStateInfo,CreateBiasCounts,"","");
  CreateTrigger("PreInitBiasCounts",PreInit,PreInitBiasCounts,"","");
  CreateTrigger("UpdateBiasCounts",PreFlip,UpdateBiasCounts,"","");
  CreateTrigger("FinalBiasCounts",PostRun,FinalBiasCounts,"","");
  CreateContainerTrigger("BiasCounts","CreateBiasCounts,PreInitBiasCounts,UpdateBiasCounts,FinalBiasCounts,VarLastChange");

  CreateTrigger("BiasStats",RunCalculations,BiasStats,"BiasCounts","");

  CreateTrigger("CreateUnsatCounts",CreateStateInfo,CreateUnsatCounts,"","");
  CreateTrigger("InitUnsatCounts",InitStateInfo,InitUnsatCounts,"","");
  CreateTrigger("UpdateUnsatCounts",PostStep,UpdateUnsatCounts,"","");
  CreateContainerTrigger("UnsatCounts","CreateUnsatCounts,InitUnsatCounts,UpdateUnsatCounts");

  CreateTrigger("UnsatCountStats",RunCalculations,UnsatCountStats,"UnsatCounts","");

  CreateTrigger("CreateNumFalseCounts",CreateStateInfo,CreateNumFalseCounts,"","");
  CreateTrigger("InitNumFalseCounts",InitStateInfo,InitNumFalseCounts,"","");
  CreateTrigger("UpdateNumFalseCounts",PostStep,UpdateNumFalseCounts,"","");
  CreateContainerTrigger("NumFalseCounts","CreateNumFalseCounts,InitNumFalseCounts,UpdateNumFalseCounts");

  CreateTrigger("CreateDistanceCounts",CreateStateInfo,CreateDistanceCounts,"","");
  CreateTrigger("InitDistanceCounts",InitStateInfo,InitDistanceCounts,"","");
  CreateTrigger("UpdateDistanceCounts",PostStep,UpdateDistanceCounts,"","");
  CreateContainerTrigger("DistanceCounts","CreateDistanceCounts,InitDistanceCounts,UpdateDistanceCounts,SolutionDistance");

  CreateTrigger("UnsatCountStats",RunCalculations,UnsatCountStats,"UnsatCounts","");
  CreateTrigger("CreateClauseLast",CreateStateInfo,CreateClauseLast,"","");
  CreateTrigger("InitClauseLast",InitStateInfo,InitClauseLast,"","");
  CreateTrigger("UpdateClauseLast",PostStep,UpdateClauseLast,"","");
  CreateContainerTrigger("ClauseLast","CreateClauseLast,InitClauseLast,UpdateClauseLast");

  CreateTrigger("CreateSQGrid",CreateStateInfo,CreateSQGrid,"LogDist","");
  CreateTrigger("InitSQGrid",PreRun,InitSQGrid,"","");
  CreateTrigger("UpdateSQGrid",PostStep,UpdateSQGrid,"BestFalse","");
  CreateTrigger("FinishSQGrid",PostRun,FinishSQGrid,"","");
  CreateContainerTrigger("SQGrid","CreateSQGrid,InitSQGrid,UpdateSQGrid,FinishSQGrid");

  CreateTrigger("CreatePenaltyStats",CreateStateInfo,CreatePenaltyStats,"","");
  CreateTrigger("InitPenaltyStats",PreRun,InitPenaltyStats,"","");
  CreateTrigger("UpdatePenaltyStatsStep",StepCalculations,UpdatePenaltyStatsStep,"","");
  CreateTrigger("UpdatePenaltyStatsRun",RunCalculations,UpdatePenaltyStatsRun,"","");
  CreateContainerTrigger("PenaltyStats","CreatePenaltyStats,InitPenaltyStats,UpdatePenaltyStatsStep,UpdatePenaltyStatsRun");

  CreateTrigger("CreateVarFlipHistory",PreStart,CreateVarFlipHistory,"","");
  CreateTrigger("UpdateVarFlipHistory",PostStep,UpdateVarFlipHistory,"","");
  CreateContainerTrigger("VarFlipHistory","CreateVarFlipHistory,UpdateVarFlipHistory");

  CreateTrigger("CreateMobilityWindow",PreStart,CreateMobilityWindow,"CreateVarFlipHistory","");
  CreateTrigger("InitMobilityWindow",PreRun,InitMobilityWindow,"","");
  CreateTrigger("UpdateMobilityWindow",PostStep,UpdateMobilityWindow,"UpdateVarFlipHistory","");
  CreateContainerTrigger("MobilityWindow","CreateMobilityWindow,InitMobilityWindow,UpdateMobilityWindow");

  CreateTrigger("CreateMobilityFixedFrequencies",PreStart,CreateMobilityFixedFrequencies,"CreateMobilityWindow","");
  CreateTrigger("InitMobilityFixedFrequencies",PreRun,InitMobilityFixedFrequencies,"InitMobilityWindow","");
  CreateTrigger("UpdateMobilityFixedFrequencies",PostStep,UpdateMobilityFixedFrequencies,"UpdateMobilityWindow","");
  CreateContainerTrigger("MobilityFixedFrequencies","CreateMobilityFixedFrequencies,InitMobilityFixedFrequencies,UpdateMobilityFixedFrequencies");

  CreateTrigger("CreateVarAgeFrequencies",PreStart,CreateVarAgeFrequencies,"","");
  CreateTrigger("InitVarAgeFrequencies",PreRun,InitVarAgeFrequencies,"","");
  CreateTrigger("UpdateVarAgeFrequencies",PreFlip,UpdateVarAgeFrequencies,"","");
  CreateContainerTrigger("VarAgeFrequencies","CreateVarAgeFrequencies,InitVarAgeFrequencies,UpdateVarAgeFrequencies,VarLastChange");

  CreateTrigger("CreateAutoCorr",PreStart,CreateAutoCorr,"","");
  CreateTrigger("InitAutoCorr",PreRun,InitAutoCorr,"","");
  CreateTrigger("UpdateAutoCorr",PostStep,UpdateAutoCorr,"","");
  CreateTrigger("CalcAutoCorr",RunCalculations,CalcAutoCorr,"","");
  CreateContainerTrigger("AutoCorr","CreateAutoCorr,InitAutoCorr,UpdateAutoCorr,CalcAutoCorr");

  CreateTrigger("InitAutoCorrOne",PreRun,InitAutoCorrOne,"","");
  CreateTrigger("UpdateAutoCorrOne",PostStep,UpdateAutoCorrOne,"","");
  CreateTrigger("CalcAutoCorrOne",RunCalculations,CalcAutoCorrOne,"","");
  CreateContainerTrigger("AutoCorrOne","InitAutoCorrOne,UpdateAutoCorrOne,CalcAutoCorrOne");

  CreateTrigger("BranchFactor",PostStep,BranchFactor,"VarScore","");
  CreateTrigger("BranchFactorW",PostStep,BranchFactorW,"VarScoreW,CheckWeighted","");

  CreateTrigger("InitStepsUpDownSide",PreRun,InitStepsUpDownSide,"","");
  CreateTrigger("UpdateStepsUpDownSide",PostStep,UpdateStepsUpDownSide,"","");
  CreateContainerTrigger("StepsUpDownSide","InitStepsUpDownSide,UpdateStepsUpDownSide");

  CreateTrigger("NumRestarts",InitData,NumRestarts,"","");

  CreateTrigger("LoadKnownSolutions",CreateStateInfo,LoadKnownSolutions,"","");

  CreateTrigger("CreateSolutionDistance",CreateStateInfo,CreateSolutionDistance,"","");
  CreateTrigger("UpdateSolutionDistance",PostStep,UpdateSolutionDistance,"","");
  CreateContainerTrigger("SolutionDistance","CreateSolutionDistance,UpdateSolutionDistance,LoadKnownSolutions");

  CreateTrigger("InitFDCRun",PreRun,InitFDCRun,"","");
  CreateTrigger("UpdateFDCRun",UpdateStateInfo,UpdateFDCRun,"UpdateSolutionDistance","");
  CreateTrigger("CalcFDCRun",RunCalculations,CalcFDCRun,"","");
  CreateContainerTrigger("FDCRun","InitFDCRun,UpdateFDCRun,CalcFDCRun,SolutionDistance,LoadKnownSolutions");

  CreateTrigger("CreateFileRandom",PostParameters,CreateFileRandom,"","");
  CreateTrigger("CloseFileRandom",FinalReports,CloseFileRandom,"","");
  CreateContainerTrigger("FileRandom","CreateFileRandom,CloseFileRandom");

  CreateTrigger("FileAbort",PostRun,FileAbort,"","");

  CreateTrigger("DynamicParms",PostRead,DynamicParms,"","");

  CreateTrigger("FlushBuffers",PreRun,FlushBuffers,"","");

  CreateTrigger("CheckWeighted",PostRead,CheckWeighted,"","");

  CreateTrigger("CreateUniqueSolutions",CreateStateInfo,CreateUniqueSolutions,"","");
  CreateTrigger("UpdateUniqueSolutions",RunCalculations,UpdateUniqueSolutions,"","");
  CreateContainerTrigger("UniqueSolutions","CreateUniqueSolutions,UpdateUniqueSolutions");

  CreateTrigger("VarsShareClauses",CreateData,CreateVarsShareClauses,"LitOccurence","");

}


char sLine[MAXCNFLINELEN];

void ReadCNF() {
  UINT32 j;
  UINT32 k;
  UINT32 bIsWCNF;
  float fDummy;
  SINT32 l;
  SINT32 iScanRet;
  BOOL bHaveWarnedUnitClause;
  

  LITTYPE *pData;
  LITTYPE *pNextLit;
  LITTYPE *pLastLit;

  FILE *filInput;

  bIsWCNF = 0;

  iNumClauses = 0;

  SetupFile(&filInput,"r",sFilenameIn,stdin,0);

  while (iNumClauses == 0) {
    fgets(sLine,MAXCNFLINELEN,filInput);
    if (strlen(sLine)==MAXCNFLINELEN-1) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXCNFLINELEN [%d]\n",MAXCNFLINELEN);
      AbnormalExit();
      exit(1);
    }

    if (strncmp(sLine,"p wcnf",6)==0) {
      bIsWCNF = 1;
    }

    if (sLine[0] =='p') {
      if (bWeighted) {
        if (bIsWCNF) {
          sscanf(sLine,"p wcnf %"SCAN32" %"SCAN32,&iNumVars,&iNumClauses);
        } else {
          ReportPrint(pRepErr,"Warning! reading .cnf file and setting all weights = 1\n");
          sscanf(sLine,"p cnf %"SCAN32" %"SCAN32,&iNumVars,&iNumClauses);
        }
      } else {
        if (bIsWCNF) {
          ReportPrint(pRepErr,"Warning! reading .wcnf file and ignoring all weights\n");
          sscanf(sLine,"p wcnf %"SCAN32" %"SCAN32,&iNumVars,&iNumClauses);
        } else {
          sscanf(sLine,"p cnf %"SCAN32" %"SCAN32,&iNumVars,&iNumClauses);
        }
      }
    } else {
      if (sLine[0] =='c') {

      } else {
        ReportPrint1(pRepErr,"Warning: Ignoring line in input file:\n   %s",sLine);
      }
    }
  }

  if ((iNumVars==0)||(iNumClauses==0)) {
    ReportPrint(pRepErr,"Error: invalid instance file\n");
    AbnormalExit();
    exit(1);
  }

  iVARSTATELen = (iNumVars >> 3) + 1;
  if ((iNumVars & 0x07)==0) {
    iVARSTATELen--;
  }
  
  aClauseLen = (UINT32 *) AllocateRAM(iNumClauses * sizeof(UINT32));
  pClauseLits = (LITTYPE **) AllocateRAM(iNumClauses * sizeof(LITTYPE *));
  if (bWeighted) {
    aClauseWeight = (FLOAT *) AllocateRAM(iNumClauses * sizeof(FLOAT));
  }
  
  pLastLit = pNextLit = pData = 0;

  iNumLits = 0;
  iMaxClauseLen = 0;
  bHaveWarnedUnitClause = 0;

  for (j=0;j<iNumClauses;j++) {

    if (bWeighted) {
      if (bIsWCNF) {
        fscanf(filInput,"%f",&fDummy);
        aClauseWeight[j] = (FLOAT) fDummy;
      } else {
        aClauseWeight[j] = 1.0f;
      }
      fTotalWeight += aClauseWeight[j];
    } else {
      if (bIsWCNF) {
        fscanf(filInput,"%f",&fDummy);
      }
    }

    pClauseLits[j] = pNextLit;
    aClauseLen[j] = 0;

    do {
      iScanRet = fscanf(filInput,"%"SCANS32,&l);

      while (iScanRet != 1) {
        if (iScanRet==0) {
          fgets(sLine,MAXCNFLINELEN,filInput);

          if (sLine[0] =='c') {
            ReportPrint1(pRepErr,"Warning: Ingoring comment line mid instance:\n   %s",sLine);
            iScanRet = fscanf(filInput,"%"SCANS32,&l);
          } else {
            ReportPrint1(pRepErr,"Error reading instance at clause [%"P32"]\n",j);
            ReportPrint1(pRepErr,"  at or near: %s\n",sLine);
            AbnormalExit();
            exit(1);
          }
        } else {
          ReportPrint1(pRepErr,"Error reading instance. at clause [%"P32"]\n",j);
          AbnormalExit();
          exit(1);
        }
      }

      if (l) {
        
        if (pNextLit >= pLastLit) {
          pData = (LITTYPE *) AllocateRAM(LITSPERCHUNK * sizeof(LITTYPE));
          pNextLit = pData;
          pLastLit = pData + LITSPERCHUNK;
          for (k=0;k<aClauseLen[j];k++) {
            *pNextLit = pClauseLits[j][k];
            pNextLit++;
          }
          pClauseLits[j] = pData;
        }

        *pNextLit = SetLitFromFile(l);

        if (GetVarFromLit(*pNextLit) > iNumVars) {
          ReportPrint2(pRepErr,"Error: Invalid Literal [%"P32"] in clause [%"P32"]\n",l,j);
          AbnormalExit();
          exit(1);
        }

        pNextLit++;
        aClauseLen[j]++;
        iNumLits++;
      }
    } while (l != 0);

    if (aClauseLen[j] > iMaxClauseLen) {
      iMaxClauseLen = aClauseLen[j];
    }

    if (aClauseLen[j] == 0) {
      ReportPrint1(pRepErr,"Error: Reading .cnf, clause [%"P32"] is empty\n",j);
      AbnormalExit();
      exit(1);
    }
    if ((!bHaveWarnedUnitClause)&&(aClauseLen[j] == 1)) {
      ReportPrint(pRepErr,"Warning! Unit clause detected: consider using a pre-processor\n");
      bHaveWarnedUnitClause = 1;
    }
  }

  AdjustLastRAM((pNextLit - pData) * sizeof(LITTYPE));

  CloseSingleFile(filInput);  

}

void CreateLitOccurence() {

  UINT32 j,k;
  LITTYPE *pLit;
  UINT32 *pCur;

  aNumLitOcc = (UINT32 *) AllocateRAM((iNumVars+1)*2*sizeof(UINT32));
  pLitClause = (UINT32 **) AllocateRAM((iNumVars+1)*2*sizeof(UINT32 *));
  aLitOccData = (UINT32 *) AllocateRAM(iNumLits*sizeof(UINT32));

  memset(aNumLitOcc,0,(iNumVars+1)*2*sizeof(UINT32));
  
  for (j=0;j<iNumClauses;j++) {
    pLit = pClauseLits[j];
    for (k=0;k<aClauseLen[j];k++) {
      aNumLitOcc[*pLit]++;
      pLit++;
    }
  } 
  
  pCur = aLitOccData;
  for (j=0;j<(iNumVars+1)*2;j++) {
    pLitClause[j] = pCur;
    pCur += aNumLitOcc[j];
  }

  memset(aNumLitOcc,0,(iNumVars+1)*2*sizeof(UINT32));

  for (j=0;j<iNumClauses;j++) {
    pLit = pClauseLits[j];
    for (k=0;k<aClauseLen[j];k++) {
      pCur = pLitClause[*pLit] + aNumLitOcc[*pLit];
      *pCur=j;
      aNumLitOcc[*pLit]++;
      pLit++;
    }
  }
}

void CreateCandidateList() {
  aCandidateList = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
  iMaxCandidates = iNumClauses;
}

char sInitLine[MAXPARMLINELEN];

void InitVarsFromFile() {
  SINT32 iLit;
  UINT32 iVar;
  UINT32 iValue;
  UINT32 j;
  FILE *filInit;
  char *pStart;
  char *pPos;

  for (j=1;j<=iNumVars;j++) {
    aVarInit[j] = 2;
  }

  if (strcmp(sFilenameVarInit,"")) {
    SetupFile(&filInit,"r",sFilenameVarInit,stdin,0);

    while (!feof(filInit)) {
      if (fgets(sInitLine,MAXPARMLINELEN,filInit)) {
        if (strlen(sInitLine)==MAXPARMLINELEN-1) {
          ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXPARMLINELEN [%d]\n",MAXPARMLINELEN);
          AbnormalExit();
          exit(1);
        }
        if ((*sInitLine)&&(*sInitLine != '#')) {
          pStart = sInitLine;
          pPos = strchr(pStart,' ');
          while (pPos) {
            if (pPos==pStart) {
              pStart++;
            } else {
              *pPos++=0;

              sscanf(pStart,"%"SCANS32,&iLit);

              if (iLit) {
                if (iLit > 0) {
                  iValue = 1;
                  iVar = (UINT32) iLit;
                } else {
                  iValue = 0;
                  iVar = (UINT32) (-iLit);
                }
                aVarInit[iVar] = iValue;
              }

              pStart = pPos;
            }
            pPos = strchr(pStart,' ');
          }
          pPos = strchr(pStart,10);
          
          if (pPos) {
            *pPos = 0;
          }
          pPos = strchr(pStart,13);
          if (pPos) {
            *pPos = 0;
          }

          if (strlen(pStart)) {
            sscanf(pStart,"%"SCANS32,&iLit);

            if (iLit) {
              if (iLit > 0) {
                iValue = 1;
                iVar = (UINT32) iLit;
              } else {
                iValue = 0;
                iVar = (UINT32) (-iLit);
              }
              aVarInit[iVar] = iValue;
            }
          }
        }
      }
    }
  }
}


void CreateDefaultStateInfo() {
  aNumTrueLit = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
  aVarValue = (BOOL *) AllocateRAM((iNumVars+1)*sizeof(BOOL));
  aVarInit = (UINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32));
}

void InitDefaultStateInfo() {
  UINT32 j,k;
  LITTYPE litCur;
  UINT32 *pClause;
  
  memset(aNumTrueLit,0,iNumClauses*sizeof(UINT32));
  iNumFalse = 0;

  for (j=1;j<=iNumVars;j++) {
    litCur = GetTrueLit(j);
    pClause = pLitClause[litCur];
    for (k=0;k<aNumLitOcc[litCur];k++) {
      aNumTrueLit[*pClause]++;
      pClause++;
      }
    }

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      iNumFalse++;
    }
  }

  if (bWeighted) {
    fSumFalseW = FLOATZERO;
    for (j=0;j<iNumClauses;j++) {
      if (aNumTrueLit[j]==0) {
        fSumFalseW += aClauseWeight[j];
      }
    }
  }
}

void DefaultInitVars() {
  UINT32 k;
  UINT32 j;
  UINT32 iVar = 0;
  BOOL bAdded;
  UINT32 iNumPos;
  UINT32 iNumNeg;
  BOOL iNextAlternating = 0;

  if (bVarInitGreedy) {
    for (j=1;j<=iNumVars;j++) {
      iNumPos = aNumLitOcc[GetPosLit(j)];
      iNumNeg = aNumLitOcc[GetNegLit(j)];

      if (iNumPos > iNumNeg) {
        aVarInit[j] = 1;
      } else {
        if (iNumNeg > iNumPos) {
          aVarInit[j] = 0;
        } else {
          /* otherwise, alternate between 0 and 1 */
          aVarInit[j] = 3;
        }
      }
    }
  }

  for (j=1;j<=iNumVars;j++) {
    if (aVarInit[j] >= 3) {
      aVarValue[j] = iNextAlternating;
      iNextAlternating = !iNextAlternating;
    } else {
      if (aVarInit[j] == 2) {
        if (RandomInt(2)) {
          aVarValue[j] = 1;
        } else {
          aVarValue[j] = 0;
        }
      } else {
        if (aVarInit[j]) {
          aVarValue[j] = 1;
        } else {
          aVarValue[j] = 0;
        }
      }
    }
  }

  if (iInitVarFlip) {

    if (iInitVarFlip > iNumVars) {
      ReportPrint(pRepErr,"Warning! -varinitflip value is greater than the number of variables\n");
      iInitVarFlip = iNumVars;
    }

    for (j=0;j<iInitVarFlip;j++) {
      do {
        bAdded = 1;
        iVar = RandomInt(iNumVars) + 1;
        if (j > 0) {
          for (k=0;k<j;k++) {
            if (aCandidateList[k] == iVar) {
              bAdded = 0;
              break;
            }
          }
        }
      } while (bAdded == 0);
      aCandidateList[j] = iVar;
      aVarValue[iVar] = !aVarValue[iVar];
    }
  }

}

void DefaultFlip() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (--aNumTrueLit[*pClause]==0) {
      iNumFalse++;
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (++aNumTrueLit[*pClause]==1) {
      iNumFalse--;
    }
    pClause++;
  }
}

void DefaultFlipW() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (--aNumTrueLit[*pClause]==0) {
      iNumFalse++;
      fSumFalseW += aClauseWeight[*pClause];
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (++aNumTrueLit[*pClause]==1) {
      iNumFalse--;
      fSumFalseW -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}

void CheckTermination() {
  if (iNumFalse <= iTarget) {
    bSolutionFound = 1;
  }
  if (bWeighted) {
    if (fSumFalseW <= fTargetW) {
      bSolutionFound = 1;
    }
  }
}

void CreateFalseClauseList() {
  aFalseList = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
  aFalseListPos = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitFalseClauseList() {
  UINT32 j;

  iNumFalseList = 0;

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      aFalseList[iNumFalseList] = j;
      aFalseListPos[j] = iNumFalseList++;      
    }
  }
}

void UpdateFalseClauseList() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      aFalseList[iNumFalseList] = *pClause;
      aFalseListPos[*pClause] = iNumFalseList++;
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalseList];
      aFalseListPos[aFalseList[iNumFalseList]] = aFalseListPos[*pClause];
    }
    pClause++;
  }
}

void FlipFalseClauseList() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {
      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];
    }
    pClause++;
  }
}

void FlipFalseClauseListW() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
      fSumFalseW += aClauseWeight[*pClause];
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {
      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];
      fSumFalseW -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}

void CreateVarScore() {
  aVarScore = (SINT32 *) AllocateRAM((iNumVars+1)*sizeof(SINT32));
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitVarScore() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;
  
  memset(aVarScore,0,(iNumVars+1)*sizeof(int));
  memset(aCritSat,0,iNumClauses*sizeof(UINT32));

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aVarScore[GetVar(j,k)]--;
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aVarScore[iVar]++;
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}

void UpdateVarScore() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      
      aVarScore[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aVarScore[GetVarFromLit(*pLit)]--;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScore[iVar]++;
        pLit++;
      }
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aVarScore[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}

void FlipVarScore() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      iNumFalse++;

      aVarScore[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aVarScore[GetVarFromLit(*pLit)]--;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      iNumFalse--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScore[iVar]++;
        pLit++;
      }
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aVarScore[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}


void CreateVarScoreW() {
  aVarScoreW = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitVarScoreW() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;
  
  for (j=1;j<=iNumVars;j++) {
    aVarScoreW[j] = FLOATZERO;
  }
  memset(aCritSat,0,iNumClauses*sizeof(UINT32));
  
  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aVarScoreW[GetVar(j,k)] -= aClauseWeight[j];
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aVarScoreW[iVar] += aClauseWeight[j];
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}

void UpdateVarScoreW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      
      aVarScoreW[iFlipCandidate] -= aClauseWeight[*pClause];
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aVarScoreW[GetVarFromLit(*pLit)] -= aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aVarScoreW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScoreW[iVar] += aClauseWeight[*pClause];
        pLit++;
      }
      aVarScoreW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aVarScoreW[aCritSat[*pClause]] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}

void FlipVarScoreW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      iNumFalse++;
      fSumFalseW += aClauseWeight[*pClause];

      aVarScoreW[iFlipCandidate] -= aClauseWeight[*pClause];
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aVarScoreW[GetVarFromLit(*pLit)] -= aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aVarScoreW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      iNumFalse--;
      fSumFalseW -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScoreW[iVar] += aClauseWeight[*pClause];
        pLit++;
      }
      aVarScoreW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aVarScoreW[aCritSat[*pClause]] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}



void FlipVarScoreFalseClauseList() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      aVarScore[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aVarScore[GetVarFromLit(*pLit)]--;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScore[iVar]++;
        pLit++;
      }
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aVarScore[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}

void CreateMakeBreak() {
  aBreakCount = (UINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aMakeCount = (UINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitMakeBreak() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;
  
  memset(aMakeCount,0,(iNumVars+1)*sizeof(UINT32));
  memset(aBreakCount,0,(iNumVars+1)*sizeof(UINT32));
  memset(aCritSat,0,iNumClauses*sizeof(UINT32));
  
  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aMakeCount[GetVar(j,k)]++;
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}


void UpdateMakeBreak() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      aBreakCount[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aMakeCount[GetVarFromLit(*pLit)]++;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        pLit++;
      }
      aBreakCount[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}


void FlipMakeBreak() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      aBreakCount[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aMakeCount[GetVarFromLit(*pLit)]++;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        pLit++;
      }
      aBreakCount[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}

void CreateMakeBreakW() {
  aBreakCountW = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aMakeCountW = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitMakeBreakW() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;

  for (j=1;j<=iNumVars;j++) {
    aMakeCountW[j] = FLOATZERO;
    aBreakCountW[j] = FLOATZERO;
  }
  memset(aCritSat,0,iNumClauses*sizeof(UINT32));
  
  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aMakeCountW[GetVar(j,k)] += aClauseWeight[j];
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aBreakCountW[iVar] += aClauseWeight[j];
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}


void UpdateMakeBreakW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      aBreakCountW[iFlipCandidate] -=  aClauseWeight[*pClause];
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aMakeCountW[GetVarFromLit(*pLit)] += aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCountW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCountW[iVar] -= aClauseWeight[*pClause];
        pLit++;
      }
      aBreakCountW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCountW[aCritSat[*pClause]] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}


void FlipMakeBreakW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      fSumFalseW += aClauseWeight[*pClause];

      aBreakCountW[iFlipCandidate] -= aClauseWeight[*pClause];
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        aMakeCountW[GetVarFromLit(*pLit)] += aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCountW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      fSumFalseW -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCountW[iVar] -= aClauseWeight[*pClause];
        pLit++;
      }
      aBreakCountW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCountW[aCritSat[*pClause]] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}

void CreateVarInFalse() {
  aVarInFalseList = (UINT32 *) AllocateRAM((iNumVars+1)* sizeof(UINT32));
  aVarInFalseListPos = (UINT32 *) AllocateRAM((iNumVars+1)* sizeof(UINT32));
}

void InitVarInFalse() {
  UINT32 j;

  iNumVarsInFalseList = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aMakeCount[j]) {
      aVarInFalseList[iNumVarsInFalseList] = j;
      aVarInFalseListPos[j] = iNumVarsInFalseList++;
    }
  }
}

void UpdateVarInFalse() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      
      aBreakCount[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {

        iVar = GetVarFromLit(*pLit);

        aMakeCount[iVar]++;

        if (aMakeCount[iVar]==1) {
          aVarInFalseList[iNumVarsInFalseList] = iVar;
          aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        }

        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }

        pLit++;
      }
      aBreakCount[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}

void FlipVarInFalse() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      iNumFalse++;

      aBreakCount[iFlipCandidate]--;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {

        iVar = GetVarFromLit(*pLit);

        aMakeCount[iVar]++;

        if (aMakeCount[iVar]==1) {
          aVarInFalseList[iNumVarsInFalseList] = iVar;
          aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        }

        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      iNumFalse--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }

        pLit++;
      }
      aBreakCount[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
    }
    pClause++;
  }
}

void CreateVarLastChange() {
  aVarLastChange = (UBIGINT *) AllocateRAM((iNumVars+1)*sizeof(UBIGINT));
}

void InitVarLastChange() {
  UINT32 j;

  for (j=0;j<=iNumVars;j++) {
    aVarLastChange[j] = iStep-1;
  }
  iVarLastChangeReset = iStep;
}

void UpdateVarLastChange() {
  aVarLastChange[iFlipCandidate] = iStep;
}


void CreateTrackChanges() {
  aChangeList = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aChangeLastStep = (UBIGINT *) AllocateRAM((iNumVars+1) * sizeof(UBIGINT));
  aChangeOldScore = (SINT32 *) AllocateRAM((iNumVars+1) * sizeof(SINT32));
}

void InitTrackChanges() {
  memset(aChangeLastStep,0,(iNumVars+1) * sizeof(UBIGINT));
}

void UpdateTrackChanges() {
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChanges = 0;

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      
      UpdateChange(iFlipCandidate);
      aVarScore[iFlipCandidate]--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChange(iVar);
        aVarScore[iVar]--;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          UpdateChange(iVar);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChange(iVar);
        aVarScore[iVar]++;
        pLit++;
      }
      UpdateChange(iFlipCandidate);
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      iVar = aCritSat[*pClause];
      UpdateChange(iVar);
      aVarScore[iVar]--;
    }
    pClause++;
  }
}

void FlipTrackChanges() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChanges = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      iNumFalse++;
      
      UpdateChange(iFlipCandidate);
      aVarScore[iFlipCandidate]--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChange(iVar);
        aVarScore[iVar]--;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          UpdateChange(iVar);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      iNumFalse--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChange(iVar);
        aVarScore[iVar]++;
        pLit++;
      }
      UpdateChange(iFlipCandidate);
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      iVar = aCritSat[*pClause];
      UpdateChange(iVar);
      aVarScore[iVar]--;
    }
    pClause++;
  }
}

void FlipTrackChangesFCL() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChanges = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
      
      UpdateChange(iFlipCandidate);
      aVarScore[iFlipCandidate]--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChange(iVar);
        aVarScore[iVar]--;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          UpdateChange(iVar);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChange(iVar);
        aVarScore[iVar]++;
        pLit++;
      }
      UpdateChange(iFlipCandidate);
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      iVar = aCritSat[*pClause];
      UpdateChange(iVar);
      aVarScore[iVar]--;
    }
    pClause++;
  }
}

void FlipDecPromVarsFCL() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChanges = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==1) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
      
      aVarScore[iFlipCandidate]--;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScore[iVar]--;
        if (aVarScore[iVar] == -1) {
          aDecPromVarsList[iNumDecPromVars++] = iVar;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      aVarScore[iVar]--;
      if (aVarScore[iVar] == -1) {
        aDecPromVarsList[iNumDecPromVars++] = iVar;
      }
    }
    pClause++;
  }

  // 2nd pass

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aVarScore[iVar]++;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aVarScore[iVar]++;
        pLit++;
      }
      aVarScore[iFlipCandidate]++;
      aCritSat[*pClause] = iFlipCandidate;
    }
    pClause++;
  }

  j=0;
  k=0;
  while (j < iNumDecPromVars) {
    iVar = aDecPromVarsList[k];
    if ((aVarScore[iVar] >= 0)||(iVar == iFlipCandidate)) {
      iNumDecPromVars--;
    } else {
      aDecPromVarsList[j++]=aDecPromVarsList[k];
    }
    k++;
  }
}


void FlipPenaltyDecPromVarsFCL() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  SINT32 iPenalty;
  SINT32 iPrevScore;
  SINT32 iCurScore;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChanges = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    if (aNumTrueLit[*pClause]==1) {
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
      
      //aVarScore[iFlipCandidate]--;
      aBreakPenaltyINT[iFlipCandidate] -= iPenalty;

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        iPrevScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
        aMakePenaltyINT[iVar] += iPenalty;
        if ((iPrevScore >= 0)&&(iPrevScore < iPenalty)) { // ie: new score now < 0
          aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    if (aNumTrueLit[*pClause]==1) {
      iVar = aCritSat[*pClause];
      iPrevScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
      //aVarScore[iVar]--;
      aBreakPenaltyINT[iVar] -= iPenalty;
      if ((iPrevScore >= 0)&&(iPrevScore < iPenalty)) { // see above
        aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
      }
    }
    pClause++;
  }

  // 2nd pass

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          //aVarScore[iVar]++;
          aBreakPenaltyINT[iVar] += iPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        //aVarScore[iVar]++;
        aMakePenaltyINT[iVar] -= iPenalty;
        pLit++;
      }
      //aVarScore[iFlipCandidate]++;
      aBreakPenaltyINT[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    pClause++;
  }

  j=0;
  k=0;
  while (j < iNumPenaltyDecPromVars) {
    iVar = aPenaltyDecPromVarsList[k];
    iCurScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
    if ((iCurScore >= 0)||(iVar == iFlipCandidate)) {
      iNumPenaltyDecPromVars--;
    } else {
      aPenaltyDecPromVarsList[j++]=aPenaltyDecPromVarsList[k];
    }
    k++;
  }
}


#define UpdateChangeW(var) {if(aChangeLastStepW[var]!=iStep) {aChangeOldScoreW[var] = aVarScoreW[var]; aChangeLastStepW[var]=iStep; aChangeListW[iNumChangesW++]=var;}}

void CreateTrackChangesW() {
  aChangeListW = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aChangeLastStepW = (UBIGINT *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aChangeOldScoreW = (FLOAT *) AllocateRAM((iNumVars+1) * sizeof(FLOAT));
}

void InitTrackChangesW() {
  memset(aChangeLastStepW,0,(iNumVars+1) * sizeof(UBIGINT));
}

void UpdateTrackChangesW() {
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChangesW = 0;

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (aNumTrueLit[*pClause]==0) { 
      
      UpdateChangeW(iFlipCandidate);
      aVarScoreW[iFlipCandidate] -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChangeW(iVar);
        aVarScoreW[iVar] -= aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          UpdateChangeW(iVar);
          aVarScoreW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (aNumTrueLit[*pClause]==1) {

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChangeW(iVar);
        aVarScoreW[iVar] += aClauseWeight[*pClause];
        pLit++;
      }
      UpdateChangeW(iFlipCandidate);
      aVarScoreW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      iVar = aCritSat[*pClause];
      UpdateChangeW(iVar);
      aVarScoreW[iVar] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}

void FlipTrackChangesW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChangesW = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      iNumFalse++;
      fSumFalseW += aClauseWeight[*pClause];
      
      UpdateChangeW(iFlipCandidate);
      aVarScoreW[iFlipCandidate] -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChangeW(iVar);
        aVarScoreW[iVar] -= aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          UpdateChangeW(iVar);
          aVarScoreW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      iNumFalse--;
      fSumFalseW -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChangeW(iVar);
        aVarScoreW[iVar] += aClauseWeight[*pClause];
        pLit++;
      }
      UpdateChangeW(iFlipCandidate);
      aVarScoreW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      iVar = aCritSat[*pClause];
      UpdateChangeW(iVar);
      aVarScoreW[iVar] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}

void FlipTrackChangesFCLW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0) {
    return;
  }

  iNumChangesW = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 

      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;
      fSumFalseW += aClauseWeight[*pClause];
      
      UpdateChangeW(iFlipCandidate);
      aVarScoreW[iFlipCandidate] -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChangeW(iVar);
        aVarScoreW[iVar] -= aClauseWeight[*pClause];
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          UpdateChangeW(iVar);
          aVarScoreW[iVar] += aClauseWeight[*pClause];
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];
      fSumFalseW -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        UpdateChangeW(iVar);
        aVarScoreW[iVar] += aClauseWeight[*pClause];
        pLit++;
      }
      UpdateChangeW(iFlipCandidate);
      aVarScoreW[iFlipCandidate] += aClauseWeight[*pClause];
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      iVar = aCritSat[*pClause];
      UpdateChangeW(iVar);
      aVarScoreW[iVar] -= aClauseWeight[*pClause];
    }
    pClause++;
  }
}


void CreateDecPromVars() {
  aDecPromVarsList = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
}

void InitDecPromVars() {

  UINT32 j;

  iNumDecPromVars = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarScore[j] < 0) {
      aDecPromVarsList[iNumDecPromVars++] = j;
    }
  }
}

void UpdateDecPromVars() {

  UINT32 j,k;
  UINT32 iVar;

  for (j=0;j<iNumChanges;j++) {
    iVar = aChangeList[j];
    if ((aVarScore[iVar] < 0)&&(aChangeOldScore[iVar] >= 0)) {
      aDecPromVarsList[iNumDecPromVars++] = iVar;
    }
  }
  j=0;
  k=0;
  while (j < iNumDecPromVars) {
    iVar = aDecPromVarsList[k];
    if ((aVarScore[iVar] >= 0)||(iVar == iFlipCandidate)) {
      iNumDecPromVars--;
    } else {
      aDecPromVarsList[j++]=aDecPromVarsList[k];
    }
    k++;
  }
}

void CreateDecPromVarsW() {

  aDecPromVarsListW = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));

}

void InitDecPromVarsW() {

  UINT32 j;

  iNumDecPromVarsW = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarScoreW[j] < FLOATZERO) {
      aDecPromVarsListW[iNumDecPromVarsW++] = j;
    }
  }
}

void UpdateDecPromVarsW() {

  UINT32 j,k;
  UINT32 iVar;

  for (j=0;j<iNumChangesW;j++) {
    iVar = aChangeListW[j];
    if ((aVarScoreW[iVar] < FLOATZERO)&&(aChangeOldScoreW[iVar] >= 0)) {
      aDecPromVarsListW[iNumDecPromVarsW++] = iVar;
    }
  }
  j=0;
  k=0;
  while (j < iNumDecPromVarsW) {
    iVar = aDecPromVarsListW[k];
    if ((aVarScoreW[iVar] >= 0)||(iVar == iFlipCandidate)) {
      iNumDecPromVarsW--;
    } else {
      aDecPromVarsListW[j++]=aDecPromVarsListW[k];
    }
    k++;
  }
}

void CreatePenaltyDecPromVars() {
  aPenaltyDecPromVarsList = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
}

void InitPenaltyDecPromVars() {

  UINT32 j;

  iNumPenaltyDecPromVars = 0;

  for (j=1;j<=iNumVars;j++) {
    //if (aVarScorej[] < 0) {
    if ((SINT32) aBreakPenaltyINT[j] - (SINT32) aMakePenaltyINT[j] < 0) {
      aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = j;
    }
  }
}

void UpdatePenaltyDecPromVars() {

  ReportPrint(pRepErr,"Unexpected Error: TODO: UpdatePenaltyDecPromVars\n");
  AbnormalExit();
  exit(1);

  // need to use a penalized changeoldscore

  //UINT32 j,k;
  //UINT32 iVar;

  //for (j=0;j<iNumChanges;j++) {
  //  iVar = aChangeList[j];
  //  if ((aVarScore[iVar] < 0)&&(aChangeOldScore[iVar] >= 0)) {
  //    aDecPromVarsList[iNumDecPromVars++] = iVar;
  //  }
  //}
  //j=0;
  //k=0;
  //while (j < iNumDecPromVars) {
  //  iVar = aDecPromVarsList[k];
  //  if ((aVarScore[iVar] >= 0)||(iVar == iFlipCandidate)) {
  //    iNumDecPromVars--;
  //  } else {
  //    aDecPromVarsList[j++]=aDecPromVarsList[k];
  //  }
  //  k++;
  //}
}

void CreateBestScoreList() {
  aBestScoreList = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aBestScoreListPos = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
}


void InitBestScoreList() {
  UINT32 j;

  iBestScore = (SINT32) iNumClauses;
  iNumBestScoreList = 0;
  
  for (j=1;j<=iNumVars;j++) {
    if (aVarScore[j] == iBestScore) {
      aBestScoreListPos[j] = iNumBestScoreList;
      aBestScoreList[iNumBestScoreList++] = j;
    }
    else if (aVarScore[j] < iBestScore) {
      iBestScore = aVarScore[j];
      iNumBestScoreList = 1;
      aBestScoreList[0] = j;
      aBestScoreListPos[j] = 0;
    }
  }
}

void UpdateBestScoreList() {
  UINT32 j;
  SINT32 iBestNewScore;
  UINT32 iVar;

  if (iNumChanges==0) {
    return;
  }

  iBestNewScore = (SINT32) iNumClauses;

  for (j=0;j<iNumChanges;j++) {
    if (aVarScore[aChangeList[j]] < iBestNewScore) {
      iBestNewScore = aVarScore[aChangeList[j]];
    };
  }

  if (iBestNewScore < iBestScore) {
    iBestScore = iBestNewScore;
    iNumBestScoreList = 0;
    for (j=0;j<iNumChanges;j++) {
      iVar = aChangeList[j];
      if (aVarScore[iVar] == iBestScore) {
        aBestScoreListPos[iVar] = iNumBestScoreList;
        aBestScoreList[iNumBestScoreList++] = iVar;
      }
    }
  } else {
    for (j=0;j<iNumChanges;j++) {
      iVar = aChangeList[j];
      if (aVarScore[iVar] == iBestScore) {
        if (aChangeOldScore[iVar] != iBestScore) {
          aBestScoreListPos[iVar] = iNumBestScoreList;
          aBestScoreList[iNumBestScoreList++] = iVar;
        }
      } else if (aChangeOldScore[iVar] == iBestScore) {
        aBestScoreList[aBestScoreListPos[iVar]] = aBestScoreList[--iNumBestScoreList];
        aBestScoreListPos[aBestScoreList[iNumBestScoreList]] = aBestScoreListPos[iVar];
      }
    }
  }
  if (iNumBestScoreList == 0) {
    InitBestScoreList();
  }
}

void CreateClausePenaltyFL() {
  aClausePenaltyFL = (FLOAT *) AllocateRAM(iNumClauses * sizeof(FLOAT));
  bClausePenaltyCreated = 1;
  bClausePenaltyFLOAT = 1;
}

void InitClausePenaltyFL() {
  UINT32 j;
  
  for (j=0;j<iNumClauses;j++) {
    aClausePenaltyFL[j] = 1.0f;
  }

  fBasePenaltyFL = 1.0f;
  fTotalPenaltyFL = (FLOAT) iNumClauses;
 
}

void InitClausePenaltyFLW() {
  UINT32 j;
  
  fTotalPenaltyFL = FLOATZERO;  

  for (j=0;j<iNumClauses;j++) {
    aClausePenaltyFL[j] = aClauseWeight[j];
    fTotalPenaltyFL += aClausePenaltyFL[j];
  }

  fBasePenaltyFL = 1.0f;

}


void CreateMakeBreakPenaltyFL() {
  aBreakPenaltyFL = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aMakePenaltyFL = (FLOAT *) AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitMakeBreakPenaltyFL() {

  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;

  for (j=1;j<=iNumVars;j++) {
    aMakePenaltyFL[j] = FLOATZERO;
    aBreakPenaltyFL[j] = FLOATZERO;
  }

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aMakePenaltyFL[GetVar(j,k)] += aClausePenaltyFL[j];
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aBreakPenaltyFL[iVar] += aClausePenaltyFL[j];
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}

void UpdateMakeBreakPenaltyFL() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  FLOAT fPenalty;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    fPenalty = aClausePenaltyFL[*pClause];
    if (aNumTrueLit[*pClause]==0) { 
      
      aBreakPenaltyFL[iFlipCandidate] -= fPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakePenaltyFL[iVar] += fPenalty;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakPenaltyFL[iVar] += fPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    fPenalty = aClausePenaltyFL[*pClause];
    if (aNumTrueLit[*pClause]==1) {

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakePenaltyFL[iVar] -= fPenalty;
        pLit++;

      }
      aBreakPenaltyFL[iFlipCandidate] += fPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakPenaltyFL[aCritSat[*pClause]] -= fPenalty;
    }
    pClause++;
  }
}


void FlipMBPFLandFCLandVIF() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  FLOAT fPenalty;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    fPenalty = aClausePenaltyFL[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      aBreakCount[iFlipCandidate]--;
      
      aBreakPenaltyFL[iFlipCandidate] -= fPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]++;
        aMakePenaltyFL[iVar] += fPenalty;

        if (aMakeCount[iVar]==1) {
          aVarInFalseList[iNumVarsInFalseList] = iVar;
          aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        }
        
        pLit++;

      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aBreakPenaltyFL[iVar] += fPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    fPenalty = aClausePenaltyFL[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        aMakePenaltyFL[iVar] -= fPenalty;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }
        
        pLit++;

      }
      aBreakCount[iFlipCandidate]++;
      aBreakPenaltyFL[iFlipCandidate] += fPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
      aBreakPenaltyFL[aCritSat[*pClause]] -= fPenalty;
    }
    pClause++;
  }
}

void FlipMBPFLandFCLandVIFandW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  FLOAT fPenalty;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    fPenalty = aClausePenaltyFL[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      fSumFalseW += aClauseWeight[*pClause];

      aBreakCount[iFlipCandidate]--;
      
      aBreakPenaltyFL[iFlipCandidate] -= fPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]++;
        aMakePenaltyFL[iVar] += fPenalty;

        if (aMakeCount[iVar]==1) {
          aVarInFalseList[iNumVarsInFalseList] = iVar;
          aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        }
        
        pLit++;

      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aBreakPenaltyFL[iVar] += fPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    fPenalty = aClausePenaltyFL[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      fSumFalseW -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        aMakePenaltyFL[iVar] -= fPenalty;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }
        
        pLit++;

      }
      aBreakCount[iFlipCandidate]++;
      aBreakPenaltyFL[iFlipCandidate] += fPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
      aBreakPenaltyFL[aCritSat[*pClause]] -= fPenalty;
    }
    pClause++;
  }
}

void CreateClausePenaltyINT() {
  aClausePenaltyINT = (UINT32 *) AllocateRAM(iNumClauses * sizeof(UINT32));
  bClausePenaltyCreated = 1;
  bClausePenaltyFLOAT = 0;
  iInitPenaltyINT = 1;
}

void InitClausePenaltyINT() {
  UINT32 j;
  
  for (j=0;j<iNumClauses;j++) {
    aClausePenaltyINT[j] = iInitPenaltyINT;
  }
  iBasePenaltyINT = iInitPenaltyINT;
  iTotalPenaltyINT = iNumClauses * iInitPenaltyINT;
 
}

void InitClausePenaltyINTW() {
  UINT32 j;

  ReportPrint(pRepErr,"Warning! InitClausePenaltyINTW() May need to be modified to suit your algorithm\n");

  iTotalPenaltyINT = 0;  

  for (j=0;j<iNumClauses;j++) { 
    aClausePenaltyINT[j] = (UINT32) aClauseWeight[j];
    iTotalPenaltyINT += aClausePenaltyINT[j];
  }

  iBasePenaltyINT = 1;

}


void CreateMakeBreakPenaltyINT() {
  aBreakPenaltyINT = (UINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aMakePenaltyINT = (UINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aCritSat = (UINT32 *) AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitMakeBreakPenaltyINT() {

  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;

  for (j=1;j<=iNumVars;j++) {
    aMakePenaltyINT[j] = 0;
    aBreakPenaltyINT[j] = 0;
  }

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aMakePenaltyINT[GetVar(j,k)] += aClausePenaltyINT[j];
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aBreakPenaltyINT[iVar] += aClausePenaltyINT[j];
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}

void UpdateMakeBreakPenaltyINT() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  UINT32 iPenalty;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    if (aNumTrueLit[*pClause]==0) { 
      
      aBreakPenaltyINT[iFlipCandidate] -= iPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakePenaltyINT[iVar] += iPenalty;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakPenaltyINT[iVar] += iPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    if (aNumTrueLit[*pClause]==1) {

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakePenaltyINT[iVar] -= iPenalty;
        pLit++;

      }
      aBreakPenaltyINT[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakPenaltyINT[aCritSat[*pClause]] -= iPenalty;
    }
    pClause++;
  }
}


void FlipMBPINTandFCLandVIF() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  UINT32 iPenalty;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      aBreakCount[iFlipCandidate]--;
      
      aBreakPenaltyINT[iFlipCandidate] -= iPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]++;
        aMakePenaltyINT[iVar] += iPenalty;

        if (aMakeCount[iVar]==1) {
          aVarInFalseList[iNumVarsInFalseList] = iVar;
          aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        }
        
        pLit++;

      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aBreakPenaltyINT[iVar] += iPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        aMakePenaltyINT[iVar] -= iPenalty;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }
        
        pLit++;

      }
      aBreakCount[iFlipCandidate]++;
      aBreakPenaltyINT[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
      aBreakPenaltyINT[aCritSat[*pClause]] -= iPenalty;
    }
    pClause++;
  }
}

void FlipMBPINTandFCLandVIFandW() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  UINT32 iPenalty;

  if (iFlipCandidate == 0) {
    return;
  }

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = !aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      fSumFalseW += aClauseWeight[*pClause];

      aBreakCount[iFlipCandidate]--;
      
      aBreakPenaltyINT[iFlipCandidate] -= iPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]++;
        aMakePenaltyINT[iVar] += iPenalty;

        if (aMakeCount[iVar]==1) {
          aVarInFalseList[iNumVarsInFalseList] = iVar;
          aVarInFalseListPos[iVar] = iNumVarsInFalseList++;
        }
        
        pLit++;

      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakCount[iVar]++;
          aBreakPenaltyINT[iVar] += iPenalty;
          aCritSat[*pClause] = iVar;
          break;
        }
        pLit++;
      }
    }
    pClause++;
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    iPenalty = aClausePenaltyINT[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      fSumFalseW -= aClauseWeight[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        aMakePenaltyINT[iVar] -= iPenalty;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }
        
        pLit++;

      }
      aBreakCount[iFlipCandidate]++;
      aBreakPenaltyINT[iFlipCandidate] += iPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
      aBreakPenaltyINT[aCritSat[*pClause]] -= iPenalty;
    }
    pClause++;
  }
}

void InitNullFlips() {
  iNumNullFlips = 0;
}

void UpdateNullFlips() {
  if (iFlipCandidate==0) {
    iNumNullFlips++;
  }
}

void InitLocalMins() {
  iNumLocalMins = 0;
}

void UpdateLocalMins() {
  if (IsLocalMinimum(bWeighted)) {
    iNumLocalMins++;
  }
}

void CreateLogDist() {
  UINT32 iMaxLogDistValues;
  FLOAT fCurValue;
  FLOAT fFactor;

  iMaxLogDistValues = ((UINT32) log10((FLOAT) iCutoff)) * iLogDistStepsPerDecade + 2;
  aLogDistValues = (UBIGINT *) AllocateRAM(iMaxLogDistValues * sizeof(UBIGINT));

  fFactor = powf(10.0f,1.0f / (float) iLogDistStepsPerDecade);
  fCurValue = 10.0f;

  iNumLogDistValues = 0;

  while (fCurValue <= (FLOAT) iCutoff) {
    aLogDistValues[iNumLogDistValues++] = (UBIGINT) fCurValue;
    fCurValue *= fFactor;
  }
  if (aLogDistValues[iNumLogDistValues-1] != iCutoff) {
    aLogDistValues[iNumLogDistValues++] = iCutoff;
  }
}

void InitBestFalse() {
  iBestNumFalse = iNumClauses+1;
  iBestStepNumFalse = iStep; 

  fBestSumFalseW = fTotalWeight + 1.0f;
  iBestStepSumFalseW = iStep;
}

void UpdateBestFalse() {
  if (iNumFalse < iBestNumFalse) {
    iBestNumFalse = iNumFalse;
    iBestStepNumFalse = iStep;
  }
  if (bWeighted) {
    if (fSumFalseW < fBestSumFalseW) {
      fBestSumFalseW = fSumFalseW;
      iBestStepSumFalseW = iStep;
    }
  }
}

void CreateSaveBest() {
  vsBest = NewVarState();
}

void UpdateSaveBest() {
  BOOL bSave = 0;

  if (bWeighted) {
    if (iBestStepSumFalseW == iStep) {
      bSave = 1;
    }
  } else {
    if (iBestStepNumFalse == iStep) {
      bSave = 1;
    }
  }
  if (bSave) {
    SetCurVarState(vsBest);
  }
}


void UpdateStartFalse() {
  if (iStep==1) {
    iStartNumFalse = iNumFalse;
    if (bWeighted) {
      fStartSumFalseW = fSumFalseW;
    }
  }
}


void CalcImproveMean() {
  if (iBestStepNumFalse <= 1) {
    fImproveMean = FLOATZERO;
  } else {
    fImproveMean = (FLOAT) (iStartNumFalse - iBestNumFalse) / (FLOAT) (iBestStepNumFalse - 1);
  }
  if (bWeighted) {
    if (iBestStepSumFalseW <= 1) {
      fImproveMeanW = FLOATZERO;
    } else {
      fImproveMeanW = (FLOAT) (fStartSumFalseW - fBestSumFalseW) / (FLOAT) (iBestStepSumFalseW - 1);
    }
  }
}


void InitFirstLM() {
  iFirstLM = 0;
  iFirstLMStep = 0;
  fFirstLMW = FLOATZERO;
  iFirstLMStepW = 0;
}

void UpdateFirstLM() {
  if (iFirstLMStep==0) {
    if (IsLocalMinimum(0)) {
      iFirstLM = iNumFalse;
      iFirstLMStep = iStep;
    }
  }
  if ((bWeighted)&&(iFirstLMStepW==FLOATZERO)) {
    if (IsLocalMinimum(1)) {
      fFirstLMW = fSumFalseW;
      iFirstLMStepW = iStep;
    }
  }
}


void CalcFirstLMRatio() {
  fFirstLMRatio = FLOATZERO;
  fFirstLMRatioW = FLOATZERO;
  if (iStartNumFalse != iBestNumFalse) {
    fFirstLMRatio = (FLOAT)(iStartNumFalse - iFirstLM) / (FLOAT)(iStartNumFalse - iBestNumFalse);
  }
  if (bWeighted) {
    if (fStartSumFalseW != fBestSumFalseW) {
      fFirstLMRatioW = (fStartSumFalseW - fFirstLMW) / (fStartSumFalseW - fBestSumFalseW);
    }
  }
}

void UpdateTrajBestLM() {

  if (iStep==1) {
    iTrajBestLMCount = 0;
    fTrajBestLMSum = FLOATZERO;
    fTrajBestLMSum2 = FLOATZERO;
    if (bWeighted) {
      iTrajBestLMCountW = 0;
      fTrajBestLMSumW = FLOATZERO;
      fTrajBestLMSum2W = FLOATZERO;
    }
  } else {
    if (iBestStepNumFalse==(iStep-1)) {
      iTrajBestLMCount++;
      fTrajBestLMSum += (FLOAT) iBestNumFalse;
      fTrajBestLMSum2 += (FLOAT) (iBestNumFalse * iBestNumFalse);
    }
    if (bWeighted) {
      if (iBestStepSumFalseW==(iStep-1)) {
        iTrajBestLMCountW++;
        fTrajBestLMSumW += fBestSumFalseW;
        fTrajBestLMSum2W += (fBestSumFalseW * fBestSumFalseW);
      }
    }
  }
}


void CalcTrajBestLM() {
  FLOAT fStdDev;
  if (iBestStepNumFalse==iStep) {
    iTrajBestLMCount++;
    fTrajBestLMSum += (FLOAT) iBestNumFalse;
    fTrajBestLMSum2 += (FLOAT) (iBestNumFalse * iBestNumFalse);
  }
  if (bWeighted) {
    if (iBestStepSumFalseW==(iStep-1)) {
      iTrajBestLMCountW++;
      fTrajBestLMSumW += fBestSumFalseW;
      fTrajBestLMSum2W += (fBestSumFalseW * fBestSumFalseW);
    }
  }
  
  CalculateStats(&fTrajBestLMMean,&fStdDev,&fTrajBestLMCV,fTrajBestLMSum,fTrajBestLMSum2,iTrajBestLMCount);
  if (bWeighted) {
    CalculateStats(&fTrajBestLMMeanW,&fStdDev,&fTrajBestLMCVW,fTrajBestLMSumW,fTrajBestLMSum2W,iTrajBestLMCountW);
  }
}


void CheckNoImprove() {
  if (iNoImprove) {
    if (iStep > (iBestStepNumFalse + iNoImprove)) {
      bTerminateRun = 1;
    }
    if (bWeighted) {
      if (iStep > (iBestStepSumFalseW + iNoImprove)) {
        bTerminateRun = 1;
      }
    }
  }
}


void CheckEarlyTerm() {
  if (iEarlyTermSteps) {
    if (iStep == iEarlyTermSteps) {
      if (bWeighted) {
        if (fEarlyTermQualW > FLOATZERO) {
          if (fBestSumFalseW > fEarlyTermQualW) {
            bTerminateRun = 1;
          }
        }
      } else {
        if (iEarlyTermQual) {
          if (iBestNumFalse > iEarlyTermQual) {
            bTerminateRun = 1;
          }
        }
      }
    }
  }
}


void CheckStrikes() {
  if (!bSolutionFound) {
    if ((iRun - iNumSolutionsFound) >= iStrikes) {
      bTerminateAllRuns = 1;
    }
  }
}


void StartSeed () {
  if (iRun==1) {
    iStartSeed = iSeed;
  } else {
    iStartSeed = RandomMax();
    RandomSeed(iStartSeed);
  }
}

void CheckTimeout() {
  double fTimeElapsed;

  if (fTimeOut > FLOATZERO) {
    fTimeElapsed = RunTimeElapsed();
    if (fTimeElapsed > (double) fTimeOut) {
      bTerminateRun = 1;
    }
  }
  if (fGlobalTimeOut > FLOATZERO) {
    fTimeElapsed = TotalTimeElapsed();
    if (fTimeElapsed > (double) fGlobalTimeOut) {
      bTerminateRun = 1;
      bTerminateAllRuns = 1;
    }
  }
}

void CheckForRestarts() {
  if (iPeriodicRestart) {
    if ((iStep % iPeriodicRestart) == 0) {
      bRestart = 1;
    }
  }
  if (iProbRestart) {
    if (RandomProb(iProbRestart)) {
      bRestart = 1;
    }
  }
  if (iStagnateRestart) {
    if (iStep > (iBestStepNumFalse + iStagnateRestart)) {
      bRestart = 1;
      InitBestFalse();
    }
    if (bWeighted) {
      if (iStep > (iBestStepSumFalseW + iStagnateRestart)) {
        bRestart = 1;
        InitBestFalse();
      }
    }
  }
}      

void CreateFlipCounts() {
  aFlipCounts = (UBIGINT *) AllocateRAM((iNumVars+1)*sizeof(UBIGINT));
}

void InitFlipCounts() {
  if (iStep == 1) {
    memset(aFlipCounts,0,(iNumVars+1)*sizeof(UBIGINT));
  }
}

void UpdateFlipCounts() {
  aFlipCounts[iFlipCandidate]++;
}

void FlipCountStats() {
  UINT32 j;
  FLOAT fSum = FLOATZERO;
  FLOAT fSum2 = FLOATZERO;
  
  for (j=1;j<(iNumVars+1);j++) {
    fSum += (FLOAT) aFlipCounts[j];
    fSum2 += (FLOAT) (aFlipCounts[j] * aFlipCounts[j]);
  }

  CalculateStats(&fFlipCountsMean,&fFlipCountsStdDev,&fFlipCountsCV,fSum,fSum2,iNumVars);
}

void CreateBiasCounts() {
  aBiasTrueCounts = (UBIGINT *) AllocateRAM((iNumVars+1)*sizeof(UBIGINT));
  aBiasFalseCounts = (UBIGINT *) AllocateRAM((iNumVars+1)*sizeof(UBIGINT));
}

void PreInitBiasCounts() {
  UINT32 j;
  if (iStep == 1) {
    memset(aBiasTrueCounts,0,(iNumVars+1)*sizeof(UBIGINT));
    memset(aBiasFalseCounts,0,(iNumVars+1)*sizeof(UBIGINT));
  } else {
    for (j=1;j<=iNumVars;j++) {
      if (aVarValue[j]) {
        aBiasTrueCounts[j] += (iStep-1-(aVarLastChange[j]));
      } else {
        aBiasFalseCounts[j] += (iStep-1-(aVarLastChange[j]));
      }
    }
  }
}

void UpdateBiasCounts() {
  if (iFlipCandidate) {
    if (aVarValue[iFlipCandidate]) {
      aBiasTrueCounts[iFlipCandidate] += (iStep-(aVarLastChange[iFlipCandidate]));
    } else {
      aBiasFalseCounts[iFlipCandidate] += (iStep-(aVarLastChange[iFlipCandidate]));
    }
  }
}

void FinalBiasCounts() {
  UINT32 j;
  for (j=1;j<=iNumVars;j++) {
    if (aVarValue[j]) {
      aBiasTrueCounts[j] += (iStep-(aVarLastChange[j]));
    } else {
      aBiasFalseCounts[j] += (iStep-(aVarLastChange[j]));
    }
  }
}

void BiasStats() {
  UBIGINT iCountSame;
  UBIGINT iCountDiff;
  UBIGINT iCountMax;
  UBIGINT iCountMin;
  UINT32 j;

  iCountSame = 0;
  iCountDiff = 0;
  iCountMax = 0;
  iCountMin = 0;

  for (j=1;j<=iNumVars;j++) {
    if (aVarValue[j]) {
      iCountSame += aBiasTrueCounts[j];
      iCountDiff += aBiasFalseCounts[j];
    } else {
      iCountSame += aBiasFalseCounts[j];
      iCountDiff += aBiasTrueCounts[j];
    }
    if (aBiasTrueCounts[j] > aBiasFalseCounts[j]) {
      iCountMax += aBiasTrueCounts[j];
      iCountMin += aBiasFalseCounts[j];
    } else {
      iCountMax += aBiasFalseCounts[j];
      iCountMin += aBiasTrueCounts[j];
    }
  }
  if (iCountSame+iCountDiff > 0) {
    fMeanFinalBias = ((FLOAT)iCountSame) / ((FLOAT)(iCountSame+iCountDiff));
  } else {
    fMeanFinalBias = FLOATZERO;
  }
  if (iCountMax+iCountMin > 0) {
    fMeanMaxBias = ((FLOAT)iCountMax) / ((FLOAT)(iCountMax+iCountMin));
  } else {
    fMeanMaxBias = FLOATZERO;
  }
}

void CreateUnsatCounts() {
  aUnsatCounts = (UBIGINT *) AllocateRAM(iNumClauses*sizeof(UBIGINT));
}

void InitUnsatCounts() {
  if (iStep == 1) {
    memset(aUnsatCounts,0,iNumClauses*sizeof(UBIGINT));
  }
}

void UpdateUnsatCounts() {
  UINT32 j;
  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      aUnsatCounts[j]++;
    }
  }
}

void UnsatCountStats() {
  UINT32 j;
  FLOAT fSum = FLOATZERO;
  FLOAT fSum2 = FLOATZERO;
  
  for (j=0;j<iNumClauses;j++) {
    fSum += (FLOAT) aUnsatCounts[j];
    fSum2 += (FLOAT) (aUnsatCounts[j] * aUnsatCounts[j]);
  }

  CalculateStats(&fUnsatCountsMean,&fUnsatCountsStdDev,&fUnsatCountsCV,fSum,fSum2,iNumClauses);
}

void CreateNumFalseCounts() {
  aNumFalseCounts = (UBIGINT *) AllocateRAM((iNumClauses+1)*sizeof(UBIGINT));
  if (iReportFalseHistCount) {
    aNumFalseCountsWindow = (UBIGINT *) AllocateRAM(iReportFalseHistCount*sizeof(UBIGINT));
  }
}

void InitNumFalseCounts() {
  if (iStep == 1) {
    memset(aNumFalseCounts,0,(iNumClauses+1)*sizeof(UBIGINT));
    if (iReportFalseHistCount) {
      memset(aNumFalseCountsWindow,0,iReportFalseHistCount*sizeof(UBIGINT));
    }
  }
}

void UpdateNumFalseCounts() {
  aNumFalseCounts[iNumFalse]++;

  if (iReportFalseHistCount) {
    if (iStep > iReportFalseHistCount) {
      aNumFalseCounts[aNumFalseCountsWindow[iStep % iReportFalseHistCount]]--;
    }
    aNumFalseCountsWindow[iStep % iReportFalseHistCount] = iNumFalse;
  }
}

void CreateDistanceCounts() {
  aDistanceCounts = (UBIGINT *) AllocateRAM((iNumVars+1)*sizeof(UBIGINT));
  if (iReportDistHistCount) {
    aDistanceCountsWindow = (UBIGINT *) AllocateRAM(iReportDistHistCount*sizeof(UBIGINT));
  }
}

void InitDistanceCounts() {
  if (iStep == 1) {
    memset(aDistanceCounts,0,(iNumVars+1)*sizeof(UBIGINT));
    if (iReportDistHistCount) {
      memset(aDistanceCountsWindow,0,iReportDistHistCount*sizeof(UBIGINT));
    }
  }
}

void UpdateDistanceCounts() {

  aDistanceCounts[iSolutionDistance]++;

  if (iReportDistHistCount) {
    if (iStep > iReportDistHistCount) {
      aDistanceCounts[aDistanceCountsWindow[iStep % iReportDistHistCount]]--;
    }
    aDistanceCountsWindow[iStep % iReportDistHistCount] = iSolutionDistance;
  }
}

void CreateClauseLast() {
  aClauseLast = (UBIGINT *) AllocateRAM(iNumClauses*sizeof(UBIGINT));
}

void InitClauseLast() {
  if (iStep == 1) {
    memset(aClauseLast,0,iNumClauses*sizeof(UBIGINT));
  }
}

void UpdateClauseLast() {
  UINT32 j;
  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      aClauseLast[j] = iStep;
    }
  }
}

UINT32 iNextSQGridCol;

void CreateSQGrid() {
  if (bWeighted) {
    aSQGridW = (FLOAT *) AllocateRAM(iNumLogDistValues * iNumRuns * sizeof(FLOAT));
  } else {
    aSQGrid = (UINT32 *) AllocateRAM(iNumLogDistValues * iNumRuns * sizeof(UINT32));
  }
}

void InitSQGrid() {
  iNextSQGridCol = 0;
}

void UpdateSQGrid() {

  if (iStep != aLogDistValues[iNextSQGridCol]) {
    return;
  }

  if (bWeighted) {
    aSQGridW[iNumLogDistValues * (iRun-1) + iNextSQGridCol++] = fBestSumFalseW;
  } else {
    aSQGrid[iNumLogDistValues * (iRun-1) + iNextSQGridCol++] = iBestNumFalse;
  }
}

void FinishSQGrid() {
  if (bWeighted) {
    while (iNextSQGridCol < iNumLogDistValues) {
      aSQGridW[iNumLogDistValues * (iRun-1) + iNextSQGridCol++] = fBestSumFalseW;
    }
  } else {
    while (iNextSQGridCol < iNumLogDistValues) {
      aSQGrid[iNumLogDistValues * (iRun-1) + iNextSQGridCol++] = iBestNumFalse;
    }
  }
}

void CreatePenaltyStats() {
  aPenaltyStatsMean = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsStddev = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsCV = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));

  aPenaltyStatsSum = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsSum2 = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));

  aPenaltyStatsMeanSum = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsMeanSum2 = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsStddevSum = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsStddevSum2 = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsCVSum = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));
  aPenaltyStatsCVSum2 = (FLOAT *) AllocateRAM(iNumClauses*sizeof(FLOAT));

  memset(aPenaltyStatsMeanSum,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsMeanSum2,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsStddevSum,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsStddevSum2,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsCVSum,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsCVSum2,0,iNumClauses*sizeof(FLOAT));

}

void InitPenaltyStats() {
  memset(aPenaltyStatsSum,0,iNumClauses*sizeof(FLOAT));
  memset(aPenaltyStatsSum2,0,iNumClauses*sizeof(FLOAT));
}

void UpdatePenaltyStatsStep() {
  UINT32 j;
  FLOAT fCurPen;

  for (j=0;j<iNumClauses;j++) {
    if (bClausePenaltyFLOAT) {
      if (bReportPenaltyReNormFraction) {
        fCurPen = aClausePenaltyFL[j] / fTotalPenaltyFL;
      } else {
        if (bReportPenaltyReNormBase) {
          fCurPen = aClausePenaltyFL[j] / fBasePenaltyFL;
        } else {
          fCurPen = aClausePenaltyFL[j];
        }
      }
    } else {
      if (bReportPenaltyReNormFraction) {
        fCurPen = ((FLOAT) aClausePenaltyINT[j]) / ((FLOAT) iTotalPenaltyINT);
      } else {
        if (bReportPenaltyReNormBase) {
          fCurPen = ((FLOAT) aClausePenaltyINT[j]) / ((FLOAT) iBasePenaltyINT);
        } else {
          fCurPen = (FLOAT) aClausePenaltyINT[j];
        }
      }
    }

    aPenaltyStatsSum[j] += fCurPen;
    aPenaltyStatsSum2[j] += (fCurPen * fCurPen);

    CalculateStats(&aPenaltyStatsMean[j],&aPenaltyStatsStddev[j],&aPenaltyStatsCV[j],aPenaltyStatsSum[j],aPenaltyStatsSum2[j],iStep);
  }
}

void UpdatePenaltyStatsRun() {
  UINT32 j;
  for (j=0;j<iNumClauses;j++) {
    aPenaltyStatsMeanSum[j] += aPenaltyStatsMean[j];
    aPenaltyStatsMeanSum2[j] += (aPenaltyStatsMean[j] * aPenaltyStatsMean[j]);
    aPenaltyStatsStddevSum[j] += aPenaltyStatsStddev[j];
    aPenaltyStatsStddevSum2[j] += (aPenaltyStatsStddev[j] * aPenaltyStatsStddev[j]);
    aPenaltyStatsCVSum[j] += aPenaltyStatsCV[j];
    aPenaltyStatsCVSum2[j] += (aPenaltyStatsCV[j] * aPenaltyStatsCV[j]);
  }
}

void CreateVarFlipHistory() {

  iVarFlipHistoryLen = 0;
  
  if (pRepMobility->bActive) {
    if (iReportMobilityDisplay == 0) {
      iReportMobilityDisplay = iNumVars;
    }

    iVarFlipHistoryLen = iReportMobilityDisplay + 1;
  }

  if ((pRepMobFixed->bActive)||(pRepMobFixedFreq->bActive)||(bMobilityColXActive)) {
    if (iMobFixedWindow == 0) {
      iMobFixedWindow = iNumVars;
    }

    if (iVarFlipHistoryLen < iMobFixedWindow + 1) {
      iVarFlipHistoryLen = iMobFixedWindow + 1;
    }
  }

  if (bMobilityColNActive) {
    if (iVarFlipHistoryLen < iNumVars + 1) {
      iVarFlipHistoryLen = iNumVars + 1;
    }
  }

  if (iVarFlipHistoryLen == 0) {
    ReportPrint1(pRepErr,"Warning! Unknown Mobility Window Size requested (setting to %"P32")\n",iNumVars);
    iVarFlipHistoryLen = iNumVars + 1;
  }

  aVarFlipHistory = (UINT32 *) AllocateRAM(iVarFlipHistoryLen * sizeof(UINT32));
}

void UpdateVarFlipHistory() {
  aVarFlipHistory[iStep % iVarFlipHistoryLen] = iFlipCandidate;
}


void CreateMobilityWindow() {
  if (iVarFlipHistoryLen == 0) {
    ReportPrint(pRepErr,"Unexpected Error: Variable Flip History Length = 0\n");
  }
  aMobilityWindowVarChange = (UINT32 *) AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aMobilityWindow = (UINT32 *) AllocateRAM((iVarFlipHistoryLen+1) * sizeof(UINT32));
  aMobilityWindowSum = (FLOAT *) AllocateRAM((iVarFlipHistoryLen+1) * sizeof(FLOAT));
  aMobilityWindowSum2 = (FLOAT *) AllocateRAM((iVarFlipHistoryLen+1) * sizeof(FLOAT));
}

void InitMobilityWindow() {
  memset(aMobilityWindow,0,(iVarFlipHistoryLen+1) * sizeof(UINT32));
  memset(aMobilityWindowSum,0,(iVarFlipHistoryLen+1) * sizeof(FLOAT));
  memset(aMobilityWindowSum2,0,(iVarFlipHistoryLen+1) * sizeof(FLOAT));
}

void UpdateMobilityWindow() {

  UINT32 iWindowLen;
  UBIGINT iStopWindow;
  SINT32 iCurPos;
  UINT32 iCurVar;

  if (iStep == 1) {
    return;
  }

  memset(aMobilityWindowVarChange,0,(iNumVars+1) * sizeof(UINT32));

  if (iStep < iVarFlipHistoryLen) {
    iStopWindow = iStep;
  } else {
    iStopWindow = iVarFlipHistoryLen;
  }

  aMobilityWindow[0] = 0;
  iCurPos = (SINT32) (iStep % iVarFlipHistoryLen);

  for (iWindowLen = 1; iWindowLen < iStopWindow; iWindowLen++) {

    aMobilityWindow[iWindowLen] = aMobilityWindow[iWindowLen-1];

    iCurVar = aVarFlipHistory[iCurPos];
    
    if (iCurVar) {

      aMobilityWindowVarChange[iCurVar] = 1 - aMobilityWindowVarChange[iCurVar];

      if (aMobilityWindowVarChange[iCurVar]) {
        aMobilityWindow[iWindowLen]++;
      } else {
        aMobilityWindow[iWindowLen]--;
      }
    }

    aMobilityWindowSum[iWindowLen] += (FLOAT) aMobilityWindow[iWindowLen];
    aMobilityWindowSum2[iWindowLen] += (FLOAT) (aMobilityWindow[iWindowLen] * aMobilityWindow[iWindowLen]);

    iCurPos--;
    if (iCurPos == -1) {
      iCurPos = (SINT32) (iVarFlipHistoryLen - 1);
    }
  }
}


void CreateMobilityFixedFrequencies() {
  if (iMobFixedWindow == 0) {
    ReportPrint(pRepErr,"Unexpected Error: Mobility Fixed Size = 0\n");
  }

  aMobilityFixedFrequencies = (UINT32 *)AllocateRAM((iMobFixedWindow+1) * sizeof(UINT32));
}

void InitMobilityFixedFrequencies() {
  memset(aMobilityFixedFrequencies,0,(iMobFixedWindow+1) * sizeof(UINT32));
}

void UpdateMobilityFixedFrequencies() {
  if (iStep <= iMobFixedWindow) {
    if (bMobilityFixedIncludeStart) {
      aMobilityFixedFrequencies[aMobilityWindow[iStep-1]]++;
    }
  } else {
    aMobilityFixedFrequencies[aMobilityWindow[iMobFixedWindow]]++;
  }
}


void CreateVarAgeFrequencies() {
  if (iMaxVarAgeFrequency == 0) {
    iMaxVarAgeFrequency = iNumVars*10;
  }
  aVarAgeFrequency = (UBIGINT *)AllocateRAM((iMaxVarAgeFrequency + 1) * sizeof(UBIGINT));
}

void InitVarAgeFrequencies() {
  memset(aVarAgeFrequency,0,(iMaxVarAgeFrequency+1) * sizeof(UBIGINT));
}

void UpdateVarAgeFrequencies() {
  UBIGINT iAge;
  if (iFlipCandidate > 0) {
    iAge = iStep - aVarLastChange[iFlipCandidate];
    if (iAge > iMaxVarAgeFrequency) {
      iAge = iMaxVarAgeFrequency;
    }
    aVarAgeFrequency[iAge]++;
  }
}


void CreateAutoCorr() {

  if (iAutoCorrMaxLen == 0) {
    iAutoCorrMaxLen = iNumVars;
  }
  aAutoCorrValues = (FLOAT *) AllocateRAM((iAutoCorrMaxLen) * sizeof(FLOAT));
  aAutoCorrStartBuffer = (FLOAT *) AllocateRAM((iAutoCorrMaxLen) * sizeof(FLOAT));
  aAutoCorrEndCircBuffer = (FLOAT *) AllocateRAM((iAutoCorrMaxLen) * sizeof(FLOAT));
  aAutoCorrCrossSum = (FLOAT *) AllocateRAM((iAutoCorrMaxLen) * sizeof(FLOAT));
}

void InitAutoCorr() {
  fAutoCorrSum = FLOATZERO;
  fAutoCorrSum2 = FLOATZERO;
  memset(aAutoCorrCrossSum,0,iAutoCorrMaxLen*sizeof(FLOAT));
}

void UpdateAutoCorr() {

  FLOAT fCurValue;
  UINT32 iLoopIndex;
  UINT32 k;
  
  if (bWeighted) {
    fCurValue = fSumFalseW;
  } else {
    fCurValue = (FLOAT) iNumFalse;
  }

  if (iStep <= (UBIGINT) iAutoCorrMaxLen) {
    aAutoCorrStartBuffer[iStep - 1] = fCurValue;
    iLoopIndex = (UINT32) (iStep-1);
  } else {
    iLoopIndex = iAutoCorrMaxLen;
  }
    
  fAutoCorrSum += fCurValue;
  fAutoCorrSum2 += (fCurValue * fCurValue);

  for (k=0; k<iLoopIndex; k++) {
    aAutoCorrCrossSum[k] += (fCurValue * aAutoCorrEndCircBuffer[(iStep - k - 1) % iAutoCorrMaxLen]);
  }

  aAutoCorrEndCircBuffer[(iStep % iAutoCorrMaxLen)] = fCurValue;

}

void CalcAutoCorr() {

  UINT32 k;
  UINT32 iLoopIndex;

  FLOAT fValue;
  FLOAT fSumStart = fAutoCorrSum;
  FLOAT fSumStart2 = fAutoCorrSum2;
  FLOAT fSumEnd = fAutoCorrSum;
  FLOAT fSumEnd2 = fAutoCorrSum2;

  if (iStep <= (UBIGINT) iAutoCorrMaxLen) {
    iLoopIndex = (UINT32) iStep-1;
    for (k = (UINT32) iStep; k<iAutoCorrMaxLen; k++) {
      aAutoCorrValues[k] = FLOATZERO;
    }
  } else {
    iLoopIndex = iAutoCorrMaxLen;
  }
  
  for (k=0; k<iLoopIndex; k++) {
    fValue = aAutoCorrStartBuffer[k];
    fSumStart -= fValue;
    fSumStart2 -= (fValue * fValue);
    fValue = aAutoCorrEndCircBuffer[(iStep - k) % iAutoCorrMaxLen];
    fSumEnd -= fValue;
    fSumEnd2 -= (fValue * fValue);
    
    aAutoCorrValues[k] = CorrelationCoeff(fSumStart,fSumStart2,fSumEnd,fSumEnd2,aAutoCorrCrossSum[k],iStep - k - 1);

  }

  for (k=0; k<iAutoCorrMaxLen; k++) {
    if (aAutoCorrValues[k] < fAutoCorrCutoff) {
      iLoopIndex = k + 1;
      break;
    }
  }

  iAutoCorrLen = iLoopIndex;

}

void InitAutoCorrOne() {
  fAutoCorrOneLast = FLOATZERO;
  fAutoCorrOneSum = FLOATZERO;
  fAutoCorrOneSum2 = FLOATZERO;
  fAutoCorrOneCrossSum = FLOATZERO;
}

void UpdateAutoCorrOne() {

  FLOAT fCurValue;
  
  if (bWeighted) {
    fCurValue = fSumFalseW;
  } else {
    fCurValue = (FLOAT) iNumFalse;
  }
  
  fAutoCorrOneSum += fCurValue;
  fAutoCorrOneSum2 += (fCurValue * fCurValue);

  if (iStep == 1) {
    fAutoCorrOneStart = fCurValue;
  } else {
    fAutoCorrOneCrossSum += (fCurValue * fAutoCorrOneLast);
  }
    
  fAutoCorrOneLast = fCurValue;

}

void CalcAutoCorrOne() {

  FLOAT fSumStart;
  FLOAT fSumStart2;
  FLOAT fSumEnd;
  FLOAT fSumEnd2;

  if (iStep == 1) {
    fAutoCorrOneVal = FLOATZERO;
    fAutoCorrOneEst = FLOATZERO;
  } else {
    fSumStart = fAutoCorrOneSum - fAutoCorrOneLast;
    fSumStart2 = fAutoCorrOneSum2 - (fAutoCorrOneLast * fAutoCorrOneLast);
    fSumEnd = fAutoCorrOneSum - fAutoCorrOneStart;
    fSumEnd2 = fAutoCorrOneSum2 - (fAutoCorrOneStart * fAutoCorrOneStart);

    fAutoCorrOneVal = CorrelationCoeff(fSumStart,fSumStart2,fSumEnd,fSumEnd2,fAutoCorrOneCrossSum,iStep - 1);

    if (fAutoCorrOneVal == FLOATZERO) {
      fAutoCorrOneEst = FLOATZERO;
    } else {
      fAutoCorrOneEst = log(fabs(fAutoCorrOneVal));
      if (fAutoCorrOneEst == 1.0f) {
        fAutoCorrOneEst = FLOATZERO;
      } else {
        fAutoCorrOneEst = -1/fAutoCorrOneEst;
      }
    }
  }

}

void BranchFactor() {
  UINT32 j;
  fBranchFactor = FLOATZERO;
  for (j=1; j<=iNumVars; j++) {
    if (aVarScore[j] == 0) {
      fBranchFactor += 1.0f;
    }
  }
  fBranchFactor /= (FLOAT) iNumVars;
}

void BranchFactorW() {
  UINT32 j;
  fBranchFactorW = FLOATZERO;
  for (j=1; j<=iNumVars; j++) {
    if (aVarScoreW[j] == FLOATZERO) {
      fBranchFactorW += 1.0f;
    }
  }
  fBranchFactorW /= (FLOAT) iNumVars;
}


UINT32 iSUDSLastNumFalse;
FLOAT fSUDSfLastSumFalseW;

void InitStepsUpDownSide() {
  iNumUpSteps = 0;
  iNumDownSteps = 0;
  iNumSideSteps = 0;
  iNumUpStepsW = 0;
  iNumDownStepsW = 0;
  iNumSideStepsW = 0;

  iSUDSLastNumFalse = iNumClauses;
  fSUDSfLastSumFalseW = fTotalWeight;
}

void UpdateStepsUpDownSide() {
  if (iNumFalse < iSUDSLastNumFalse) {
    iNumDownSteps++;
  } else {
    if (iNumFalse > iSUDSLastNumFalse) {
      iNumUpSteps++;
    } else {
      iNumSideSteps++;      
    }
  }
  iSUDSLastNumFalse = iNumFalse;

  if (bWeighted) {
    if (fSumFalseW < fSUDSfLastSumFalseW) {
      iNumDownStepsW++;
    } else {
      if (fSumFalseW > fSUDSfLastSumFalseW) {
        iNumUpStepsW++;
      } else {
        iNumSideStepsW++;
      }
    }
    fSUDSfLastSumFalseW = fSumFalseW;
  }
}



void NumRestarts() {
  if (iStep == 1) {
    iNumRestarts = 0;
  } else {
    iNumRestarts++;
  }
}  


 
void LoadKnownSolutions() {

  VARSTATE vsKnownNew;
  FILE *filKnown;
  char *sKnownLine;

  UINT32 iLineLen = iNumVars + 5;

  if (strcmp(sFilenameSoln,"")) {
    SetupFile(&filKnown,"r",sFilenameSoln,stdin,0);

    sKnownLine = (char *) AllocateRAM((iLineLen + 2) * sizeof(char));
    vsKnownNew = NewVarState();

    while (!feof(filKnown)) {
      if (fgets(sKnownLine,(int) iLineLen,filKnown)) {
        if ((*sKnownLine)&&(*sKnownLine != '#')) {
          if (SetCurVarStateString(vsKnownNew, sKnownLine)) {
            AddToVarStateList(&vslKnownSoln,vsKnownNew);
            bKnownSolutions = 1;
          }
        }
      }
    }
    CloseSingleFile(filKnown);
  } else {
    ReportPrint(pRepErr,"Warning! Expecting [-filesol] to specify file with known solutions\n");
  }
}

void CreateSolutionDistance() {
  vsSolutionDistanceCurrent = NewVarState();
}

void UpdateSolutionDistance() {
  SetCurVarState(vsSolutionDistanceCurrent);
  vsSolutionDistanceClosest = FindClosestVarState(&vslKnownSoln, vsSolutionDistanceCurrent);
  iSolutionDistance = HammingDistVarState(vsSolutionDistanceClosest,vsSolutionDistanceCurrent);
}

void InitFDCRun() {
  fFDCRunHeightDistanceSum = FLOATZERO;
  fFDCRunHeightSum = FLOATZERO;
  fFDCRunHeightSum2 = FLOATZERO;
  fFDCRunDistanceSum = FLOATZERO;
  fFDCRunDistanceSum2 = FLOATZERO;
  iFDCRunCount = 0;
  fFDCRun = FLOATZERO;
}

void UpdateFDCRun() {

  FLOAT fDist;
  FLOAT fHeight;

  if (IsLocalMinimum(bWeighted)) {
    
    fDist = (FLOAT) (iSolutionDistance);

    if (bWeighted) {
      fHeight = fSumFalseW;
    } else {
      fHeight = (FLOAT) iNumFalse;
    }

    fFDCRunHeightDistanceSum += fDist * fHeight;
    fFDCRunHeightSum += fHeight;
    fFDCRunHeightSum2 += fHeight * fHeight;
    fFDCRunDistanceSum += fDist;
    fFDCRunDistanceSum2 += fDist * fDist;
    iFDCRunCount++;

  }
}

void CalcFDCRun() {
  fFDCRun = CorrelationCoeff(fFDCRunHeightSum, fFDCRunHeightSum2, fFDCRunDistanceSum, fFDCRunDistanceSum2, fFDCRunHeightDistanceSum, iFDCRunCount);
}

void DynamicParms() {
  ActivateDynamicParms();
}


void FlushBuffers() {
  if (bReportFlush) {
    fflush(NULL);
  }
}

void CheckWeighted() {
  if (!bWeighted) {
    ReportPrint(pRepErr,"Unexpected Error: some weighted features unavailable for current (unweighted) algorithm\n");
    AbnormalExit();
    exit(1);
  }
}


void CreateUniqueSolutions() {
  vsCheckUnique = NewVarState();
}

void UpdateUniqueSolutions() {
  if (bSolutionFound) {
    SetCurVarState(vsCheckUnique);
    if (AddUniqueToVarStateList(&vslUnique,vsCheckUnique)) {
      iNumUniqueSolutions++;
      if (iNumUniqueSolutions == iFindUnique) {
        bTerminateAllRuns = 1;
      }
      iLastUnique = iRun;
    }
  }
}


void CreateVarsShareClauses() {

  UINT32 j,k,l,m;
  UINT32 iVar;
  UINT32 iVar2;
  BOOL bAlreadyShareClause;
  LITTYPE *pLit;
  LITTYPE *pLit2;

  aNumVarsShareClause = (UINT32 *) AllocateRAM((iNumVars+1)*sizeof(UINT32));
  pVarsShareClause = (UINT32 **) AllocateRAM((iNumVars+1)*sizeof(UINT32 *));

  memset(aNumVarsShareClause,0,(iNumVars+1)*sizeof(UINT32));

  for (j=0;j<iNumClauses;j++) {
    pLit = pClauseLits[j];
    for (k=0;k<aClauseLen[j];k++) {
      iVar = GetVarFromLit(*pLit);
      aNumVarsShareClause[iVar] += (aClauseLen[j] - 1);
      pLit++;
    }
  } 
  
  for (j=0;j<(iNumVars+1);j++) {
    pVarsShareClause[j] = (UINT32 *) AllocateRAM(aNumVarsShareClause[j]*sizeof(UINT32));;
  }

  memset(aNumVarsShareClause,0,(iNumVars+1)*sizeof(UINT32));

  for (j=0;j<iNumClauses;j++) {
    pLit = pClauseLits[j];
    for (k=0;k<aClauseLen[j];k++) {
      iVar = GetVarFromLit(*pLit);
      pLit2 = pClauseLits[j];
      for (l=0;l<aClauseLen[j];l++) {
        iVar2 = GetVarFromLit(*pLit2);
        if ((l != k)&&(iVar != iVar2)) {
          bAlreadyShareClause = 0;
          for (m=0;m<aNumVarsShareClause[iVar];m++) {
            if (pVarsShareClause[iVar][m] == iVar2) {
              bAlreadyShareClause = 1;
              break;
            }
          }
          if (!bAlreadyShareClause) {
            pVarsShareClause[iVar][aNumVarsShareClause[iVar]] = iVar2;
            aNumVarsShareClause[iVar]++;
          }
        }
        pLit2++;
      }
      pLit++;
    }
  } 
}

#ifdef __cplusplus

}
#endif
