/* encoding: UTF-8 */

#include "plcmp_goal.h"
#include "plcmp_parser_sm.h"
#include "plcmp_tables.h"


typedef enum parser_sm_state_e {
    PARSER_STATE_SUCCESSFUL_FINISH,
    PARSER_STATE_FAILURE_FINISH,
    PARSER_STATE_GO_NEXT,
    PARSER_STATE_GO_ALTERNATE,
    PARSER_STATE_GO_PREV,
    PARSER_STATE_GO_FORWARD,
    PARSER_STATE_GO_END_RULE,
    PARSER_STATE_ADD_ACHIEVED_GOAL,
    PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL,
    PARSER_STATE_SYNTAX_ANALYSIS_ERROR,
    PARSER_STATE_NOTHING_TODO,
} parser_sm_state_t;

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

static enum plcmp_parser_sm_error_code_e check_initial_parameters(void)
{

    return (!g_p_src_text || g_csrc_ind < 0
            || !g_goals_interim || !g_goals_achieved) ?
                PLCMP_PARSER_SM_UNITIALIZED_ESSENTIAL_PARAMS_ERROR :
                PLCMP_PARSER_SM_SUCCESS;
}


static enum parser_sm_state_e go_end_rule(void)
{
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

    return PARSER_STATE_NOTHING_TODO;
}

static enum parser_sm_state_e go_next(void)
{
    g_s_crl_ind = rules[g_s_crl_ind].next;
    return PARSER_STATE_GO_FORWARD;
}

static enum parser_sm_state_e go_forward(void)
{
    ++g_csrc_ind;

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
        else if (adj_reach_mtrx[ascii_rel[(int)g_p_src_text[g_csrc_ind]]]
                               [rules[g_s_crl_ind].sym])
        {
            plcmp_goal_add_interim(g_goals_interim,
                                   rules[g_s_crl_ind].sym,
                                   g_csrc_ind,
                                   g_s_crl_ind);
            g_s_crl_ind = inputs[ascii_rel[(int)g_p_src_text[g_csrc_ind]]];
            return PARSER_STATE_GO_NEXT;
        }
        else
        {
            --g_csrc_ind;
            return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                      : PARSER_STATE_GO_PREV;
        }
    } 
    else if (ascii_rel[(int)g_p_src_text[g_csrc_ind]] != rules[g_s_crl_ind].sym)
    {
        --g_csrc_ind;
        return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                  : PARSER_STATE_GO_PREV;
    }
    else
    {
        return PARSER_STATE_GO_NEXT;
    }

    return PARSER_STATE_NOTHING_TODO;
}

static enum parser_sm_state_e go_alternate(void)
{
    g_s_crl_ind = rules[g_s_crl_ind].alt;
    return PARSER_STATE_GO_FORWARD;
}

static enum parser_sm_state_e go_prev(void)
{
    g_s_crl_ind = rules[g_s_crl_ind].prev;

    if (SYM_NON_TERM != type_of_sym(rules[g_s_crl_ind].sym))
    {
        if (rules[g_s_crl_ind].prev > 0)
        {
            --g_csrc_ind;
            return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE :
                                        PARSER_STATE_GO_PREV;
        }
        else
        {
            g_s_crl_ind = g_goals_interim->last->rules_saved_ind;
            plcmp_goal_remove_last_interim(g_goals_interim);
            if (999 == g_s_crl_ind)
            {
                return PARSER_STATE_SYNTAX_ANALYSIS_ERROR;
            }
            else
            {
                --g_csrc_ind;
                return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE :
                                            PARSER_STATE_GO_PREV;
            }
        }

        return PARSER_STATE_NOTHING_TODO;
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

            return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE :
                                            PARSER_STATE_GO_PREV;
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

                return rules[g_s_crl_ind].alt ? PARSER_STATE_GO_ALTERNATE :
                                            PARSER_STATE_GO_PREV;
            }

            return PARSER_STATE_NOTHING_TODO;
        }
        else
        {
            /* TODO */
            PLCMP_UTILS_ASSERT(0);
            return PARSER_STATE_NOTHING_TODO;
        }
    }

    return PARSER_STATE_NOTHING_TODO;
}

static enum parser_sm_state_e go_add_achieved_goal(void)
{
    plcmp_goal_add_achieved(g_goals_achieved, 
                            rules[g_s_crl_ind].sym,
                            g_goals_interim->last->src_text_left_ind,
                            0,
                            g_csrc_ind,
                            g_s_crl_ind);
    g_s_crl_ind = inputs[rules[g_s_crl_ind].sym];
    return PARSER_STATE_GO_NEXT;
}

static enum parser_sm_state_e go_add_achieved_last_interim_goal(void)
{
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
}

enum plcmp_parser_sm_error_code_e plcmp_parser_sm_run(void)
{
    plcmp_parser_sm_error_code_t err_code = PLCMP_PARSER_SM_SUCCESS;
    parser_sm_state_t next_state = PARSER_STATE_SUCCESSFUL_FINISH;

    err_code = check_initial_parameters();
    if (PLCMP_PARSER_SM_SUCCESS != err_code)
    {
        goto error;
    }

    /* Check reachability of goal "PRO" by current terminal symbol */
    if (!adj_reach_mtrx[ascii_rel[(int)g_p_src_text[g_csrc_ind]]][SYM_PRO])
    {
        err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
        goto error;
    }

    /* Let's start. Set the first interim goal "PRO" */
    plcmp_goal_add_interim(g_goals_interim, SYM_PRO, g_csrc_ind, 999);
    g_src_indmax = g_csrc_ind;
    g_s_crl_ind = inputs[ascii_rel[(int)g_p_src_text[g_csrc_ind]]];

    next_state = PARSER_STATE_GO_NEXT;
    do {
        switch (next_state)
        {
            case PARSER_STATE_GO_NEXT:
                next_state = go_next();
                break;
            case PARSER_STATE_GO_PREV:
                next_state = go_prev();
                break;
            case PARSER_STATE_GO_ALTERNATE:
                next_state = go_alternate();
                break;
            case PARSER_STATE_GO_FORWARD:
                next_state = go_forward();
                break;
            case PARSER_STATE_GO_END_RULE:
                next_state = go_end_rule();
                break;
            case PARSER_STATE_ADD_ACHIEVED_GOAL:
                next_state = go_add_achieved_goal();
                break;
            case PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL:
                next_state = go_add_achieved_last_interim_goal();
                break;
            case PARSER_STATE_SUCCESSFUL_FINISH:
                err_code = PLCMP_PARSER_SM_SUCCESS;
                break;
            /* Failure */
            case PARSER_STATE_SYNTAX_ANALYSIS_ERROR:
                err_code = PLCMP_PARSER_SM_SYNTAX_ERROR;
                next_state = PARSER_STATE_FAILURE_FINISH;
                break;
            case PARSER_STATE_NOTHING_TODO:
                err_code = PLCMP_PARSER_SM_WRONG_STATE_ERROR;
                next_state = PARSER_STATE_FAILURE_FINISH;
                break;
            default:
                err_code = PLCMP_PARSER_SM_UNKNOWN_STATE_ERROR;
                next_state = PARSER_STATE_FAILURE_FINISH;
                break;
        }
    } while (!(PARSER_STATE_SUCCESSFUL_FINISH == next_state
               || PARSER_STATE_FAILURE_FINISH == next_state));

    error:

    return err_code;
}