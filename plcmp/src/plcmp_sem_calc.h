/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_H
#define PLCMP_SEM_CALC_H

#include <alloca.h>

#include "plcmp_target.h"
#include "utils.h"

#define PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN 20
#define PLCMP_SEM_CALC_MAX_ERRMSG_LEN 100

#if PLCMP_SEM_CALC_MAX_ERRMSG_LEN < PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN
#error macro PLCMP_SEM_CALC_MAX_ERRMSG_LEN less than \
       macro PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN
#endif

/* Enumerate defines error codes of semantic calculator module */
typedef enum plcmp_sem_calc_error_code_e {
    PLCMP_SEM_CALCULATOR_SUCCESS = 0,
    PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR,
    PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR,
    PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR,
    PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE_ERROR,
    PLCMP_SEM_CALCULATOR_CONCAT_ERROR,
    PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG_ERROR
} plcmp_sem_calc_error_code_t;

/* Structure defines content of the error data of semantic calculator module */
typedef struct plcmp_sem_calc_error_data_s {
    plcmp_sem_calc_error_code_t err_code;
    union {
        char identifier[9];
        char identifier_type[9];
        char operation[3];
    } data;
    char src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN + 1];
} plcmp_sem_calc_error_data_t;

struct sym_s {
    char name[8];
    char type;
    size_t capacity;
    size_t char_init_len;
    char INIT[50];
};

/* Subroutine constructs error message 
 * by error data of semantic calculator module */
char const* plcmp_sem_calc_errmsg_by_errdata(
    plcmp_sem_calc_error_data_t const *err_data,
    char errmsg[]);

#define PLCMP_SEM_CALC_ERRMSG_BY_ERRDATA(err_data)                          \
    plcmp_sem_calc_errmsg_by_errdata(err_data,                              \
                                     alloca(PLCMP_SEM_CALC_MAX_ERRMSG_LEN))

/* Subroutine for semantic calculation 
 * of the achieved targets made by syntax analyzer 
 * and for generation output assembler file */
struct plcmp_sem_calc_error_data_s plcmp_sem_calc_gen_asm_code(
    char const p_asm_fp_name[],
    char const compact_pl1_src_text[],
    targets_achieved_stack_t const *p_targets_achieved);

#endif /* PLCMP_SEM_CALC_H */
