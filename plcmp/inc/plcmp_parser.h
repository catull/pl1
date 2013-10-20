/* encoding: UTF-8 */

#ifndef PLCMP_PARSER_H
#define PLCMP_PARSER_H

#include "plcmp_goal.h"

#define PLCMP_PARSER_SRC_TEXT_PART_LEN 20

/* Enumerate defines error codes of syntax analyzer module */
typedef enum plcmp_parser_error_code_e {
    PLCMP_PARSER_SUCCESS = 0,
    PLCMP_PARSER_SYNTAX_ERROR,
    PLCMP_PARSER_INTERNAL_ERROR
} plcmp_parser_error_code_t;

/* Structure defines content of the error data of syntax analyzer module */
typedef struct plcmp_parser_error_data_s {
    plcmp_parser_error_code_t err_code;
    char src_text_part[PLCMP_PARSER_SRC_TEXT_PART_LEN + 1];
} plcmp_parser_error_data_t;

/* Subroutine constructs error message 
 * by error data of syntax analyzer module */
char const* plcmp_parser_errmsg_by_errdata(
    plcmp_parser_error_data_t const *err_data,
    char errmsg[]);

/* Subroutine of syntax analyzer. 
 * It constructs parse tree and returns error data if it will be */
struct plcmp_parser_error_data_s plcmp_parser_syntax_analysis(
    char const compact_pl1_src_text[],
    goals_achieved_stack_t **p_goals_achieved);

#endif /* PLCMP_PARSER_H */