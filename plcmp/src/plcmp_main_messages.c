/* encoding: UTF-8 */

#include "plcmp_main_messages.h"
#include "plcmp_main_error.h"

#include "plcmp_lex_analyzer.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_sem_calc.h"

/* Subroutine constructs error message by error code of main module */
static inline char const* plcmp_main_messages_errmsg_by_errcode(
    plcmp_main_error_code_t err_code)
{
    switch (err_code)
    {
        case PLCMP_MAIN_SUCCESS:
            return "No error occured";
        case PLCMP_MAIN_WRONG_NUM_CLI_PAR_ERROR:
            return "Wrong number of command line parameters";
        case PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH_ERROR:
            return "Wrong path to PL1-file with the source text";
        case PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION_ERROR:
            return "Wrong input file extension with the source text";
        case PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE_ERROR:
            return "Couldn't find file with the source text";
        case PLCMP_MAIN_READING_PL1_FILE_ERROR:
            return "Error occured while reading file with the source text";
        case PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW_ERROR:
            return "Overflow of the program buffer "
                   "while reading file with the source text";
        case PLCMP_MAIN_LEX_ANALYZER_ERROR:
            return "Error in lexical analyzer";
        case PLCMP_MAIN_SYNT_ANALYZER_ERROR:
            return "Error in syntax analyzer";
        case PLCMP_MAIN_SEM_CALCULATOR_ERROR:
            return "Error in semantic calculator";
        default:
            return "Unknown error code for generating error message";
    }
}

/* Subroutine prints translation's result using main error data */
void plcmp_main_messages_print_translation_result(
    plcmp_main_error_data_t const *err_data)
{
    if (PLCMP_MAIN_SUCCESSFUL_TRANSLATION == err_data->main_err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        char errmsg[100] = { '\0' };

        printf("Translation is interrupted\nReason: %s\n",
               plcmp_main_messages_errmsg_by_errcode(err_data->main_err_code));
        switch(err_data->main_err_code)
        {
            case PLCMP_MAIN_LEX_ANALYZER_ERROR:
                printf("Lexical analyzer error message: %s\n",
                       plcmp_lex_analyzer_errmsg_by_errdata(
                           &err_data->lex_analyzer_err_data,
                           errmsg));
                break;
            case PLCMP_MAIN_SYNT_ANALYZER_ERROR:
                printf("Syntax analyzer error message: %s\n",
                       plcmp_synt_analyzer_errmsg_by_errdata(
                           &err_data->synt_analyzer_err_data,
                           errmsg));
                break;
            case PLCMP_MAIN_SEM_CALCULATOR_ERROR:
                printf("Semantic calculator error message: %s\n",
                       plcmp_sem_calc_errmsg_by_errdata(
                           &err_data->sem_calc_err_data,
                           errmsg));
                break;
            default:
                break;
        }
    }
}