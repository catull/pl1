/* encoding: UTF-8 */

#include <stddef.h>

#include "plcmp_goal.h"
#include "plcmp_parser_sm.h"
#include "plcmp_tables.h"

typedef enum parser_sm_state_e {
    PARSER_STATE_SUCCESSFUL_FINISH,
    PARSER_STATE_FAILURE_FINISH,
    PARSER_STATE_CHECK_INITIAL_PARAMS,
    PARSER_STATE_START_PROCESS,
    PARSER_STATE_GO_NEXT,
    PARSER_STATE_GO_ALTERNATE,
    PARSER_STATE_GO_PREV,
    PARSER_STATE_GO_FORWARD,
    PARSER_STATE_GO_END_RULE,
    PARSER_STATE_ADD_ACHIEVED_GOAL,
    PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL,
    PARSER_STATE_COUNT_STATES
} parser_sm_state_t;

typedef enum parser_sm_state_e parser_sm_state_handler_t(
    plcmp_parser_sm_error_code_t *err_code);

char const *g_p_src_text;
/* Current index in the compact text */
int g_csrc_ind = -1;
/* Current index in the table of grammar rules */
int g_s_crl_ind = -1;
/* Maximum index of processed compact source text 
 * It is necessary for sending part of source text if error will occur */
int g_src_indmax = -1;

goals_interim_stack_t *g_goals_interim = NULL;
goals_achieved_stack_t *g_goals_achieved = NULL;

#define X(s) static parser_sm_state_handler_t s;
#define PARSER_SM_STATE_HANDLERS_TABLE      \
    X(check_initial_params)                 \
    X(start_process)                        \
    X(go_next)                              \
    X(go_alternate)                         \
    X(go_prev)                              \
    X(go_forward)                           \
    X(go_end_rule)                          \
    X(go_add_achieved_goal)                 \
    X(go_add_achieved_last_interim_goal)

PARSER_SM_STATE_HANDLERS_TABLE

#undef X
#undef PARSER_SM_STATE_HANDLERS_TABLE

static parser_sm_state_handler_t *handlers[PARSER_STATE_COUNT_STATES] = {
    [PARSER_STATE_SUCCESSFUL_FINISH] = NULL,
    [PARSER_STATE_FAILURE_FINISH] = NULL,
    [PARSER_STATE_CHECK_INITIAL_PARAMS] = check_initial_params,
    [PARSER_STATE_START_PROCESS] = start_process,
    [PARSER_STATE_GO_NEXT] = go_next,
    [PARSER_STATE_GO_ALTERNATE] = go_alternate,
    [PARSER_STATE_GO_PREV] = go_prev,
    [PARSER_STATE_GO_FORWARD] = go_forward,
    [PARSER_STATE_GO_END_RULE] = go_end_rule,
    [PARSER_STATE_ADD_ACHIEVED_GOAL] = go_add_achieved_goal,
    [PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL] = 
        go_add_achieved_last_interim_goal
};

static enum parser_sm_state_e check_initial_params(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    if (!g_p_src_text || g_csrc_ind < 0
        || !g_goals_interim || !g_goals_achieved)
    {
        *err_code = PLCMP_PARSER_SM_UNITIALIZED_ESSENTIAL_PARAMS_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }

    return PARSER_STATE_START_PROCESS;
}

static enum parser_sm_state_e start_process(
    plcmp_parser_sm_error_code_t *err_code)
{
    sym_t term = ascii_rel[(int)g_p_src_text[g_csrc_ind]];
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    /* Check reachability of goal "PRO" by current terminal symbol */
    if (SYM_INCORRECT == term || !adj_reach_mtrx[term][SYM_PRO])
    {
        *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }

    /* Let's start. Set the first interim goal "PRO" */
    plcmp_goal_add_interim(g_goals_interim, SYM_PRO, g_csrc_ind, 999);
    g_src_indmax = g_csrc_ind;
    g_s_crl_ind = inputs[term];

    return PARSER_STATE_GO_NEXT;
}

static enum parser_sm_state_e go_next(
    plcmp_parser_sm_error_code_t *err_code)
{
    (void)err_code;

    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_crl_ind = rules[g_s_crl_ind].next;
    return PARSER_STATE_GO_FORWARD;
}

static enum parser_sm_state_e go_alternate(
    plcmp_parser_sm_error_code_t *err_code)
{
    (void)err_code;

    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_crl_ind = rules[g_s_crl_ind].alt;
    return PARSER_STATE_GO_FORWARD;
}

static enum parser_sm_state_e go_prev(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    g_s_crl_ind = rules[g_s_crl_ind].prev;

    if (SYM_NON_TERM != type_of_sym(rules[g_s_crl_ind].sym))
    {
        if (rules[g_s_crl_ind].prev > 0)
        {
            --g_csrc_ind;
            return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                          : PARSER_STATE_GO_PREV;
        }
        else
        {
            g_s_crl_ind = g_goals_interim->last->rules_saved_ind;
            plcmp_goal_remove_last_interim(g_goals_interim);
            if (999 == g_s_crl_ind)
            {
                *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
                return PARSER_STATE_FAILURE_FINISH;
            }
            else
            {
                --g_csrc_ind;
                return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                              : PARSER_STATE_GO_PREV;
            }
        }
    }
    else
    {
        if (rules[g_s_crl_ind].prev > 0)
        {
            plcmp_goal_add_interim(g_goals_interim,
                                   g_goals_achieved->last->sym,
                                   g_goals_achieved->last->src_text_beg_ind,
                                   g_goals_achieved->last->rules_saved_ind);
            g_s_crl_ind = g_goals_achieved->last->rules_reach_goal_ind;
            plcmp_goal_remove_last_achieved(g_goals_achieved);

            return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                          : PARSER_STATE_GO_PREV;
        }
        else if (!rules[g_s_crl_ind].prev)
        {
            if (g_goals_interim->last->sym == g_goals_achieved->last->sym)
            {
                g_s_crl_ind = g_goals_interim->last->rules_saved_ind;
                plcmp_goal_remove_last_interim(g_goals_interim);

                return PARSER_STATE_GO_NEXT;
            }
            else
            {
                g_s_crl_ind = g_goals_achieved->last->rules_reach_goal_ind;
                plcmp_goal_remove_last_achieved(g_goals_achieved);

                return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                              : PARSER_STATE_GO_PREV;
            }
        }
        else
        {
            /* TODO */
            PLCMP_UTILS_ASSERT(0);
        }
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

static enum parser_sm_state_e go_forward(
    plcmp_parser_sm_error_code_t *err_code)
{
    sym_t term = SYM_INCORRECT;
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    ++g_csrc_ind;
    term = ascii_rel[(int)g_p_src_text[g_csrc_ind]];

    if (g_csrc_ind > g_src_indmax)
    {
        g_src_indmax = g_csrc_ind;
    }

    if (SYM_NON_TERM == type_of_sym(rules[g_s_crl_ind].sym))
    {
        if (SYM_END_RULE == rules[rules[g_s_crl_ind].next].sym)
        {
            return PARSER_STATE_GO_END_RULE;
        }
        else if (SYM_INCORRECT == term)
        {
            *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
            return PARSER_STATE_FAILURE_FINISH;
        }
        else if (adj_reach_mtrx[term][rules[g_s_crl_ind].sym])
        {
            plcmp_goal_add_interim(g_goals_interim,
                                   rules[g_s_crl_ind].sym,
                                   g_csrc_ind,
                                   g_s_crl_ind);
            g_s_crl_ind = inputs[term];

            return PARSER_STATE_GO_NEXT;
        }
        else
        {
            --g_csrc_ind;

            return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                          : PARSER_STATE_GO_PREV;
        }
    }
    else if (SYM_INCORRECT == term)
    {
        *err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
        return PARSER_STATE_FAILURE_FINISH;
    }
    else if (term != rules[g_s_crl_ind].sym)
    {
        --g_csrc_ind;

        return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                      : PARSER_STATE_GO_PREV;
    }
    else
    {
        return PARSER_STATE_GO_NEXT;
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

static enum parser_sm_state_e go_end_rule(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    --g_csrc_ind;

    if (rules[g_s_crl_ind].sym == g_goals_interim->last->sym)
    {
        return PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL;
    }
    else if (adj_reach_mtrx[rules[g_s_crl_ind].sym]
                           [g_goals_interim->last->sym])
    {
        return PARSER_STATE_ADD_ACHIEVED_GOAL;
    }
    else
    {
        return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                      : PARSER_STATE_GO_PREV;
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

static enum parser_sm_state_e go_add_achieved_goal(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    plcmp_goal_add_achieved(g_goals_achieved, 
                            rules[g_s_crl_ind].sym,
                            g_goals_interim->last->src_text_left_ind,
                            0,
                            g_csrc_ind,
                            g_s_crl_ind);
    g_s_crl_ind = inputs[rules[g_s_crl_ind].sym];

    return PARSER_STATE_GO_NEXT;
}

static enum parser_sm_state_e go_add_achieved_last_interim_goal(
    plcmp_parser_sm_error_code_t *err_code)
{
    *err_code = PLCMP_PARSER_SM_SUCCESS;

    plcmp_goal_add_achieved(g_goals_achieved,
                            g_goals_interim->last->sym,
                            g_goals_interim->last->src_text_left_ind,
                            g_goals_interim->last->rules_saved_ind,
                            g_csrc_ind,
                            g_s_crl_ind);
    if (SYM_PRO == g_goals_achieved->last->sym)
    {
        return PARSER_STATE_SUCCESSFUL_FINISH;
    }
    else 
    {
        if (adj_reach_mtrx[g_goals_interim->last->sym]
                          [g_goals_interim->last->sym])
        {
            g_s_crl_ind = inputs[rules[g_s_crl_ind].sym];
        }
        else
        {
            g_s_crl_ind = g_goals_interim->last->rules_saved_ind;
            plcmp_goal_remove_last_interim(g_goals_interim);
        }

        return PARSER_STATE_GO_NEXT;
    }

    *err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
    return PARSER_STATE_FAILURE_FINISH;
}

enum plcmp_parser_sm_error_code_e plcmp_parser_sm_run(void)
{
    plcmp_parser_sm_error_code_t err_code = PLCMP_PARSER_SM_SUCCESS;
    parser_sm_state_t next_state = PARSER_STATE_CHECK_INITIAL_PARAMS;

    for (next_state = PARSER_STATE_CHECK_INITIAL_PARAMS; handlers[next_state]; )
    {
        next_state = handlers[next_state](&err_code);
    }

    return err_code;
}

void plcmp_parser_sm_clear_params(void)
{
    g_p_src_text = NULL;
    g_csrc_ind = -1;
    g_s_crl_ind = -1;
    g_src_indmax = -1;
}