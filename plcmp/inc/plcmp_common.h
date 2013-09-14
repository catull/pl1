/* encoding: UTF-8 */

#ifndef PLCMP_COMMON_H
#define PLCMP_COMMON_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* The limited size of the source text */
#define MAXNISXTXT 50
/* The limited size of the table of the syntax rules */
#define NSINT 233
/* The limited size of the stack of the goals */
#define NCEL 20
/* The limited size of the stack of the goals achieved */
#define NDST 500
/* The limited size of the table of inputs */
#define NVXOD 56
/* The limited size of the string of compact text */
#define NSTROKA 200
/* The limited size of the list of non-terminals */
#define NNETRM 18
/* The limited size of the output text */
#define MAXLTXT 50
/* The limited size of formatted interpreted fragment of the source text */
#define MAXFORMT 30
/* The limited size of the tables of names and labels */
#define NSYM 100
/* The limited length of the line of 
 * the source text (80 symbols +1 symbol for endline)*/
#define LINELEN 81

#define TRUE 1
#define FALSE 0


/* Structure for the stack of goals */
typedef struct goals_stack_s {
    char CEL1[4];
    int CEL2;
    int CEL3;
} goals_stack_t;

/* */
typedef struct cel_s {
    unsigned int count;
    goals_stack_t *p_cel_stack;
} cel_t;

/* Structure for the stack of goals achieved */
typedef struct goals_achieved_stack_s {
    char DST1[4];
    int DST2;
    int DST3;
    int DST4;
    int DST5; 
} goals_achieved_stack_t;

/* */
typedef struct dst_s {
    unsigned int count;
    goals_achieved_stack_t *p_dst_stack;
} dst_t;


#define PLCMP_COMMON_ASSERT(condition) assert(condition)

/*
 * Macro allocates memory
 *
 * @param1:
 * 'pointer' has any pointer type
 */
#define PLCMP_COMMON_MALLOCATE_MEM(pointer, size)                              \
    do {                                                                       \
        (pointer) = malloc(size);                                              \
        if (NULL == (pointer))                                                 \
        {                                                                      \
            printf("Error of allocating memory with size = %lu"                \
                   "bytes for " #pointer ". Assert\n", size);                  \
            PLCMP_COMMON_ASSERT(0);                                            \
        }                                                                      \
    } while(0)

/*
 * Macro allocates memory and sets to zero
 *
 * @param1:
 * 'pointer' has any pointer type
 * @param2:
 * 'number_of_elements' has 'size_t' type
 * @param3:
 * 'size_of_element' has 'size_t' type
 */
#define PLCMP_COMMON_CALLOCATE_MEM(pointer,                                    \
                                   number_of_elements,                         \
                                   size_of_element)                            \
    do {                                                                       \
        (pointer) = calloc((number_of_elements), (size_of_element));           \
        if (NULL == (pointer))                                                 \
        {                                                                      \
            printf("Error of clear allocating memory with "                    \
                   "number_of_elements = %i size_of_element = %lu"             \
                   "bytes for " #pointer ". Assert\n",                         \
                   (number_of_elements),                                       \
                   (size_of_element));                                         \
            PLCMP_COMMON_ASSERT(0);                                            \
        }                                                                      \
    } while(0)


/*
 * Macro deallocates earlier allocated memory
 *
 * @param1:
 * 'pointer' has any pointer type
 *
 * @param2:
 * 'what' has 'char*' type
 */
#define PLCMP_COMMON_RELEASE_MEM(pointer)                                      \
    do {                                                                       \
        if (pointer)                                                           \
        {                                                                      \
            free(pointer);                                                     \
            (pointer) = NULL;                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            printf("Try to release NULL-pointer: " #pointer ". Assert\n");     \
        }                                                                      \
    } while(0)

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
#define PLCMP_COMMON_ALLOC_MEM_FOR_STR(p_str_for, str_length)                  \
    do {                                                                       \
        PLCMP_COMMON_MALLOCATE_MEM(p_str_for,                                  \
                                   sizeof(char) * ((str_length) + 1));         \
        p_str_for[(str_length)] = '\0';                                        \
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
#define PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_fp_str_to, p_fp_str_from)     \
    do {                                                                       \
        PLCMP_COMMON_ALLOC_MEM_FOR_STR(p_fp_str_to, strlen(p_fp_str_from));    \
        strcpy(p_fp_str_to, p_fp_str_from);                                    \
    } while(0)

#endif /* PLCMP_COMMON_H */

int streq(char const *str_1, char const *str_2);
