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
    PLCMP_MAIN_SYNT_ANALYZER_ERROR,
    PLCMP_MAIN_SEM_CALCULATOR_ERROR
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
 *
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
 * It hasn't to be 'NULL'-pointer
 *
 */
#define PLCMP_MAIN_MAKE_ASM_FILE_PATH_BY_PL1_FILE_PATH(p_asm_fp_name, p_pl1_fp_name)    \
    do {                                                                                \
        PLCMP_MAIN_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, p_pl1_fp_name);             \
        asm_fp_len = strlen(p_asm_fp_name);                                             \
        p_asm_fp_name[asm_fp_len - 4] = '\0';                                           \
        strcat(p_asm_fp_name, ".ass");                                                  \
    } while(0)

/* 
 * Macro creates stack for goals for using later by syntax analyzer (parser)
 * 
 * @param1:
 * 'goals' has type 'cel_t'
 * Its field 'cel_stack' has to be NULL-pointer
 *
 */
#define PLCMP_MAIN_CREATE_GOALS_STACK(goals)                                    \
    do {                                                                        \
        goals.count = 0;                                                        \
                                                                                \
        goals.cel_stack = calloc(NCEL, sizeof(goals_stack_t));                  \
        if (NULL == goals.cel_stack)                                            \
        {                                                                       \
            printf("Error of allocating memory for stack of goals. Assert\n");  \
            PLCMP_COMMON_ASSERT(0);                                             \
        }                                                                       \
    } while(0)

/* 
 * Macro creates stack for goals achieved for using later
 * by syntax analyzer (parser) and semantic calculator 
 * 
 * @param1:
 * 'goals_achieved' has type 'dst_t'
 * Its field 'dst_stack' has to be NULL-pointer
 *
 */
#define PLCMP_MAIN_CREATE_GOALS_ACHIEVED_STACK(goals_achieved)                          \
    do {                                                                                \
        goals_achieved.count = 0;                                                       \
                                                                                        \
        goals_achieved.dst_stack = calloc(NDST, sizeof(goals_achieved_stack_t));        \
        if (NULL == goals.cel_stack)                                                    \
        {                                                                               \
            printf("Error of allocating memory for stack of achieved goals. Assert\n"); \
            PLCMP_COMMON_ASSERT(0);                                                     \
        }                                                                               \
    } while(0)

/* 
 * Macro destroys stack of goals 
 * 
 * @param1:
 * 'goals' has type 'cel_t'
 * Its field 'cel_stack' hasn't to be 'NULL'-pointer
 *
 */
#define PLCMP_MAIN_DESTROY_GOALS_STACK(goals)                       \
    do {                                                            \
        goals.count = 0;                                            \
                                                                    \
        PLCMP_COMMON_RELEASE_MEM(goals.cel_stack);                  \
    } while(0)

/* 
 * Macro destroys stack of goals achieved
 * 
 * @param1:
 * 'goals_achieved' has type 'dst_t'
 * Its field 'dst_stack' hasn't to be 'NULL'-pointer
 *
 */
#define PLCMP_MAIN_DESTROY_GOALS_ACHIEVED_STACK(goals_achieved)     \
    do {                                                            \
        goals_achieved.count = 0;                                   \
                                                                    \
        PLCMP_COMMON_RELEASE_MEM(goals_achieved.dst_stack);         \
    } while(0)

#endif /* PLCMP_MAIN_H */