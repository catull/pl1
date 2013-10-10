/* encoding: UTF-8 */

#ifndef PLCMP_TABLES_H
#define PLCMP_TABLES_H

#include "plcmp_common.h"
#include "plcmp_utils.h"

typedef struct synt_rule_s {
    int next;
    int prev;
    sym_design_t sym_design;
    int alt;
} synt_rule_t;

typedef enum typeof_symbol_s {
    TERM,
    NON_TERM
} typeof_sym_t;

typedef struct input_sym_s {
    sym_title_t sym_title;
    unsigned int synt_rule_tb_ind;
    typeof_sym_t type;
} input_sym_t;

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
extern synt_rule_t synt_rules_table[NSINT];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
extern input_sym_t input_syms_table[NVXOD];

/* Adjacency matrix which will become reachability matrix afterward */
extern char adj_reach_mtrx[NVXOD][NNETRM];

/* Function finds necessary string with sought-for symbol in the 
 * input_syms_table-table and returns string-index of input_syms_table-table.
 * 
 * @param1: const char *p_str_symbol
 * C-string containing name of sought-for symbol
 *
 * @param2: unsigned int symbol_str_len
 * Length of the string of sought-for symbol
 */
unsigned int plcmp_tables_get_synt_rules_stroke_ind(
    const char *p_str_symbol,
    unsigned int symbol_str_len);

void plcmp_tables_build_reach_mtrx(void);

#endif /* PLCMP_TABLES_H */
