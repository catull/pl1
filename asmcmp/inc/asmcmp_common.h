/* encoding: UTF-8 */

#ifndef ASMCMP_COMMON_H
#define ASMCMP_COMMON_H

#include <assert.h>

#define DL_ASSTEXT 16
#define DL_OBJTEXT 50                             /*длина об'ектн. текста   */
#define NSYM 10                                   /*размер табл.символов    */
#define NPOP 6                                    /*размер табл.псевдоопер. */
#define NOP  6                                    /*размер табл.операций    */
/* The limited length of the line of the assembler source text */
#define LINELEN 80

#define TRUE 1
#define FALSE 0

#define ASMCMP_COMMON_ASSERT(condition) assert(condition)

/*
 * Macro allocates memory for new 'p_str_for' string with 'str_length' length
 *
 * @param1:
 * 'p_str_for' has type 'char*'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'str_length' has type 'size_t'
 */
#define ASMCMP_COMMON_ALLOC_MEM_FOR_STR(p_str_for, str_length)                  \
    do {                                                                        \
        size_t __str_length = (str_length);                                     \
                                                                                \
        p_str_for = malloc(sizeof(char)*(__str_length + 1));                    \
        if (NULL == p_str_for)                                                  \
        {                                                                       \
            printf("Error of allocating memory for new string. Assert\n");      \
            ASMCMP_COMMON_ASSERT(0);                                            \
        }                                                                       \
        p_str_for[__str_length] = '\0';                                         \
    } while(0)

/* 
 * Macro reasonably allocates memory for new 'p_fp_str_to' string of file path
 * and copies 'p_fp_str_from' string to
 *
 * @param1:
 * 'p_fp_str_to' has type 'char*'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'p_fp_str_from' has type 'char*' or 'char const*'
 *
 */
#define ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_fp_str_to, p_fp_str_from)              \
    do {                                                                                 \
        ASMCMP_COMMON_ALLOC_MEM_FOR_STR(p_fp_str_to, strlen(p_fp_str_from));             \
        strcpy(p_fp_str_to, p_fp_str_from);                                              \
    } while(0)

/*
 * Macro deallocates earlier allocated memory
 *
 * @param1:
 * 'pointer' has any pointer type
 */
#define ASMCMP_COMMON_RELEASE_MEM(pointer)      \
    do {                                        \
        if (pointer)                            \
        {                                       \
            free(pointer);                      \
            pointer = NULL;                     \
        }                                       \
    } while(0)

#endif