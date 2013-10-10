/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_goal.h"

/* Subroutine adds a new goal into stack of goals */
void plcmp_goal_add_interim(goals_interim_stack_t *p_goals,
                            char const *goal_title,
                            int src_text_begin_index,
                            int src_text_end_index)
{
    unsigned int *restrict count = &p_goals->count;
    strcpy(p_goals->stack[*count].sym_title, goal_title);
    p_goals->stack[*count].CEL2 = src_text_begin_index;
    p_goals->stack[*count].CEL3 = src_text_end_index;
    ++(*count);
}

/* Subroutine removes last goal from the stack of goals */
void plcmp_goal_remove_last_interim(goals_interim_stack_t *p_goals)
{
    unsigned int *restrict count = &p_goals->count;
    memset(&p_goals->stack[*count],
           0,
           sizeof(p_goals->stack[*count]));
    --(*count);
}

/* Subroutine adds a goal achieved into stack of goals achieved */
void plcmp_goal_add_achieved(goals_achieved_stack_t *p_goals_achieved,
                             char const *goal_achieved_title,
                             int src_text_begin_index,
                             int sint_index,
                             int src_text_end_index,
                             int next_goal_sint_index)
{
    unsigned int *restrict count = &p_goals_achieved->count;
    strcpy(p_goals_achieved->stack[*count].sym_title, goal_achieved_title);
    p_goals_achieved->stack[*count].DST2 = src_text_begin_index;
    p_goals_achieved->stack[*count].DST3 = sint_index;
    p_goals_achieved->stack[*count].DST4 = src_text_end_index;
    p_goals_achieved->stack[*count].DST5 = next_goal_sint_index;
    ++(*count);
}

/* Subroutine removes last goal achieved from the stack of goals achieved */
void plcmp_goal_remove_last_achieved(goals_achieved_stack_t *p_goals_achieved)
{
    unsigned int *restrict count = &p_goals_achieved->count;
    memset(&p_goals_achieved->stack[*count],
           0,
           sizeof(p_goals_achieved->stack[*count]));
    --(*count);
}