/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_common.h"
#include "plcmp_tables.h"
#include "plcmp_lex_analyzer.h"
#include "plcmp_synt_analyzer.h"

/* Subroutine adds a new goal into stack of goals */
static void add_goal(cel_t *p_goals, char *T1, int T2, int T3)
{
    #define new p_goals->count

    strcpy(p_goals->cel_stack[new].CEL1, T1);
    p_goals->cel_stack[new].CEL2 = T2;
    p_goals->cel_stack[new].CEL3 = T3;

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
static void add_goal_achieved(dst_t *p_goals_achieved, char *T1, int T2, int T3, int T4, int T5)
{
    #define new p_goals_achieved->count

    strcpy(p_goals_achieved->dst_stack[new].DST1, T1);
    p_goals_achieved->dst_stack[new].DST2 = T2;
    p_goals_achieved->dst_stack[new].DST3 = T3;
    p_goals_achieved->dst_stack[new].DST4 = T4;
    p_goals_achieved->dst_stack[new].DST5 = T5;

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

/* Function of syntax analyzer. It constructs parse tree and return error data if it will be */
struct plcmp_synt_analyzer_error_data_s plcmp_synt_analyzer_syntax_analyzer(char const compact_pl1_src_text[],
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
    int i4 = 0;

    /* Create stack of goals */
    PLCMP_SYNT_ANALYZER_CREATE_GOALS_STACK(goals);

    /* Clear error data structure for later using and set default successful value for error code */
    memset(&err_data, 0, sizeof(plcmp_synt_analyzer_error_data_t));
    err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS;

    add_goal(p_goals, "PRO", i, 999);

    if (!TPR[numb(&compact_pl1_src_text[i], 1)][numb("PRO", 3)])
    {
        PLCMP_SYNT_ANALYZER_DESTROY_GOALS_STACK(goals);

        /* Prepare error data for return to main module */
        err_data.err_code = PLCMP_SYNT_ANALYZER_FAILURE;

        memcpy(err_data.src_text_part, &compact_pl1_src_text[i4], PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }

    L2:

    j = VXOD[numb(&compact_pl1_src_text[i], 1)].VX;

    L3:

    j = SINT[j].POSL;

    L31:

    ++i;

    if (i > i4)
    {
        i4 = i;
    }

    if ('T' == VXOD[numb(SINT[j].DER, 3)].TYP)
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

            if (TPR[numb(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)][numb(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)])
            {
                j = VXOD[numb(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)].VX;
                goto L3;
            }

            L6:

            j = p_goals->cel_stack[p_goals->count - 1].CEL3;
            remove_last_goal(p_goals);

            goto L3;
        }

        if (!TPR[numb(SINT[j].DER, 3)][numb(p_goals->cel_stack[p_goals->count - 1].CEL1, 3)])
        {
            goto L9;
        }

        add_goal_achieved(p_goals_achieved, 
                          SINT[j].DER,
                          p_goals->cel_stack[p_goals->count - 1].CEL2,
                          0,
                          i,
                          j);
        j = VXOD[numb(SINT[j].DER, 3)].VX;
        goto L3;
    }

    if (!TPR[numb(&compact_pl1_src_text[i], 1)][numb(SINT[j].DER, 3)])
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

    if ((VXOD[numb(SINT[j].DER, 3)].TYP == 'N' ) && (SINT[j].PRED > 0))
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

    if ((VXOD[numb(SINT[j].DER, 3)].TYP == 'N' ) && (SINT[j].PRED == 0))
    {

        if (!strcmp(p_goals->cel_stack[p_goals->count - 1].CEL1, p_goals_achieved->dst_stack[p_goals_achieved->count - 1].DST1))
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
        err_data.err_code = PLCMP_SYNT_ANALYZER_FAILURE;
        
        memcpy(err_data.src_text_part, &compact_pl1_src_text[i4], PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN);
        err_data.src_text_part[PLCMP_SYNT_ANALYZER_SRC_TEXT_PART_LEN] = '\0';

        return err_data;
    }
    else
    {
        goto L8;
    }
}

char* plcmp_synt_analyzer_errmsg_by_errdata(plcmp_synt_analyzer_error_data_t const *err_data, char *errmsg)
{
    switch (err_data->err_code)
    {
        case PLCMP_SYNT_ANALYZER_SUCCESS:
        {
            strcpy(errmsg, "No error occured");
            break;
        }
        case PLCMP_SYNT_ANALYZER_FAILURE:
        {
            strcpy(errmsg, "Error in syntax of the source text: ");
            strcat(errmsg, err_data->src_text_part);
            break;
        }
        default:
        {
            strcpy(errmsg, "Unknown error code in error data for generating error message");
            break;
        }
    }
    return errmsg;
}
