/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_goal.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

/* Subroutine adds a new goal into stack of goals */
void plcmp_goal_add_interim(goals_interim_stack_t *intr_goals,
                            char const *goal_title,
                            index_t src_text_left_ind,
                            tb_rules_ind_t tb_rules_ind)
{
    unsigned int *restrict count = &intr_goals->count;
    strcpy(intr_goals->stack[*count].sym_title, goal_title);
    intr_goals->stack[*count].src_text_left_ind = src_text_left_ind;
    intr_goals->stack[*count].tb_rules_ind = tb_rules_ind;
    intr_goals->last = &intr_goals->stack[*count];
    ++(*count);
}

/* Subroutine removes last goal from the stack of goals */
void plcmp_goal_remove_last_interim(goals_interim_stack_t *intr_goals)
{
    if (intr_goals->count)
    {
        unsigned int *restrict count = &intr_goals->count;
        memset(&intr_goals->stack[*count - 1],
               0,
               sizeof(intr_goals->stack[*count - 1]));
        --(*count);
        intr_goals->last = (*count) ? &intr_goals->stack[*count - 1] : NULL;
    }
    else
    {
        printf("Trying to remove last interim goal from the "
               "empty stack of interim goals. Assert\n");
        PLCMP_UTILS_ASSERT(0);
    }
}

/* Subroutine adds a goal achieved into stack of goals achieved */
void plcmp_goal_add_achieved(goals_achieved_stack_t *p_goals_achieved,
                             char const *goal_achieved_title,
                             index_t src_text_beg_ind,
                             tb_rules_ind_t tb_rules_ind,
                             index_t src_text_end_ind,
                             tb_rules_ind_t tb_rules_next_goal_ind)
{
    unsigned int *restrict count = &p_goals_achieved->count;
    strcpy(p_goals_achieved->stack[*count].sym_title, goal_achieved_title);
    p_goals_achieved->stack[*count].src_text_beg_ind = src_text_beg_ind;
    p_goals_achieved->stack[*count].tb_rules_ind = tb_rules_ind;
    p_goals_achieved->stack[*count].src_text_end_ind = src_text_end_ind;
    p_goals_achieved->stack[*count].tb_rules_next_goal_ind = 
        tb_rules_next_goal_ind;
    p_goals_achieved->last = &p_goals_achieved->stack[*count];
    ++(*count);
}

/* Subroutine removes last goal achieved from the stack of goals achieved */
void plcmp_goal_remove_last_achieved(goals_achieved_stack_t *p_goals_achieved)
{
    if (p_goals_achieved->count)
    {
        unsigned int *restrict count = &p_goals_achieved->count;
        memset(&p_goals_achieved->stack[*count - 1],
               0,
               sizeof(p_goals_achieved->stack[*count - 1]));
        p_goals_achieved->last =
            (*count) ? &p_goals_achieved->stack[*count - 1] : NULL;
        --(*count);
    }
    else
    {
        printf("Trying to remove last achieved goal from the "
               "empty stack of achieved goals. Assert\n");
        PLCMP_UTILS_ASSERT(0);
    }
}