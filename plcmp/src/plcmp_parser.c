/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>

#include "plcmp_parser.h"
#include "plcmp_parser_sm.h"
#include "plcmp_tables.h"
#include "plcmp_target.h"
#include "utils.h"

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
            err_data.err_code = PLCMP_PARSER_WRONG_ERROR_CODE_TRANSF_ERROR;
            break;
        default:
            err_data.err_code = PLCMP_PARSER_UNKNOWN_ERROR;
            break;
    }

    switch (sm_err_code)
    {
        case PLCMP_PARSER_SM_SUCCESS:
        case PLCMP_PARSER_SM_UNITIALIZED_ESSENTIAL_PARAMS_ERROR:
            err_data.src_text_part[0] = '\0';
            break;
        case PLCMP_PARSER_SM_SYNTAX_ERROR:
        case PLCMP_PARSER_SM_WRONG_STATE_ERROR:
        default:
            strncpy(err_data.src_text_part,
                    p_src_text,
                    PLCMP_PARSER_SRC_TEXT_PART_LEN);
            err_data.src_text_part[PLCMP_PARSER_SRC_TEXT_PART_LEN] = '\0';
            break;
    }

    return err_data;
}

char const* plcmp_parser_errmsg_by_errdata(
    plcmp_parser_error_data_t const *err_data,
    char errmsg[])
{
    switch (err_data->err_code)
    {
        case PLCMP_PARSER_SUCCESS:
            (void)strcpy(errmsg, "No error occured");
            break;
        case PLCMP_PARSER_SYNTAX_ERROR:
            (void)strcpy(errmsg, "Error in syntax of the source text.");
            break;
        case PLCMP_PARSER_INTERNAL_ERROR:
            (void)strcpy(errmsg, "Internal error of syntax analyzer occurred.");
            break;
        case PLCMP_PARSER_WRONG_ERROR_CODE_TRANSF_ERROR:
            (void)strcpy(errmsg, "Internal error of formatting data.");
            break;
        case PLCMP_PARSER_UNKNOWN_ERROR:
            (void)strcpy(errmsg, "Unknown error");
            break;
        default:
            (void)strcpy(errmsg,
                   "Unknown error code in error data "
                   "for generating error message.");
            break;
    }

    return utils_strcats(errmsg,
                   " Part of source text: ",
                   err_data->src_text_part,
                   NULL);
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
    extern int g_cur_src_i;
    extern int g_src_indmax;

    UTILS_ASSERT(p_targets_achieved);

    plcmp_tables_build_reach_mtrx();
    plcmp_tables_init_ascii_relation();

    g_targets_interim = plcmp_target_create_targets_interim_stack();
    g_targets_achieved = plcmp_target_create_targets_achieved_stack();
    g_p_src_text = compact_pl1_src_text;
    g_cur_src_i = 0;

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
