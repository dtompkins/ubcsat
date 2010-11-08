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

void PickGNovletyPlus();

void InitAdaptG2WSatNoise();
void AdaptG2WSatNoise();

void AddGNoveltyPlus() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("gnovelty+","",0,
    "gNovelty+",
    "Pham, Thornton, Gretton, Sattar [Li, Huang  [SAT 05] and Li, Wei, Zhang [AI 07]",
    "PickGNovletyPlus",
    "DefaultProcedures,Flip+PenaltyDecPromVars+FCL,PenaltyDecPromVars,FalseClauseList,VarLastChange,PenClauseList,AdaptNoveltyNoise",
    "default","default");
  
  //AddParmProbability(&pCurAlg->parmList,"-novnoise","novelty noise [default %s]","","",&iNovNoise,0.50);
  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability [default %s]","with probability PR, select a random variable from a~randomly selected unsat clause","",&iNovWpDp,0.01);
  AddParmProbability(&pCurAlg->parmList,"-ps","smooth probabilty [default %s]","after a scaling step, ~smooth penalties with probability PR","",&iPs,0.4);

  CreateTrigger("PickGNovletyPlus",ChooseCandidate,PickGNovletyPlus,"","");


  //pCurAlg = CreateAlgorithm("adaptg2wsat","",0,
  //  "Adaptive G2WSat (generalized)",
  //  "Li, Wei, Zhang  [SAT 07]",
  //  "PickG2WSatGeneral,ConfigureG2WSatGeneral,InitAdaptG2WSatNoise,AdaptG2WSatNoise",
  //  "DefaultProcedures,Flip+TrackChanges+FCL,DecPromVars,FalseClauseList,VarLastChange",
  //  "default","default");

  //AddParmUInt(&pCurAlg->parmList,"-prom","G2WSAT Decreasing Promising Variable ID [default %s]","0: best~1: oldest variable~2: longest promising","",&iG2WsatPromisingSelectID,0);
  //AddParmUInt(&pCurAlg->parmList,"-div","G2WSAT Diversification ID [default %s]","0: random (Novelty+)~1: oldest (Novelty++)~2: random[n-1] excl. youngest (Novelty-1)~3: random[n-2] excl. best 2  (Novelty-2)","",&iG2WsatDiversificationSelectID,0);
  //AddParmUInt(&pCurAlg->parmList,"-base","G2WSAT Base Algorithm ID [default %s]","0: Novelty~1: Novelty+p~2: Top2","",&iG2WsatBaseAlgSelectID,0);

  //AddParmFloat(&pCurAlg->parmList,"-phi","adjustment parameter phi [default %s]","phi determines the rate of change of noise","",&fAdaptPhi,0.1);
  //AddParmFloat(&pCurAlg->parmList,"-theta","adjustment parameter theta [default %s]","theta determines the stagnation detection","",&fAdaptTheta,0.2f);

  //CreateTrigger("InitAdaptG2WSatNoise",PostInit,InitAdaptG2WSatNoise,"","");
  //CreateTrigger("AdaptG2WSatNoise",PostFlip,AdaptG2WSatNoise,"InitAdaptG2WSatNoise","");

  //todo... add theta & phi
}

void PickGNovletyPlus() {

  UINT32 iLoopMax;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE litPick;
  UINT32 iVar;
  SINT32 iScore;
  LITTYPE *pLit;
  UINT32 j;
  UINT32 k;

  UINT32 iYoungestVar;
  SINT32 iSecondBestScore;
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  if (iNumFalse) {
    if (RandomProb(iNovWpDp)) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else if (iNumPenaltyDecPromVars > 0 ) {
      iFlipCandidate = aPenaltyDecPromVarsList[0];
      iBestScore = (SINT32) aBreakPenaltyINT[iFlipCandidate] - (SINT32) aMakePenaltyINT[iFlipCandidate];
      for (j=1;j<iNumPenaltyDecPromVars;j++) {
        iVar = aPenaltyDecPromVarsList[j];
        iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
        if (iScore < iBestScore) {
          iFlipCandidate = iVar;
          iBestScore = iScore;
        } else {
          /* If there is a tie, prefer the 'oldest' variable */
          if (iScore == iBestScore) {
            if (aVarLastChange[iVar] < aVarLastChange[iFlipCandidate]) {
              iFlipCandidate = iVar;
            }
          }
        }
      }
    } else {

      // Note: as per original author's gNovelty+ code, perform Clause penalty update 1st

      /* for each false clause, increae the clause penalty by 1 */
      iTotalPenaltyINT += iNumFalse;
      for(j=0;j<iNumFalse;j++) {
        iClause = aFalseList[j];
        aClausePenaltyINT[iClause]++;
        if (aClausePenaltyINT[iClause]==2) {
          aPenClauseList[iNumPenClauseList] = iClause;
          aPenClauseListPos[iClause] = iNumPenClauseList++;
        }
        pLit = pClauseLits[iClause];
        for (k=0;k<aClauseLen[iClause];k++) {
          iVar = GetVarFromLit(*pLit);
          aMakePenaltyINT[iVar]++;
          if ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1) { // todo step-1 accurate here?
            aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
          }
          pLit++;
        }
      }

      /* with probability, perform smoothing */
      if (RandomProb(iPs)) {
        iLoopMax = iNumPenClauseList;
        iTotalPenaltyINT -= iNumPenClauseList;
        for (j=0;j<iLoopMax;j++) {
          iClause = aPenClauseList[j];
          aClausePenaltyINT[iClause]--;
          if (aClausePenaltyINT[iClause]==1) {
            aPenClauseList[aPenClauseListPos[iClause]] = aPenClauseList[--iNumPenClauseList]; // can be better... fix in PAWS too
            aPenClauseListPos[aPenClauseList[iNumPenClauseList]] = aPenClauseListPos[iClause];
          }
          if (aNumTrueLit[iClause]==1) {
            iVar = aCritSat[iClause];
            aBreakPenaltyINT[iVar]--;
            if ((SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar] == -1) { // todo (see above) -- also, orig. authors don't do this
              aPenaltyDecPromVarsList[iNumPenaltyDecPromVars++] = iVar;
            }
          }
          if (aNumTrueLit[iClause]==0) {
            pLit = pClauseLits[iClause];
            for (k=0;k<aClauseLen[iClause];k++) {
              aMakePenaltyINT[GetVarFromLit(*pLit)]--;
              pLit++;
            }
          }
        }
      }

      /* now pick according Penalized Adaptive Novelty+ */

      iBestScore = (SINT32) iTotalPenaltyINT;
      iSecondBestScore = (SINT32) iTotalPenaltyINT;

      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      pLit = pClauseLits[iClause];
      iYoungestVar = GetVarFromLit(*pLit);
      for (j=0;j<iClauseLen;j++) {
        iVar = GetVarFromLit(*pLit);
        iScore = (SINT32) aBreakPenaltyINT[iVar] - (SINT32) aMakePenaltyINT[iVar];
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
      if (RandomProb(iNovNoise)) { // TODO: ORIGINAL AUTHORS HAD SOMETHING WEIRD HERE :: if ( (_num_getScore(bestVar) == 0) || (random()%100 < noise) ) {
        iFlipCandidate = iSecondBestVar;
      }
    }
  }
}


#ifdef __cplusplus
}
#endif

