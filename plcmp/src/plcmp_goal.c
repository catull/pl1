/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_goal.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

void plcmp_goal_add_interim(goals_interim_stack_t *interim_goals,
                            sym_t sym,
                            index_t src_text_left_ind,
                            index_t rules_saved_ind)
{
    interim_goals->last = &interim_goals->stack[interim_goals->count];
    interim_goals->last->sym = sym;
    interim_goals->last->src_text_left_ind = src_text_left_ind;
    interim_goals->last->rules_saved_ind = rules_saved_ind;
    ++(interim_goals->count);
}

void plcmp_goal_remove_last_interim(
    goals_interim_stack_t *interim_goals)
{
    if (interim_goals->count)
    {
        memset(interim_goals->last, 0, sizeof(*interim_goals->last));
        --interim_goals->count;
        interim_goals->last = interim_goals->count ? 
            &interim_goals->stack[interim_goals->count - 1] : NULL;
    }
    else
    {
        PLCMP_UTILS_ASSERT_MSG(
            0,
            "Trying to remove last interim goal from the "
            "empty stack of interim goals");
    }
}

void plcmp_goal_add_achieved(goals_achieved_stack_t *goals_achieved,
                             sym_t sym,
                             index_t src_text_beg_ind,
                             index_t rules_saved_ind,
                             index_t src_text_end_ind,
                             index_t rules_reach_goal_ind)
{
    goals_achieved->last = &goals_achieved->stack[goals_achieved->count];
    goals_achieved->last->sym = sym;
    goals_achieved->last->src_text_beg_ind = src_text_beg_ind;
    goals_achieved->last->rules_saved_ind = rules_saved_ind;
    goals_achieved->last->src_text_end_ind = src_text_end_ind;
    goals_achieved->last->rules_reach_goal_ind = rules_reach_goal_ind;
    ++(goals_achieved->count);
}

void plcmp_goal_remove_last_achieved(
    goals_achieved_stack_t *goals_achieved)
{
    if (goals_achieved->count)
    {

        memset(goals_achieved->last, 0, sizeof(*goals_achieved->last));
        --goals_achieved->count;
        goals_achieved->last = goals_achieved->count ? 
            &goals_achieved->stack[goals_achieved->count - 1] : NULL;
    }
    else
    {
        PLCMP_UTILS_ASSERT_MSG(
            0,
            "Trying to remove last achieved goal from the "
            "empty stack of achieved goals");
    }
}