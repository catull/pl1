/* encoding: UTF-8 */
 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "plcmp_tables.h"
#include "plcmp_common.h"
#include "plcmp_sem_calc.h"

/* This array of structures is the table of labels' names of variables
 * which is being filled on the first process of semantic calculation
 * and is being used on the second process of semantic calculation 
 */
struct sym_s
{
    char name[8];
    char type;
    size_t capacity;
    size_t char_init_len;
    char INIT[50];
} SYM[NSYM];

/* Count of SYM-s */
static int ISYM = 0;

/* Array of pointers to SYM-s for processing concat operation for string */
static struct sym_s *p_char_syms[9];
/* Count of 'p_char_syms' */
static int char_syms_size = 0;

/* Pointer to compact source text */
static char const *g_p_compact_pl1_src_text;

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

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
static char FORMT[MAXFORMT][9];
/* Formatted array index */
static int IFORMT;

/* Array for storage of output text written in IBM 370 ASSEMBLER */
static char assembler_out_text[MAXLTXT][80];
/* Output double array row index */
static int IASSTXT;

/* Subroutine prepares error data for sending its to caller
 * Error code in 'p_err_data->err_code' controls what kind of data will be written */
static void cook_error_data(plcmp_sem_calc_error_data_t *p_err_data, 
                            goals_achieved_stack_t goal_achieved)
{
    switch (p_err_data->err_code)
    {
        case PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR:
            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR:
            memcpy(p_err_data->src_text_part,
                   &g_p_compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier_type, FORMT[1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR:
            memcpy(p_err_data->src_text_part,
                   &g_p_compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier_type, FORMT[IFORMT - 1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR:
            memcpy(p_err_data->src_text_part,
                   &g_p_compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier, FORMT[IFORMT - 1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR:
        {
            size_t formt_len = strlen(FORMT[IFORMT - 1]);
            size_t oper_len = 0;
            memcpy(p_err_data->src_text_part,
                   &g_p_compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            p_err_data->data.operation[oper_len] = 
                g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len];
            ++oper_len;
            if ('!' == g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len] &&
                '!' == g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len + 1])
            {
                p_err_data->data.operation[oper_len] = 
                    g_p_compact_pl1_src_text[goal_achieved.DST4 - formt_len + 1];
                ++oper_len;
            }
            p_err_data->data.operation[oper_len] = '\0';

            break;
        }
        case PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR:
            memcpy(p_err_data->src_text_part,
                   &g_p_compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier, FORMT[1]);

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

static void FORM(goals_achieved_stack_t achieved_goal)
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

/* Subroutine is for semantic calculation 
 * non-terminal "AVI" on the first and
 * the second phases of semantic calculation 
 * Here "AVI" is "arithmetic expression"
 */
static enum plcmp_sem_calc_error_code_e AVI(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
        {
            unsigned int i;
            goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);

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

            break;
        }
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "BUK" on the first and
 * the second phases of semantic calculation 
 * Here "BUK" is "letter"
 */
static enum plcmp_sem_calc_error_code_e BUK(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "CIF" on the first and
 * the second phases of semantic calculation 
 * Here "CIF" is "digit" 
 */
static enum plcmp_sem_calc_error_code_e CIF(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IDE" on the first and
 * the second phases of semantic calculation 
 * Here "IDE" is "identifier"
 */
static enum plcmp_sem_calc_error_code_e IDE(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IPE" on the first and
 * the second phases of semantic calculation 
 * Here "IPE" is "variable name"
 */
static enum plcmp_sem_calc_error_code_e IPE(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IPR" on the first and
 * the second phases of semantic calculation 
 * Here "IPR" is "program name"
 */
static enum plcmp_sem_calc_error_code_e IPR(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "LIT" on the first and
 * the second phases of semantic calculation 
 * Here "LIT" is "literal"
 */
static enum plcmp_sem_calc_error_code_e LIT(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "MAN" on the first and
 * the second phases of semantic calculation 
 * Here "MAN" is "mantissa"
 */
static enum plcmp_sem_calc_error_code_e MAN(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "ODC" on the first and
 * the second phases of semantic calculation 
 * Here "ODC" is "PL1 operator of variable declaration 'DCL' "
 */
static enum plcmp_sem_calc_error_code_e ODC(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
        {
            int i;
            int init_pos;
            goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);

            FORM(goal_achieved);

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
        case 2:
        {
            return PLCMP_SEM_CALCULATOR_SUCCESS;
        }
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OEN" on the first and
 * the second phases of semantic calculation 
 * Here "OEN" is "PL1 operator of epilogue 'END' of program "
 */
static enum plcmp_sem_calc_error_code_e OEN(int entry, void const *param)
{
    goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);
    switch (entry)
    {
        case 1:
        {
            unsigned int i;

            FORM(goal_achieved);

            for (i = 0; i < ISYM; i++)
            {
                if (!strcmp(SYM[i].name, FORMT[1]) && (SYM[i].type == 'P'))
                {
                    return PLCMP_SEM_CALCULATOR_SUCCESS;
                }
            }

            return PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR;
        }
        case 2:
        {
            char RAB[20];
            unsigned int i;
            
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
        default:
        {
            break;
        }
    }

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OPA" on the first and
 * the second phases of semantic calculation 
 * Here "OPA" is "operator of arithmetic assignment"
 */
static enum plcmp_sem_calc_error_code_e OPA(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            return PLCMP_SEM_CALCULATOR_SUCCESS;
        case 2:
        {
            int i;
            goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);

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
        default:
        {
            break;
        }
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OPR" on the first and
 * the second phases of semantic calculation 
 * Here "OPR" is "PL1 operator of prologue 'PROC' of program "
 */
static enum plcmp_sem_calc_error_code_e OPR(int entry, void const *param)
{
    goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);
    switch (entry)
    {
        case 1:
        {
            FORM(goal_achieved);

            /* Save program name as identifier */
            strcpy(SYM[ISYM].name, FORMT[0]);
            /* Set type 'P' */
            SYM[ISYM].type = 'P';
            /* Capacity is 0 */
            SYM[ISYM].capacity = 0;
            ++ISYM;

            break;
        }
        case 2:
        {
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

            break;
        }
        default:
        {
            break;
        }
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "PRO" on the first and
 * the second phases of semantic calculation 
 * Here "PRO" is "program"
 */
static enum plcmp_sem_calc_error_code_e PRO(int entry, void const *param)
{
    switch(entry)
    {
        case 1:
            break;
        case 2:
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

            break;
        }
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "RZR" on the first and
 * the second phases of semantic calculation 
 * Here "RZR" is "capacity"
 */
static enum plcmp_sem_calc_error_code_e RZR(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}


/* Subroutine is for semantic calculation 
 * non-terminal "TEL" on the first and
 * the second phases of semantic calculation 
 * Here "TEL" is "body of the program"
 */
static enum plcmp_sem_calc_error_code_e TEL(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine is for semantic calculation 
 * non-terminal "ZNK" on the first and
 * the second phases of semantic calculation 
 * Here "ZNK" is "sign"
 */
static enum plcmp_sem_calc_error_code_e ZNK(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine for semantic calculation
 * non-ternimal "STC" on the first and
 * the second phases of semantic calculation.
 * Here "STC" is "constant string enclosed into single quotation marks"
 */
static enum plcmp_sem_calc_error_code_e STC(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* Subroutine for semantic calculation of the achieved goals made by syntax analyzer 
 * and for generation output assembler file */
struct plcmp_sem_calc_error_data_s plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name,
                                                               char const compact_pl1_src_text[],
                                                               dst_t const *p_goals_achieved)
{
    int sem_calc_phase;
    plcmp_sem_calc_error_data_t err_data;

    /* Set global pointer to compact text for easier working with */
    g_p_compact_pl1_src_text = compact_pl1_src_text;

    /* Clear error data structure for later using and set default successful error code */
    memset(&err_data, 0, sizeof(plcmp_sem_calc_error_data_t));
    err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS;

    enum plcmp_sem_calc_error_code_e (*handler[NNETRM])(int, void const*) = {
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
        /*   17  */ STC
    };

    /* Clear buffer string of the assembler output file */
    clear_assembler_card();

    /* First and second phases of semantic calculation */
    for (sem_calc_phase = 1; sem_calc_phase < 3; sem_calc_phase++)
    {
        int dst_index;
        for (dst_index = 0; dst_index < p_goals_achieved->count; dst_index++)
        {
            int hand_num = plcmp_tables_sym_sint_ind(p_goals_achieved->dst_stack[dst_index].DST1, 3);
            switch(hand_num + 1)
            {
                /* PRO */
                case 13:
                    err_data.err_code = handler[hand_num](sem_calc_phase, p_asm_fp_name);
                    break;
                /* other */
                default:
                    err_data.err_code = handler[hand_num](sem_calc_phase, &p_goals_achieved->dst_stack[dst_index]);
                    break;    
            }
            
            if (PLCMP_SEM_CALCULATOR_SUCCESS != err_data.err_code)
            {
                /* Error occured while calculation superposition 
                 * of the functions of the goals achieved.
                 * Prepare and send error data to the caller 
                 */
                cook_error_data(&err_data, p_goals_achieved->dst_stack[dst_index]);
                return err_data;
            }
        }
    }

    return err_data;
}

/* Subroutine constructs error message by error data of semantic calculator module */
char* plcmp_sem_calc_errmsg_by_errdata(plcmp_sem_calc_error_data_t const *err_data, char *errmsg)
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
            strcpy(errmsg, "Mismatch of the name of the procedure in prologue-epilogue");
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
            strcpy(errmsg, "Capacity of the destination string is less than "
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