/* encoding: UTF-8 */

#ifndef PLCMP_GOAL_H
#define PLCMP_GOAL_H

#include "plcmp_common.h"
#include "plcmp_utils.h"
#include "plcmp_tables.h"

/* Structure for the stack of goals */
typedef struct goal_interim_s {
    sym_t sym;
    index_t src_text_left_ind;
    index_t rules_saved_ind;
} goal_interim_t;

/* */
typedef struct goals_interim_stack_s {
    unsigned int count;
    goal_interim_t *last;
    goal_interim_t stack[];
} goals_interim_stack_t;

/* Structure for the stack of goals achieved */
typedef struct goal_achieved_s {
    sym_t sym;
    index_t src_text_beg_ind;
    index_t rules_saved_ind;
    index_t src_text_end_ind;
    index_t rules_reach_goal_ind; 
} goal_achieved_t;

/* */
typedef struct goals_achieved_stack_s {
    unsigned int count;
    goal_achieved_t *last;
    goal_achieved_t stack[];
} goals_achieved_stack_t;

/* Subroutine creates stack for goals achieved for using later
 * by syntax analyzer (parser) and semantic calculator */
static inline goals_achieved_stack_t*
    plcmp_goal_create_goals_achieved_stack(void)
{
    goals_achieved_stack_t *goals_achieved = NULL;
    return PLCMP_UTILS_CALLOC_MEM(
        goals_achieved,
        1,
        sizeof(goals_achieved_stack_t) + NDST * sizeof(goal_achieved_t));
}

/* Subroutine destroys stack of goals achieved */
static inline void plcmp_goal_destroy_goals_achieved_stack(
    goals_achieved_stack_t **goals_achieved)
{
    PLCMP_UTILS_ASSERT(NULL != goals_achieved && NULL != *goals_achieved);
    PLCMP_UTILS_RELEASE_MEM(*goals_achieved);
}

/* Subroutine creates stack for goals for using 
 * later by syntax analyzer (parser) */
static inline goals_interim_stack_t*
    plcmp_goal_create_goals_interim_stack(void)
{
    goals_interim_stack_t *goals = NULL;
    return PLCMP_UTILS_CALLOC_MEM(
        goals,
        1,
        sizeof(goals_interim_stack_t) + NCEL * sizeof(goal_interim_t));
}

/* Subroutine destroys stack of goals */
static inline void plcmp_goal_destroy_goals_interim_stack(
    goals_interim_stack_t **goals)
{
    PLCMP_UTILS_ASSERT(NULL != goals && NULL != *goals);
    PLCMP_UTILS_RELEASE_MEM(*goals);
}

/* Subroutine adds a new goal into stack of goals */
void plcmp_goal_add_interim(goals_interim_stack_t *interim_goals,
                            sym_t sym,
                            index_t src_text_left_ind,
                            index_t rules_saved_ind);

/* Subroutine removes last goal from the stack of goals */
void plcmp_goal_remove_last_interim(
    goals_interim_stack_t *interim_goals);

/* Subroutine adds a goal achieved into stack of goals achieved */
void plcmp_goal_add_achieved(goals_achieved_stack_t *goals_achieved,
                             sym_t sym,
                             index_t src_text_beg_ind,
                             index_t rules_saved_ind,
                             index_t src_text_end_ind,
                             index_t rules_reach_goal_ind);

/* Subroutine removes last goal achieved from the stack of goals achieved */
void plcmp_goal_remove_last_achieved(
    goals_achieved_stack_t *goals_achieved);

#endif /* PLCMP_GOAL_H */