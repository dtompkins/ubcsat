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

#ifndef UBCSAT_LIT_H
#define UBCSAT_LIT_H

#include "ubcsat-types.h"

#ifdef __cplusplus 
namespace ubcsat {
#endif

typedef UINT32 LITTYPE;

#define GetTrueLit(V) (((V) << 1) + 1 - aVarValue[V])
#define GetFalseLit(V) (((V) << 1) + aVarValue[V])

#define GetPosLit(V) (((V) << 1))
#define GetNegLit(V) (((V) << 1) + 1)

#define GetNegatedLit(L) ((L) ^ 1)

#define IsLitNegated(L) ((L) & 1)

#define GetVarFromLit(L) ((L) >> 1)
#define GetVar(c,p) (pClauseLits[c][p] >> 1)

#define IsLitTrue(L) (aVarValue[(L) >> 1] ^ (L & 1))

#define SetLitFromFile(V) ((LITTYPE) (((V) < 0) ? (((-(V)) << 1) + 1) : ((V) << 1)))

#ifdef __cplusplus
}
#endif
#endif
