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

#ifndef UBCSAT_ALGORITHMS_H
#define UBCSAT_ALGORITHMS_H

#include "ubcsat-types.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif


void AddAlgorithms();

/* gsat.c */

void AddGSat();

/* gwsat.c */

void AddGWSat();
extern PROBABILITY iWp;

/* gsat-tabu.c */

void AddGSatTabu();
extern UINT32 iTabuTenure;

/* hsat.c */

void AddHSat();
void PickHSat();
void PickHSatW();

/* hwsat.c */

void AddHWSat();

/* jack.c */

void AddJack();

/* walksat.c */

void AddWalkSat();

/* walksat-tabu.c */

void AddWalkSatTabu();
void PickWalkSatTabu();
extern UINT32 iWalkSATTabuClause;

/* novelty.c */

void AddNovelty();
void AddNoveltyPlus();
void AddNoveltyPlusPlus();
void PickNoveltyPlus();
void PickNoveltyPlusPlus();

void PickNoveltyVarScore();
void PickNoveltyPlusVarScore();
void PickNoveltyPlusPlusVarScore();

extern PROBABILITY iNovNoise;
extern PROBABILITY iNovWpDp;

/* novelty+p.c */

void AddNoveltyPlusP();
void PickNoveltyP();
void PickNoveltyPlusP();
void PickNoveltyPlusPlusP();

/* rnovelty.c */

void AddRNovelty();
void AddRNoveltyPlus();

/* adaptnovelty.c */

void AddAdaptNoveltyPlus();
void InitAdaptNoveltyNoise();
void AdaptNoveltyNoiseAdjust();
extern UBIGINT iLastAdaptStep;
extern UINT32 iLastAdaptNumFalse;
extern UBIGINT iLastAdaptSumFalseWeight;
extern UINT32 iInvPhi;
extern UINT32 iInvTheta;
extern FLOAT fAdaptPhi;
extern FLOAT fAdaptTheta;

/* saps.c */

void AddSAPS();
void ScaleSAPS();
extern FLOAT fAlpha;
extern FLOAT fRho;
extern FLOAT fPenaltyImprove;
extern PROBABILITY iPs;
extern PROBABILITY iRPs;

/* paws.c */

void AddPAWS();
extern PROBABILITY iPAWSFlatMove;
extern UINT32 *aPenClauseList;
extern UINT32 *aPenClauseListPos;
extern UINT32 iNumPenClauseList;

/* ddfw.c */

void AddDDFW();

/* g2wsat.c */

void AddG2WSat();
extern FXNPTR fxnG2WsatPromisingSelect;
extern FXNPTR fxnG2WsatDiversificationSelect;
extern FXNPTR fxnG2WsatBaseAlgSelect;
extern UINT32 iG2WsatPromisingSelectID;
extern UINT32 iG2WsatDiversificationSelectID;
extern UINT32 iG2WsatBaseAlgSelectID;
void PromisingSelectOldest();
void PromisingSelectBest();
void DiversificationRandom();
void DiversificationOldest();
void DiversificationMinusOne();

/* sparrow.c */
void AddSparrow();

/* vw.c */

void AddVW();
BOOL BoundedExpProbability (SINT32 iExpProbability);

extern FLOAT fVW2Smooth;
extern FLOAT fVW2WeightFactor;
extern FLOAT *aVW2Weights;
extern FLOAT fVW2WeightMean;

/* rots.c */

void AddRoTS();
void PickRoTS();

extern UINT32 iTabuTenureInterval;
extern UINT32 iTabuTenureLow;
extern UINT32 iTabuTenureHigh;

/* irots.c */

void AddIRoTS();

/* samd.c */

void AddSAMD();

/* random.c */

void AddRandom();

/* derandomized.c */

void AddDerandomized();
extern UINT32 *aNextClauseLit;
extern UINT32 iClausePick;

/* rgsat.c */

void AddRGSat();

/* weighted.c */

void AddWeighted();


#ifdef __cplusplus
}
#endif
#endif
