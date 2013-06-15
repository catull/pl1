#ifndef PLCMP_MAIN_H
#define PLCMP_MAIN_H

#include "plcmp_common.h"

typedef enum plcmp_main_error_code_e {
    PLCMP_MAIN_SUCCESS = 0, PLCMP_MAIN_SUCCESSFUL_TRANSLATION = 0,
    PLCMP_MAIN_WRONG_NUM_CLI_PAR,
    PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH,
    PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION,
    PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE,
    PLCMP_MAIN_ERROR_READING_PL1_FILE,
    PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW,

} plcmp_main_error_code_t;

/* 
 * Macro reasonably allocates memory for new 'p_fp_str_to' string of file path
 * and copies 'p_fp_str_from' string to
 *
 * @param1:
 * 'p_fp_str_to' has type 'char'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'p_fp_str_from' has type 'char' or 'char const'
 */
#define PLCMP_MAIN_ALLOC_MEM_AND_COPY_FP_STR(p_fp_str_to, p_fp_str_from)                \
    do {                                                                                \
        PLCMP_COMMON_ALLOC_MEM_FOR_STR(p_fp_str_to, strlen(p_fp_str_from));             \
        strcpy(p_fp_str_to, p_fp_str_from);                                             \
    } while(0)

/*
 * Macro makes file path with '.ass' extension
 * from the file path with '.pli' extension
 *
 * @param1:
 * 'p_asm_fp_name' has type 'char'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'p_pl1_fp_name' has type 'char' or 'char const'
 */
#define PLCMP_MAIN_MAKE_ASM_FILE_PATH_BY_PL1_FILE_PATH(p_asm_fp_name, p_pl1_fp_name)    \
    do {                                                                                \
        PLCMP_MAIN_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, p_pl1_fp_name);             \
        asm_fp_len = strlen(p_asm_fp_name);                                             \
        p_asm_fp_name[asm_fp_len - 4] = '\0';                                           \
        strcat(p_asm_fp_name, ".ass");                                                  \
    } while(0)



#endif /* PLCMP_MAIN_H */