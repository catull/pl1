/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_tables.h"
#include "plcmp_target.h"
#include "utils.h"

target_interim_t plcmp_target_add_interim(
    targets_interim_stack_t *stack,
    sym_t sym,
    index_t src_text_left_i,
    index_t saved_location_of_entry_i)
{
    stack->last = &stack->stack[stack->count];
    stack->last->sym = sym;
    stack->last->src_text_left_i = src_text_left_i;
    stack->last->saved_location_of_entry_i = saved_location_of_entry_i;
    ++(stack->count);
    return *(stack->last);
}

target_interim_t plcmp_target_remove_last_interim(
    targets_interim_stack_t *stack)
{
    if (stack->count)
    {
        target_interim_t target = *(stack->last);
        memset(stack->last, 0, sizeof(*stack->last));
        --stack->count;
        stack->last = stack->count ? &stack->stack[stack->count - 1] : NULL;
        return target;
    }
    else
    {
        UTILS_ASSERT_MSG(
            0,
            "Trying to remove last interim target from the "
            "empty stack of interim targets");
    }
}

target_achieved_t plcmp_target_add_achieved(
    targets_achieved_stack_t *stack,
    sym_t sym,
    index_t src_text_left_i,
    index_t saved_location_of_entry_i,
    index_t src_text_right_i,
    index_t saved_location_of_end_rule_i)
{
    stack->last = &stack->stack[stack->count];
    stack->last->sym = sym;
    stack->last->src_text_left_i = src_text_left_i;
    stack->last->saved_location_of_entry_i = saved_location_of_entry_i;
    stack->last->src_text_right_i = src_text_right_i;
    stack->last->saved_location_of_end_rule_i = saved_location_of_end_rule_i;
    ++(stack->count);
    return *(stack->last);
}

target_achieved_t plcmp_target_remove_last_achieved(
    targets_achieved_stack_t *stack)
{
    if (stack->count)
    {
        target_achieved_t target = *(stack->last);
        memset(stack->last, 0, sizeof(*stack->last));
        --stack->count;
        stack->last = stack->count ? &stack->stack[stack->count - 1] : NULL;
        return target;
    }
    else
    {
        UTILS_ASSERT_MSG(
            0,
            "Trying to remove last achieved target from the "
            "empty stack of achieved targets");
    }
}