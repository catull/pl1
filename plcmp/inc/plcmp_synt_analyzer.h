/* encoding: UTF-8 */

#ifndef PLCMP_SYNT_ANALYZER_H
#define PLCMP_SYNT_ANALYZER_H

#include "plcmp_common.h"

#define PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN 20

typedef enum plcmp_synt_analyzer_error_code_e {
    PLCMP_SYNT_ANALYZER_SUCCESS = 0,
    PLCMP_SYNT_ANALYZER_FAILURE = 1
} plcmp_synt_analyzer_error_code_t;

typedef struct plcmp_synt_analyzer_error_data_s {
    plcmp_synt_analyzer_error_code_t err_code;
    char src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN + 1];
} plcmp_synt_analyzer_error_data_t;


/* 
 * Macro creates stack for goals for using later by syntax analyzer (parser)
 * 
 * @param1:
 * 'goals' has type 'cel_t'
 * Its field 'cel_stack' has to be NULL-pointer
 *
 */
#define PLCMP_SYNT_ANALYZER_CREATE_GOALS_STACK(goals)                           \
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
 * Macro destroys stack of goals 
 * 
 * @param1:
 * 'goals' has type 'cel_t'
 * Its field 'cel_stack' hasn't to be 'NULL'-pointer
 *
 */
#define PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals)              \
    do {                                                            \
        goals.count = 0;                                            \
                                                                    \
        PLCMP_COMMON_RELEASE_MEM(goals.cel_stack);                  \
    } while(0)


struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analyzer(dst_t *p_goals_achieved);

char* plcmp_synt_analyzer_errmsg_by_errdata(plcmp_synt_analyzer_error_data_t const *err_data, char *errmsg);

#endif /* PLCMP_SYNT_ANALYZER_H */