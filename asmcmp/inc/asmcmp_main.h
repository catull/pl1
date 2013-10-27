/* encoding: UTF-8 */

#ifndef ASMCMP_MAIN_H
#define ASMCMP_MAIN_H

#include "asmcmp_common.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_machine_oper.h"

/* Enumerate defines error codes of main module */
typedef enum asmcmp_main_error_code_e {
    ASMCMP_MAIN_SUCCESS = 0, ASMCMP_MAIN_SUCCESSFUL_TRANSLATION = ASMCMP_MAIN_SUCCESS,
    ASMCMP_MAIN_WRONG_NUM_CLI_PAR,
    ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH,
    ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION,
    ASMCMP_MAIN_NOT_FOUND_INPUT_ASM_FILE,
    ASMCMP_MAIN_ERROR_READING_ASM_FILE,
    ASMCMP_MAIN_PROGRAM_BUFFER_OVERFLOW,
    ASMCMP_MAIN_PSEUDO_OPER_ERROR,
    ASMCMP_MAIN_MACHINE_OPER_ERROR,
    ASMCMP_MAIN_OPERATION_CODE_ERROR,
    ASMCMP_MAIN_CANT_WRITE_TEX_FILE_ERROR,
    ASMCMP_MAIN_WRONG_WRITE_TEX_FILE_ERROR
} asmcmp_main_error_code_t;

typedef struct __asmcmp_main_error_data_s {
    asmcmp_main_error_code_t main_err_code;
    int card_number;
} __asmcmp_main_error_data_t;

typedef struct asmcmp_main_error_data_s {
    __asmcmp_main_error_data_t main_err_data;
    asmcmp_pseudo_oper_error_code_t pseudo_oper_err_code;
    asmcmp_machine_oper_error_code_t machine_oper_err_code;
} asmcmp_main_error_data_t;


#endif /* ASMCMP_MAIN_H */