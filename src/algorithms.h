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

/* walksat.c */

void AddWalkSat();
UINT32 PickClauseWCS();
UINT32 PickClauseWCSPen();

/* walksat-tabu.c */

void AddWalkSatTabu();
void PickWalkSatTabu();
extern UINT32 iWalkSATTabuClause;

/* novelty.c */

void AddNovelty();
void AddNoveltyTabu();
void AddNoveltyPlus();
void AddNoveltyPlusTabu();
void AddNoveltyPlusPlus();
void AddNoveltyPlusPlusTabu();
void AddNoveltyPlusPlusPrime();
void AddNoveltyPromising();
void AddNoveltyPromisingTabu();
void AddNoveltyPlusPromising();
void AddNoveltyPlusPromisingTabu();
void AddNoveltyPlusPlusPromising();
void AddNoveltyPlusPlusPromisingTabu();
void PickNoveltyPlusPlusPromising();
void AddNoveltyPlusPlusPrimePromising();
void AddNoveltyPlusFC();
void PickNoveltyTabu();
void PickNoveltyPromisingTabu();
void PickNoveltyTabu();
void PickNoveltyPlusPromising();
void PickNoveltyPlusPromisingTabu();
void PickNoveltyPlusPlus();
void PickNoveltyPlusPlusPromisingTabu();
void PickNoveltyPlusPlusTabu();
void PickNoveltyPlusPlusPrime();
void PickNoveltyPlusPlusPrimePromising();
void PickNoveltyPlusPlusW();
void PickNoveltyPlusPlusPrimePromisingTabu();
void AddNoveltyPlusPlusPrimePromisingTabu();
void PickNoveltyPlusFC();
void PickNoveltyPromisingFC();
UINT32 SelectClause();
SINT32 BestLookAheadScore();
SINT32 BestLookAheadPenScore();

extern PROBABILITY iNovNoise;
extern PROBABILITY iDp;
UINT32 iSelectClause;
/* rnovelty.c */

void AddRNovelty();
void AddRNoveltyPlus();

/* adaptnovelty.c */

void AddAdaptNoveltyPlus();
void AddAdaptNovelty();
extern UINT32 iLastAdaptStep;
extern UINT32 iLastAdaptNumFalse;
extern FLOAT fLastAdaptSumFalseW;
extern  UINT32 iInvPhi;
extern  UINT32 iInvTheta;
extern  UINT32 iPromInvPhi;
extern  UINT32 iPromInvTheta;
extern int intNovNoise;
extern int intDp;
extern BOOL bAdaptPromWalkProb;


extern PROBABILITY iWpWalk;
extern BOOL bAdaptWalkProb;

/* saps.c */

void AddSAPS();
void PickSAPS();

extern FLOAT fAlpha;
extern FLOAT fRho;
extern FLOAT fPenaltyImprove;
extern PROBABILITY iPs;
extern PROBABILITY iRPs;
extern const FLOAT fMaxClausePenalty;

/* paws.c */

void AddPAWS();
extern PROBABILITY iPAWSFlatMove;
extern UINT32 iPAWSMaxInc;
extern UINT32 *aPenClauseList;
extern UINT32 *aPenClauseListPos;
extern UINT32 iNumPenClauseList;


/* ddfw.c */

void AddDDFW();

/* g2wsat.c */

void AddSatenstein();
void AddAdaptG2WSatPlus();

extern BOOL bTabu;
extern BOOL bVarInFalse;
extern BOOL bPromisingList;
extern UINT32 iTieBreaking;
extern BOOL bPerformNoveltyAlternate;
extern UINT32 iUpdateSchemePromList;
extern UINT32 iAdaptiveNoiseScheme;
extern PROBABILITY iPromNovNoise;
extern PROBABILITY iPromDp;
extern PROBABILITY iPromWp;
extern UINT32 iScoringMeasure;
UINT32 TieBreaking();

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

/* vw.c */

void AddVW1();
void AddVW2();
void PickVW1();
void PickVW2();
void PickVW1Tabu();
void PickVW2Tabu();
void PickVW2Automated();
extern FLOAT *aVW2Weights;

