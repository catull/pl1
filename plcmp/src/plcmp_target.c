/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_tables.h"
#include "plcmp_target.h"
#include "plcmp_utils.h"

target_interim_t plcmp_target_add_interim(
    targets_interim_stack_t *targets_interim,
    sym_t sym,
    index_t src_text_left_ind,
    index_t rules_saved_ind)
{
    targets_interim->last = &targets_interim->stack[targets_interim->count];
    targets_interim->last->sym = sym;
    targets_interim->last->src_text_left_ind = src_text_left_ind;
    targets_interim->last->rules_saved_ind = rules_saved_ind;
    ++(targets_interim->count);
    return *(targets_interim->last);
}

target_interim_t plcmp_target_remove_last_interim(
    targets_interim_stack_t *targets_interim)
{
    if (targets_interim->count)
    {
        target_interim_t goal = *(targets_interim->last);
        memset(targets_interim->last, 0, sizeof(*targets_interim->last));
        --targets_interim->count;
        targets_interim->last = targets_interim->count ? 
            &targets_interim->stack[targets_interim->count - 1] : NULL;
        return goal;
    }
    else
    {
        PLCMP_UTILS_ASSERT_MSG(
            0,
            "Trying to remove last interim goal from the "
            "empty stack of interim goals");
    }
}

target_achieved_t plcmp_target_add_achieved(
    targets_achieved_stack_t *targets_achieved,
    sym_t sym,
    index_t src_text_beg_ind,
    index_t rules_saved_ind,
    index_t src_text_end_ind,
    index_t rules_reach_target_ind)
{
    targets_achieved->last = &targets_achieved->stack[targets_achieved->count];
    targets_achieved->last->sym = sym;
    targets_achieved->last->src_text_beg_ind = src_text_beg_ind;
    targets_achieved->last->rules_saved_ind = rules_saved_ind;
    targets_achieved->last->src_text_end_ind = src_text_end_ind;
    targets_achieved->last->rules_reach_target_ind = rules_reach_target_ind;
    ++(targets_achieved->count);
    return *(targets_achieved->last);
}

target_achieved_t plcmp_target_remove_last_achieved(
    targets_achieved_stack_t *targets_achieved)
{
    if (targets_achieved->count)
    {
        target_achieved_t goal = *(targets_achieved->last);
        memset(targets_achieved->last, 0, sizeof(*targets_achieved->last));
        --targets_achieved->count;
        targets_achieved->last = targets_achieved->count ? 
            &targets_achieved->stack[targets_achieved->count - 1] : NULL;
        return goal;
    }
    else
    {
        PLCMP_UTILS_ASSERT_MSG(
            0,
            "Trying to remove last achieved goal from the "
            "empty stack of achieved goals");
    }
}