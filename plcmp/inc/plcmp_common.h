#ifndef PLCMP_COMMON_H
#define PLCMP_COMMON_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* The limited size of the source text */
#define MAXNISXTXT 50
/* The limited size of the table of the syntax rules */
#define NSINT 201
/* The limited size of the stack of the goals */
#define NCEL 20
/* The limited size of the stack of the achivements */
#define NDST 500
/* The limited size of the table of inputs */
#define NVXOD 53
/* The limited size of the string of compact text */
#define NSTROKA 200
/* The limited size of the list of non-terminals */
#define NNETRM 16
/* The limited size of the output text */
#define MAXLTXT 50
/* The limited size of formatted interpreted fragment of the source text */
#define MAXFORMT 30
/* The limited size of the tables of names and labels */
#define NSYM 100
/* The limited length of the line of the source text */
#define LINELEN 80



#define TRUE 1
#define FALSE 0


#define PLCMP_COMMON_ASSERT(condition) assert(condition)


/*
 * Macro allocates memory for new 'p_str_for' string with 'str_length' length
 *
 * @param1:
 * 'p_str_for' has type 'char'
 * It has to be 'NULL'-pointer
 *
 * @param2:
 * 'str_length' has type 'size_t'
 */
#define PLCMP_COMMON_ALLOC_MEM_FOR_STR(p_str_for, str_length)                   \
    do {                                                                        \
        size_t __str_length = (str_length);                                     \
                                                                                \
        p_str_for = malloc(sizeof(char)*(__str_length + 1));                    \
        if (NULL == p_str_for)                                                  \
        {                                                                       \
            printf("Error of allocating memory for new string. Assert\n");      \
            PLCMP_COMMON_ASSERT(0);                                             \
        }                                                                       \
        p_str_for[__str_length] = '\0';                                         \
    } while(0)


/*
 * Macro deallocates earlier allocated memory
 *
 * @param1:
 * 'pointer' has any pointer type except 'void*' or 'void const *'
 */
#define PLCMP_COMMON_RELEASE_MEM(pointer)       \
    do {                                        \
        if (pointer)                            \
        {                                       \
            free(pointer);                      \
        }                                       \
        pointer = NULL;                         \
    } while(0)

/* Cycles counters */
extern int I4;

extern int I; /* current index in compact text */
extern int J; /* current index in the table of grammar rules */
extern int K; /* current index in the stack of goals */
extern int L; /* current index in the stack of achieved goals */


#endif /* PLCMP_COMMON_H */