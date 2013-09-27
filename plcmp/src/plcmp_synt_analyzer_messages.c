/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_common.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_synt_analyzer_messages.h"

/* Subroutine constructs error message by 
 * error data of syntax analyzer module */
char* plcmp_synt_analyzer_messages_errmsg_by_errdata(
    plcmp_synt_analyzer_error_data_t const *err_data,
    char *errmsg)
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