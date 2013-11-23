/* encoding: UTF-8 */

#ifndef PLCMP_TABLES_H
#define PLCMP_TABLES_H

#include "plcmp_common.h"
#include "plcmp_symbols.h"
#include "utils.h"

#define ASCII_SYMBOLS_COUNT 256

#define NO_INPUT 0
typedef enum input_e {
    INPUT_SYM_AVI               = 164,
    INPUT_SYM_BUK               = 93,
    INPUT_SYM_CIF               = 136,
    INPUT_SYM_IDE               = 96,
    INPUT_SYM_IPE               = 153,
    INPUT_SYM_IPR               = 113,
    INPUT_SYM_LIT               = 161,
    INPUT_SYM_MAN               = 143,
    INPUT_SYM_ODC               = 176,
    INPUT_SYM_OEN               = NO_INPUT,
    INPUT_SYM_OPA               = NO_INPUT,
    INPUT_SYM_OPR               = 171,
    INPUT_SYM_PRO               = NO_INPUT,
    INPUT_SYM_RZR               = 139,
    INPUT_SYM_TEL               = 179,
    INPUT_SYM_ZNK               = NO_INPUT,
    INPUT_SYM_STC               = 204,
    INPUT_SYM_CON               = NO_INPUT,
    INPUT_SYM_A                 = 33,
    INPUT_SYM_B                 = 36,
    INPUT_SYM_C                 = 39,
    INPUT_SYM_D                 = 42,
    INPUT_SYM_E                 = 74,
    INPUT_SYM_M                 = 84,
    INPUT_SYM_P                 = 87,
    INPUT_SYM_X                 = 90,
    INPUT_SYM_0                 = 1,
    INPUT_SYM_1                 = 4,
    INPUT_SYM_2                 = 9,
    INPUT_SYM_3                 = 12,
    INPUT_SYM_4                 = 15,
    INPUT_SYM_5                 = 18,
    INPUT_SYM_6                 = 21,
    INPUT_SYM_7                 = 24,
    INPUT_SYM_8                 = 27,
    INPUT_SYM_9                 = 30,
    INPUT_SYM_PLUS              = 107,
    INPUT_SYM_MINUS             = 110,
    INPUT_SYM_COLON             = NO_INPUT,
    INPUT_SYM_I                 = NO_INPUT,
    INPUT_SYM_R                 = NO_INPUT,
    INPUT_SYM_N                 = NO_INPUT,
    INPUT_SYM_O                 = NO_INPUT,
    INPUT_SYM_T                 = NO_INPUT,
    INPUT_SYM_S                 = NO_INPUT,
    INPUT_SYM_LEFT_PARENTHESIS  = NO_INPUT,
    INPUT_SYM_RIGHT_PARENTHESIS = NO_INPUT,
    INPUT_SYM_SPACE             = NO_INPUT,
    INPUT_SYM_SEMICOLON         = NO_INPUT,
    INPUT_SYM_L                 = 201,
    INPUT_SYM_F                 = NO_INPUT,
    INPUT_SYM_ASSIGN            = NO_INPUT,
    INPUT_SYM_H                 = NO_INPUT,
    INPUT_SYM_MUL               = 198,
    INPUT_SYM_SINGLE_QUOTE      = NO_INPUT,
    INPUT_SYM_EXCL_POINT        = 225
} input_t;

typedef struct rule_s {
    index_t next;
    index_t prev;
    sym_t sym;
    index_t alt;
} rule_t;

/* ASCII relation to symbols */
sym_t ascii_rel[ASCII_SYMBOLS_COUNT];

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
extern rule_t const rules[];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
extern input_t const inputs[SYM_COUNT];

/* Adjacency matrix which will become reachability matrix afterward */
extern bool_t adj_reach_mtrx[SYM_COUNT][SYM_NTERMS_COUNT];

void plcmp_tables_build_reach_mtrx(void);
void plcmp_tables_init_ascii_relation(void);
enum sym_type_e type_of_sym(sym_t sym);

#endif /* PLCMP_TABLES_H */
