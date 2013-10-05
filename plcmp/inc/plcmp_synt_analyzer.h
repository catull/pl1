/* encoding: UTF-8 */

#ifndef PLCMP_SYNT_ANALYZER_H
#define PLCMP_SYNT_ANALYZER_H

#include "plcmp_goal.h"

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

/* Subroutine constructs error message 
 * by error data of syntax analyzer module */
char* plcmp_synt_analyzer_errmsg_by_errdata(
    plcmp_synt_analyzer_error_data_t const *err_data,
    char errmsg[]);

/* Subroutine of syntax analyzer. 
 * It constructs parse tree and returns error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    dst_t *p_goals_achieved);

#endif /* PLCMP_SYNT_ANALYZER_H */
