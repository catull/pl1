/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_sem_calc_messages.h"

/* Subroutine constructs error message by 
 * error data of semantic calculator module */
char* plcmp_sem_calc_messages_errmsg_by_errdata(
    plcmp_sem_calc_error_data_t const *err_data,
    char *errmsg)
{
    switch (err_data->err_code)
    {
        case PLCMP_SEM_CALCULATOR_SUCCESS:
        {
            strcpy(errmsg, "No error occured");
            break;
        }
        case PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR:
        {
            strcpy(errmsg,
                   "Mismatch of the name of the "
                   "procedure in prologue-epilogue");
            break;
        }
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR:
        {
            strcpy(errmsg, "Not allowed identifier type '");
            strcat(errmsg, err_data->data.identifier_type);
            strcat(errmsg, "' in the source text '");
            strcat(errmsg, err_data->src_text_part);
            strcat(errmsg, "'");
            break;
        }
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR:
        {
            strcpy(errmsg, "Not allowed identifier type '");
            strcat(errmsg, err_data->data.identifier_type);
            strcat(errmsg, "' in the source text '");
            strcat(errmsg, err_data->src_text_part);
            strcat(errmsg, "'");
            break;
        }
        case PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR:
        {
            strcpy(errmsg, "Not determined identifier '");
            strcat(errmsg, err_data->data.identifier);
            strcat(errmsg, "' in the source text '");
            strcat(errmsg, err_data->src_text_part);
            strcat(errmsg, "'");
            break;
        }
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR:
        {
            strcpy(errmsg, "Not allowed operation '");
            strcat(errmsg, err_data->data.operation);
            strcat(errmsg, "' in the source text '");
            strcat(errmsg, err_data->src_text_part);
            strcat(errmsg, "'");
            break;
        }
        case PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR:
        {
            strcpy(errmsg, "Repeated declaration of the identifier '");
            strcat(errmsg, err_data->data.identifier);
            strcat(errmsg, "' in the source text '");
            strcat(errmsg, err_data->src_text_part);
            strcat(errmsg, "'");
            break;
        }
        case PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE_ERROR:
        {
            strcpy(errmsg, "Can't write to assembler file");
            break;
        }
        case PLCMP_SEM_CALCULATOR_CONCAT_ERROR:
        {
            strcpy(errmsg, 
                   "Capacity of the destination string is less than "
                   "total length of strings which are being concatenated");
            break;
        }
        case PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG_ERROR:
        {
            strcpy(errmsg, "String initializer is very long for this capacity");
            break;
        }
        default:
        {
            strcpy(errmsg, "Unknown error code for generating error message");
            break;
        }
    }
    return errmsg;
}