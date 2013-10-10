/* encoding: UTF-8 */

#include <string.h>
#include <ctype.h>

#include "plcmp_utils.h"
#include "plcmp_sem_calc.h"
#include "plcmp_sem_calc_second_handlers.h"

extern struct sym_s SYM[NSYM];
extern int ISYM;

extern char const *g_p_compact_pl1_src_text;

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
extern char FORMT[MAXFORMT][9];
/* Formatted array index */
extern int IFORMT;

/* Array of pointers to SYM-s for processing concat operation for string */
static struct sym_s *p_char_syms[9];
/* Count of 'p_char_syms' */
static int char_syms_size = 0;

/* This struct is assembler card
 * It is template to generate the output file record by IBM 370 assembler */
static struct assembler_card_s {
    char METKA[8];
    char PROB1;
    char OPERAC[5];
    char PROB2;
    char OPERAND[12];
    char PROB3;
    char COMM[52];
} assembler_card;

/* Array for storage of output text written in IBM 370 ASSEMBLER */
static char assembler_out_text[MAXLTXT][80];
/* Output double array row index */
static int IASSTXT;

/* Subroutine clears assembler card.
 * Fresh card should contains ' ' (space symbols) */
static void clear_assembler_card(void)
{
    memset(&assembler_card, ' ', sizeof(assembler_card));
}

/* Record new assembler command by assembler card
 * into assembler commands' array 
 * and clear the temporary assembler card 
 */
static void record_assembler_card(void)
{
    memcpy(assembler_out_text[IASSTXT], &assembler_card, 80);
    ++IASSTXT;
    clear_assembler_card();
}

/* Function translates 'long int' value written as string of ASCII-code
 * into normal 'long int' internal representation of machine */
static long int long_int_str_to_long_int(char const *str_long_value)
{
    long int long_value = 0;
    int i = 0;

    while ('B' != str_long_value[i])
    {
        long_value <<= 1;
        if ('1' == str_long_value[i])
        {
            ++long_value;
        }
        ++i;
    }

    return long_value;
}

static void FORM(goal_achieved_t achieved_goal)
{
    int i, j;

    for (IFORMT = 0; IFORMT < MAXFORMT; IFORMT++)
    {
        memset(FORMT[IFORMT], '\0', sizeof(FORMT[IFORMT]));
    }

    IFORMT = 0;
    j = achieved_goal.DST2;

    FORM1:

    for (i = j; i <= achieved_goal.DST4 + 1; i++)
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

enum plcmp_sem_calc_error_code_e SAVI(void const *param)
{
    unsigned int i = 0;
    goal_achieved_t goal_achieved = *((goal_achieved_t*)param);

    FORM(goal_achieved);

    if (1 == IFORMT)
    {
        for (i = 0; i < ISYM; i++)
        {
            if (!strcmp(SYM[i].name, FORMT[0]) &&
                (strlen(SYM[i].name) == strlen(FORMT[0])))
            {
                switch (SYM[i].type)
                {
                    case 'B':
                        if (SYM[i].capacity <= 15)
                        {
                            memcpy(assembler_card.OPERAC, "LH", 2);
                        }
                        else
                        {
                            memcpy(assembler_card.OPERAC, "L", 1);
                        }

                        strcpy(assembler_card.OPERAND, "RRAB,");
                        strcat(assembler_card.OPERAND, FORMT[0]);
                        assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
                        memcpy(assembler_card.COMM, "Load variable into register", 27);
                        record_assembler_card();

                        return PLCMP_SEM_CALCULATOR_SUCCESS;

                    case 'C':
                        p_char_syms[char_syms_size] = &SYM[i];
                        ++char_syms_size;
                        return PLCMP_SEM_CALCULATOR_SUCCESS;
                    case 'P':
                    default:
                        return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR;
                }
            }
        }

        return PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR;
    }
    else
    {
        size_t formt_len = strlen(FORMT[IFORMT - 1]);

        for (i = 0; i < ISYM; i++)
        {
            if (!strcmp(SYM[i].name, FORMT[IFORMT - 1]) &&
                (strlen(SYM[i].name) == formt_len))
            {
                switch (SYM[i].type)
                {
                    case 'B':

                        switch (g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len])
                        {
                            case '+':
                                if (SYM[i].capacity <= 15)
                                {
                                    memcpy(assembler_card.OPERAC, "AH", 2);
                                }
                                else
                                {
                                    memcpy(assembler_card.OPERAC, "A", 1);
                                }
                                break;

                            case '-':
                                if (SYM[i].capacity <= 15)
                                {
                                    memcpy(assembler_card.OPERAC, "SH", 2);
                                }
                                else
                                {
                                    memcpy(assembler_card.OPERAC, "S", 1);
                                }
                                break;

                            case '!':
                                switch (g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len - 1])
                                {
                                    case '!':
                                        break;
                                    default:
                                        return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR;
                                }
                                break;

                            default:
                                return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR;
                        }

                        strcpy(assembler_card.OPERAND, "RRAB,");
                        strcat(assembler_card.OPERAND, FORMT[IFORMT - 1]);
                        assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
                        memcpy(assembler_card.COMM, "Formation of intermediate value", 31);
                        record_assembler_card();

                        return PLCMP_SEM_CALCULATOR_SUCCESS;

                    case 'C':

                        switch (g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len])
                        {
                            case '!':
                                switch (g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len - 1])
                                {
                                    case '!':
                                        p_char_syms[char_syms_size] = &SYM[i];
                                        ++char_syms_size;
                                        break;
                                    default:
                                        return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR;
                                }
                                break;

                            case '+':
                            case '-':
                            default:
                                return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR;
                        }

                        return PLCMP_SEM_CALCULATOR_SUCCESS;

                    case 'P':
                    default:
                        return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR;
                }
            }
        }

        return PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR;
    }

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SBUK(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SCIF(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SIDE(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SIPE(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SIPR(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SLIT(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SMAN(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SODC(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SOEN(void const *param)
{
    goal_achieved_t goal_achieved = *((goal_achieved_t*)param);
    char RAB[20] = { '\0' };
    unsigned int i = 0;
    
    FORM(goal_achieved);

    /* Formation of the mnemonic machine operation 'BCR'
     * for returning to the caller */
    memcpy(assembler_card.OPERAC, "BCR", 3);
    memcpy(assembler_card.OPERAND, "15,14", 5);
    memcpy(assembler_card.COMM, "Exit from the program", 21);
    record_assembler_card();

    /* Formation of the 'DC' pseudo operations 
     * for each label which have been saved into SYM-table
     * except label of name of the program 
     */
    for (i = 0; i < ISYM; i++)
    {
        if (isalpha(SYM[i].name[0]))
        {
            int t_flag = 'P' != SYM[i].type;
            if (t_flag)
            {
                strcpy(assembler_card.METKA, SYM[i].name);
                assembler_card.METKA[strlen(assembler_card.METKA)] = ' ';
                memcpy(assembler_card.OPERAC, "DC", 2);
            }

            switch(SYM[i].type)
            {
                case 'B':
                    strcpy(assembler_card.OPERAND, SYM[i].capacity <= 15 ? "H\'" : "F\'");
                    strcat(assembler_card.OPERAND, gcvt(long_int_str_to_long_int(SYM[i].INIT), 10, &RAB[0]));
                    break;
                case 'C':
                {
                    char buffer[2];
                    strcpy(assembler_card.OPERAND, "CL");

                    sprintf(buffer, "%lu", SYM[i].capacity);
                    strcat(assembler_card.OPERAND, buffer);

                    strcat(assembler_card.OPERAND, "\'");
                    strcat(assembler_card.OPERAND, SYM[i].INIT);

                    break;
                }
                default:
                    break;
            }

            if (t_flag)
            {
                assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = '\'';
                memcpy(assembler_card.COMM, "Variable definition", 19);
                record_assembler_card();
            }
        }
    }

    /* Formation of 'EQU' pseudo operation
     * Definition of the number of RBASE register */
    memcpy(assembler_card.METKA, "RBASE", 5);
    memcpy(assembler_card.OPERAC, "EQU", 3);
    memcpy(assembler_card.OPERAND, "15", 2);
    record_assembler_card();

    /* Formation of 'EQU' pseudo operation
     * Defininition of the number of RRAB register */
    memcpy(assembler_card.METKA, "RRAB", 4);
    memcpy(assembler_card.OPERAC, "EQU", 3);
    memcpy(assembler_card.OPERAND, "4", 1);
    record_assembler_card();

    /* Formation of 'END' pseudo operation */
    memcpy(assembler_card.OPERAC, "END", 3);
    strcpy(assembler_card.OPERAND, FORMT[1]);
    assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
    memcpy(assembler_card.COMM, "End of the program", 18);
    record_assembler_card();

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SOPA(void const *param)
{
    int i = 0;
    goal_achieved_t goal_achieved = *((goal_achieved_t*)param);

    FORM(goal_achieved);

    for (i = 0; i < ISYM; i++)
    {
        if (!strcmp(SYM[i].name, FORMT[0]) &&
            strlen(SYM[i].name) == strlen(FORMT[0]))
        {
            switch (SYM[i].type)
            {
                case 'B':
                    if (SYM[i].capacity <= 15)
                    {
                        memcpy(assembler_card.OPERAC, "STH", 3);
                    }
                    else
                    {
                        memcpy(assembler_card.OPERAC, "ST", 2);
                    }

                    strcpy(assembler_card.OPERAND, "RRAB,");
                    strcat(assembler_card.OPERAND, FORMT[0]);
                    assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
                    memcpy(assembler_card.COMM, "Formation of value of the arithmetic expression", 47);
                    record_assembler_card();

                    return PLCMP_SEM_CALCULATOR_SUCCESS;

                case 'C':
                {
                    int j;

                    size_t final_necessary_razr = 0;

                    for (j = 0; j < char_syms_size; j++)
                    {
                        final_necessary_razr += p_char_syms[j]->char_init_len;
                    }

                    if (final_necessary_razr > SYM[i].capacity)
                    {
                        return PLCMP_SEM_CALCULATOR_CONCAT_ERROR;
                    }

                    size_t offset = 0;

                    for (j = 0; j < char_syms_size; j++)
                    {
                        size_t str_len;
                        char buffer[10];

                        if (0 == j)
                        {
                            /* Formation of 'LA' operation */
                            memcpy(assembler_card.OPERAC, "LA", 2);
                            strcpy(assembler_card.OPERAND, "RRAB,");
                            strcat(assembler_card.OPERAND, SYM[i].name);
                            assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
                            memcpy(assembler_card.COMM, "Load operand's address to register", 34);
                            record_assembler_card();

                            memcpy(assembler_card.COMM, "Move second string to the first", 31);
                        }
                        else
                        {
                            memcpy(assembler_card.COMM, "Concat fir. and sec. strings. Result is in the first", 52);
                        }

                        /* Formation of 'MVC' operation */
                        memcpy(assembler_card.OPERAC, "MVC", 3);
                        /* D1 */
                        sprintf(buffer, "%lu", offset);
                        strcpy(assembler_card.OPERAND, buffer);
                        strcat(assembler_card.OPERAND, "(");
                        /* L */
                        str_len = p_char_syms[j]->char_init_len;
                        sprintf(buffer, "%lu", str_len);
                        strcat(assembler_card.OPERAND, buffer);
                        /* B1 */
                        strcat(assembler_card.OPERAND, ",RRAB),");
                        /* D2 */
                        strcat(assembler_card.OPERAND, p_char_syms[j]->name);
                        assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
                        record_assembler_card();

                        offset += str_len;
                        p_char_syms[j] = NULL;
                    }
                    char_syms_size = 0;

                    return PLCMP_SEM_CALCULATOR_SUCCESS;
                }
                case 'P':
                default:
                    return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR;
            }
        }
    }

    return PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR;
}

enum plcmp_sem_calc_error_code_e SOPR(void const *param)
{
    goal_achieved_t goal_achieved = *((goal_achieved_t*)param);
    FORM(goal_achieved);

    /* Formation of pseudo operation 'START' */
    strcpy(assembler_card.METKA, FORMT[0]);
    assembler_card.METKA[strlen(assembler_card.METKA)] = ' ';
    memcpy(assembler_card.OPERAC, "START", 5);
    memcpy(assembler_card.OPERAND, "0", 1);
    memcpy(assembler_card.COMM, "Start of the program", 20);
    record_assembler_card();

    /* Formation of mnemonic machine operation 'BALR' */
    memcpy(assembler_card.OPERAC, "BALR", 4 );
    memcpy(assembler_card.OPERAND, "RBASE,0", 7);
    memcpy(assembler_card.COMM, "Load base register", 18);
    record_assembler_card();

    /* Formation of pseudo operation 'USING' */
    memcpy(assembler_card.OPERAC, "USING", 5);
    memcpy(assembler_card.OPERAND, "*,RBASE", 7);
    memcpy(assembler_card.COMM, "Assign base register", 20);
    record_assembler_card();

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SPRO(void const *param)
{
    char const *p_asm_fp_name = param;
    FILE *p_asm_f = fopen(p_asm_fp_name, "wb");

    if (NULL == p_asm_f)
    {
        return PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE_ERROR;
    }
    else
    {
        int i;
        for (i = 0; i < IASSTXT; i++)
        {
            fwrite(assembler_out_text[i], 80, 1, p_asm_f);
            fwrite("\n", 1, 1, p_asm_f);
        }
        
        fclose(p_asm_f);
    }

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SRZR(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e STEL(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SZNK(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SSTC(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

enum plcmp_sem_calc_error_code_e SCON(void const *param)
{
    (void)param;
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}