/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_goal.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

/* Subroutine adds a new goal into stack of goals */
void plcmp_goal_add_interim(goals_interim_stack_t *intr_goals,
                            char const *goal_title,
                            index_t src_text_left_ind,
                            tb_rules_ind_t tb_rules_reach_ind)
{
    intr_goals->last = &intr_goals->stack[intr_goals->count];
    strcpy(intr_goals->last->sym_title, goal_title);
    intr_goals->last->src_text_left_ind = src_text_left_ind;
    intr_goals->last->tb_rules_reach_ind = tb_rules_reach_ind;
    ++intr_goals->count;
}

/* Subroutine removes last goal from the stack of goals */
void plcmp_goal_remove_last_interim(goals_interim_stack_t *restrict intr_goals)
{
    if (intr_goals->count)
    {
        memset(intr_goals->last, 0, sizeof(*intr_goals->last));
        --intr_goals->count;
        intr_goals->last = intr_goals->count ? 
            &intr_goals->stack[intr_goals->count - 1] : NULL;
    }
    else
    {
        PLCMP_UTILS_ASSERT(
            0,
            "Trying to remove last interim goal from the "
            "empty stack of interim goals");
    }
}

/* Subroutine adds a goal achieved into stack of goals achieved */
void plcmp_goal_add_achieved(goals_achieved_stack_t *p_goals_achieved,
                             char const *goal_achieved_title,
                             index_t src_text_beg_ind,
                             tb_rules_ind_t tb_rules_reach_ind,
                             index_t src_text_end_ind,
                             tb_rules_ind_t tb_rules_next_goal_ind)
{
    p_goals_achieved->last = &p_goals_achieved->stack[p_goals_achieved->count];
    strcpy(p_goals_achieved->last->sym_title, goal_achieved_title);
    p_goals_achieved->last->src_text_beg_ind = src_text_beg_ind;
    p_goals_achieved->last->tb_rules_reach_ind = tb_rules_reach_ind;
    p_goals_achieved->last->src_text_end_ind = src_text_end_ind;
    p_goals_achieved->last->tb_rules_next_goal_ind = tb_rules_next_goal_ind;
    ++p_goals_achieved->count;
}

/* Subroutine removes last goal achieved from the stack of goals achieved */
void plcmp_goal_remove_last_achieved(goals_achieved_stack_t *p_goals_achieved)
{
    if (p_goals_achieved->count)
    {

        memset(p_goals_achieved->last, 0, sizeof(*p_goals_achieved->last));
        --p_goals_achieved->count;
        p_goals_achieved->last = p_goals_achieved->count ? 
            &p_goals_achieved->stack[p_goals_achieved->count - 1] : NULL;
    }
    else
    {
        PLCMP_UTILS_ASSERT(
            0,
            "Trying to remove last achieved goal from the "
            "empty stack of achieved goals");
    }
}