/* encoding: UTF-8 */

#include <stddef.h>

#include "plcmp_target.h"
#include "plcmp_parser_sm.h"
#include "plcmp_tables.h"

typedef enum parser_sm_state_e {
    PARSER_STATE_SUCCESSFUL_FINISH,
    PARSER_STATE_FAILURE_FINISH,
    PARSER_STATE_GO_CHECK_INITIAL_PARAMS,
    PARSER_STATE_GO_START_PROCESS,
    PARSER_STATE_GO_NEXT_RULE,
    PARSER_STATE_GO_ALT_RULE,
    PARSER_STATE_GO_PREV_RULE,
    PARSER_STATE_GO_END_RULE,
    PARSER_STATE_GO_FORWARD,
    PARSER_STATE_GO_GET_IN_NEW_INTERIM_TARGET,
    PARSER_STATE_GO_GET_OUT_FROM_LAST_INTERIM_TARGET,
    PARSER_STATE_GO_CANCEL_LAST_INTERIM_TARGET,
    PARSER_STATE_GO_ADD_ACHIEVED_TARGET,
    PARSER_STATE_GO_GET_IN_THE_END_OF_LAST_ACHIEVED_TARGET,
    PARSER_STATE_GO_CANCEL_LAST_ACHIEVED_TARGET,
    PARSER_STATE_GO_ADD_ACHIEVED_LAST_INTERIM_TARGET,
    PARSER_STATE_COUNT_STATES
} parser_sm_state_t;

typedef enum parser_sm_state_e parser_sm_state_handler_t(
    plcmp_parser_sm_error_code_t *err_code);

char const *g_p_src_text;
/* Current index in the compact text */
int g_cur_src_i = -1;
/* Current index in the table of grammar rules */
int g_s_cur_rule_i = -1;
/* Maximum index of processed compact source text 
 * It is necessary for sending part of source text if error will occur */
int g_src_indmax = -1;

targets_interim_stack_t *g_targets_interim;
targets_achieved_stack_t *g_targets_achieved;

/* PARSER_STATE_GO_CHECK_INITIAL_PARAMS */
static enum parser_sm_state_e go_check_initial_params(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    if (!g_p_src_text || g_cur_src_i < 0
        || !g_targets_interim || !g_targets_achieved)
    {
        *err_code = PLCMP_PARSER_SM_UNITIALIZED_ESSENTIAL_PARAMS_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }

    return PARSER_STATE_GO_START_PROCESS;
}

/* PARSER_STATE_GO_START_PROCESS */
static enum parser_sm_state_e go_start_process(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;
    sym_t term = ascii_rel[(int)g_p_src_text[g_cur_src_i]];

    /* Check reachability of target "PRO" by current terminal symbol */
    if (SYM_INCORRECT == term || !adj_reach_mtrx[term][SYM_PRO])
    {
        *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }

    /* Let's start. Set the first interim target "PRO" */
    g_src_indmax = g_cur_src_i;
    (void)plcmp_target_add_interim(g_targets_interim,
                                   SYM_PRO,
                                   g_cur_src_i,
                                   INCORRECT_INDEX);
    g_s_cur_rule_i = inputs[term];

    return PARSER_STATE_GO_NEXT_RULE;
}

/* PARSER_STATE_GO_NEXT_RULE */
static enum parser_sm_state_e go_next_rule(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_cur_rule_i = rules[g_s_cur_rule_i].next;
    return PARSER_STATE_GO_FORWARD;
}

/* PARSER_STATE_GO_ALT_RULE */
static enum parser_sm_state_e go_alt_rule(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_cur_rule_i = rules[g_s_cur_rule_i].alt;
    return PARSER_STATE_GO_FORWARD;
}

/* PARSER_STATE_GO_PREV_RULE */
static enum parser_sm_state_e go_prev_rule(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_cur_rule_i = rules[g_s_cur_rule_i].prev;

    if (SYM_NON_TERM == type_of_sym(rules[g_s_cur_rule_i].sym))
    {
        if (rules[g_s_cur_rule_i].prev > 0)
        {
            return PARSER_STATE_GO_CANCEL_LAST_ACHIEVED_TARGET;
        }
        else if (!rules[g_s_cur_rule_i].prev)
        {
            if (g_targets_interim->last->sym == g_targets_achieved->last->sym)
            {
                return PARSER_STATE_GO_GET_OUT_FROM_LAST_INTERIM_TARGET;
            }
            else
            {
                return PARSER_STATE_GO_GET_IN_THE_END_OF_LAST_ACHIEVED_TARGET;
            }
        }
    }
    else
    {
        if (rules[g_s_cur_rule_i].prev > 0)
        {
            --g_cur_src_i;
            return rules[g_s_cur_rule_i].alt ? PARSER_STATE_GO_ALT_RULE
                                             : PARSER_STATE_GO_PREV_RULE;
        }
        else
        {
            return PARSER_STATE_GO_CANCEL_LAST_INTERIM_TARGET;
        }
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

/* PARSER_STATE_GO_END_RULE */
static enum parser_sm_state_e go_end_rule(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    --g_cur_src_i;

    if (rules[g_s_cur_rule_i].sym == g_targets_interim->last->sym)
    {
        return PARSER_STATE_GO_ADD_ACHIEVED_LAST_INTERIM_TARGET;
    }
    else if (adj_reach_mtrx[rules[g_s_cur_rule_i].sym]
                           [g_targets_interim->last->sym])
    {
        return PARSER_STATE_GO_ADD_ACHIEVED_TARGET;
    }
    else
    {
        return rules[g_s_cur_rule_i].alt ? PARSER_STATE_GO_ALT_RULE
                                         : PARSER_STATE_GO_PREV_RULE;
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

/* PARSER_STATE_GO_FORWARD */
static enum parser_sm_state_e go_forward(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;
    sym_t term = SYM_INCORRECT;

    ++g_cur_src_i;
    term = ascii_rel[(int)g_p_src_text[g_cur_src_i]];

    if (g_cur_src_i > g_src_indmax)
    {
        g_src_indmax = g_cur_src_i;
    }

    if (SYM_NON_TERM == type_of_sym(rules[g_s_cur_rule_i].sym))
    {
        if (SYM_END_RULE == rules[rules[g_s_cur_rule_i].next].sym)
        {
            return PARSER_STATE_GO_END_RULE;
        }
        else if (SYM_INCORRECT == term)
        {
            *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
            return PARSER_STATE_FAILURE_FINISH;
        }
        else if (adj_reach_mtrx[term][rules[g_s_cur_rule_i].sym])
        {
            return PARSER_STATE_GO_GET_IN_NEW_INTERIM_TARGET;
        }
        else
        {
            --g_cur_src_i;
            return rules[g_s_cur_rule_i].alt ? PARSER_STATE_GO_ALT_RULE
                                             : PARSER_STATE_GO_PREV_RULE;
        }
    }
    else if (SYM_INCORRECT == term)
    {
        *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }
    else if (term == rules[g_s_cur_rule_i].sym)
    {
        return PARSER_STATE_GO_NEXT_RULE;
    }
    else
    {
        --g_cur_src_i;
        return rules[g_s_cur_rule_i].alt ? PARSER_STATE_GO_ALT_RULE
                                      : PARSER_STATE_GO_PREV_RULE;
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

/* PARSER_STATE_GO_GET_IN_NEW_INTERIM_TARGET */
static enum parser_sm_state_e go_add_interim_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    sym_t term = ascii_rel[(int)g_p_src_text[g_cur_src_i]];
    if (SYM_INCORRECT != term)
    {
        (void)plcmp_target_add_interim(g_targets_interim,
                                       rules[g_s_cur_rule_i].sym,
                                       g_cur_src_i,
                                       g_s_cur_rule_i);
        g_s_cur_rule_i = inputs[term];
        return PARSER_STATE_GO_NEXT_RULE;
    }

    *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

/* PARSER_STATE_GO_GET_OUT_FROM_LAST_INTERIM_TARGET */
static enum parser_sm_state_e go_get_out_from_last_interim_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_cur_rule_i =
        plcmp_target_remove_last_interim(g_targets_interim).rules_saved_ind;

    return PARSER_STATE_GO_NEXT_RULE;
}

/* PARSER_STATE_GO_CANCEL_LAST_INTERIM_TARGET */
static enum parser_sm_state_e go_cancel_last_interim_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    target_interim_t target =
        plcmp_target_remove_last_interim(g_targets_interim);

    if (SYM_PRO == target.sym)
    {
        *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }
    else
    {
        g_s_cur_rule_i = target.rules_saved_ind;
        --g_cur_src_i;
        return rules[g_s_cur_rule_i].alt ? PARSER_STATE_GO_ALT_RULE
                                      : PARSER_STATE_GO_PREV_RULE;
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

/* PARSER_STATE_GO_ADD_ACHIEVED_TARGET */
static enum parser_sm_state_e go_add_achieved_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    (void)plcmp_target_add_achieved(g_targets_achieved, 
                                    rules[g_s_cur_rule_i].sym,
                                    g_targets_interim->last->src_text_left_ind,
                                    0,
                                    g_cur_src_i,
                                    g_s_cur_rule_i);
    g_s_cur_rule_i = inputs[rules[g_s_cur_rule_i].sym];

    return PARSER_STATE_GO_NEXT_RULE;
}

/* PARSER_STATE_GO_GET_IN_THE_END_OF_LAST_ACHIEVED_TARGET */
static enum parser_sm_state_e go_get_in_the_end_of_last_achieved_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_cur_rule_i = 
        plcmp_target_remove_last_achieved(
            g_targets_achieved).rules_reach_target_ind;

    return rules[g_s_cur_rule_i].alt ? PARSER_STATE_GO_ALT_RULE
                                     : PARSER_STATE_GO_PREV_RULE;
}

/* PARSER_STATE_GO_CANCEL_LAST_ACHIEVED_TARGET */
static enum parser_sm_state_e go_cancel_last_achieved_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    (void)plcmp_target_add_interim(
        g_targets_interim,
        g_targets_achieved->last->sym,
        g_targets_achieved->last->src_text_beg_ind,
        g_targets_achieved->last->rules_saved_ind);

    return PARSER_STATE_GO_GET_IN_THE_END_OF_LAST_ACHIEVED_TARGET;
}

/* PARSER_STATE_GO_ADD_ACHIEVED_LAST_INTERIM_TARGET */
static enum parser_sm_state_e go_add_achieved_last_interim_target(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;
    target_achieved_t target = plcmp_target_add_achieved(
                                g_targets_achieved,
                                g_targets_interim->last->sym,
                                g_targets_interim->last->src_text_left_ind,
                                g_targets_interim->last->rules_saved_ind,
                                g_cur_src_i,
                                g_s_cur_rule_i);
    if (SYM_PRO == target.sym)
    {
        return PARSER_STATE_SUCCESSFUL_FINISH;
    }
    else 
    {
        if (adj_reach_mtrx[rules[g_s_cur_rule_i].sym]
                          [rules[g_s_cur_rule_i].sym])
        {
            g_s_cur_rule_i = inputs[rules[g_s_cur_rule_i].sym];
            return PARSER_STATE_GO_NEXT_RULE;
        }
        else
        {
            return PARSER_STATE_GO_GET_OUT_FROM_LAST_INTERIM_TARGET;
        }
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

enum plcmp_parser_sm_error_code_e plcmp_parser_sm_run(void)
{
    plcmp_parser_sm_error_code_t err_code = PLCMP_PARSER_SM_SUCCESS;
    parser_sm_state_t next_state = PARSER_STATE_GO_CHECK_INITIAL_PARAMS;

    parser_sm_state_handler_t *handlers[PARSER_STATE_COUNT_STATES] = {
        [PARSER_STATE_SUCCESSFUL_FINISH] =
            NULL,
        [PARSER_STATE_FAILURE_FINISH] =
            NULL,
        [PARSER_STATE_GO_CHECK_INITIAL_PARAMS] =
            go_check_initial_params,
        [PARSER_STATE_GO_START_PROCESS] =
            go_start_process,
        [PARSER_STATE_GO_NEXT_RULE] =
            go_next_rule,
        [PARSER_STATE_GO_ALT_RULE] =
            go_alt_rule,
        [PARSER_STATE_GO_PREV_RULE] =
            go_prev_rule,
        [PARSER_STATE_GO_END_RULE] =
            go_end_rule,
        [PARSER_STATE_GO_FORWARD] =
            go_forward,
        [PARSER_STATE_GO_GET_IN_NEW_INTERIM_TARGET] =
            go_add_interim_target,
        [PARSER_STATE_GO_GET_OUT_FROM_LAST_INTERIM_TARGET] =
            go_get_out_from_last_interim_target,
        [PARSER_STATE_GO_CANCEL_LAST_INTERIM_TARGET] =
            go_cancel_last_interim_target,
        [PARSER_STATE_GO_ADD_ACHIEVED_TARGET] =
            go_add_achieved_target,
        [PARSER_STATE_GO_GET_IN_THE_END_OF_LAST_ACHIEVED_TARGET] =
            go_get_in_the_end_of_last_achieved_target,
        [PARSER_STATE_GO_CANCEL_LAST_ACHIEVED_TARGET] =
            go_cancel_last_achieved_target,
        [PARSER_STATE_GO_ADD_ACHIEVED_LAST_INTERIM_TARGET] =
            go_add_achieved_last_interim_target
    };

    for (next_state = PARSER_STATE_GO_CHECK_INITIAL_PARAMS;
         handlers[next_state]; )
    {
        next_state = handlers[next_state](&err_code);
    }

    return err_code;
}

void plcmp_parser_sm_clear_params(void)
{
    g_p_src_text = NULL;
    g_cur_src_i = -1;
    g_s_cur_rule_i = -1;
    g_src_indmax = -1;
    g_targets_interim = NULL;
    g_targets_achieved = NULL;
}