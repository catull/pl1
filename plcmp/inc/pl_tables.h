#ifndef PL_TABLES_H
#define PL_TABLES_H

#include "pl_common.h"

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
struct sint_s {
    int POSL;
    int PRED;
    char DER[4];
    int ALT;
} SINT[NSINT];

/* Table that is used as a stack of achievements */
struct dst_s {
    char DST1[4];
    int DST2;
    int DST3;
    int DST4;
    int DST5;
} DST[NDST];

/* Table that is used as a stack of goals */
struct cel_s {
    char CEL1[4];
    int CEL2;
    int CEL3;
} CEL[NCEL];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
struct vxod_s {
    char SYM[4];
    int VX;
    char TYP;
} VXOD[NVXOD];

char TPR[NVXOD][NNETRM];

#endif

