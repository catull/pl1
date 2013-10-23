/* encoding: UTF-8 */

#include <string.h>

#include "plcmp_utils.h"
#include "plcmp_sem_calc.h"
#include "plcmp_sem_calc_first_handlers.h"

extern char const *g_p_compact_pl1_src_text;

extern struct sym_s SYM[NSYM];
extern int ISYM;

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
extern char FORMT[MAXFORMT][9];
/* Formatted array index */
extern int IFORMT;

static void FORM(target_achieved_t achieved_target)
{
    int i, j;

    for (IFORMT = 0; IFORMT < MAXFORMT; IFORMT++)
    {
        memset(FORMT[IFORMT], '\0', sizeof(FORMT[IFORMT]));
    }

    IFORMT = 0;
    j = achieved_target.src_text_left_i;

    FORM1:

    for (i = j; i <= achieved_target.src_text_right_i + 1; i++)
    {

        if ('\'' == g_p_compact_pl1_src_text[i] ||
            ':'  == g_p_compact_pl1_src_text[i] ||
            ' '  == g_p_compact_pl1_src_text[i] ||
            '('  == g_p_compact_pl1_src_text[i] ||
            ')'  == g_p_compact_pl1_src_text[i] ||
            ';'  == g_p_compact_pl1_src_text[i] ||
            '+'  == g_p_compact_pl1_src_text[i] ||
            '-'  == g_p_compact_pl1_src_text[i] ||
            '='  == g_p_compact_pl1_src_text[i] ||
            '*'  == g_p_compact_pl1_src_text[i])
        {
            FORMT[IFORMT][i - j] = '\0';
            IFORMT++;
            j = i + 1;
            goto FORM1;
        }
        else if ('!' == g_p_compact_pl1_src_text[i] &&
                 '!' == g_p_compact_pl1_src_text[i + 1])
        {
            FORMT[IFORMT][i - j] = '\0';
            IFORMT++;
            j = i + 2;
            goto FORM1;
        }
        else
        {

            FORMT[IFORMT][i - j] = g_p_compact_pl1_src_text[i];
        }
    }

    return;
}

enum plcmp_sem_calc_error_code_e FAVI(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FBUK(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FCIF(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FIDE(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FIPE(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FIPR(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FLIT(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FMAN(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FODC(void const *param)
{
    int i;
    int init_pos;
    target_achieved_t target_achieved = *((target_achieved_t*)param);

    FORM(target_achieved);

    for (i = 0; i < ISYM; i++)
    {
        if (!strcmp(SYM[i].name, FORMT[1])
            && strlen(SYM[i].name) == strlen(FORMT[1]))
        {
            return PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR;
        }
    }

    strcpy(SYM[ISYM].name, FORMT[1]);

    if (!(strcmp(FORMT[2], "BIN") || strcmp(FORMT[3], "FIXED")))
    {
        init_pos = 5;
        SYM[ISYM].type = 'B';
    }
    else if (!strcmp(FORMT[2], "CHAR"))
    {
        init_pos = 4;
        SYM[ISYM].type = 'C';
    }
    else
    {
        SYM[ISYM].type = 'U';
        return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR;
    }

    SYM[ISYM].capacity = atoi(FORMT[init_pos - 1]);

    if (!strcmp(FORMT[init_pos], "INIT"))
    {
        switch(SYM[ISYM].type)
        {
            case 'B':
                strcpy(SYM[ISYM].INIT, FORMT[init_pos + 1]);           
                break;
            case 'C':
            {
                #define init_value_pos (init_pos + 2)

                SYM[ISYM].char_init_len = strlen(FORMT[init_value_pos]);

                if (SYM[ISYM].capacity < SYM[ISYM].char_init_len)
                {
                    return PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG_ERROR;
                }

                memcpy(SYM[ISYM].INIT, FORMT[init_value_pos], SYM[ISYM].char_init_len);
                memset(&SYM[ISYM].INIT[SYM[ISYM].char_init_len], ' ', SYM[ISYM].capacity - SYM[ISYM].char_init_len);
                SYM[ISYM].INIT[SYM[ISYM].capacity] = '\0';

                #undef init_value_pos

                break;
            }
            default:
                return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR;
        }
    }
    else
    {
        switch(SYM[ISYM].type)
        {
            case 'B':
                strcpy(SYM[ISYM].INIT, "0B");
                break;
            case 'C':
                memset(SYM[ISYM].INIT, ' ', SYM[ISYM].capacity);
                SYM[ISYM].INIT[SYM[ISYM].capacity] = '\0';
                break;
            default:
                return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR;
        }
    }

    ++ISYM;

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FOEN(void const *param)
{
    target_achieved_t target_achieved = *((target_achieved_t*)param);
    unsigned int i = 0;

    FORM(target_achieved);

    for (i = 0; i < ISYM; i++)
    {
        if (!strcmp(SYM[i].name, FORMT[1]) && (SYM[i].type == 'P'))
        {
            return PLCMP_SEM_CALCULATOR_SUCCESS;
        }
    }

    return PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR;
}

enum plcmp_sem_calc_error_code_e FOPA(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FOPR(void const *param)
{
    target_achieved_t target_achieved = *((target_achieved_t*)param);

    FORM(target_achieved);

    /* Save program name as identifier */
    strcpy(SYM[ISYM].name, FORMT[0]);
    /* Set type 'P' */
    SYM[ISYM].type = 'P';
    /* Capacity is 0 */
    SYM[ISYM].capacity = 0;
    ++ISYM;

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FPRO(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FRZR(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FTEL(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FZNK(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FSTC(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e FCON(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}