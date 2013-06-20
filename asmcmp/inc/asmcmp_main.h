/* encoding: UTF-8 */

#ifndef ASMCMP_MAIN_H
#define ASMCMP_MAIN_H

#include "asmcmp_common.h"

typedef enum asmcmp_main_error_code_e {
    ASMCMP_MAIN_SUCCESS = 0, ASMCMP_MAIN_SUCCESSFUL_TRANSLATION = 0,
    ASMCMP_MAIN_WRONG_NUM_CLI_PAR,
    ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH,
    ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION,
    ASMCMP_MAIN_NOT_FOUND_INPUT_ASM_FILE,
    ASMCMP_MAIN_ERROR_READING_ASM_FILE,
    ASMCMP_MAIN_PROGRAM_BUFFER_OVERFLOW
} asmcmp_main_error_code_t;

typedef struct asmcmp_main_error_data_s {
    asmcmp_main_error_code_t main_err_code;
} asmcmp_main_error_data_t;


/*
 * Macro makes file path with '.txt' extension
 * from the file path with '.ass' extension
 *
 * @param1:
 * 'p_tex_fp_name' has type 'char*'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'p_asm_fp_name' has type 'char*' or 'char const*'
 * It hasn't to be 'NULL'-pointer
 *
 */
#define ASMCMP_MAIN_MAKE_TEX_FILE_PATH_BY_ASM_FILE_PATH(p_tex_fp_name, p_asm_fp_name)   \
    do {                                                                                \
        ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_tex_fp_name, p_asm_fp_name);          \
        size_t tex_fp_len = strlen(p_tex_fp_name);                                      \
        p_tex_fp_name[tex_fp_len - 4] = '\0';                                           \
        strcat(p_tex_fp_name, ".tex");                                                  \
    } while(0)

#endif