/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_utils.h"
#include "plcmp_goal.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_tables.h"

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
    PLCMP_UTILS_ASSERT(err_data);
    memset(err_data, 0, sizeof(plcmp_synt_analyzer_error_data_t));
    err_data->err_code = PLCMP_SYNT_ANALYZER_SUCCESS;
}

/* Subroutine of syntax analyzer. 
 * It constructs parse tree and returns error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    goals_achieved_stack_t *p_goals_achieved)
{
    /* It's stack of goals and pointer on it.
     * Later stack will be normally created by function call */
    goals_interim_stack_t goals, *p_goals = &goals;

    plcmp_synt_analyzer_error_data_t err_data;

    /* Current index in the compact text */
    int i = 0;
    /* Current index in the table of grammar rules */
    int j = 0;
    /* Maximum index of processed compact source text 
     * It is necessary for sending part of source text if error will occur */
    int i_max = 0;

    /* Create stack of goals */
    plcmp_goal_create_goals_interim_stack(p_goals);
    /* Clear error data structure for later using 
     * and set default successful value for error code */
    plcmp_synt_analyzer_set_default_err_data(&err_data);

    /* Construct reachability matrix */
    plcmp_tables_build_reach_mtrx();

    plcmp_goal_add_interim(p_goals, "PRO", i, 999);

    if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(
                            &compact_pl1_src_text[i], 1)]
                       [plcmp_tables_get_synt_rules_stroke_ind("PRO", 3)])
    {
        plcmp_goal_destroy_goals_interim_stack(p_goals);

        /* Prepare error data for return to main module */
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;

        memcpy(err_data.src_text_part,
               &compact_pl1_src_text[i_max],
               PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }

    L2:

    j = input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(&compact_pl1_src_text[i], 1)].synt_rule_tb_ind;

    L3:

    j = synt_rules_table[j].POSL;

    L31:

    ++i;

    if (i > i_max)
    {
        i_max = i;
    }

    if (TERM == input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].type)
    {
        if (compact_pl1_src_text[i] == synt_rules_table[j].DER[0])
        {
            goto L3;
        }
        else
        {
            goto L8;
        }
    }

    if ('*' == synt_rules_table[synt_rules_table[j].POSL].DER[0])
    {
        --i;

        if (!strcmp(synt_rules_table[j].DER, p_goals->stack[p_goals->count - 1].title))
        {
            plcmp_goal_add_achieved(p_goals_achieved,
                                    p_goals->stack[p_goals->count - 1].title,
                                    p_goals->stack[p_goals->count - 1].CEL2,
                                    p_goals->stack[p_goals->count - 1].CEL3,
                                    i,
                                    j);

            if (!strcmp(p_goals->stack[p_goals->count - 1].title, "PRO"))
            {
                /* Successful finish of the syntax analysis */
                plcmp_goal_destroy_goals_interim_stack(p_goals);
                return err_data;
            }

            if (adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(p_goals->stack[p_goals->count - 1].title, 3)][plcmp_tables_get_synt_rules_stroke_ind(p_goals->stack[p_goals->count - 1].title, 3)])
            {
                j = input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(p_goals->stack[p_goals->count - 1].title, 3)].synt_rule_tb_ind;
                goto L3;
            }

            L6:

            j = p_goals->stack[p_goals->count - 1].CEL3;
            plcmp_goal_remove_last_interim(p_goals);

            goto L3;
        }

        if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)][plcmp_tables_get_synt_rules_stroke_ind(p_goals->stack[p_goals->count - 1].title, 3)])
        {
            goto L9;
        }

        plcmp_goal_add_achieved(p_goals_achieved, 
                          synt_rules_table[j].DER,
                          p_goals->stack[p_goals->count - 1].CEL2,
                          0,
                          i,
                          j);
        j = input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].synt_rule_tb_ind;
        goto L3;
    }

    if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(&compact_pl1_src_text[i], 1)]
                       [plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)])
    {
        goto L8;
    }

    plcmp_goal_add_interim(p_goals, synt_rules_table[j].DER, i, j);
    goto L2;

    L8:

    --i;

    L9:

    if (synt_rules_table[j].ALT != 0)
    {
        j = synt_rules_table[j].ALT;
        goto L31;
    }

    j = synt_rules_table[j].PRED;

    if ((input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].type == NON_TERM ) && (synt_rules_table[j].PRED > 0))
    {
        plcmp_goal_add_interim(p_goals,
                       p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].title,
                       p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST2,
                       p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST3);

        L10:

        j = p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST5;
        plcmp_goal_remove_last_achieved(p_goals_achieved);
        goto L9;
    }

    if ((input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].type == NON_TERM ) && (synt_rules_table[j].PRED == 0))
    {

        if (!strcmp(p_goals->stack[p_goals->count - 1].title,
                    p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].title))
        {
            goto L6;
        }
        else
        {
            goto L10;
        }

    }

    if (synt_rules_table[j].PRED > 0)
    {
        goto L8;
    }

    j = p_goals->stack[p_goals->count - 1].CEL3;
    plcmp_goal_remove_last_interim(p_goals);

    if (999 == j)
    {
        plcmp_goal_destroy_goals_interim_stack(p_goals);

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
