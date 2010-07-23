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

void PickDCRWalk();
void CreateClausePickCount();
void InitClausePickCount();
void UpdateClausePickCount();
void CreateNextClauseLit();
void InitNextClauseLit();
void UpdateNextClauseLit();

UINT32 iClausePick;
UINT32 *aClausePickCount;  
UINT32 *aNextClauseLit;


void PickDANOVP();
void InitAdaptNoveltyNoiseDet();
void AdaptNoveltyNoiseDet();

UINT32 iCountNovNoise0;
UINT32 iCountNovNoise1;


void AddDerandomized() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("dcrwalk","",FALSE,
    "Deterministic Conflict-Directed Random Walk (For Academic Interest)",
    "Tompkins, Hoos [AI 2006]",
    "PickDCRWalk",
    "DefaultProcedures,Flip+FalseClauseList",
    "default","default");

  CreateTrigger("PickDCRWalk",ChooseCandidate,PickDCRWalk,"ClausePickCount,NextClauseLit,UnsatCounts","UpdateUnsatCounts,UpdateNextClauseLit,UpdateClausePickCount");

  CreateTrigger("CreateClausePickCount",CreateStateInfo,CreateClausePickCount,"","");
  CreateTrigger("InitClausePickCount",InitStateInfo,InitClausePickCount,"","");
  CreateTrigger("UpdateClausePickCount",PreFlip,UpdateClausePickCount,"","");
  CreateContainerTrigger("ClausePickCount","CreateClausePickCount,InitClausePickCount,UpdateClausePickCount");

  CreateTrigger("CreateNextClauseLit",CreateStateInfo,CreateNextClauseLit,"","");
  CreateTrigger("InitNextClauseLit",InitStateInfo,InitNextClauseLit,"","");
  CreateTrigger("UpdateNextClauseLit",PreFlip,UpdateNextClauseLit,"","");
  CreateContainerTrigger("NextClauseLit","CreateNextClauseLit,InitNextClauseLit,UpdateNextClauseLit");

  pCurAlg = CreateAlgorithm("danov+","",FALSE,
    "Deterministic Adaptive Novelty Plus (For Academic Interest)",
    "Tompkins, Hoos [AI 2006]",
    "PickDANOVP",
    "DefaultProcedures,Flip+FalseClauseList,AdaptNoveltyNoiseDet,VarLastChange",
    "default","default");

  AddParmProbability(&pCurAlg->parmList,"-wp","pseudo-walk probability [default %s]","with frequency 1/PR, select a variable~from a an unsat clause to flip","",&iWp,0.01);

  CreateTrigger("PickDANOVP",ChooseCandidate,PickDANOVP,"ClausePickCount,NextClauseLit,UnsatCounts","UpdateUnsatCounts,UpdateClausePickCount,UpdateNextClauseLit");

  CreateTrigger("InitAdaptNoveltyNoiseDet",PostInit,InitAdaptNoveltyNoiseDet,"","");
  CreateTrigger("AdaptNoveltyNoiseDet",PostFlip,AdaptNoveltyNoiseDet,"InitAdaptNoveltyNoiseDet","");

}

void CreateClausePickCount() {
  aClausePickCount = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitClausePickCount() {
  memset(aClausePickCount,0,iNumClauses*sizeof(UINT32));
}

void UpdateClausePickCount() {
  aClausePickCount[iClausePick]++;
}

void CreateNextClauseLit() {
  aNextClauseLit = AllocateRAM(iNumClauses*sizeof(UINT32));
}

void InitNextClauseLit() {
  memset(aNextClauseLit,0,iNumClauses*sizeof(UINT32));
}

void UpdateNextClauseLit() {
  aNextClauseLit[iClausePick]++;
  if (aNextClauseLit[iClausePick]==aClauseLen[iClausePick]) {
    aNextClauseLit[iClausePick] = 0;
  }
}

void PickDCRWalk() {
  
  UINT32 j;
  FLOAT fClausePickScore;
  FLOAT fBestClausePickScore = FLOATMAX;
  UINT32 iClause;

  LITTYPE litPick;

  iClausePick = iNumClauses - 1;

  if (iNumFalse) {
    for (j=0;j<iNumFalse;j++) {
      iClause = aFalseList[j];
      fClausePickScore = ((FLOAT) aClausePickCount[iClause]) / ((FLOAT) (++aUnsatCounts[iClause]));
      if ((fClausePickScore < fBestClausePickScore)||((fClausePickScore == fBestClausePickScore)&&(iClause < iClausePick))) {
        fBestClausePickScore = fClausePickScore;
        iClausePick = iClause;
      }
    }
    litPick = (pClauseLits[iClausePick][aNextClauseLit[iClausePick]]);
    iFlipCandidate = GetVarFromLit(litPick);

    aClausePickCount[iClausePick]++;

    aNextClauseLit[iClausePick]++;
    if (aNextClauseLit[iClausePick]==aClauseLen[iClausePick]) {
      aNextClauseLit[iClausePick] = 0;
    }
  }
}


void PickDANOVP() {
  
  UINT32 j;

  FLOAT fCurNoiseCalc;

  LITTYPE litPick;

  UINT32 i;
  SINT32 iScore;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  UINT32 *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;

  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;


  if (iNumFalse) {

    iClausePick = aFalseList[iStep % iNumFalse];

    if (iWp) {
      if ((iStep % ProbToInvInt(iWp))==0) {
        litPick = (pClauseLits[iClausePick][aNextClauseLit[iClausePick]]);
        iFlipCandidate = GetVarFromLit(litPick);

        aNextClauseLit[iClausePick]++;
        if (aNextClauseLit[iClausePick]==aClauseLen[iClausePick]) {
          aNextClauseLit[iClausePick] = 0;
        }
        return;
      }
    }
 
    iBestScore = iNumClauses;
    iSecondBestScore = iNumClauses;
      
    iClauseLen = aClauseLen[iClausePick];

    pLit = pClauseLits[iClausePick];

    iYoungestVar = GetVarFromLit(*pLit);

    for (j=0;j<iClauseLen;j++) {

      iScore = 0;

      iVar = GetVarFromLit(*pLit);

      iNumOcc = aNumLitOcc[*pLit];
      pClause = pLitClause[*pLit];
  
      for (i=0;i<iNumOcc;i++) {
        if (aNumTrueLit[*pClause]==0) {
          iScore--;
        }
        pClause++;
      }

      iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
      pClause = pLitClause[GetNegatedLit(*pLit)];
  
      for (i=0;i<iNumOcc;i++) {
        if (aNumTrueLit[*pClause]==1) {
          iScore++;
        }
        pClause++;
      }

      if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar]) {
        iYoungestVar = iVar;
      }

      if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
        iSecondBestVar = iBestVar;
        iBestVar = iVar;
        iSecondBestScore = iBestScore;
        iBestScore = iScore;
      } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
        iSecondBestVar = iVar;
        iSecondBestScore = iScore;
      }

      pLit++;
    }

    iFlipCandidate = iBestVar;

    if (iFlipCandidate != iYoungestVar) {
      return;
    }

    if (iNovNoise) {

      fCurNoiseCalc = ((FLOAT) iCountNovNoise1) / (FLOAT) (iCountNovNoise0 + iCountNovNoise1);

      if (fCurNoiseCalc < ProbToFloat(iNovNoise)) {
        iFlipCandidate = iSecondBestVar;
        iCountNovNoise1++;
      } else {
        iCountNovNoise0++;
      }
    }
  }
}

void InitAdaptNoveltyNoiseDet() {
  iLastAdaptStep=iStep;
  iLastAdaptNumFalse=iNumFalse;
  fLastAdaptSumFalseW=fTotalWeight;
  iNovNoise = 0;
  iCountNovNoise0 = 1;
  iCountNovNoise1 = 1;
}

void AdaptNoveltyNoiseDet() {
  
  if (iStep-iLastAdaptStep > iNumClauses/iInvTheta) {
    
    iNovNoise += (PROBABILITY) ((UINT32MAX - iNovNoise)/iInvPhi);
    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;
    if (iNovNoise == 0) {
      iCountNovNoise0 = 1;
      iCountNovNoise1 = 1;
    } else {
      iCountNovNoise1 = (iNovNoise >> 24) & (0xFF);
      iCountNovNoise0 = 256 - iCountNovNoise1;
    }
  } else if (iNumFalse < iLastAdaptNumFalse) {

    iNovNoise -= (PROBABILITY) (iNovNoise / iInvPhi / 2);

    iLastAdaptStep = iStep;
    iLastAdaptNumFalse = iNumFalse;

    if (iNovNoise == 0) {
      iCountNovNoise0 = 1;
      iCountNovNoise1 = 1;
    } else {
      iCountNovNoise1 = (iNovNoise >> 24) & (0xFF);
      iCountNovNoise0 = 256 - iCountNovNoise1;
    }
  }
}


