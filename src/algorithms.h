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

/* hybrid.c */

void AddHybrid();
void UpdateHybridInfo();
void InitHybridInfo();
extern FLOAT fVW2WeightMax;

/* ncvw.c */

void AddNCVW();

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
extern FLOAT fLastAdaptSumFalseW;
extern UINT32 iInvPhi;
extern UINT32 iInvTheta;
extern FLOAT fAdaptPhi;
extern FLOAT fAdaptTheta;

/* saps.c */

void AddSAPS();
void PickSAPS();
void PostFlipRSAPS();
void ScaleSAPS();
extern FLOAT fAlpha;
extern FLOAT fRho;
extern FLOAT fPenaltyImprove;
extern PROBABILITY iPs;
extern PROBABILITY iRPs;

/* paws.c */

void AddPAWS();
extern PROBABILITY iPAWSFlatMove;

/* ddfw.c */

void AddDDFW();

/* g2wsat.c */

void AddG2WSat();
void AdaptG2WSatNoise();
extern UINT32 iG2WsatSubAlgID;
extern UINT32 iG2WsatPromSelectID;
extern FXNPTR fxnG2WsatNovelty;

/* vw.c */

void AddVW();
void PickVW2Auto();
void UpdateVW2Auto();
BOOL BoundedExpProbability (SINT32 iExpProbability);

extern FLOAT fVW2Smooth;
extern FLOAT fVW2WeightFactor;
extern FLOAT *aVW2Weights;
extern FLOAT fVW2WeightMean;
extern SINT32 iMaxExpProbability;

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

/* rgsat.c */

void AddRGSat();

/* weighted.c */

void AddWeighted();

