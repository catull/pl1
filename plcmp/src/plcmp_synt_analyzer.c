/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_goal.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

static char const *g_p_src_text;
/* Current index in the compact text */
static int csrc_ind;
/* Current index in the table of grammar rules */
static int crl_ind;
/* Maximum index of processed compact source text 
 * It is necessary for sending part of source text if error will occur */
static int src_indmax;

static goals_interim_stack_t *g_goals_interim;
static goals_achieved_stack_t *g_goals_achieved;

char* plcmp_synt_analyzer_errmsg_by_errdata(
    plcmp_synt_analyzer_error_data_t const *err_data,
    char errmsg[])
{
    switch (err_data->err_code)
    {
        case PLCMP_SYNT_ANALYZER_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case PLCMP_SYNT_ANALYZER_SYNTAX_ERROR:
            strcats(errmsg,
                    "Error in syntax of the source text: ",
                    err_data->src_text_part,
                    NULL);
            break;
        case PLCMP_SYNT_ANALYZER_WRONG_STATE:
            strcats(errmsg,
                    "Unknown state of syntax analyzer. "
                    "Part of source text: ",
                    err_data->src_text_part,
                    NULL);
            break;
        default:
            strcpy(errmsg,
                   "Unknown error code in error data "
                   "for generating error message");
            break;
    }
    return errmsg;
}

void plcmp_tables_build_reach_mtrx(void)
{
    int i1 = 0;
    for (; i1 < SYM_NTERMS_COUNT; i1++)
    {
        int i2 = 0;
        for (; i2 < SYM_COUNT; i2++)
        {
            if (adj_reach_mtrx[i2][i1] && (i1 != i2))
            {
                int i3 = 0;
                for (; i3 < SYM_NTERMS_COUNT; i3++)
                {
                    adj_reach_mtrx[i2][i3] = 
                        adj_reach_mtrx[i2][i3] || adj_reach_mtrx[i1][i3];
                }
            }
        }
    }
}

typedef enum parser_state_e {
    PARSER_STATE_SUCCESSFUL_FINISH,
    PARSER_STATE_FAILURE_FINISH,
    PARSER_STATE_GO_NEXT,
    PARSER_STATE_GO_ALTERNATE,
    PARSER_STATE_GO_PREV,
    PARSER_STATE_GO_FORWARD,
    PARSER_STATE_ADD_ACHIEVED_GOAL,
    PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL,
    PARSER_STATE_SYNTAX_ANALYSIS_ERROR,
    PARSER_STATE_NOTHING_TODO,
} parser_state_t;

static enum parser_state_e go_next(void)
{
    crl_ind = rules[crl_ind].next;
    return PARSER_STATE_GO_FORWARD;
}

static enum parser_state_e go_forward(void)
{
    while (1)
    {
        ++csrc_ind;

        if (csrc_ind > src_indmax)
        {
            src_indmax = csrc_ind;
        }

        if (SYM_NON_TERM == type_of_sym(rules[crl_ind].sym))
        {
            if (SYM_END_RULE == rules[rules[crl_ind].next].sym)
            {
                --csrc_ind;
                if (rules[crl_ind].sym == g_goals_interim->last->sym)
                {
                    return PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL;
                }
                else if (adj_reach_mtrx[rules[crl_ind].sym]
                                       [g_goals_interim->last->sym])
                {
                    return PARSER_STATE_ADD_ACHIEVED_GOAL;
                }
                else
                {
                    return rules[crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                              : PARSER_STATE_GO_PREV;
                }
            }
            else if (adj_reach_mtrx[ascii_rel[(int)g_p_src_text[csrc_ind]]]
                                   [rules[crl_ind].sym])
            {
                plcmp_goal_add_interim(g_goals_interim,
                                       rules[crl_ind].sym,
                                       csrc_ind,
                                       crl_ind);
                crl_ind = inputs[ascii_rel[(int)g_p_src_text[csrc_ind]]];
                return PARSER_STATE_GO_NEXT;
            }
            else
            {
                --csrc_ind;
                return rules[crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                          : PARSER_STATE_GO_PREV;
            }
        } 
        else if (ascii_rel[(int)g_p_src_text[csrc_ind]] != rules[crl_ind].sym)
        {
            --csrc_ind;
            return rules[crl_ind].alt ? PARSER_STATE_GO_ALTERNATE
                                      : PARSER_STATE_GO_PREV;
        }
        else
        {
            return PARSER_STATE_GO_NEXT;
        }
    }
}

static enum parser_state_e go_alternate(void)
{
    crl_ind = rules[crl_ind].alt;
    return PARSER_STATE_GO_FORWARD;
}

static enum parser_state_e go_prev(void)
{
    while (1)
    {
        crl_ind = rules[crl_ind].prev;
        if (SYM_NON_TERM != type_of_sym(rules[crl_ind].sym))
        {
            if (rules[crl_ind].prev > 0)
            {
                --csrc_ind;
                if (rules[crl_ind].alt)
                {
                    return PARSER_STATE_GO_ALTERNATE;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                crl_ind = g_goals_interim->last->rules_saved_ind;
                plcmp_goal_remove_last_interim(g_goals_interim);
                if (999 == crl_ind)
                {
                    return PARSER_STATE_SYNTAX_ANALYSIS_ERROR;
                }
                else
                {
                    --csrc_ind;
                    if (rules[crl_ind].alt)
                    {
                        return PARSER_STATE_GO_ALTERNATE;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
        else
        {
            if (rules[crl_ind].prev > 0)
            {
                plcmp_goal_add_interim(g_goals_interim,
                                       g_goals_achieved->last->sym,
                                       g_goals_achieved->last->src_text_beg_ind,
                                       g_goals_achieved->last->rules_saved_ind);
                crl_ind = g_goals_achieved->last->rules_reach_goal_ind;
                plcmp_goal_remove_last_achieved(g_goals_achieved);

                if (rules[crl_ind].alt)
                {
                    return PARSER_STATE_GO_ALTERNATE;
                }
                else
                {
                    continue;
                }
            }
            else if (!rules[crl_ind].prev)
            {
                if (g_goals_interim->last->sym == g_goals_achieved->last->sym)
                {
                    crl_ind = g_goals_interim->last->rules_saved_ind;
                    plcmp_goal_remove_last_interim(g_goals_interim);
                    return PARSER_STATE_GO_NEXT;
                }
                else
                {
                    crl_ind = g_goals_achieved->last->rules_reach_goal_ind;
                    plcmp_goal_remove_last_achieved(g_goals_achieved);

                    if (rules[crl_ind].alt)
                    {
                        return PARSER_STATE_GO_ALTERNATE;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            else
            {
                /* TODO */
                PLCMP_UTILS_ASSERT(0);
            }
        }
    }
}

static enum parser_state_e go_add_achieved_goal(void)
{
    plcmp_goal_add_achieved(g_goals_achieved, 
                            rules[crl_ind].sym,
                            g_goals_interim->last->src_text_left_ind,
                            0,
                            csrc_ind,
                            crl_ind);
    crl_ind = inputs[rules[crl_ind].sym];
    return PARSER_STATE_GO_NEXT;
}

static enum parser_state_e go_add_achieved_last_interim_goal(void)
{
    plcmp_goal_add_achieved(g_goals_achieved,
                            g_goals_interim->last->sym,
                            g_goals_interim->last->src_text_left_ind,
                            g_goals_interim->last->rules_saved_ind,
                            csrc_ind,
                            crl_ind);
    if (SYM_PRO == g_goals_achieved->last->sym)
    {
        return PARSER_STATE_SUCCESSFUL_FINISH;
    }
    else 
    {
        if (adj_reach_mtrx[g_goals_interim->last->sym]
                          [g_goals_interim->last->sym])
        {
            crl_ind = inputs[rules[crl_ind].sym];
        }
        else
        {
            crl_ind = g_goals_interim->last->rules_saved_ind;
            plcmp_goal_remove_last_interim(g_goals_interim);
        }

        return PARSER_STATE_GO_NEXT;
    }
}

static struct plcmp_synt_analyzer_error_data_s
    plcmp_synt_analyzer_form_err_data(plcmp_synt_analyzer_error_code_t err_code)
{
    plcmp_synt_analyzer_error_data_t err_data = {
        .err_code = err_code
    };
    strncpy(err_data.src_text_part,
            &g_p_src_text[src_indmax],
            PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
    err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';
    return err_data;
}

struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    goals_achieved_stack_t **p_goals_achieved)
{
    parser_state_t next_state = PARSER_STATE_NOTHING_TODO;
    plcmp_synt_analyzer_error_data_t err_data = {
        .err_code = PLCMP_SYNT_ANALYZER_SUCCESS
    };

    g_p_src_text = compact_pl1_src_text;

    PLCMP_UTILS_ASSERT(p_goals_achieved);

    g_goals_interim = plcmp_goal_create_goals_interim_stack();
    g_goals_achieved = plcmp_goal_create_goals_achieved_stack();
    plcmp_tables_build_reach_mtrx();

    /* Check reachability of goal "PRO" by current terminal symbol */
    if (!adj_reach_mtrx[ascii_rel[(int)g_p_src_text[csrc_ind]]][SYM_PRO])
    {
        err_data = plcmp_synt_analyzer_form_err_data(
            PLCMP_SYNT_ANALYZER_SYNTAX_ERROR);
        goto error;
    }
    /* Set the first interim goal "PRO" */
    plcmp_goal_add_interim(g_goals_interim, SYM_PRO, csrc_ind, 999);

    /* Let's start */
    src_indmax = csrc_ind = 0;
    crl_ind = inputs[ascii_rel[(int)g_p_src_text[csrc_ind]]];
    next_state = PARSER_STATE_GO_NEXT;

    do {
        switch (next_state)
        {
            case PARSER_STATE_SUCCESSFUL_FINISH:
                err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS;
                break;
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
            case PARSER_STATE_ADD_ACHIEVED_GOAL:
                next_state = go_add_achieved_goal();
                break;
            case PARSER_STATE_ADD_ACHIEVED_LAST_INTERIM_GOAL:
                next_state = go_add_achieved_last_interim_goal();
                break;
            case PARSER_STATE_SYNTAX_ANALYSIS_ERROR:
            case PARSER_STATE_NOTHING_TODO:
            default:
                err_data = plcmp_synt_analyzer_form_err_data(
                    (next_state == PARSER_STATE_SYNTAX_ANALYSIS_ERROR) ?
                                    PLCMP_SYNT_ANALYZER_SYNTAX_ERROR   :
                                    PLCMP_SYNT_ANALYZER_WRONG_STATE);
                next_state = PARSER_STATE_FAILURE_FINISH;
                break;
        }
    } while (!(PARSER_STATE_SUCCESSFUL_FINISH == next_state
               || PARSER_STATE_FAILURE_FINISH == next_state));

    error:

    plcmp_goal_destroy_goals_interim_stack(&g_goals_interim);

    if (PLCMP_SYNT_ANALYZER_SUCCESS != err_data.err_code)
    {
        plcmp_goal_destroy_goals_achieved_stack(&g_goals_achieved);
    }
    *p_goals_achieved = g_goals_achieved;

    return err_data;
}
