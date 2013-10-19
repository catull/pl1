/* encoding: UTF-8 */

#ifndef PLCMP_PARSER_SM_H
#define PLCMP_PARSER_SM_H

typedef enum plcmp_parser_sm_error_code_e {
    PLCMP_PARSER_SM_SUCCESS = 0,
    PLCMP_PARSER_SM_SYNTAX_ERROR,
    PLCMP_PARSER_SM_WRONG_STATE_ERROR,
    PLCMP_PARSER_SM_UNKNOWN_STATE_ERROR,
    PLCMP_PARSER_SM_UNITIALIZED_ESSENTIAL_PARAMS_ERROR
} plcmp_parser_sm_error_code_t;

enum plcmp_parser_sm_error_code_e plcmp_parser_sm_run(void);

#endif /* PLCMP_PARSER_SM_H */