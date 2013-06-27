/* encoding: UTF-8 */

#ifndef PLCMP_TABLES_H
#define PLCMP_TABLES_H

#include "plcmp_common.h"

typedef struct sint_s {
    int POSL;
    int PRED;
    char DER[4];
    int ALT;
} sint_t;

typedef struct vxod_s {
    char SYM[4];
    int VX;
    char TYP;
} vxod_t;

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
extern sint_t SINT[NSINT];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
extern vxod_t VXOD[NVXOD];

/* Adjacency matrix which will become reachability matrix afterward */
extern char TPR[NVXOD][NNETRM];

/* Function finds necessary string with sought-for symbol in the VXOD-table 
 * and returns string-index of VXOD-table.
 * 
 * @param1: const char *p_str_symbol
 * C-string containing name of sought-for symbol
 *
 * @param2: unsigned int symbol_str_len
 * Length of the string of sought-for symbol
 */
unsigned int plcmp_tables_sym_sint_ind(const char *p_str_symbol, unsigned int symbol_str_len);

#endif /* PLCMP_TABLES_H */
