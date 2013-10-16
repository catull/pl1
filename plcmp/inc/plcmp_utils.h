/* encoding: UTF-8 */

#ifndef PLCMP_UTILS_H
#define PLCMP_UTILS_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLCMP_UTILS_ASSERT(condition) assert((condition))

#define PLCMP_UTILS_ASSERT_MSG(condition, format, ...)                         \
    do {                                                                       \
        if (!(condition))                                                      \
        {                                                                      \
            fprintf(stderr, format "\n", ##__VA_ARGS__);                       \
            PLCMP_UTILS_ASSERT((condition));                                   \
        }                                                                      \
    } while (0)

#define PLCMP_UTILS_MALLOC_MEM(pointer, size)                                  \
    ({                                                                         \
        (pointer) = malloc((size));                                            \
        if (NULL == (pointer))                                                 \
        {                                                                      \
            PLCMP_UTILS_ASSERT_MSG(                                            \
                0,                                                             \
                "Error of allocating memory with size = %lu "                  \
                "bytes for '" #pointer "' pointer",                            \
                (size));                                                       \
        }                                                                      \
        (pointer);                                                             \
    })

#define PLCMP_UTILS_CALLOC_MEM(pointer,                                        \
                               number_of_elements,                             \
                               size_of_element)                                \
    ({                                                                         \
        (pointer) = calloc((number_of_elements), (size_of_element));           \
        if (NULL == (pointer))                                                 \
        {                                                                      \
            PLCMP_UTILS_ASSERT_MSG(                                            \
                0,                                                             \
                "Error of clear allocating memory with "                       \
                "number_of_elements = %i size_of_element = %lu "               \
                "bytes for '" #pointer "' pointer",                            \
                (number_of_elements),                                          \
                (size_of_element));                                            \
        }                                                                      \
        (pointer);                                                             \
    })

#define PLCMP_UTILS_RELEASE_MEM(pointer)                                       \
    do {                                                                       \
        if ((pointer))                                                         \
        {                                                                      \
            free((pointer));                                                   \
            (pointer) = NULL;                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            PLCMP_UTILS_ASSERT_MSG(                                            \
                0,                                                             \
                "Trying release NULL-pointer: '" #pointer "'");                \
        }                                                                      \
    } while(0)

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
#define PLCMP_UTILS_MAKE_ASM_FILE_PATH_BY_PL1_FILE_PATH(p_asm_fp_name,         \
                                                        p_pl1_fp_name)         \
    do {                                                                       \
        PLCMP_UTILS_ASSERT(p_pl1_fp_name);                                     \
        p_asm_fp_name = strdup((p_pl1_fp_name));                               \
        p_asm_fp_name[strlen((p_asm_fp_name)) - 4] = '\0';                     \
        strcat(p_asm_fp_name, ".ass");                                         \
    } while (0)

#define ARRAY_SIZE(array) sizeof((array)) / sizeof((array)[0])

/* Check equality of two strings */
int streq(char const *restrict str_1, char const *restrict str_2);

/* Subroutine concatenates strings will be passed trought the parameters and
 * puts result to 'str' destination string
 * The last parameter must be a NULL-pointer */
char* strcats(char *restrict str, ...);

#endif /* PLCMP_UTILS_H */
