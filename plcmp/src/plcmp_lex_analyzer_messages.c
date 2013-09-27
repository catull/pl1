/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_lex_analyzer.h"
#include "plcmp_lex_analyzer_messages.h"

/* Subroutine constructs error message 
 * by error code of lexical analyzer module */
char* plcmp_lex_analyzer_messages_errmsg_by_errdata(
    plcmp_lex_analyzer_error_data_t const *err_data,
    char *errmsg)
{
    switch (err_data->err_code)
    {
        case PLCMP_LEX_ANALYZER_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case PLCMP_LEX_ANALYZER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW:
            strcpy(errmsg,
                   "Overflow of the compact text buffer "
                   "while lexical analysis was processing");
            break;
        default:
            strcpy(errmsg, "Unknown error code for generating error message");
            break;
    }
    return errmsg;
}