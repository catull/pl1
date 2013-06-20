/* encoding: UTF-8 
 * russian letters: (Cyrillic) KOI8-U
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "plcmp_tables.h"
#include "plcmp_common.h"
#include "plcmp_lex_analyzer.h"
#include "plcmp_sem_calc.h"

sym_t SYM[NSYM];
/* текущий индекс таблицы имен */
int ISYM = 0;

/* */
sym_t *p_char_syms[9];
int char_syms_size = 0;

/* This struct is type of assembler card
 * It is template to generate the output file record by IBM 370 assembler */
static struct assembler_card {
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
char FORMT[MAXFORMT][9];
/* Formatted array index */
int IFORMT;

/* Array for storage of output text written in IBM 370 ASSEMBLER */
static char assembler_out_text[MAXLTXT][80];
/* Output array index */
static int IASSTXT;

/* Subroutine prepares error data for sending its to caller
 * Error code in 'p_err_data->err_code' controls which data will be written */
static void plcmp_sem_calc_cook_error_data(plcmp_sem_calc_error_data_t *p_err_data, 
                                           goals_achieved_stack_t goal_achieved)
{
    switch (p_err_data->err_code)
    {
        case PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR:
            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR:
            memcpy(p_err_data->src_text_part,
                   &compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier_type, FORMT[1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_EXPR_ERROR:
            memcpy(p_err_data->src_text_part,
                   &compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier_type, FORMT[IFORMT - 1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_DETERNINED_IDENT_ERROR:
            memcpy(p_err_data->src_text_part,
                   &compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier, FORMT[IFORMT - 1]);

            break;
        case PLCMP_SEM_CALCULATOR_NOT_ALLOWED_OPERATION_ERROR:
        {
            size_t formt_len = strlen(FORMT[IFORMT - 1]);
            size_t oper_len = 0;
            memcpy(p_err_data->src_text_part,
                   &compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            p_err_data->data.operation[oper_len] = 
                compact_pl1_src_text[goal_achieved.DST4 - formt_len];
            ++oper_len;
            if ('!' == compact_pl1_src_text[goal_achieved.DST4 - formt_len] &&
                '!' == compact_pl1_src_text[goal_achieved.DST4 - formt_len + 1])
            {
                p_err_data->data.operation[oper_len] = 
                    compact_pl1_src_text[goal_achieved.DST4 - formt_len + 1];
                ++oper_len;
            }
            p_err_data->data.operation[oper_len] = '\0';

            break;
        }
        case PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR:
            memcpy(p_err_data->src_text_part,
                   &compact_pl1_src_text[goal_achieved.DST2],
                   PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN);
            p_err_data->src_text_part[PLCMP_SEM_CALCULATOR_SRC_TEXT_PART_LEN] = '\0';

            strcpy(p_err_data->data.identifier, FORMT[1]);

            break;
        case PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE:
            break;
        case PLCMP_SEM_CALCULATOR_CONCAT_ERROR:
            break;
        case PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG:
            break;
        default:
            break;
    }
}


/* Clear assembler card. Fresh card should contains ' ' (space symbols) */
static void plcmp_sem_calc_clear_assembler_card(void)
{
    memset(&assembler_card, ' ', sizeof(assembler_card));
}


/* Record new assembler command by assembler card
 * into assembler commands' array 
 * and clear the temporary assembler card */
static void ZKARD(void)
{
    memcpy(assembler_out_text[IASSTXT], &assembler_card, 80);
    ++IASSTXT;
    plcmp_sem_calc_clear_assembler_card();
}

/* Function translates 'long int' value written as string of ASCII-code
 * into normal 'long int' internal representation of machine */
static long int li_str_to_li(char const *str_long_value)
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

/* п р о г р а м м а */
/* представления фрагмента*/
/* плотного текста в виде */
/* массива 9-ти символьных*/
/* лексем */
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

        if ('\'' == compact_pl1_src_text[i] ||
            ':'  == compact_pl1_src_text[i] ||
            ' '  == compact_pl1_src_text[i] ||
            '('  == compact_pl1_src_text[i] ||
            ')'  == compact_pl1_src_text[i] ||
            ';'  == compact_pl1_src_text[i] ||
            '+'  == compact_pl1_src_text[i] ||
            '-'  == compact_pl1_src_text[i] ||
            '='  == compact_pl1_src_text[i] ||
            '*'  == compact_pl1_src_text[i])
        {
            FORMT[IFORMT][i - j] = '\0';
            IFORMT++;
            j = i + 1;
            goto FORM1;
        }
        else if ('!' == compact_pl1_src_text[i] &&
                 '!' == compact_pl1_src_text[i + 1])
        {
            FORMT[IFORMT][i - j] = '\0';
            IFORMT++;
            j = i + 2;
            goto FORM1;
        }
        else
        {

            FORMT[IFORMT][i - j] = compact_pl1_src_text[i];
        }
    }

    return;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала AVI на пер-*/
/* вом проходе.   Здесь   */
/* AVI -   "арифм.выраж." */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала AVI на вто-*/
/* ром проходе.   Здесь   */
/* AVI -   "арифм.выраж." */
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
                    if (!strcmp(SYM[i].NAME, FORMT[0]) &&
                        (strlen(SYM[i].NAME) == strlen(FORMT[0])))
                    {
                        switch (SYM[i].TYPE)
                        {
                            case 'B':
                                if (SYM[i].RAZR <= 15)
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
                                ZKARD();

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
                    if (!strcmp(SYM[i].NAME, FORMT[IFORMT - 1]) &&
                        (strlen(SYM[i].NAME) == formt_len))
                    {
                        switch (SYM[i].TYPE)
                        {
                            case 'B':

                                switch (compact_pl1_src_text[goal_achieved.DST4 - formt_len])
                                {
                                    case '+':
                                        if (SYM[i].RAZR <= 15)
                                        {
                                            memcpy(assembler_card.OPERAC, "AH", 2);
                                        }
                                        else
                                        {
                                            memcpy(assembler_card.OPERAC, "A", 1);
                                        }
                                        break;

                                    case '-':
                                        if (SYM[i].RAZR <= 15)
                                        {
                                            memcpy(assembler_card.OPERAC, "SH", 2);
                                        }
                                        else
                                        {
                                            memcpy(assembler_card.OPERAC, "S", 1);
                                        }
                                        break;

                                    case '!':
                                        switch (compact_pl1_src_text[goal_achieved.DST4 - formt_len - 1])
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
                                ZKARD();

                                return PLCMP_SEM_CALCULATOR_SUCCESS;

                            case 'C':

                                switch (compact_pl1_src_text[goal_achieved.DST4 - formt_len])
                                {
                                    case '!':
                                        switch (compact_pl1_src_text[goal_achieved.DST4 - formt_len - 1])
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала BUK на пер-*/
/* вом проходе.   Здесь   */
/* BUK -   "буква"        */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала BUK на вто-*/
/* ром проходе.   Здесь   */
/* BUK -   "буква"        */
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала CIF на пер-*/
/* вом проходе.   Здесь   */
/* CIF -   "цифра"        */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала CIF на вто-*/
/* ром проходе.   Здесь   */
/* CIF -   "цифра"        */
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IDE на пер-*/
/* вом проходе.   Здесь   */
/* IDE -   "идентификатор"*/

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IDE на вто-*/
/* ром проходе.   Здесь   */
/* IDE -   "идентификатор"*/
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPE на пер-*/
/* вом проходе.   Здесь   */
/* IPE - "имя переменной" */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPE на вто-*/
/* ром проходе.   Здесь   */
/* IPE - "имя переменной" */
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPR на пер-*/
/* вом проходе.   Здесь   */
/* IPR -   "имя программы" */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPR на вто-*/
/* ром проходе.   Здесь   */
/* IPR -   "имя программы" */
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала LIT на пер-*/
/* вом проходе.   Здесь   */
/* LIT -   "литерал"      */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала LIT на вто-*/
/* ром проходе.   Здесь   */
/* LIT -   "литерал"      */
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала MAN на пер-*/
/* вом проходе.   Здесь   */
/* MAN -   "мантисса"     */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала MAN на вто-*/
/* ром проходе.   Здесь   */
/* MAN -   "мантисса"     */
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ODC на пер-*/
/* вом проходе.   Здесь   */
/* ODC - "операт.ПЛ1- DCL"*/

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ODC на вто-*/
/* ром проходе.   Здесь   */
/* ODC - "операт.ПЛ1- DCL"*/
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
                if (!strcmp(SYM[i].NAME, FORMT[1])
                    && strlen(SYM[i].NAME) == strlen(FORMT[1]))
                {
                    return PLCMP_SEM_CALCULATOR_REPEATED_DCL_IDENT_ERROR;
                }
            }

            strcpy(SYM[ISYM].NAME, FORMT[1]);

            if (!(strcmp(FORMT[2], "BIN") || strcmp(FORMT[3], "FIXED")))
            {
                init_pos = 5;
                SYM[ISYM].TYPE = 'B';
            }
            else if (!strcmp(FORMT[2], "CHAR"))
            {
                init_pos = 4;
                SYM[ISYM].TYPE = 'C';
            }
            else
            {
                SYM[ISYM].TYPE = 'U';
                return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR;
            }

            SYM[ISYM].RAZR = atoi(FORMT[init_pos - 1]);

            if (!strcmp(FORMT[init_pos], "INIT"))
            {
                switch(SYM[ISYM].TYPE)
                {
                    case 'B':
                        strcpy(SYM[ISYM].INIT, FORMT[init_pos + 1]);           
                        break;
                    case 'C':
                    {
                        #define init_value_pos (init_pos + 2)
                        #define init_value_len strlen(FORMT[init_value_pos])

                        if (SYM[ISYM].RAZR < strlen(FORMT[init_value_pos]))
                        {
                            return PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG;
                        }

                        strcpy(SYM[ISYM].INIT, FORMT[init_value_pos]);
                        memset(&(SYM[ISYM].INIT)[init_value_len], '\0', SYM[ISYM].RAZR - init_value_len);

                        #undef init_value_pos
                        #undef init_value_len

                        break;
                    }
                    default:
                        return PLCMP_SEM_CALCULATOR_NOT_ALLOWED_IDENT_TYPE_DCL_ERROR;
                }
            }
            else
            {
                switch(SYM[ISYM].TYPE)
                {
                    case 'B':
                        strcpy(SYM[ISYM].INIT, "0B");
                        break;
                    case 'C':
                        memset(SYM[ISYM].INIT, '\0', SYM[ISYM].RAZR);
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OEN на пер-*/
/* вом проходе.   Здесь   */
/* OEN - "операт.ПЛ1-END" */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OEN на вто-*/
/* ром проходе.   Здесь   */
/* OEN - "операт.ПЛ1-END" */
/* программа    формирует */
/* эпилог ассемблеровского*/
/* эквивалента ПЛ1-прог-  */
/* раммы                  */

static enum plcmp_sem_calc_error_code_e OEN(int entry, void const *param)
{
    goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);
    switch (entry)
    {
        case 1:
        {
            unsigned int i;

            FORM(goal_achieved); /* форматирование ПЛ1-оператора END */

            /* если вторй терм оператора END записан в табл */
            for (i = 0; i < ISYM; i++)
            {
                /* SYM и его тип = "P",то:*/
                if (!strcmp(SYM[i].NAME, FORMT[1]) && 
                    (SYM[i].TYPE == 'P') && 
                    (strlen(SYM[i].NAME) == strlen(FORMT[1])))
                {
                    /* Successful completion of the function */
                    return PLCMP_SEM_CALCULATOR_SUCCESS;
                }
            }

            /* иначе завершение программы по ошибке */
            return PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR;
        }
        case 2:
        {
            char RAB[20];
            unsigned int i;
            /* форматируем ПЛ1-оператор END */
            FORM(goal_achieved);

            /* формируем код безусловного возврата управления в вызывающую программу */
            memcpy(assembler_card.OPERAC, "BCR", 3);
            /* операнды команды и поле построчного коментария */
            memcpy(assembler_card.OPERAND, "15,14", 5);
            memcpy(assembler_card.COMM, "Exit from the program", 21);

            /* запомнить опреацию Ассемблера */
            ZKARD();

            /* далее идет блок формирования декларативных
             * псевдоопераций DC для каждого идентификатора
             * попавшего в табл.SYM
             */
            for (i = 0; i < ISYM; i++)
            {
                if (isalpha(SYM[i].NAME[0]))
                {
                    int t_flag = 'P' != SYM[i].TYPE;
                    if (t_flag)
                    {
                        strcpy(assembler_card.METKA, SYM[i].NAME);
                        assembler_card.METKA[strlen(assembler_card.METKA)] = ' ';
                        memcpy(assembler_card.OPERAC, "DC", 2);
                    }

                    switch(SYM[i].TYPE)
                    {
                        case 'B':
                            strcpy(assembler_card.OPERAND, SYM[i].RAZR <= 15 ? "H\'" : "F\'");
                            strcat(assembler_card.OPERAND, gcvt(li_str_to_li(SYM[i].INIT), 10, &RAB[0]));
                            break;
                        case 'C':
                            strcpy(assembler_card.OPERAND, "C\'");
                            strcat(assembler_card.OPERAND, SYM[i].INIT);
                            break;
                        default:
                            break;
                    }

                    if (t_flag)
                    {
                        assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = '\'';
                        memcpy(assembler_card.COMM, "Variable definition", 19);
                        ZKARD();
                    }
                }
            }

            /* далее идет блок декла- */
            /* ративных ассемблеровс- */
            /* ких EQU-операторов, оп-*/
            /* ределяющих базовый и   */
            /* рабочий регистры общего*/
            /* назначения */

            memcpy(assembler_card.METKA, "RBASE", 5);
            memcpy(assembler_card.OPERAC, "EQU", 3);
            memcpy(assembler_card.OPERAND, "15", 2);

            ZKARD();

            memcpy(assembler_card.METKA, "RRAB", 4);
            memcpy(assembler_card.OPERAC, "EQU", 3);
            memcpy(assembler_card.OPERAND, "5", 1);

            ZKARD();

            memcpy(assembler_card.OPERAC, "END", 3);

            i = 0;

            while (FORMT[1][i] != '\0')
            {
                assembler_card.OPERAND[i] = FORMT[1][i];
                ++i;
            }

            memcpy(assembler_card.COMM, "End of the program", 18);

            ZKARD();

            return PLCMP_SEM_CALCULATOR_SUCCESS;
        }
        default:
        {
            break;
        }
    }

    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPA на пер-*/
/* вом проходе.   Здесь   */
/* OPA - "операт.присваи- */
/* вания арифметический   */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPA на вто-*/
/* ром проходе.   Здесь   */
/* OPA - "операт.присваи- */
/* вания арифметический   */
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
                if (!strcmp(SYM[i].NAME, FORMT[0]) &&
                    strlen(SYM[i].NAME) == strlen(FORMT[0]))
                {
                    switch (SYM[i].TYPE)
                    {
                        case 'B':
                            if (SYM[i].RAZR <= 15)
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
                            ZKARD();

                            return PLCMP_SEM_CALCULATOR_SUCCESS;

                        case 'C':
                        {
                            int j;
                            size_t offset = 0;

                            size_t final_necessary_razr = 0;
                            for (j = 0; j < char_syms_size; j++)
                            {
                                final_necessary_razr += strlen(p_char_syms[j]->INIT);
                            }

                            if (final_necessary_razr > SYM[i].RAZR)
                            {
                                return PLCMP_SEM_CALCULATOR_CONCAT_ERROR;
                            }

                            for (j = 0; j < char_syms_size; j++)
                            {
                                /* Format of command: 
                                 * MVC D1(L,B1),D2(B2) */
                                char buffer[10];
                                size_t str_len = strlen(p_char_syms[j]->INIT);

                                /* command */
                                memcpy(assembler_card.OPERAC, "MVC", 3);

                                /* D1 */
                                sprintf(buffer, "%lu", offset);
                                strcpy(assembler_card.OPERAND, buffer);

                                strcat(assembler_card.OPERAND, "(");

                                /* L */
                                sprintf(buffer, "%lu", str_len);
                                strcat(assembler_card.OPERAND, buffer);

                                strcat(assembler_card.OPERAND, ",");
                                /* B1 */
                                strcat(assembler_card.OPERAND, SYM[i].NAME);
                                /* D2 */
                                strcat(assembler_card.OPERAND, "),0(");
                                /* B2 */
                                strcat(assembler_card.OPERAND, p_char_syms[j]->NAME);
                                
                                strcat(assembler_card.OPERAND, ")");

                                assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';

                                if (0 == j)
                                {
                                    memcpy(assembler_card.COMM, "Move second string to the first", 31);
                                }
                                else
                                {
                                    memcpy(assembler_card.COMM, "Concat fir. and sec. strings. Result is in the first", 52);   
                                }

                                ZKARD();

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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPR на пер-*/
/* вом проходе.   Здесь   */
/* OPR - "операт.ПЛ1-PROC"*/

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPR на вто-*/
/* ром проходе.   Здесь   */
/* OPR - "операт.ПЛ1-PROC"*/
/* программа    формирует */
/* пролог ассемблеровского*/
/* эквивалента  исходной  */
/* ПЛ1-программы          */
static enum plcmp_sem_calc_error_code_e OPR(int entry, void const *param)
{
    goals_achieved_stack_t goal_achieved = *((goals_achieved_stack_t*)param);
    switch (entry)
    {
        case 1:
        {
            FORM(goal_achieved);                                        /* форматируем оператор   */
            /* ПЛ1 PROC               */

            strcpy(SYM[ISYM].NAME, FORMT[0]);          /* перепишем имя ПЛ1-прог-*/
            /* раммы в табл. SYM,     */

            SYM[ISYM].TYPE = 'P';                        /* установим тип этого    */
            /* имени = 'P'            */
            SYM[ISYM].RAZR = 0;                  /* установим разрядность  */
            ++ISYM;
            /* равной 0               */

                                                   /* успешное завершение    */
            /* программы              */
            break;
        }
        case 2:
        {
            unsigned int i = 0;
            FORM(goal_achieved);
            /* форматируем оператор   */
            /* ПЛ1 - "начало процедур-*/
            /* ного блока"            */

            while ('\0' != FORMT[0][i])
            {
                assembler_card.METKA[i] = FORMT[0][i];  /* нулевой терм используем */
                ++i;
            }


            /* как метку в START-псев-*/
            /* дооперации Ассемблера  */

            /* достраиваем код и операнды в START-псевдооперации Ассемблера */
            memcpy(assembler_card.OPERAC, "START", 5);
            memcpy(assembler_card.OPERAND, "0", 1);
            memcpy(assembler_card.COMM, "Start of the program", 20);

            /* запоминаем карту Ассемблера */
            ZKARD();

            /* формируем BALR-операцию Ассемблера */
            memcpy(assembler_card.OPERAC, "BALR", 4 );
            memcpy(assembler_card.OPERAND, "RBASE,0", 7);
            memcpy(assembler_card.COMM, "Load base register", 18);

            /* и запоминаем ее */
            ZKARD();

            /* формируем USING-псевдооперацию Ассемблера */
            memcpy(assembler_card.OPERAC, "USING", 5);
            memcpy(assembler_card.OPERAND, "*,RBASE", 7);
            memcpy(assembler_card.COMM, "Assign base register", 20);

            /* и запоминаем ее */
            ZKARD();
            break;
        }
        default:
        {
            break;
        }
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала PRO на пер-*/
/* вом проходе.   Здесь   */
/* PRO - "программа"      */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала PRO на вто-*/
/* ром проходе.   Здесь   */
/* PRO - "программа"      */
/* прогр.формирует выходной файл */
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
                return PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE;
            }
            else
            {
                int i;
                for (i = 0; i < IASSTXT; i++)
                {
                    fwrite(assembler_out_text[i], 80, 1, p_asm_f);
                    fwrite("\n", 1, 1, p_asm_f);
                }
            }

            fclose(p_asm_f);
            break;
        }
        default:
            break;
    }
    return PLCMP_SEM_CALCULATOR_SUCCESS;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала RZR на пер-*/
/* вом проходе.   Здесь   */
/* RZR - "разрядность"    */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала RZR на вто-*/
/* ром проходе.   Здесь   */
/* RZR - "разрядность"    */
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


/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала TEL на пер-*/
/* вом проходе.   Здесь   */
/* TEL - "тело программы" */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала TEL на вто-*/
/* ром проходе.   Здесь   */
/* TEL - "тело программы" */

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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ZNK на пер-*/
/* вом проходе.   Здесь   */
/* ZNK - "знак операции"  */

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ZNK на вто-*/
/* ром проходе.   Здесь   */
/* ZNK - "знак операции"  */

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

/* Function for semantic calculation
 * of the non-ternimal STC on the first
 * and second phases of semantic calculation.
 * STC is "constant string enclosed into single quotation marks " */

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

/*  п р о г р а м м а     */
/* управления абстрактной */
/* ЭВМ  -  семантического */
/* вычислителя, интерпре- */
/* тирующего абстрактную  */
/* программу, сформирован-*/
/* ную синтаксическим ана-*/
/* лизатором в стеке дос- */
/* тигнутых целей.        */

/* Суть алгоритма управления в последовательной */
/* интерпретации строк стека достижений в направлении от дна к вершине */
struct plcmp_sem_calc_error_data_s plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name,
                                                               dst_t const *p_goals_achieved)
{
    int sem_calc_phase;
    plcmp_sem_calc_error_data_t err_data;

    /* Clear error data structure for later using and set default successful error code */
    memset(&err_data, 0, sizeof(plcmp_sem_calc_error_data_t));
    err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS;

    /* При этом каждая строка воспринимается как команда абстрактной ЭВМ со  
     * следующими полями: 
     * - DST.DST1 - код операции
     * - DST.DST2 - левая граница интерпретируемого фрагмента исх.текста;
     * - DST.DST4 - правая граница интерпретируемого фрагмента исх.текста
     */

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
    plcmp_sem_calc_clear_assembler_card();

    /* First and second phases of semantic calculation */
    for (sem_calc_phase = 1; sem_calc_phase < 3; sem_calc_phase++)
    {
        int dst_index;
        for (dst_index = 0; dst_index < p_goals_achieved->count; dst_index++)
        {
            int hand_num = numb(p_goals_achieved->dst_stack[dst_index].DST1, 3);
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
                plcmp_sem_calc_cook_error_data(&err_data, p_goals_achieved->dst_stack[dst_index]);
                return err_data;
            }
        }
    }

    return err_data;
}

char* plcmp_sem_calc_errmsg_by_errdata(plcmp_sem_calc_error_data_t const *err_data, char *errmsg)
{
    switch (err_data->err_code)
    {
        case PLCMP_SEM_CALCULATOR_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case PLCMP_SEM_CALCULATOR_MISMATCH_PROC_NAME_PROL_EPIL_ERROR:
            strcpy(errmsg, "Mismatch of the name of the procedure in prologue-epilogue");
            break;
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
        case PLCMP_SEM_CALCULATOR_CANT_WRITE_ASS_FILE:
            strcpy(errmsg, "Can't write to assembler file");
            break;
        case PLCMP_SEM_CALCULATOR_CONCAT_ERROR:
            strcpy(errmsg, "Capacity of the destination string is less than "
                           "total length of strings which are being concatenated");
            break;
        case PLCMP_SEM_CALCULATOR_CHAR_INIT_VERY_LONG:
            strcpy(errmsg, "String initializer is very long for this capacity");
            break;
        default:
            strcpy(errmsg, "Unknown error code for generating error message");
            break;

    }
    return errmsg;
}