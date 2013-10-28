/* encoding: UTF-8 */

#ifndef PLCMP_UTILS_H
#define PLCMP_UTILS_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Check equality of two strings */
int utils_streq(char const *restrict str_1, char const *restrict str_2);

/* Subroutine concatenates strings will be passed trought the parameters and
 * puts result to 'str' destination string
 * The last parameter must be a NULL-pointer */
char* utils_strcats(char *restrict str, ...);

/* Subroutine swaps bytes in input data by 'p_src' pointer 
 * and puts result into data by 'p_dst' pointer 
 * Number of swapped bytes is 'count_bytes' 
 */
void utils_swap_bytes(uint8_t *p_src, uint8_t *p_dst, size_t count_bytes);

char* utils_copy_file_path_and_change_extension(char const *fp,
                                                char const *new_ext);

#define UTILS_ASSERT(condition) assert((condition))

#define UTILS_ASSERT_MSG(condition, format, ...)                               \
    do {                                                                       \
        if (!(condition))                                                      \
        {                                                                      \
            fprintf(stderr, format "\n", ##__VA_ARGS__);                       \
            UTILS_ASSERT((condition));                                         \
        }                                                                      \
    } while (0)

#define UTILS_MALLOC_MEM(size)                                                 \
    ({                                                                         \
        void *pointer = malloc((size));                                        \
        if (NULL == pointer)                                                   \
        {                                                                      \
            UTILS_ASSERT_MSG(                                                  \
                0,                                                             \
                "Error of allocating memory with size = %lu bytes",            \
                (size));                                                       \
        }                                                                      \
        pointer;                                                               \
    })

#define UTILS_CALLOC_MEM(number_of_elements, size_of_element)                  \
    ({                                                                         \
        void *pointer = calloc((number_of_elements), (size_of_element));       \
        if (NULL == (pointer))                                                 \
        {                                                                      \
            UTILS_ASSERT_MSG(                                                  \
                0,                                                             \
                "Error of clear allocating memory with "                       \
                "number_of_elements = %i size_of_element = %lu bytes",         \
                (number_of_elements),                                          \
                (size_of_element));                                            \
        }                                                                      \
        pointer;                                                               \
    })

#define UTILS_RELEASE_MEM(pointer)                                             \
    do {                                                                       \
        if ((pointer))                                                         \
        {                                                                      \
            free((pointer));                                                   \
            (pointer) = NULL;                                                  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            UTILS_ASSERT_MSG(                                                  \
                0,                                                             \
                "Trying release NULL-pointer: '" #pointer "'");                \
        }                                                                      \
    } while(0)

#define ARRAY_SIZE(array) sizeof((array)) / sizeof((array)[0])

#endif /* PLCMP_UTILS_H */
