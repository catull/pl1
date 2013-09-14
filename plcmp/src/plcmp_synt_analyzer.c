/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_common.h"
#include "plcmp_tables.h"
#include "plcmp_synt_analyzer.h"

/* Subroutine adds a new goal into stack of goals */
static void add_goal(cel_t *p_goals,
                     char const *goal_name,
                     int src_text_begin_index,
                     int src_text_end_index)
{
    unsigned int *restrict count = &p_goals->count;
    strcpy(p_goals->p_cel_stack[*count].CEL1, goal_name);
    p_goals->p_cel_stack[*count].CEL2 = src_text_begin_index;
    p_goals->p_cel_stack[*count].CEL3 = src_text_end_index;
    ++(*count);
}

/* Subroutine removes last goal from the stack of goals */
static void remove_last_goal(cel_t *p_goals)
{
    unsigned int *restrict count = &p_goals->count;
    memset(&p_goals->p_cel_stack[*count], 0, sizeof(p_goals->p_cel_stack[*count]));
    --(*count);
}

/* Subroutine adds a goal achieved into stack of goals achieved */
static void add_goal_achieved(dst_t *p_goals_achieved,
                              char const *goal_achieved_name,
                              int src_text_begin_index,
                              int sint_index,
                              int src_text_end_index,
                              int next_goal_sint_index)
{
    unsigned int *restrict count = &p_goals_achieved->count;
    strcpy(p_goals_achieved->p_dst_stack[*count].DST1, goal_achieved_name);
    p_goals_achieved->p_dst_stack[*count].DST2 = src_text_begin_index;
    p_goals_achieved->p_dst_stack[*count].DST3 = sint_index;
    p_goals_achieved->p_dst_stack[*count].DST4 = src_text_end_index;
    p_goals_achieved->p_dst_stack[*count].DST5 = next_goal_sint_index;
    ++(*count);
}

/* Subroutine removes last goal achieved from the stack of goals achieved */
static void remove_last_goal_achieved(dst_t *p_goals_achieved)
{
    unsigned int *restrict count = &p_goals_achieved->count;
    memset(&p_goals_achieved->p_dst_stack[*count],
           0,
           sizeof(p_goals_achieved->p_dst_stack[*count]));
    --(*count);
}

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

    /* Construct adjacency matrix */
    plcmp_tables_build_reach_mtrx();

    add_goal(p_goals, "PRO", i, 999);

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
            add_goal_achieved(p_goals_achieved,
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
            remove_last_goal(p_goals);

            goto L3;
        }

        if (!adj_reach_mtrx[plcmp_tables_get_synt_rules_stroke_ind(synt_rules_table[j].DER, 3)][plcmp_tables_get_synt_rules_stroke_ind(p_goals->p_cel_stack[p_goals->count - 1].CEL1, 3)])
        {
            goto L9;
        }

        add_goal_achieved(p_goals_achieved, 
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

    add_goal(p_goals, synt_rules_table[j].DER, i, j);
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
        add_goal(p_goals,
                 p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST1,
                 p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST2,
                 p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST3);

        L10:

        j = p_goals_achieved->p_dst_stack[p_goals_achieved->count - 1].DST5;
        remove_last_goal_achieved(p_goals_achieved);
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
    remove_last_goal(p_goals);

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

/* Subroutine constructs error message by error data of syntax analyzer module */
char* plcmp_synt_analyzer_errmsg_by_errdata(plcmp_synt_analyzer_error_data_t const *err_data, char *errmsg)
{
    switch (err_data->err_code)
    {
        case PLCMP_SYNT_ANALYZER_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case PLCMP_SYNT_ANALYZER_SYNTAX_ERROR:
            strcpy(errmsg, "Error in syntax of the source text: ");
            strcat(errmsg, err_data->src_text_part);
            break;
        default:
            strcpy(errmsg, "Unknown error code in error data for generating error message");
            break;
    }
    return errmsg;
}
