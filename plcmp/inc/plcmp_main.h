/* encoding: UTF-8 */

#ifndef PLCMP_MAIN_H
#define PLCMP_MAIN_H

#include <string.h>

#include "plcmp_common.h"

/*
 * Macro makes file path with '.ass' extension
 * from the file path with '.pli' extension
 *
 * @param1:
 * 'p_asm_fp_name' has type 'char*'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'p_pl1_fp_name' has type 'char*' or 'char const*'
 * It hasn't to be 'NULL'-pointer
 *
 */
#define PLCMP_MAIN_MAKE_ASM_FILE_PATH_BY_PL1_FILE_PATH(p_asm_fp_name,          \
                                                       p_pl1_fp_name)          \
    do {                                                                       \
        PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, p_pl1_fp_name);  \
        size_t asm_fp_len = strlen(p_asm_fp_name);                             \
        p_asm_fp_name[asm_fp_len - 4] = '\0';                                  \
        strcat(p_asm_fp_name, ".ass");                                         \
    } while (0)

#endif /* PLCMP_MAIN_H */
