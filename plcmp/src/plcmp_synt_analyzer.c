/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_goal.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

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
    for (; i1 < NNETRM; i1++)
    {
        int i2 = 0;
        for (; i2 < NVXOD; i2++)
        {
            if (adj_reach_mtrx[i2][i1] && (i1 != i2))
            {
                int i3 = 0;
                for (; i3 < NNETRM; i3++)
                {
                    adj_reach_mtrx[i2][i3] = 
                        adj_reach_mtrx[i2][i3] || adj_reach_mtrx[i1][i3];
                }
            }
        }
    }
}

static inline int next_rule_ind(int j)
{
    return synt_rules_table[j].next;
}

static inline int prev_rule_ind(int j)
{
    return synt_rules_table[j].prev;
}

static inline int alt_rule_ind(int j)
{
    return synt_rules_table[j].alt;
}

/* Subroutine sets default values for syntax analyzer's error data structure */
static inline void plcmp_synt_analyzer_set_default_err_data(
    plcmp_synt_analyzer_error_data_t *err_data)
{
    PLCMP_UTILS_ASSERT(err_data, "");
    memset(err_data, 0, sizeof(plcmp_synt_analyzer_error_data_t));
    err_data->err_code = PLCMP_SYNT_ANALYZER_SUCCESS;
}

struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    goals_achieved_stack_t **p_goals_achieved)
{
    goals_interim_stack_t *goals_interim = NULL;
    goals_achieved_stack_t *goals_achieved = NULL;
    plcmp_synt_analyzer_error_data_t err_data;

    /* Current index in the compact text */
    int i = 0;
    /* Current index in the table of grammar rules */
    int j = 0;
    /* Maximum index of processed compact source text 
     * It is necessary for sending part of source text if error will occur */
    int i_max = 0;

    PLCMP_UTILS_ASSERT(p_goals_achieved, "");

    plcmp_synt_analyzer_set_default_err_data(&err_data);
    goals_interim = plcmp_goal_create_goals_interim_stack();
    goals_achieved = plcmp_goal_create_goals_achieved_stack();
    plcmp_tables_build_reach_mtrx();

    /* Let's start */
    plcmp_goal_add_interim(goals_interim, "PRO", i, 999);
    if (!adj_reach_mtrx[inputs_tb_ind(&compact_pl1_src_text[i], 1)]
                       [inputs_tb_ind("PRO", 3)])
    {
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;
        memcpy(err_data.src_text_part,
               &compact_pl1_src_text[i_max],
               PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';
        goto error;
    }

    j = input_syms_table[inputs_tb_ind(&compact_pl1_src_text[i], 1)].tb_rules_ind;
    j = next_rule_ind(j);

    L1:

    ++i;

    if (i > i_max)
    {
        i_max = i;
    }

    if (TERM == input_syms_table[inputs_tb_ind(synt_rules_table[j].sym_design, 3)].type)
    {
        if (compact_pl1_src_text[i] == synt_rules_table[j].sym_design[0])
        {
            j = next_rule_ind(j);
            goto L1;
        }
        else
        {
            --i;
            goto L2;
        }
    }

    if ('*' == synt_rules_table[next_rule_ind(j)].sym_design[0])
    {
        --i;

        if (streq(synt_rules_table[j].sym_design,
                  goals_interim->last->sym_title))
        {
            plcmp_goal_add_achieved(goals_achieved,
                                    goals_interim->last->sym_title,
                                    goals_interim->last->src_text_left_ind,
                                    goals_interim->last->tb_rules_reach_ind,
                                    i,
                                    j);

            if (streq(goals_interim->last->sym_title, "PRO"))
            {
                goto success;
            }

            if (adj_reach_mtrx[inputs_tb_ind(goals_interim->last->sym_title, 3)]
                              [inputs_tb_ind(goals_interim->last->sym_title, 3)])
            {
                j = input_syms_table[inputs_tb_ind(goals_interim->last->sym_title, 3)].tb_rules_ind;
                j = next_rule_ind(j);
                goto L1;
            }

            j = goals_interim->last->tb_rules_reach_ind;
            plcmp_goal_remove_last_interim(goals_interim);

            j = next_rule_ind(j);
            goto L1;
        }

        if (!adj_reach_mtrx[inputs_tb_ind(synt_rules_table[j].sym_design, 3)]
                           [inputs_tb_ind(goals_interim->last->sym_title, 3)])
        {
            goto L2;
        }

        plcmp_goal_add_achieved(goals_achieved, 
                                synt_rules_table[j].sym_design,
                                goals_interim->last->src_text_left_ind,
                                0,
                                i,
                                j);
        j = input_syms_table[inputs_tb_ind(synt_rules_table[j].sym_design, 3)].tb_rules_ind;
        j = next_rule_ind(j);
        goto L1;
    }

    if (!adj_reach_mtrx[inputs_tb_ind(&compact_pl1_src_text[i], 1)]
                       [inputs_tb_ind(synt_rules_table[j].sym_design, 3)])
    {
        --i;
        goto L2;
    }

    plcmp_goal_add_interim(goals_interim, synt_rules_table[j].sym_design, i, j);
    j = input_syms_table[inputs_tb_ind(&compact_pl1_src_text[i], 1)].tb_rules_ind;
    j = next_rule_ind(j);
    goto L1;

    L2:

    if (alt_rule_ind(j) != 0)
    {
        j = alt_rule_ind(j);
        goto L1;
    }

    j = prev_rule_ind(j);

    if (NON_TERM == input_syms_table[inputs_tb_ind(synt_rules_table[j].sym_design, 3)].type)
    {
        if (prev_rule_ind(j) > 0)
        {
            plcmp_goal_add_interim(goals_interim,
                                   goals_achieved->last->sym_title,
                                   goals_achieved->last->src_text_beg_ind,
                                   goals_achieved->last->tb_rules_reach_ind);

            j = goals_achieved->last->tb_rules_next_goal_ind;
            plcmp_goal_remove_last_achieved(goals_achieved);
            goto L2;
        }

        if (prev_rule_ind(j) == 0)
        {
            if (streq(goals_interim->last->sym_title,
                      goals_achieved->last->sym_title))
            {
                j = goals_interim->last->tb_rules_reach_ind;
                plcmp_goal_remove_last_interim(goals_interim);

                j = next_rule_ind(j);
                goto L1;
            }
            else
            {
                j = goals_achieved->last->tb_rules_next_goal_ind;
                plcmp_goal_remove_last_achieved(goals_achieved);
                goto L2;
            }
        }
    }

    if (prev_rule_ind(j) > 0)
    {
        --i;
        goto L2;
    }

    j = goals_interim->last->tb_rules_reach_ind;
    plcmp_goal_remove_last_interim(goals_interim);

    if (999 == j)
    {
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;
        memcpy(err_data.src_text_part,
               &compact_pl1_src_text[i_max], 
               PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';
        goto error;
    }
    else
    {
        --i;
        goto L2;
    }

    error:
    plcmp_goal_destroy_goals_achieved_stack(&goals_achieved);

    success:
    plcmp_goal_destroy_goals_interim_stack(&goals_interim);
    *p_goals_achieved = goals_achieved;

    return err_data;
}
