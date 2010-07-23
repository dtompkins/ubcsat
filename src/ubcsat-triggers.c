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
    This file contains the code to make the various data triggers work
*/

/***** Trigger ReadCNF *****/

void ReadCNF();

UINT32 iNumVars = 0;
UINT32 iNumClauses = 0;
UINT32 iNumLits = 0;

UINT32 *aClauseLen;
LITTYPE **pClauseLits;

FLOAT *aClauseWeight;
FLOAT fTotalWeight;

UINT32 iVARSTATELen = 0;


/***** Trigger LitOccurence *****/

void CreateLitOccurence();

UINT32 *aNumLitOcc;
UINT32 *aLitOccData;
UINT32 **pLitClause;


/***** Trigger CandidateList *****/

void CreateCandidateList();

UINT32 *aCandidateList;
UINT32 iNumCandidates;


/***** Trigger InitVarsFromFile *****/
/***** Trigger DefaultInitVars *****/
  
void InitVarsFromFile();
void DefaultInitVars();

UINT32 *aVarInit;
UINT32 iInitVarFlip = 0;
BOOL bVarInitGreedy = FALSE;


/***** Trigger DefaultStateInfo *****/

void CreateDefaultStateInfo();
void InitDefaultStateInfo();

UINT32 *aNumTrueLit;
UINT32 *aVarValue;
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

UINT32 iNumVarsInFalseList = 0;
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

UINT32 *aVarLastChange;
UINT32 iVarLastChangeReset;


/***** Trigger TrackChanges *****/
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

UINT32 iNumChanges;
UINT32 *aChangeList;
SINT32 *aChangeOldScore;
UINT32 *aChangeLastStep;


/***** Trigger BestScoreList *****/

void CreateBestScoreList();
void InitBestScoreList();
void UpdateBestScoreList();

UINT32 iNumBestScoreList;
UINT32 *aBestScoreList;
UINT32 *aBestScoreListPos;


/***** Trigger ClausePenalty *****/

void CreateClausePenalty();
void InitClausePenalty();

FLOAT *aClausePenalty;
BOOL bClausePenaltyCreated = FALSE;
FLOAT fBasePenalty;
FLOAT fTotalPenalty;



/***** Trigger MakeBreakPenalty *****/

void CreateMakeBreakPenalty();
void InitMakeBreakPenalty();
void UpdateMakeBreakPenalty();
void FlipMBPandFCLandVIF();

FLOAT *aMakePenalty;
FLOAT *aBreakPenalty;


/***** Trigger NullFlips *****/

UINT32 iNumNullFlips;

void InitNullFlips();
void UpdateNullFlips();


/***** Trigger VarStateBackup *****/

void CreateVarStateBackup();
void BackupVarState();
void RestoreVarState();

UINT32 *aVarStateBackup;


/***** Trigger LogDist *****/

void CreateLogDist();

UINT32 *aLogDistValues;
UINT32 iNumLogDistValues;
UINT32 iLogDistStepsPerDecade = 10;


/***** Trigger BestFalse *****/

void InitBestFalse();
void UpdateBestFalse();

UINT32 iBestNumFalse;
UINT32 iBestStepNumFalse;
FLOAT fBestSumFalseW;
UINT32 iBestStepSumFalseW;

/***** Trigger SaveBest *****/

void CreateSaveBest();
void UpdateSaveBest();

UINT32 *aVarStateBest;

/***** Trigger NoImprove *****/

void CheckNoImprove();

UINT32 iNoImprove;


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

UINT32 *aFlipCounts;


/***** Trigger UnsatCounts *****/

void CreateUnsatCounts();
void InitUnsatCounts();
void UpdateUnsatCounts();

UINT32 *aUnsatCounts;


/***** Trigger ClauseLast *****/

void CreateClauseLast();
void InitClauseLast();
void UpdateClauseLast();

UINT32 *aClauseLast;


/***** Trigger ClauseLast *****/

void CreateSQGrid();
void InitSQGrid();
void UpdateSQGrid();
void FinishSQGrid();

FLOAT *aSQGridW;
UINT32 *aSQGrid;



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
  CreateTrigger("Flip+TrackChanges",FlipCandidate,FlipTrackChanges,"TrackChanges","DefaultFlip,UpdateTrackChanges");

  CreateTrigger("CreateBestScoreList",CreateStateInfo,CreateBestScoreList,"","");
  CreateTrigger("InitBestScoreList",InitStateInfo,InitBestScoreList,"InitVarScore","");
  CreateTrigger("UpdateBestScoreList",UpdateStateInfo,UpdateBestScoreList,"","");
  CreateContainerTrigger("BestScoreList","TrackChanges,CreateBestScoreList,InitBestScoreList,UpdateBestScoreList");

  CreateTrigger("CreateClausePenalty",CreateStateInfo,CreateClausePenalty,"","");
  CreateTrigger("InitClausePenalty",InitStateInfo,InitClausePenalty,"","");
  CreateContainerTrigger("ClausePenalty","CreateClausePenalty,InitClausePenalty");

  CreateTrigger("CreateMakeBreakPenalty",CreateStateInfo,CreateMakeBreakPenalty,"CreateClausePenalty","");
  CreateTrigger("InitMakeBreakPenalty",InitStateInfo,InitMakeBreakPenalty,"InitClausePenalty","");
  CreateTrigger("UpdateMakeBreakPenalty",UpdateStateInfo,UpdateMakeBreakPenalty,"","");
  CreateContainerTrigger("MakeBreakPenalty","CreateMakeBreakPenalty,InitMakeBreakPenalty,UpdateMakeBreakPenalty");
  CreateTrigger("Flip+MBP+FCL+VIF",FlipCandidate,FlipMBPandFCLandVIF,"MakeBreakPenalty,FalseClauseList,VarInFalse","DefaultFlip,UpdateMakeBreakPenalty,UpdateFalseClauseList,UpdateVarInFalse");

  CreateTrigger("InitNullFlips",PreRun,InitNullFlips,"","");
  CreateTrigger("UpdateNullFlips",UpdateStateInfo,UpdateNullFlips,"","");
  CreateContainerTrigger("NullFlips","InitNullFlips,UpdateNullFlips");

  CreateTrigger("CreateVarStateBackup",CreateStateInfo,CreateVarStateBackup,"","");

  CreateTrigger("LogDist",CreateData,CreateLogDist,"","");

  CreateTrigger("InitBestFalse",PreRun,InitBestFalse,"","");
  CreateTrigger("UpdateBestFalse",PostFlip,UpdateBestFalse,"","");
  CreateContainerTrigger("BestFalse","InitBestFalse,UpdateBestFalse");

  CreateTrigger("CreateSaveBest",CreateStateInfo,CreateSaveBest,"","");
  CreateTrigger("UpdateSaveBest",PostFlip,UpdateSaveBest,"BestFalse","");
  CreateContainerTrigger("SaveBest","CreateSaveBest,UpdateSaveBest");

  CreateTrigger("NoImprove",CheckTerminate,CheckNoImprove,"BestFalse","");

  CreateTrigger("StartSeed",PreRun,StartSeed,"","");

  CreateTrigger("SetupCountRandom",PreStart,SetupCountRandom,"","");
  CreateTrigger("InitCountRandom",PreRun,InitCountRandom,"","");
  CreateContainerTrigger("CountRandom","SetupCountRandom,InitCountRandom");
  
  CreateTrigger("CheckTimeout",CheckTerminate,CheckTimeout,"","");

  CreateTrigger("CheckForRestarts",CheckRestart,CheckForRestarts,"","");

  CreateTrigger("CreateFlipCounts",CreateStateInfo,CreateFlipCounts,"","");
  CreateTrigger("InitFlipCounts",InitStateInfo,InitFlipCounts,"","");
  CreateTrigger("UpdateFlipCounts",UpdateStateInfo,UpdateFlipCounts,"","");
  CreateContainerTrigger("FlipCounts","InitFlipCounts,CreateFlipCounts,UpdateFlipCounts");

  CreateTrigger("CreateUnsatCounts",CreateStateInfo,CreateUnsatCounts,"","");
  CreateTrigger("InitUnsatCounts",InitStateInfo,InitUnsatCounts,"","");
  CreateTrigger("UpdateUnsatCounts",PostFlip,UpdateUnsatCounts,"","");
  CreateContainerTrigger("UnsatCounts","InitUnsatCounts,CreateUnsatCounts,UpdateUnsatCounts");

  CreateTrigger("CreateClauseLast",CreateStateInfo,CreateClauseLast,"","");
  CreateTrigger("InitClauseLast",InitStateInfo,InitClauseLast,"","");
  CreateTrigger("UpdateClauseLast",PostFlip,UpdateClauseLast,"","");
  CreateContainerTrigger("ClauseLast","InitClauseLast,CreateClauseLast,UpdateClauseLast");

  CreateTrigger("CreateSQGrid",CreateStateInfo,CreateSQGrid,"LogDist","");
  CreateTrigger("InitSQGrid",PreRun,InitSQGrid,"","");
  CreateTrigger("UpdateSQGrid",PostStep,UpdateSQGrid,"BestFalse","");
  CreateTrigger("FinishSQGrid",PostRun,FinishSQGrid,"","");
  CreateContainerTrigger("SQGrid","CreateSQGrid,InitSQGrid,UpdateSQGrid,FinishSQGrid");

}


char sLine[MAXCNFLINELEN];

void ReadCNF() {
	UINT32 j;
  UINT32 k;
  UINT32 bIsWCNF;
  float fDummy;
  SINT32 l;
  

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
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXCNFLINELEN [%u]\n",MAXCNFLINELEN);
      AbnormalExit();
    }

    if (strncmp(sLine,"p wcnf",6)==0)
      bIsWCNF = 1;

		if (sLine[0] =='p') {
      if (bWeighted) {
        if (bIsWCNF) {
			    sscanf(sLine,"p wcnf %u %u",&iNumVars,&iNumClauses);
        } else {
          ReportPrint(pRepErr,"Warning! reading .cnf file and setting all weights = 1\n");
          sscanf(sLine,"p cnf %u %u",&iNumVars,&iNumClauses);
        }
      } else {
        if (bIsWCNF) {
          ReportPrint(pRepErr,"Warning! reading .wcnf file and ignoring all weights\n");
          sscanf(sLine,"p wcnf %u %u",&iNumVars,&iNumClauses);
        } else {
          sscanf(sLine,"p cnf %u %u",&iNumVars,&iNumClauses);
        }
      }
		}
	}
  
  aClauseLen = AllocateRAM(iNumClauses * sizeof(UINT32));
  pClauseLits = AllocateRAM(iNumClauses * sizeof(LITTYPE *));
  if (bWeighted)
      aClauseWeight = AllocateRAM(iNumClauses * sizeof(FLOAT));
  
  pLastLit = pNextLit = pData = 0;

  iNumLits = 0;

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
			fscanf(filInput,"%d",&l);
      if (l) {
        
        if (pNextLit == pLastLit) {
          pData = AllocateRAM(LITSPERCHUNK * sizeof(LITTYPE));
          pNextLit = pData;
          pLastLit = pData + LITSPERCHUNK;
          for (k=0;k<aClauseLen[j];k++) {
            *pNextLit = pClauseLits[j][k];
            pNextLit++;
          }
          pClauseLits[j] = pData;
        }

        *pNextLit = SetLitFromFile(l);
        pNextLit++;
        aClauseLen[j]++;
        iNumLits++;
      }
		} while (l != 0);
	}

  AdjustLastRAM((pNextLit - pData) * sizeof(LITTYPE));

  iVARSTATELen = (iNumVars / 32) + 1;
  if ((iNumVars % 32)==0) iVARSTATELen--;

  CloseSingleFile(filInput);  

}

void CreateLitOccurence() {

  UINT32 j,k;
  LITTYPE *pLit;
  LITTYPE *pCur;

  aNumLitOcc = AllocateRAM((iNumVars+1)*2*sizeof(UINT32));
  pLitClause = AllocateRAM((iNumVars+1)*2*sizeof(UINT32 *));
  aLitOccData = AllocateRAM(iNumLits*sizeof(UINT32));

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
  aCandidateList = AllocateRAM(iNumClauses*sizeof(UINT32));
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
          ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXPARMLINELEN [%u]\n",MAXPARMLINELEN);
          AbnormalExit();
        }
        if ((*sInitLine)&&(*sInitLine != '#')) {
          pStart = sInitLine;
          pPos = strchr(pStart,' ');
          while (pPos) {
            if (pPos==pStart) {
              pStart++;
            } else {
              *pPos++=0;

              sscanf(pStart,"%d",&iLit);

              if (iLit) {
                if (iLit > 0) {
                  iValue = 1;
                  iVar = iLit;
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
          
          if (pPos) 
            *pPos = 0;
          pPos = strchr(pStart,13);
          if (pPos) 
            *pPos = 0;

          if (strlen(pStart)) {
            sscanf(pStart,"%d",&iLit);

            if (iLit) {
              if (iLit > 0) {
                iValue = 1;
                iVar = iLit;
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
  aNumTrueLit = AllocateRAM(iNumClauses*sizeof(UINT32));
  aVarValue = AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aVarInit = AllocateRAM((iNumVars+1)*sizeof(UINT32));
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
    fSumFalseW = 0.0f;
    for (j=0;j<iNumClauses;j++) {
      if (aNumTrueLit[j]==0) {
        fSumFalseW += aClauseWeight[j];;
      }
    }
  }
}

void DefaultInitVars() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar = 0;
  BOOL bAdded;
  UINT32 iNumPos;
  UINT32 iNumNeg;

  for (j=1;j<=iNumVars;j++) {
    if (aVarInit[j] == 2) {
      aVarValue[j] = RandomInt(2);
    } else {
      aVarValue[j] = aVarInit[j];
    }
  }

  if (iInitVarFlip) {
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
      aVarValue[iVar] = 1 - aVarValue[iVar];
    }
  }

  if (bVarInitGreedy) {
    for (j=1;j<=iNumVars;j++) {
      iNumPos = aNumLitOcc[GetPosLit(j)];
      iNumNeg = aNumLitOcc[GetNegLit(j)];

      if (iNumPos > iNumNeg) {
        aVarInit[j] = 1;
      } else if (iNumNeg > iNumPos) {
        aVarInit[j] = 0;
      } 
      /* otherwise, whatever other value was already determined */
    }
  }
}

void DefaultFlip() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    if (--aNumTrueLit[*pClause++]==0) {
      iNumFalse++;
    }
  }

  pClause = pLitClause[litWasFalse];
  for (j=0;j<aNumLitOcc[litWasFalse];j++) {
    if (++aNumTrueLit[*pClause++]==1) {
      iNumFalse--;
    }
  }
}

void DefaultFlipW() {

  UINT32 j;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  UINT32 *pClause;

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    bSolutionFound = TRUE;
  }
  if (bWeighted) {
    if (fSumFalseW <= fTargetW) {
      bSolutionFound = TRUE;
    }
  }
}

void CreateFalseClauseList() {
  aFalseList = AllocateRAM(iNumClauses*sizeof(UINT32));
  aFalseListPos = AllocateRAM(iNumClauses*sizeof(UINT32));
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

  if (iFlipCandidate == 0)
    return;

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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
  aVarScore = AllocateRAM((iNumVars+1)*sizeof(SINT32));
  aCritSat = AllocateRAM(iNumClauses*sizeof(UINT32));
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

  if (iFlipCandidate == 0)
    return;

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
  }
}


void CreateVarScoreW() {
  aVarScoreW = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aCritSat = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitVarScoreW() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;
  
  for (j=1;j<=iNumVars;j++) {
    aVarScoreW[j] = 0.0f;
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

  if (iFlipCandidate == 0)
    return;

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
  }
}

void CreateMakeBreak() {
  aBreakCount = AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aMakeCount = AllocateRAM((iNumVars+1)*sizeof(UINT32));
  aCritSat = AllocateRAM(iNumClauses*sizeof(UINT32));
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

  if (iFlipCandidate == 0)
    return;

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
  }
}

void CreateMakeBreakW() {
  aBreakCountW = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aMakeCountW = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aCritSat = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitMakeBreakW() {
  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;

  for (j=1;j<=iNumVars;j++) {
    aMakeCountW[j] = 0.0f;
    aBreakCountW[j] = 0.0f;
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

  if (iFlipCandidate == 0)
    return;

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
  }
}

void CreateVarInFalse() {
  aVarInFalseList = AllocateRAM((iNumVars+1)* sizeof(UINT32));
  aVarInFalseListPos = AllocateRAM((iNumVars+1)* sizeof(UINT32));
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

  if (iFlipCandidate == 0)
    return;

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
  }
}

void CreateVarLastChange() {
  aVarLastChange = AllocateRAM((iNumVars+1)*sizeof(int));
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


#define UpdateChange(var) {if(aChangeLastStep[var]!=iStep) {aChangeOldScore[var] = aVarScore[var]; aChangeLastStep[var]=iStep; aChangeList[iNumChanges++]=var;}}

void CreateTrackChanges() {
  aChangeList = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aChangeLastStep = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aChangeOldScore = AllocateRAM((iNumVars+1) * sizeof(UINT32));
}

void InitTrackChanges() {
  memset(aChangeLastStep,0,(iNumVars+1) * sizeof(UINT32));
}

void UpdateTrackChanges() {
  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  if (iFlipCandidate == 0)
    return;

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
    *pClause++;
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

  if (iFlipCandidate == 0)
    return;

  iNumChanges = 0;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

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
    *pClause++;
  }
}

void CreateBestScoreList() {
  aBestScoreList = AllocateRAM((iNumVars+1) * sizeof(UINT32));
  aBestScoreListPos = AllocateRAM((iNumVars+1) * sizeof(UINT32));
}


void InitBestScoreList() {
  UINT32 j;

  iBestScore = iNumClauses;
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

  iBestNewScore = iNumClauses;

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

void CreateClausePenalty() {
  aClausePenalty = AllocateRAM(iNumClauses * sizeof(FLOAT));
  bClausePenaltyCreated = 1;
}

void InitClausePenalty() {
  UINT32 j;
  
  for (j=0;j<iNumClauses;j++) {
    aClausePenalty[j] = 1.0f;
  }

  fBasePenalty = 1.0f;
  fTotalPenalty = (FLOAT) iNumClauses;
 
}

void CreateMakeBreakPenalty() {
  aBreakPenalty = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aMakePenalty = AllocateRAM((iNumVars+1)*sizeof(FLOAT));
  aCritSat = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitMakeBreakPenalty() {

  UINT32 j;
  UINT32 k;
  UINT32 iVar;
  LITTYPE *pLit;

  for (j=1;j<=iNumVars;j++) {
    aMakePenalty[j] = 0.0f;
    aBreakPenalty[j] = 0.0f;
  }

  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0) {
      for (k=0;k<aClauseLen[j];k++) {
        aMakePenalty[GetVar(j,k)] += aClausePenalty[j];
      }
    } else if (aNumTrueLit[j]==1) {
      pLit = pClauseLits[j];
      for (k=0;k<aClauseLen[j];k++) {
        if IsLitTrue(*pLit) {
          iVar = GetVarFromLit(*pLit);
          aBreakPenalty[iVar] += aClausePenalty[j];
          aCritSat[j] = iVar;
          break;
        }
        pLit++;
      }
    }
  }
}

void UpdateMakeBreakPenalty() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  FLOAT fPenalty;

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetFalseLit(iFlipCandidate);
  litWasFalse = GetTrueLit(iFlipCandidate);

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    fPenalty = aClausePenalty[*pClause];
    if (aNumTrueLit[*pClause]==0) { 
      
      aBreakPenalty[iFlipCandidate] -= fPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakePenalty[iVar] += fPenalty;
        pLit++;
      }
    }
    if (aNumTrueLit[*pClause]==1) {
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        if (IsLitTrue(*pLit)) {
          iVar = GetVarFromLit(*pLit);
          aBreakPenalty[iVar] += fPenalty;
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
    fPenalty = aClausePenalty[*pClause];
    if (aNumTrueLit[*pClause]==1) {

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakePenalty[iVar] -= fPenalty;
        pLit++;

      }
      aBreakPenalty[iFlipCandidate] += fPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakPenalty[aCritSat[*pClause]] -= fPenalty;
    }
    *pClause++;
  }
}


void FlipMBPandFCLandVIF() {

  UINT32 j;
  UINT32 k;
  UINT32 *pClause;
  UINT32 iVar;
  LITTYPE litWasTrue;
  LITTYPE litWasFalse;
  LITTYPE *pLit;

  FLOAT fPenalty;

  if (iFlipCandidate == 0)
    return;

  litWasTrue = GetTrueLit(iFlipCandidate);
  litWasFalse = GetFalseLit(iFlipCandidate);

  aVarValue[iFlipCandidate] = 1 - aVarValue[iFlipCandidate];

  pClause = pLitClause[litWasTrue];
  for (j=0;j<aNumLitOcc[litWasTrue];j++) {
    fPenalty = aClausePenalty[*pClause];
    aNumTrueLit[*pClause]--;
    if (aNumTrueLit[*pClause]==0) { 
      
      aFalseList[iNumFalse] = *pClause;
      aFalseListPos[*pClause] = iNumFalse++;

      aBreakCount[iFlipCandidate]--;
      
      aBreakPenalty[iFlipCandidate] -= fPenalty;
      
      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]++;
        aMakePenalty[iVar] += fPenalty;

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
          aBreakPenalty[iVar] += fPenalty;
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
    fPenalty = aClausePenalty[*pClause];
    aNumTrueLit[*pClause]++;
    if (aNumTrueLit[*pClause]==1) {

      aFalseList[aFalseListPos[*pClause]] = aFalseList[--iNumFalse];
      aFalseListPos[aFalseList[iNumFalse]] = aFalseListPos[*pClause];

      pLit = pClauseLits[*pClause];
      for (k=0;k<aClauseLen[*pClause];k++) {
        iVar = GetVarFromLit(*pLit);
        aMakeCount[iVar]--;
        aMakePenalty[iVar] -= fPenalty;

        if (aMakeCount[iVar]==0) {
          aVarInFalseList[aVarInFalseListPos[iVar]] = aVarInFalseList[--iNumVarsInFalseList];
          aVarInFalseListPos[aVarInFalseList[iNumVarsInFalseList]] = aVarInFalseListPos[iVar];
        }
        
        pLit++;

      }
      aBreakCount[iFlipCandidate]++;
      aBreakPenalty[iFlipCandidate] += fPenalty;
      aCritSat[*pClause] = iFlipCandidate;
    }
    if (aNumTrueLit[*pClause]==2) {
      aBreakCount[aCritSat[*pClause]]--;
      aBreakPenalty[aCritSat[*pClause]] -= fPenalty;
    }
    *pClause++;
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

void CreateVarStateBackup() {
  aVarStateBackup = AllocateRAM((iNumVars+1)*sizeof(UINT32));
}

void BackupVarState() {
  UINT32 j;
  for (j=1;j<=iNumVars;j++) {
    aVarStateBackup[j] = aVarValue[j];
  }
}

void RestoreVarState() {
  UINT32 j;
  for (j=1;j<=iNumVars;j++) {
    aVarValue[j] = aVarStateBackup[j];
  }
}

void CreateLogDist() {
  UINT32 iMaxLogDistValues;
  FLOAT fCurValue;
  FLOAT fFactor;

  iMaxLogDistValues = ((UINT32) log10((FLOAT) iCutoff)) * iLogDistStepsPerDecade + 2;
  aLogDistValues = AllocateRAM(iMaxLogDistValues * sizeof(UINT32));

  fFactor = pow(10.0f,1.0f / (FLOAT) iLogDistStepsPerDecade);
  fCurValue = 10.0f;

  iNumLogDistValues = 0;

  while (fCurValue <= (FLOAT) iCutoff) {
    aLogDistValues[iNumLogDistValues++] = (UINT32) fCurValue;
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
  aVarStateBest = AllocateRAM((iNumVars+1)*sizeof(UINT32));
}

void UpdateSaveBest() {
  UINT32 j;
  BOOL bSave = FALSE;

  if (bWeighted) {
    if (iBestStepSumFalseW == iStep) {
      bSave = TRUE;
    }
  } else {
    if (iBestStepNumFalse == iStep) {
      bSave = TRUE;
    }
  }
  if (bSave) {
    for (j=1;j<=iNumVars;j++) {
      aVarStateBest[j] = aVarValue[j];
    }
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

  if (iTimeOut) {
    fTimeElapsed = TimeElapsed();
    if (fTimeElapsed > (double) iTimeOut) {
      bTerminateAllRuns = TRUE;
    }
  }
}

void CheckForRestarts() {
  if (iPeriodicRestart) {
    if ((iStep % iPeriodicRestart) == 0) {
      bRestart = TRUE;
    }
  }
  if (iProbRestart) {
    if (RandomProb(iProbRestart)) {
      bRestart = TRUE;
    }
  }
  if (iStagnateRestart) {
    if (iStep > (iBestStepNumFalse + iStagnateRestart)) {
      bRestart = TRUE;
      InitBestFalse();
    }
    if (bWeighted) {
      if (iStep > (iBestStepSumFalseW + iStagnateRestart)) {
        bRestart = TRUE;
        InitBestFalse();
      }
    }
  }
}      

void CreateFlipCounts() {
  aFlipCounts = AllocateRAM((iNumVars+1)*sizeof(UINT32));
}

void InitFlipCounts() {
  memset(aFlipCounts,0,(iNumVars+1)*sizeof(UINT32));
}

void UpdateFlipCounts() {
  aFlipCounts[iFlipCandidate]++;
}

void CreateUnsatCounts() {
  aUnsatCounts = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitUnsatCounts() {
  memset(aUnsatCounts,0,iNumClauses*sizeof(UINT32));
}

void UpdateUnsatCounts() {
  UINT32 j;
  for (j=0;j<iNumClauses;j++) {
    if (aNumTrueLit[j]==0)
      aUnsatCounts[j]++;
  }
}

void CreateClauseLast() {
  aClauseLast = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitClauseLast() {
  memset(aClauseLast,0,iNumClauses*sizeof(UINT32));
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
    aSQGridW = AllocateRAM(iNumLogDistValues * iNumRuns * sizeof(FLOAT));
  } else {
    aSQGrid = AllocateRAM(iNumLogDistValues * iNumRuns * sizeof(UINT32));
  }
}

void InitSQGrid() {
  iNextSQGridCol = 0;
}

void UpdateSQGrid() {

  if (iStep != aLogDistValues[iNextSQGridCol])
    return;

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


