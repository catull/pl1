/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_H
#define PLCMP_SEM_CALC_H

#include "plcmp_common.h"

#define PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN 20

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
extern char FORMT[MAXFORMT][9];
/* Formatted array index */
extern int IFORMT;

typedef enum plcmp_sem_calc_error_code_e {
    PLCMP_SEM_CALCULATOR_SUCCESS = 0,
    PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR,
    PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR,
    PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE,
    PLCMP_SEM_CALCULATOR_CONCAT_ERROR,
    PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG
} plcmp_sem_calc_error_code_t;

typedef struct plcmp_sem_calc_error_data_s {
    plcmp_sem_calc_error_code_t err_code;
    union {
        char identifier[9];
        char identifier_type[9];
        char operation[3];
    } data;
    char src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN + 1];
} plcmp_sem_calc_error_data_t;


/* This struct is type of the table of labels' names of variables
 * which is being filled on the first process of semantic calculation
 * and is being used on the second process of semantic calculation 
 */
typedef struct sym_s
{
    char NAME[8];
    char TYPE;
    unsigned int RAZR;
    char INIT[50];
} sym_t;

struct plcmp_sem_calc_error_data_s plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name,
                                                               dst_t const *p_goals_achieved);

char* plcmp_sem_calc_errmsg_by_errdata(plcmp_sem_calc_error_data_t const *err_data, char *errmsg);

#endif /* PLCMP_SEM_CALC_H */