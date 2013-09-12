/* encoding: UTF-8 */

#ifndef PLCMP_TABLES_H
#define PLCMP_TABLES_H

#include "plcmp_common.h"

typedef struct synt_rule_s {
    int POSL;
    int PRED;
    char DER[4];
    int ALT;
} synt_rule_t;

typedef struct input_sym_s {
    char SYM[4];
    int VX;
    char TYP;
} input_sym_t;

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
extern synt_rule_t synt_rules_table[NSINT];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
extern input_sym_t input_syms_table[NVXOD];

/* Adjacency matrix which will become reachability matrix afterward */
extern char TPR[NVXOD][NNETRM];

/* Function finds necessary string with sought-for symbol in the input_syms_table-table 
 * and returns string-index of input_syms_table-table.
 * 
 * @param1: const char *p_str_symbol
 * C-string containing name of sought-for symbol
 *
 * @param2: unsigned int symbol_str_len
 * Length of the string of sought-for symbol
 */
unsigned int plcmp_tables_sym_sint_ind(const char *p_str_symbol, unsigned int symbol_str_len);

#endif /* PLCMP_TABLES_H */
