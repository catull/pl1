/* encoding: UTF-8 */

#ifndef PLCMP_TABLES_H
#define PLCMP_TABLES_H

#include "plcmp_common.h"
#include "plcmp_utils.h"

typedef struct rule_s {
    index_t next;
    index_t prev;
    sym_design_t sym_design;
    index_t alt;
} rule_t;

typedef enum type_sym_s {
    TERM,
    NON_TERM
} sym_type_t;

typedef struct input_s {
    sym_title_t sym_title;
    index_t tb_rules_ind;
    sym_type_t type;
} input_t;

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
extern rule_t const synt_rules_table[NSINT];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
extern input_t const input_syms_table[NVXOD];

/* Adjacency matrix which will become reachability matrix afterward */
extern bool_t adj_reach_mtrx[NVXOD][NNETRM];

/* Function finds necessary string with sought-for symbol in the 'input_syms_table'-table 
 * and returns string-index of input_syms_table-table.
 * 
 * @param1: const char *p_str_symbol
 * C-string containing name of sought-for symbol
 *
 * @param2: unsigned int symbol_str_len
 * Length of the string of sought-for symbol
 */
index_t inputs_tb_ind(char const *p_str_symbol, unsigned int symbol_str_len);

#endif /* PLCMP_TABLES_H */
