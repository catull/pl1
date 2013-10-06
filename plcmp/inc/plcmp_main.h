/* encoding: UTF-8 */

#ifndef PLCMP_MAIN_H
#define PLCMP_MAIN_H

#include <string.h>

#include "plcmp_utils.h"
#include "plcmp_lex_analyzer.h"
#include "plcmp_sem_calc.h"
#include "plcmp_synt_analyzer.h"

/* Enumerate defines error codes of main module */
typedef enum plcmp_main_error_code_e {
    PLCMP_MAIN_SUCCESS = 0,
    PLCMP_MAIN_SUCCESSFUL_TRANSLATION = PLCMP_MAIN_SUCCESS,
    PLCMP_MAIN_WRONG_NUM_CLI_PAR_ERROR,
    PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH_ERROR,
    PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION_ERROR,
    PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE_ERROR,
    PLCMP_MAIN_READING_PL1_FILE_ERROR,
    PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW_ERROR,
    PLCMP_MAIN_LEX_ANALYZER_ERROR,
    PLCMP_MAIN_SYNT_ANALYZER_ERROR,
    PLCMP_MAIN_SEM_CALCULATOR_ERROR
} plcmp_main_error_code_t;

/* Structure defines content of the error data of main module */
typedef struct plcmp_main_error_data_s {
    plcmp_main_error_code_t main_err_code;
    plcmp_lex_analyzer_error_data_t lex_analyzer_err_data;
    plcmp_synt_analyzer_error_data_t synt_analyzer_err_data;
    plcmp_sem_calc_error_data_t sem_calc_err_data;
} plcmp_main_error_data_t;

#endif /* PLCMP_MAIN_H */
