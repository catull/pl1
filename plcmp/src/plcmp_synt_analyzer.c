/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_common.h"
#include "plcmp_goal.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_tables.h"

/* Subroutine of syntax analyzer. 
 * It constructs parse tree and returns error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(
    char const compact_pl1_src_text[],
    dst_t *p_goals_achieved)
{
    /* It's stack of goals and pointer on it.
     * Later stack will be normally created by macro */
    cel_t goals, *p_goals = &goals;

    plcmp_synt_analyzer_error_data_t err_data;

    /* Current index in the compact text */
    int i = 0;
    /* Current index in the table of grammar rules */
    int j = 0;
    /* Maximum index of processed compact source text 
     * It needs for sending part of source text if error will occur */
    int i_max = 0;

    /* Create stack of goals */
    PLCMP_SYNT_ANALYZER_CREATE_GOALS_STACK(goals);

    /* Clear error data structure for later using 
     * and set default successful value for error code */
    memset(&err_data, 0, sizeof(plcmp_synt_analyzer_error_data_t));
    err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS;

    /* Construct reachability matrix */
    plcmp_tables_build_reach_mtrx();

    plcmp_goal_add(p_goals, "PRO", i, 999);

    if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(
                            &compact_pl1_src_text[i], 1)]
                       [plcmp_tables_get_synt_rules_stroke_ind("PRO", 3)])
    {
        PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals);

        /* Prepare error data for return to main module */
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;

        memcpy(err_data.src_text_part,
               &compact_pl1_src_text[i_max],
               PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }

    L2:

    j = input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(&compact_pl1_src_text[i], 1)].VX;

    L3:

    j = synt_rules_table[j].POSL;

    L31:

    ++i;

    if (i > i_max)
    {
        i_max = i;
    }

    if ('T' == input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].TYP)
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

        if (!strcmp(synt_rules_table[j].DER, p_goals->p_cel_stack[p_goals->count - 1].CEL1))
        {
            plcmp_goal_add_achieved(p_goals_achieved,
                              p_goals->p_cel_stack[p_goals->count - 1].CEL1,
                              p_goals->p_cel_stack[p_goals->count - 1].CEL2,
                              p_goals->p_cel_stack[p_goals->count - 1].CEL3,
                              i,
                              j);

            if (!strcmp(p_goals->p_cel_stack[p_goals->count - 1].CEL1, "PRO"))
            {
                /* Successful finish of the syntax analysis */
                PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals);
                return err_data;
            }

            if (adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(p_goals->p_cel_stack[p_goals->count - 1].CEL1, 3)][plcmp_tables_get_synt_rules_stroke_ind(p_goals->p_cel_stack[p_goals->count - 1].CEL1, 3)])
            {
                j = input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(p_goals->p_cel_stack[p_goals->count - 1].CEL1, 3)].VX;
                goto L3;
            }

            L6:

            j = p_goals->p_cel_stack[p_goals->count - 1].CEL3;
            plcmp_goal_remove_last(p_goals);

            goto L3;
        }

        if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)][plcmp_tables_get_synt_rules_stroke_ind(p_goals->p_cel_stack[p_goals->count - 1].CEL1, 3)])
        {
            goto L9;
        }

        plcmp_goal_add_achieved(p_goals_achieved, 
                          synt_rules_table[j].DER,
                          p_goals->p_cel_stack[p_goals->count - 1].CEL2,
                          0,
                          i,
                          j);
        j = input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].VX;
        goto L3;
    }

    if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(&compact_pl1_src_text[i], 1)]
                       [plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)])
    {
        goto L8;
    }

    plcmp_goal_add(p_goals, synt_rules_table[j].DER, i, j);
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

    if ((input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].TYP == 'N' ) && (synt_rules_table[j].PRED > 0))
    {
        plcmp_goal_add(p_goals,
                 p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST1,
                 p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST2,
                 p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST3);

        L10:

        j = p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST5;
        plcmp_goal_remove_last_achieved(p_goals_achieved);
        goto L9;
    }

    if ((input_syms_table[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)].TYP == 'N' ) && (synt_rules_table[j].PRED == 0))
    {

        if (!strcmp(p_goals->p_cel_stack[p_goals->count - 1].CEL1,
                    p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST1))
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

    j = p_goals->p_cel_stack[p_goals->count - 1].CEL3;
    plcmp_goal_remove_last(p_goals);

    if (999 == j)
    {
        PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals);

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
