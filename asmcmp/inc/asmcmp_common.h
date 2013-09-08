/* encoding: UTF-8 */

#ifndef ASMCMP_COMMON_H
#define ASMCMP_COMMON_H

#include <assert.h>
#include <stdint.h>

#define ASMTEXT_MAX_LEN 50                      /* Max row-length of the source assembler text */ 
#define OBJTEXT_MAX_LEN ASMTEXT_MAX_LEN         /* Length of the object text */
#define NSYM 10                                 /* Size of the table of symbols */
#define NPOP 6                                  /* Size of the table of pseudo operations */
#define NOP 8                                   /* Size of the table of machine operations */

/* The limited length of the line of the assembler source text 
 * It has to be 81 (80 + 1) because 'fread' function reads 
 * newline-symbol too in the end of the each line of assembler code read */
#define LINELEN 81

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
#define ASMCMP_COMMON_MALLOC_MEM_FOR_STR(p_str_for, str_length)                             \
    do {                                                                                    \
        size_t __str_length = (str_length);                                                 \
                                                                                            \
        p_str_for = malloc(sizeof(char)*(__str_length + 1));                                \
        if (NULL == p_str_for)                                                              \
        {                                                                                   \
            printf("Error of allocating memory for new string. Assert\n");                  \
            ASMCMP_COMMON_ASSERT(0);                                                        \
        }                                                                                   \
        p_str_for[__str_length] = '\0';                                                     \
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
#define ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_fp_str_to, p_fp_str_from)                 \
    do {                                                                                    \
        ASMCMP_COMMON_MALLOC_MEM_FOR_STR(p_fp_str_to, strlen(p_fp_str_from));               \
        strcpy(p_fp_str_to, p_fp_str_from);                                                 \
    } while(0)

/*
 * Macro deallocates earlier allocated memory
 *
 * @param1:
 * 'pointer' has any pointer type
 */
#define ASMCMP_COMMON_RELEASE_MEM(pointer)                                                  \
    do {                                                                                    \
        if (pointer)                                                                        \
        {                                                                                   \
            free(pointer);                                                                  \
            pointer = NULL;                                                                 \
        }                                                                                   \
    } while(0)


int streq(char const *str_1, char const *str_2);

/* Subroutine swaps bytes in input data by 'p_src' pointer 
 * and puts result into data by 'p_dst' pointer 
 * Number of swapped bytes is 'count_bytes' 
 */
void asmcmp_common_swap_bytes(uint8_t *p_src, uint8_t *p_dst, size_t count_bytes);

#endif /* ASMCMP_COMMON_H */
