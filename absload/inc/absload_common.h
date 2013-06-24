/* encoding: UTF-8 */
/* russian letters: KOI8 (Cyrillic) */

#ifndef ABSLOAD_COMMON_H
#define ABSLOAD_COMMON_H

#define NSPIS 5                                 /*разм.списка загр.прогр. */
#define NOBJ 50                                /*разм.масс.об'ектных карт*/
#define DOBLZ 1024                              /*длина области загрузки  */
#define NOP 6                                    /*кол-во обрабатываемых   */
                                                  /* команд                 */

#define TRUE 1
#define FALSE 0

#define ABSLOAD_COMMON_ASSERT(condition) assert(condition)

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
#define ABSLOAD_COMMON_MALLOC_MEM_FOR_STR(p_str_for, str_length)                \
    do {                                                                        \
        size_t __str_length = (str_length);                                     \
                                                                                \
        p_str_for = malloc(sizeof(char)*(__str_length + 1));                    \
        if (NULL == p_str_for)                                                  \
        {                                                                       \
            printf("Error of allocating memory for new string. Assert\n");      \
            ABSLOAD_COMMON_ASSERT(0);                                           \
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
#define ABSLOAD_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_fp_str_to, p_fp_str_from)              \
    do {                                                                                  \
        ABSLOAD_COMMON_MALLOC_MEM_FOR_STR(p_fp_str_to, strlen(p_fp_str_from));            \
        strcpy(p_fp_str_to, p_fp_str_from);                                               \
    } while(0)

/*
 * Macro deallocates earlier allocated memory
 *
 * @param1:
 * 'pointer' has any pointer type
 */
#define ABSLOAD_COMMON_RELEASE_MEM(pointer)     \
    do {                                        \
        if (pointer)                            \
        {                                       \
            free(pointer);                      \
            pointer = NULL;                     \
        }                                       \
    } while(0)

#endif
