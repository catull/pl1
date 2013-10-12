/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_goal.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

/* Subroutine constructs error message by 
 * error data of syntax analyzer module */
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

/* Subroutine sets default values for syntax analyzer's error data structure */
static inline void plcmp_synt_analyzer_set_default_err_data(
    plcmp_synt_analyzer_error_data_t *err_data)
{
    PLCMP_UTILS_ASSERT(err_data, "");
    memset(err_data, 0, sizeof(plcmp_synt_analyzer_error_data_t));
    err_data->err_code = PLCMP_SYNT_ANALYZER_SUCCESS;
}

/* Subroutine of syntax analyzer. 
 * It constructs parse tree and returns error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    goals_achieved_stack_t **p_goals_achieved)
{
    /* It's stack of interim goals
     * Later stack will be normally created by function call */
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

    /* Clear error data structure for later using 
     * and set default successful value for error code */
    plcmp_synt_analyzer_set_default_err_data(&err_data);

    /* Create stack of interim goals */
    goals_interim = plcmp_goal_create_goals_interim_stack();
    /* Create stack of achieved goals */
    *p_goals_achieved = plcmp_goal_create_goals_achieved_stack();
    goals_achieved = *p_goals_achieved;
    /* Construct reachability matrix */
    plcmp_tables_build_reach_mtrx();

    /* Let's start */
    plcmp_goal_add_interim(goals_interim, "PRO", i, 999);
    if (!adj_reach_mtrx[plcmp_tables_get_input_syms_tb_ind(
                            &compact_pl1_src_text[i], 1)]
                       [plcmp_tables_get_input_syms_tb_ind("PRO", 3)])
    {
        plcmp_goal_destroy_goals_interim_stack(&goals_interim);

        /* Prepare error data for return to main module */
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;

        memcpy(err_data.src_text_part,
               &compact_pl1_src_text[i_max],
               PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }

    L2:

    j = input_syms_table[plcmp_tables_get_input_syms_tb_ind(&compact_pl1_src_text[i], 1)].synt_rule_tb_ind;

    L3:

    j = synt_rules_table[j].next;

    L31:

    ++i;

    if (i > i_max)
    {
        i_max = i;
    }

    if (TERM == input_syms_table[plcmp_tables_get_input_syms_tb_ind(synt_rules_table[j].sym_design, 3)].type)
    {
        if (compact_pl1_src_text[i] == synt_rules_table[j].sym_design[0])
        {
            goto L3;
        }
        else
        {
            goto L8;
        }
    }

    if ('*' == synt_rules_table[synt_rules_table[j].next].sym_design[0])
    {
        --i;

        if (streq(synt_rules_table[j].sym_design, goals_interim->last->sym_title))
        {
            plcmp_goal_add_achieved(goals_achieved,
                                    goals_interim->last->sym_title,
                                    goals_interim->last->src_text_left_ind,
                                    goals_interim->last->tb_rules_ind,
                                    i,
                                    j);

            if (streq(goals_interim->last->sym_title, "PRO"))
            {
                /* Successful finish of the syntax analysis */
                plcmp_goal_destroy_goals_interim_stack(&goals_interim);
                return err_data;
            }

            if (adj_reach_mtrx[plcmp_tables_get_input_syms_tb_ind(goals_interim->last->sym_title, 3)]
                              [plcmp_tables_get_input_syms_tb_ind(goals_interim->last->sym_title, 3)])
            {
                j = input_syms_table[plcmp_tables_get_input_syms_tb_ind(goals_interim->last->sym_title, 3)].synt_rule_tb_ind;
                goto L3;
            }

            L6:

            j = goals_interim->last->tb_rules_ind;
            plcmp_goal_remove_last_interim(goals_interim);

            goto L3;
        }

        if (!adj_reach_mtrx[plcmp_tables_get_input_syms_tb_ind(synt_rules_table[j].sym_design, 3)]
                           [plcmp_tables_get_input_syms_tb_ind(goals_interim->last->sym_title, 3)])
        {
            goto L9;
        }

        plcmp_goal_add_achieved(goals_achieved, 
                                synt_rules_table[j].sym_design,
                                goals_interim->last->src_text_left_ind,
                                0,
                                i,
                                j);
        j = input_syms_table[plcmp_tables_get_input_syms_tb_ind(synt_rules_table[j].sym_design, 3)].synt_rule_tb_ind;
        goto L3;
    }

    if (!adj_reach_mtrx[plcmp_tables_get_input_syms_tb_ind(&compact_pl1_src_text[i], 1)]
                       [plcmp_tables_get_input_syms_tb_ind(synt_rules_table[j].sym_design, 3)])
    {
        goto L8;
    }

    plcmp_goal_add_interim(goals_interim, synt_rules_table[j].sym_design, i, j);
    goto L2;

    L8:

    --i;

    L9:

    if (synt_rules_table[j].alt != 0)
    {
        j = synt_rules_table[j].alt;
        goto L31;
    }

    j = synt_rules_table[j].prev;

    if (input_syms_table[plcmp_tables_get_input_syms_tb_ind(synt_rules_table[j].sym_design, 3)].type == NON_TERM)
    {
        if (synt_rules_table[j].prev > 0)
        {
            plcmp_goal_add_interim(goals_interim,
                                   goals_achieved->last->sym_title,
                                   goals_achieved->last->src_text_beg_ind,
                                   goals_achieved->last->tb_rules_ind);

            L10:

            j = goals_achieved->last->tb_rules_next_goal_ind;
            plcmp_goal_remove_last_achieved(goals_achieved);
            goto L9;
        }

        if (synt_rules_table[j].prev == 0)
        {
            if (streq(goals_interim->last->sym_title,
                      goals_achieved->last->sym_title))
            {
                goto L6;
            }
            else
            {
                goto L10;
            }
        }
    }

    if (synt_rules_table[j].prev > 0)
    {
        goto L8;
    }

    j = goals_interim->last->tb_rules_ind;
    plcmp_goal_remove_last_interim(goals_interim);

    if (999 == j)
    {
        plcmp_goal_destroy_goals_interim_stack(&goals_interim);

        /* Prepare error data for return to main module */
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;
        
        memcpy(err_data.src_text_part, &compact_pl1_src_text[i_max], PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }
    else
    {
        goto L8;
    }
}
