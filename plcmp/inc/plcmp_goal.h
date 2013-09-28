/* encoding: UTF-8 */

#ifndef PLCMP_GOAL_H
#define PLCMP_GOAL_H

#include "plcmp_common.h"

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

/* Subroutine creates stack for goals achieved for using later
 * by syntax analyzer (parser) and semantic calculator */
static inline void plcmp_goal_create_goals_achieved_stack(dst_t *goals_achieved)
{
    PLCMP_COMMON_ASSERT(NULL != goals_achieved);
    goals_achieved->count = 0;
    PLCMP_COMMON_CALLOC_MEM(goals_achieved->p_dst_stack,
                            NDST,
                            sizeof(goals_achieved_stack_t));
}

/* Subroutine destroys stack of goals achieved */
static inline void plcmp_goal_destroy_goals_achieved_stack(
    dst_t *goals_achieved)
{
    PLCMP_COMMON_ASSERT(NULL != goals_achieved);
    goals_achieved->count = 0;
    PLCMP_COMMON_RELEASE_MEM(goals_achieved->p_dst_stack);
}

/* Subroutine creates stack for goals for using 
 * later by syntax analyzer (parser) */
static inline void plcmp_goal_create_goals_stack(cel_t *goals)
{
    PLCMP_COMMON_ASSERT(NULL != goals);
    goals->count = 0;
    PLCMP_COMMON_CALLOC_MEM(goals->p_cel_stack,
                            NCEL,
                            sizeof(goals_stack_t));
}

/* Subroutine destroys stack of goals */
static inline void plcmp_goal_destroy_goals_stack(cel_t *goals)
{
    PLCMP_COMMON_ASSERT(NULL != goals);
    goals->count = 0;
    PLCMP_COMMON_RELEASE_MEM(goals->p_cel_stack);
}

void plcmp_goal_add(cel_t *p_goals,
                    char const *goal_name,
                    int src_text_begin_index,
                    int src_text_end_index);

void plcmp_goal_remove_last(cel_t *p_goals);

void plcmp_goal_add_achieved(dst_t *p_goals_achieved,
                             char const *goal_achieved_name,
                             int src_text_begin_index,
                             int sint_index,
                             int src_text_end_index,
                             int next_goal_sint_index);

void plcmp_goal_remove_last_achieved(dst_t *p_goals_achieved);

#endif /* PLCMP_GOAL_H */