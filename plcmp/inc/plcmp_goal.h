/* encoding: UTF-8 */

#ifndef PLCMP_GOAL_H
#define PLCMP_GOAL_H

#include "plcmp_common.h"
#include "plcmp_utils.h"
#include "plcmp_tables.h"

/* Structure for the stack of goals */
typedef struct goal_interim_s {
    sym_title_t sym_title;
    index_t src_text_left_ind;
    tb_rules_ind_t tb_rules_ind;
} goal_interim_t;

/* */
typedef struct goals_interim_stack_s {
    unsigned int count;
    goal_interim_t *stack;
} goals_interim_stack_t;

/* Structure for the stack of goals achieved */
typedef struct goal_achieved_s {
    sym_title_t sym_title;
    index_t src_text_beg_ind;
    tb_rules_ind_t tb_rules_ind;
    index_t src_text_end_ind;
    tb_rules_ind_t tb_rules_next_goal_ind; 
} goal_achieved_t;

/* */
typedef struct goals_achieved_stack_s {
    unsigned int count;
    goal_achieved_t *stack;
} goals_achieved_stack_t;

/* Subroutine creates stack for goals achieved for using later
 * by syntax analyzer (parser) and semantic calculator */
static inline void plcmp_goal_create_goals_achieved_stack(
    goals_achieved_stack_t *goals_achieved)
{
    PLCMP_UTILS_ASSERT(NULL != goals_achieved);
    goals_achieved->count = 0;
    PLCMP_UTILS_CALLOC_MEM(goals_achieved->stack,
                           NDST,
                           sizeof(goal_achieved_t));
}

/* Subroutine destroys stack of goals achieved */
static inline void plcmp_goal_destroy_goals_achieved_stack(
    goals_achieved_stack_t *goals_achieved)
{
    PLCMP_UTILS_ASSERT(NULL != goals_achieved);
    goals_achieved->count = 0;
    PLCMP_UTILS_RELEASE_MEM(goals_achieved->stack);
}

/* Subroutine creates stack for goals for using 
 * later by syntax analyzer (parser) */
static inline void plcmp_goal_create_goals_interim_stack(
    goals_interim_stack_t *goals)
{
    PLCMP_UTILS_ASSERT(NULL != goals);
    goals->count = 0;
    PLCMP_UTILS_CALLOC_MEM(goals->stack,
                           NCEL,
                           sizeof(goal_interim_t));
}

/* Subroutine destroys stack of goals */
static inline void plcmp_goal_destroy_goals_interim_stack(
    goals_interim_stack_t *goals)
{
    PLCMP_UTILS_ASSERT(NULL != goals);
    goals->count = 0;
    PLCMP_UTILS_RELEASE_MEM(goals->stack);
}

/* Subroutine adds a new goal into stack of goals */
void plcmp_goal_add_interim(goals_interim_stack_t *intr_goals,
                            char const *goal_title,
                            index_t src_text_left_ind,
                            tb_rules_ind_t tb_rules_ind);

/* Subroutine removes last goal from the stack of goals */
void plcmp_goal_remove_last_interim(goals_interim_stack_t *intr_goals);

/* Subroutine adds a goal achieved into stack of goals achieved */
void plcmp_goal_add_achieved(goals_achieved_stack_t *p_goals_achieved,
                             char const *goal_achieved_title,
                             index_t src_text_beg_ind,
                             tb_rules_ind_t tb_rules_ind,
                             index_t src_text_end_ind,
                             tb_rules_ind_t tb_rules_next_goal_ind);

/* Subroutine removes last goal achieved from the stack of goals achieved */
void plcmp_goal_remove_last_achieved(goals_achieved_stack_t *p_goals_achieved);

#endif /* PLCMP_GOAL_H */