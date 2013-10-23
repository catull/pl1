/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_target.h"
#include "plcmp_parser.h"
#include "plcmp_parser_sm.h"
#include "plcmp_tables.h"
#include "plcmp_utils.h"

static struct plcmp_parser_error_data_s plcmp_parser_form_err_data(
    plcmp_parser_sm_error_code_t sm_err_code,
    char const *p_src_text)
{
    plcmp_parser_error_data_t err_data;
    memset(&err_data, 0, sizeof(err_data));

    switch (sm_err_code)
    {
        case PLCMP_PARSER_SM_SYNTAX_ERROR:
            err_data.err_code = PLCMP_PARSER_SYNTAX_ERROR;
            break;
        case PLCMP_PARSER_SM_WRONG_STATE_ERROR:
        case PLCMP_PARSER_SM_UNITIALIZED_ESSENTIAL_PARAMS_ERROR:
            err_data.err_code = PLCMP_PARSER_INTERNAL_ERROR;
            break;
        case PLCMP_PARSER_SM_SUCCESS:
        default:
            PLCMP_UTILS_ASSERT(0);
            break;
    }

    strncpy(err_data.src_text_part, p_src_text, PLCMP_PARSER_SRC_TEXT_PART_LEN);
    err_data.src_text_part[PLCMP_PARSER_SRC_TEXT_PART_LEN] = '\0';

    return err_data;
}

char const* plcmp_parser_errmsg_by_errdata(
    plcmp_parser_error_data_t const *err_data,
    char errmsg[])
{
    switch (err_data->err_code)
    {
        case PLCMP_PARSER_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case PLCMP_PARSER_SYNTAX_ERROR:
            strcats(errmsg,
                    "Error in syntax of the source text: ",
                    err_data->src_text_part,
                    NULL);
            break;
        case PLCMP_PARSER_INTERNAL_ERROR:
            strcats(errmsg,
                    "Internal error of syntax analyzer occurred. "
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

struct plcmp_parser_error_data_s plcmp_parser_syntax_analysis(
    char const compact_pl1_src_text[],
    targets_achieved_stack_t **p_targets_achieved)
{
    plcmp_parser_error_data_t err_data = {
        .err_code = PLCMP_PARSER_SUCCESS
    };
    plcmp_parser_sm_error_code_t sm_err_code = PLCMP_PARSER_SM_SUCCESS;

    /* External variables of parser's state machine */
    extern targets_interim_stack_t *g_targets_interim;
    extern targets_achieved_stack_t *g_targets_achieved;
    extern char const *g_p_src_text;
    extern int g_csrc_ind;
    extern int g_src_indmax;

    PLCMP_UTILS_ASSERT(p_targets_achieved);

    plcmp_tables_build_reach_mtrx();
    plcmp_tables_init_ascii_relation();

    g_targets_interim = plcmp_target_create_targets_interim_stack();
    g_targets_achieved = plcmp_target_create_targets_achieved_stack();
    g_p_src_text = compact_pl1_src_text;
    g_csrc_ind = 0;

    /* Run state machine */
    sm_err_code = plcmp_parser_sm_run();
    if (PLCMP_PARSER_SM_SUCCESS != sm_err_code)
    {
        if (g_src_indmax < 0)
        {
            g_src_indmax = 0;
        }
        err_data = plcmp_parser_form_err_data(sm_err_code,
                                              &g_p_src_text[g_src_indmax]);
    }

    plcmp_target_destroy_targets_interim_stack(&g_targets_interim);
    if (PLCMP_PARSER_SUCCESS != err_data.err_code)
    {
        plcmp_target_destroy_targets_achieved_stack(&g_targets_achieved);
    }
    *p_targets_achieved = g_targets_achieved;

    plcmp_parser_sm_clear_params();

    return err_data;
}
