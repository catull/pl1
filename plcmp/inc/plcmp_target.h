/* encoding: UTF-8 */

#ifndef PLCMP_TARGET_H
#define PLCMP_TARGET_H

#include <stddef.h>
#include <stdint.h>

#include "plcmp_common.h"
#include "plcmp_utils.h"
#include "plcmp_tables.h"

typedef uint32_t stack_counter_t;

/* Structure for the stack of goals */
typedef struct target_interim_s {
    sym_t sym;
    index_t src_text_left_ind;
    index_t rules_saved_ind;
} target_interim_t;

/* */
typedef struct targets_interim_stack_s {
    stack_counter_t count;
    target_interim_t *last;
    target_interim_t stack[];
} targets_interim_stack_t;

/* Structure for the stack of goals achieved */
typedef struct target_achieved_s {
    sym_t sym;
    index_t src_text_beg_ind;
    index_t rules_saved_ind;
    index_t src_text_end_ind;
    index_t rules_reach_target_ind; 
} target_achieved_t;

/* */
typedef struct targets_achieved_stack_s {
    stack_counter_t count;
    target_achieved_t *last;
    target_achieved_t stack[];
} targets_achieved_stack_t;

/* Subroutine creates stack for goals achieved for using later
 * by syntax analyzer (parser) and semantic calculator */
static inline targets_achieved_stack_t*
    plcmp_target_create_targets_achieved_stack(void)
{
    targets_achieved_stack_t *targets_achieved = NULL;
    return PLCMP_UTILS_CALLOC_MEM(
        targets_achieved,
        1,
        sizeof(targets_achieved_stack_t) + NDST * sizeof(target_achieved_t));
}

/* Subroutine destroys stack of goals achieved */
static inline void plcmp_target_destroy_targets_achieved_stack(
    targets_achieved_stack_t **targets_achieved)
{
    PLCMP_UTILS_ASSERT(NULL != targets_achieved && NULL != *targets_achieved);
    PLCMP_UTILS_RELEASE_MEM(*targets_achieved);
}

/* Subroutine creates stack for goals for using 
 * later by syntax analyzer (parser) */
static inline targets_interim_stack_t*
    plcmp_target_create_targets_interim_stack(void)
{
    targets_interim_stack_t *goals = NULL;
    return PLCMP_UTILS_CALLOC_MEM(
        goals,
        1,
        sizeof(targets_interim_stack_t) + NCEL * sizeof(target_interim_t));
}

/* Subroutine destroys stack of goals */
static inline void plcmp_target_destroy_targets_interim_stack(
    targets_interim_stack_t **goals)
{
    PLCMP_UTILS_ASSERT(NULL != goals && NULL != *goals);
    PLCMP_UTILS_RELEASE_MEM(*goals);
}

/* Subroutine adds a new goal into stack of goals */
target_interim_t plcmp_target_add_interim(targets_interim_stack_t *interim_targets,
                                      sym_t sym,
                                      index_t src_text_left_ind,
                                      index_t rules_saved_ind);

/* Subroutine removes last goal from the stack of goals */
target_interim_t plcmp_target_remove_last_interim(
    targets_interim_stack_t *interim_targets);

/* Subroutine adds a goal achieved into stack of goals achieved */
target_achieved_t plcmp_target_add_achieved(targets_achieved_stack_t *targets_achieved,
                                        sym_t sym,
                                        index_t src_text_beg_ind,
                                        index_t rules_saved_ind,
                                        index_t src_text_end_ind,
                                        index_t rules_reach_target_ind);

/* Subroutine removes last goal achieved from the stack of goals achieved */
target_achieved_t plcmp_target_remove_last_achieved(
    targets_achieved_stack_t *targets_achieved);

#endif /* PLCMP_TARGET_H */