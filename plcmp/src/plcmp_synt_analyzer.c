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
    #define new p_goals->count

    strcpy(p_goals->cel_stack[new].CEL1, goal_name);
    p_goals->cel_stack[new].CEL2 = src_text_begin_index;
    p_goals->cel_stack[new].CEL3 = src_text_end_index;

    #undef new

    ++p_goals->count;
}

/* Subroutine removes last goal from the stack of goals */
static void remove_last_goal(cel_t *p_goals)
{
    #define last p_goals->count - 1

    memset(p_goals->cel_stack[last].CEL1, '\0', sizeof(p_goals->cel_stack[last].CEL1));
    p_goals->cel_stack[last].CEL2 = 0;
    p_goals->cel_stack[last].CEL3 = 0;

    #undef last

    --p_goals->count;
}

/* Subroutine adds a goal achieved into stack of goals achieved */
static void add_goal_achieved(dst_t *p_goals_achieved,
                              char const *goal_achieved_name,
                              int src_text_begin_index,
                              int sint_index,
                              int src_text_end_index,
                              int next_goal_sint_index)
{
    #define new p_goals_achieved->count

    strcpy(p_goals_achieved->dst_stack[new].DST1, goal_achieved_name);
    p_goals_achieved->dst_stack[new].DST2 = src_text_begin_index;
    p_goals_achieved->dst_stack[new].DST3 = sint_index;
    p_goals_achieved->dst_stack[new].DST4 = src_text_end_index;
    p_goals_achieved->dst_stack[new].DST5 = next_goal_sint_index;

    #undef new

    ++p_goals_achieved->count;
}

/* Subroutine removes last goal achieved from the stack of goals achieved */
static void remove_last_goal_achieved(dst_t *p_goals_achieved)
{
    #define last p_goals_achieved->count - 1

    memset(p_goals_achieved->dst_stack[last].DST1, '\0', sizeof(p_goals_achieved->dst_stack[last].DST1));
    p_goals_achieved->dst_stack[last].DST2 = 0;
    p_goals_achieved->dst_stack[last].DST3 = 0;
    p_goals_achieved->dst_stack[last].DST4 = 0;
    p_goals_achieved->dst_stack[last].DST5 = 0;

    #undef last

    --p_goals_achieved->count;
}

/* Subroutine constructs table of the
 * successors of the adjacency matrix
 * of the Uorshell's algorithm
 */
static void build_tpr(void)
{
    int i1;
    for (i1 = 0; i1 < NNETRM; i1++)
    {
        int i2;
        for (i2 = 0; i2 < NVXOD; i2++)
        {
            if (TPR[i2][i1] && (i1 != i2))
            {
                int i3;
                for (i3 = 0; i3 < NNETRM; i3++)
                {
                    TPR[i2][i3] |= TPR[i1][i3];
                }
            }
        }
    }
}

/* Subroutine of syntax analyzer. It constructs parse tree and returns error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analysis(char const compact_pl1_src_text[],
                                                                            dst_t *p_goals_achieved)
{
    /* It's stack of goals and pointer on it.
     * Later stack will be normally created by macro */
    cel_t goals, *p_goals = &goals;

    plcmp_synt_analyzer_error_data_t err_data;

    /* Current index in the compact text */
    int i = 0;
    /* Current index in the table of grammar rules */
    int j;
    /* Maximum index of processed compact source text 
     * It needs for sending part of source text if error will occur */
    int i_max = 0;

    /* Create stack of goals */
    PLCMP_SYNT_ANALYZER_CREATE_GOALS_STACK(goals);

    /* Clear error data structure for later using and set default successful value for error code */
    memset(&err_data, 0, sizeof(plcmp_synt_analyzer_error_data_t));
    err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS;

    /* Construct adjacency matrix */
    build_tpr();

    add_goal(p_goals, "PRO", i, 999);

    if (!TPR[plcmp_tables_sym_sint_ind(&compact_pl1_src_text[i], 1)][plcmp_tables_sym_sint_ind("PRO", 3)])
    {
        PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals);

        /* Prepare error data for return to main module */
        err_data.err_code = PLCMP_SYNT_ANALYZER_SYNTAX_ERROR;

        memcpy(err_data.src_text_part, &compact_pl1_src_text[i_max], PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }

    L2:

    j = VXOD[plcmp_tables_sym_sint_ind(&compact_pl1_src_text[i], 1)].VX;

    L3:

    j = SINT[j].POSL;

    L31:

    ++i;

    if (i > i_max)
    {
        i_max = i;
    }

    if ('T' == VXOD[plcmp_tables_sym_sint_ind(SINT[j].DER, 3)].TYP)
    {
        if (compact_pl1_src_text[i] == SINT[j].DER[0])
        {
            goto L3;
        }
        else
        {
            goto L8;
        }
    }

    if ('*' == SINT[SINT[j].POSL].DER[0])
    {
        --i;

        if (!strcmp(SINT[j].DER, p_goals->cel_stack[p_goals->count - 1].CEL1))
        {
            add_goal_achieved(p_goals_achieved,
                              p_goals->cel_stack[p_goals->count - 1].CEL1,
                              p_goals->cel_stack[p_goals->count - 1].CEL2,
                              p_goals->cel_stack[p_goals->count - 1].CEL3,
                              i,
                              j);

            if (!strcmp(p_goals->cel_stack[p_goals->count - 1].CEL1, "PRO"))
            {
                /* Successful finish of the syntax analysis */
                PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals);
                return err_data;
            }

            if (TPR[plcmp_tables_sym_sint_ind(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)][plcmp_tables_sym_sint_ind(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)])
            {
                j = VXOD[plcmp_tables_sym_sint_ind(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)].VX;
                goto L3;
            }

            L6:

            j = p_goals->cel_stack[p_goals->count - 1].CEL3;
            remove_last_goal(p_goals);

            goto L3;
        }

        if (!TPR[plcmp_tables_sym_sint_ind(SINT[j].DER, 3)][plcmp_tables_sym_sint_ind(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)])
        {
            goto L9;
        }

        add_goal_achieved(p_goals_achieved, 
                          SINT[j].DER,
                          p_goals->cel_stack[p_goals->count - 1].CEL2,
                          0,
                          i,
                          j);
        j = VXOD[plcmp_tables_sym_sint_ind(SINT[j].DER, 3)].VX;
        goto L3;
    }

    if (!TPR[plcmp_tables_sym_sint_ind(&compact_pl1_src_text[i], 1)][plcmp_tables_sym_sint_ind(SINT[j].DER, 3)])
    {
        goto L8;
    }

    add_goal(p_goals, SINT[j].DER, i, j);
    goto L2;

    L8:

    --i;

    L9:

    if (SINT[j].ALT != 0)
    {
        j = SINT[j].ALT;
        goto L31;
    }

    j = SINT[j].PRED;

    if ((VXOD[plcmp_tables_sym_sint_ind(SINT[j].DER, 3)].TYP == 'N' ) && (SINT[j].PRED > 0))
    {
        add_goal(p_goals,
                 p_goals_achieved->dst_stack[p_goals_achieved->count - 1].DST1,
                 p_goals_achieved->dst_stack[p_goals_achieved->count - 1].DST2,
                 p_goals_achieved->dst_stack[p_goals_achieved->count - 1].DST3);

        L10:

        j = p_goals_achieved->dst_stack[p_goals_achieved->count - 1].DST5;
        remove_last_goal_achieved(p_goals_achieved);
        goto L9;
    }

    if ((VXOD[plcmp_tables_sym_sint_ind(SINT[j].DER, 3)].TYP == 'N' ) && (SINT[j].PRED == 0))
    {

        if (!strcmp(p_goals->cel_stack[p_goals->count - 1].CEL1,
                    p_goals_achieved->dst_stack[p_goals_achieved->count - 1].DST1))
        {
            goto L6;
        }
        else
        {
            goto L10;
        }

    }

    if (SINT[j].PRED > 0)
    {
        goto L8;
    }

    j = p_goals->cel_stack[p_goals->count - 1].CEL3;
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
