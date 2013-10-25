/* encoding: UTF-8 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "plcmp_utils.h"
#include "plcmp_target.h"
#include "plcmp_sem_calc.h"
#include "plcmp_sem_calc_handlers.h"
#include "plcmp_symbols.h"
#include "plcmp_tables.h"

/* This array of structures is the table of labels' names of variables
 * which is being filled on the first process of semantic calculation
 * and is being used on the second process of semantic calculation 
 */
struct sym_s SYM[NSYM];

/* Count of SYM-s */
int ISYM = 0;

/* Pointer to compact source text */
char const *g_p_compact_pl1_src_text;

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
char FORMT[MAXFORMT][9];
/* Formatted array index */
int IFORMT;

/* Subroutine prepares error data for sending its to caller
 * Error code in 'p_err_data->err_code' controls what kind of data will be written */
static void cook_error_data(plcmp_sem_calc_error_data_t *p_err_data, 
                            target_achieved_t target_achieved)
{
    switch (p_err_data->err_code)
    {
        case PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR:
            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR:
            (void)memcpy(
                p_err_data->src_text_part,
                &g_p_compact_pl1_src_text[target_achieved.src_text_left_i],
                PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN]
                = '\0';

            (void)strcpy(p_err_data->data.identifier_type, FORMT[1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR:
            (void)memcpy(
                    p_err_data->src_text_part,
                    &g_p_compact_pl1_src_text[target_achieved.src_text_left_i],
                    PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN]
                = '\0';

            (void)strcpy(p_err_data->data.identifier_type, FORMT[IFORMT - 1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR:
            (void)memcpy(
                    p_err_data->src_text_part,
                    &g_p_compact_pl1_src_text[target_achieved.src_text_left_i],
                    PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN]
                = '\0';

            (void)strcpy(p_err_data->data.identifier, FORMT[IFORMT - 1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR:
        {
            size_t formt_len = strlen(FORMT[IFORMT - 1]);
            size_t oper_len = 0;
            (void)memcpy(
                    p_err_data->src_text_part,
                    &g_p_compact_pl1_src_text[target_achieved.src_text_left_i],
                    PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN]
                = '\0';

            p_err_data->data.operation[oper_len] = 
                g_p_compact_pl1_src_text[target_achieved.src_text_right_i
                    - formt_len];
            ++oper_len;
            if ('!' == g_p_compact_pl1_src_text[target_achieved.src_text_right_i
                    - formt_len] &&
                '!' == g_p_compact_pl1_src_text[target_achieved.src_text_right_i
                    - formt_len + 1])
            {
                p_err_data->data.operation[oper_len] = 
                    g_p_compact_pl1_src_text[target_achieved.src_text_right_i
                        - formt_len + 1];
                ++oper_len;
            }
            p_err_data->data.operation[oper_len] = '\0';

            break;
        }
        case PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR:
            (void)memcpy(
                    p_err_data->src_text_part,
                    &g_p_compact_pl1_src_text[target_achieved.src_text_left_i],
                    PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN]
                = '\0';

            (void)strcpy(p_err_data->data.identifier, FORMT[1]);

            break;
        case PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE_ERROR:
            break;
        case PLCMP_SEM_CALCULATOR_CONCAT_ERROR:
            break;
        case PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG_ERROR:
            break;
        default:
            break;
    }
}

char const* plcmp_sem_calc_errmsg_by_errdata(
    plcmp_sem_calc_error_data_t const *err_data,
    char errmsg[])
{
    PLCMP_UTILS_ASSERT(err_data && errmsg);

    switch (err_data->err_code)
    {
        case PLCMP_SEM_CALCULATOR_SUCCESS:
        {
            (void)strcpy(errmsg, "No error occured");
        }
        break;
        case PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR:
        {
            (void)strcpy(errmsg,
                   "Mismatch of the name of the "
                   "procedure in prologue-epilogue");
        }
        break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR:
        {
            (void)strcats(errmsg,
                    "Not allowed identifier type '",
                    err_data->data.identifier_type,
                    "' in the source text '",
                    err_data->src_text_part,
                    "'",
                    NULL);
        }
        break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR:
        {
            (void)strcats(errmsg,
                    "Not allowed identifier type '",
                    err_data->data.identifier_type,
                    "' in the source text '",
                    err_data->src_text_part,
                    "'",
                    NULL);
        }
        break;
        case PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR:
        {
            (void)strcats(errmsg,
                    "Not determined identifier '",
                    err_data->data.identifier,
                    "' in the source text '",
                    err_data->src_text_part,
                    "'",
                    NULL);
        }
        break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR:
        {
            (void)strcats(errmsg,
                    "Not allowed operation '",
                    err_data->data.operation,
                    "' in the source text '",
                    err_data->src_text_part,
                    "'",
                    NULL);
        }
        break;
        case PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR:
        {
            (void)strcats(errmsg,
                    "Repeated declaration of the identifier '",
                    err_data->data.identifier,
                    "' in the source text '",
                    err_data->src_text_part,
                    "'",
                    NULL);
        }
        break;
        case PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE_ERROR:
        {
            (void)strcpy(errmsg, "Can't write to assembler file");
        }
        break;
        case PLCMP_SEM_CALCULATOR_CONCAT_ERROR:
        {
            (void)strcpy(errmsg, 
                   "Capacity of the destination string is less than "
                   "total length of strings which are being concatenated");
        }
        break;
        case PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG_ERROR:
        {
            (void)strcpy(errmsg,
                         "String initializer is very long for this capacity");
        }
        break;
        default:
        {
            (void)strcpy(errmsg,
                         "Unknown error code for generating error message");
        }
        break;
    }
    return errmsg;
}

struct plcmp_sem_calc_error_data_s  plcmp_sem_calc_gen_asm_code(
    char const p_asm_fp_name[],
    char const compact_pl1_src_text[],
    targets_achieved_stack_t const *p_targets_achieved)
{
    int phase = 0;
    plcmp_sem_calc_error_data_t err_data;

    /* Set global pointer to compact text for easier working with */
    g_p_compact_pl1_src_text = compact_pl1_src_text;

    /* Clear error data structure for later using and set default successful error code */
    memset(&err_data, 0, sizeof(plcmp_sem_calc_error_data_t));
    err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS;

    plcmp_sem_calc_handler_t *handler[SYM_NTERMS_COUNT] = {
        /*    1  */ AVI,
        /*    2  */ BUK,
        /*    3  */ CIF,
        /*    4  */ IDE,
        /*    5  */ IPE,
        /*    6  */ IPR,
        /*    7  */ LIT,
        /*    8  */ MAN,
        /*    9  */ ODC,
        /*   10  */ OEN,
        /*   11  */ OPA,
        /*   12  */ OPR,
        /*   13  */ PRO,
        /*   14  */ RZR,
        /*   15  */ TEL,
        /*   16  */ ZNK,
        /*   17  */ STC,
        /*   18  */ CON
    };

    /* First and second phases of semantic calculation */
    for (phase = SEM_CALC_FIRST_PHASE;
         phase < SEM_CALC_COUNT_PHASES;
         phase = SEM_CALC_NEXT_PHASE(phase))
    {
        unsigned int dst_index = 0;
        for (dst_index = 0; dst_index < p_targets_achieved->count; dst_index++)
        {
            index_t hand_num = p_targets_achieved->stack[dst_index].sym;
            switch (hand_num + 1)
            {
                /* PRO */
                case 13:
                    err_data.err_code = 
                        handler[hand_num](phase, p_asm_fp_name);
                    break;
                /* other */
                default:
                    err_data.err_code =
                        handler[hand_num](
                            phase,
                            &p_targets_achieved->stack[dst_index]);
                    break;
            }
            
            if (PLCMP_SEM_CALCULATOR_SUCCESS != err_data.err_code)
            {
                /* Error occured while calculation superposition 
                 * of the functions of the targets achieved.
                 * Prepare and send error data to the caller 
                 */
                cook_error_data(&err_data, p_targets_achieved->stack[dst_index]);
                goto error;
            }
        }
    }

    error:
    return err_data;
}