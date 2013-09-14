/* encoding: UTF-8 */

#ifndef PLCMP_SYNT_ANALYZER_H
#define PLCMP_SYNT_ANALYZER_H

#include "plcmp_common.h"

#define PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN 20

/* Enumerate defines error codes of syntax analyzer module */
typedef enum plcmp_synt_analyzer_error_code_e {
    PLCMP_SYNT_ANALYZER_SUCCESS = 0,
    PLCMP_SYNT_ANALYZER_SYNTAX_ERROR = 1
} plcmp_synt_analyzer_error_code_t;

/* Structure defines content of the error data of syntax analyzer module */
typedef struct plcmp_synt_analyzer_error_data_s {
    plcmp_synt_analyzer_error_code_t err_code;
    char src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN + 1];
} plcmp_synt_analyzer_error_data_t;


/* 
 * Macro creates stack for goals for using later by syntax analyzer (parser)
 * 
 * @param1:
 * 'goals' has type 'cel_t'
 * Its field 'p_cel_stack' has to be NULL-pointer
 *
 */
#define PLCMP_SYNT_ANALYZER_CREATE_GOALS_STACK(goals)                          \
    do {                                                                       \
        goals.count = 0;                                                       \
        PLCMP_COMMON_CALLOCATE_MEM(goals.p_cel_stack,                          \
                                   NCEL,                                       \
                                   sizeof(goals_stack_t));                     \
    } while(0)

/* 
 * Macro destroys stack of goals 
 * 
 * @param1:
 * 'goals' has type 'cel_t'
 * Its field 'p_cel_stack' hasn't to be 'NULL'-pointer
 *
 */
#define PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals)                         \
    do {                                                                       \
        goals.count = 0;                                                       \
        PLCMP_COMMON_RELEASE_MEM(goals.p_cel_stack);                           \
    } while(0)


/* Subroutine of syntax analyzer. 
 * It constructs parse tree and returns error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    dst_t *p_goals_achieved);

/* Subroutine constructs error message 
 * by error data of syntax analyzer module */
char* plcmp_synt_analyzer_errmsg_by_errdata(
    plcmp_synt_analyzer_error_data_t const *err_data,
    char *errmsg);

#endif /* PLCMP_SYNT_ANALYZER_H */
