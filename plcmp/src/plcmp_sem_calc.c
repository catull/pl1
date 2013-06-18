#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "plcmp_tables.h"
#include "plcmp_common.h"
#include "plcmp_lex_analyzer.h"
#include "plcmp_sem_calc.h"

/* This struct is type of assembler card
 * It is template to generate the output file record by IBM 370 assembler */
static struct assembler_card_un {
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
static char ASSTXT[MAXLTXT][80];
/* Output array index */
static int IASSTXT;

static void plcmp_sem_calc_clear_assembler_card(void)
{
    memset(&assembler_card, ' ', sizeof(assembler_card));
}

/* п р о г р а м м а      */
/* записи очередной сгене-*/
/* рированной записи вы-  */
/* ходного файла в массив */
/* ASSTXT                 */
static void ZKARD(void)
{
    memcpy(ASSTXT[IASSTXT], &assembler_card, 80);
    ++IASSTXT;
    plcmp_sem_calc_clear_assembler_card();
}

/* п р о г р а м м а      */
/* перевода двоичной      */
/* константы из ASCIIz-вида во внутреннее представление типа long int */
static long int VALUE(char *s)
{
    long int S;
    int i;

    i = 0;
    S = 0;
    while ( *(s + i) != 'B' )
    {
        S <<= 1;
        if ( *(s + i) == '1' )
        {
            S++;
        }
        i++;
    }

    return S;
}

/* п р о г р а м м а */
/* представления фрагмента*/
/* плотного текста в виде */
/* массива 9-ти символьных*/
/* лексем */
static void FORM(int dst_index)
{
    int i, j;

    for (IFORMT = 0; IFORMT < MAXFORMT; IFORMT++)
    {
        memset(FORMT[IFORMT], '\0', sizeof(FORMT[IFORMT]));
    }

    IFORMT = 0;
    j = DST[dst_index].DST2;

    FORM1:

    for (i = j; i <= DST[dst_index].DST4 + 1; i++)
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
static int AVI(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
        {
            unsigned int i;
            int dst_index = *((int *)param);

            FORM(dst_index);

            if (1 == IFORMT)
            {
                for (i = 0; i < ISYM; i++ )
                {
                    if (!strcmp(SYM[i].NAME, FORMT[0]) &&
                        (strlen(SYM[i].NAME) == strlen(FORMT[0])))
                    {
                        if ('B' == SYM[i].TYPE)
                        {
                            if (strcmp(SYM[i].RAZR, "15") <= 0)
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

                            return 0;
                        }
                        else
                        {
                            return 3;
                        }
                    }
                }

                return 4;
            }
            else
            {
                for (i = 0; i < ISYM; i++)
                {
                    if (!strcmp(SYM[i].NAME, FORMT[IFORMT - 1]) &&
                        (strlen(SYM[i].NAME) == strlen(FORMT[IFORMT - 1])))
                    {
                        if (SYM[i].TYPE == 'B')
                        {
                            if (compact_pl1_src_text[DST[dst_index].DST4 - strlen(FORMT[IFORMT-1])] == '+')
                            {
                                if (strcmp(SYM[i].RAZR, "15") <= 0 )
                                {
                                    memcpy(assembler_card.OPERAC, "AH", 2);
                                }
                                else
                                {
                                    memcpy(assembler_card.OPERAC, "A", 1);
                                }
                            }
                            else
                            {
                                if (compact_pl1_src_text[DST[dst_index].DST4 - strlen(FORMT[IFORMT-1])] == '-')
                                {
                                    if (strcmp(SYM[i].RAZR, "15") <= 0)
                                    {
                                        memcpy(assembler_card.OPERAC, "SH", 2);
                                    }
                                    else
                                    {
                                        memcpy(assembler_card.OPERAC, "S", 1);
                                    }
                                }
                                else
                                {
                                    return 5;
                                }

                            }

                            strcpy(assembler_card.OPERAND, "RRAB," );
                            strcat(assembler_card.OPERAND, FORMT[IFORMT-1]);
                            assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';
                            memcpy(assembler_card.COMM, "Formation of intermediate value", 31);
                            ZKARD();

                            return 0;
                        }
                        else
                        {
                            return 3;
                        }
                    }
                }

                return 4;

            }

            break;
        }
        default:
            break;
    }
    return 0;
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
static int BUK(int entry, void const *param)
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
    return 0;
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
static int CIF(int entry, void const *param)
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
    return 0;
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
static int IDE(int entry, void const *param)
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
    return 0;
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
static int IPE(int entry, void const *param)
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
    return 0;
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
static int IPR(int entry, void const *param)
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
    return 0;
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
static int LIT(int entry, void const *param)
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
    return 0;
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
static int MAN(int entry, void const *param)
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
    return 0;
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
static int ODC(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
        {
            int i;
            int init_pos;
            int dst_index = *((int *)param);

            FORM(dst_index);

            for (i = 0; i < ISYM; i++)
            {
                if (!strcmp(SYM[i].NAME, FORMT[1])
                    && strlen(SYM[i].NAME) == strlen(FORMT[1]))
                {
                    return 6;
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
                return 2;
            }

            strcpy(SYM[ISYM].RAZR, FORMT[init_pos - 1]);

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

                        strcpy(SYM[ISYM].INIT, FORMT[init_value_pos]);
                        memset(&(SYM[ISYM].INIT)[init_value_len], ' ', atoi(SYM[ISYM].RAZR) - init_value_len);

                        #undef init_value_pos
                        #undef init_value_len

                        break;
                    }
                    default:
                        return 2;
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
                        memset(SYM[ISYM].INIT, ' ', atoi(SYM[ISYM].RAZR));
                        break;
                    default:
                        return 2;
                }
            }

            ++ISYM;

            return 0;
        }
        case 2:
        {
            return 0;
        }
        default:
            break;
    }
    return 0;
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

static int OEN(int entry, void const *param)
{
    int dst_index = *((int *)param);
    switch (entry)
    {
        case 1:
        {
            unsigned int i;

            FORM(dst_index); /* форматирование ПЛ1-оператора END */

            /* если вторй терм оператора END записан в табл */
            for (i = 0; i < ISYM; i++)
            {
                /* SYM и его тип = "P",то:*/
                if (!strcmp(SYM[i].NAME, FORMT[1]) && 
                    (SYM[i].TYPE == 'P') && 
                    (strlen(SYM[i].NAME) == strlen(FORMT[1])))
                {
                    /* Successful completion of the function */
                    return 0;
                }
            }

            /* иначе завершение программы по ошибке */
            return 1;
        }
        case 2:
        {
            char RAB[20];
            unsigned int i;
            /* форматируем ПЛ1-оператор END */
            FORM(dst_index);

            /* формируем код безусловного возврата управления в вызывающую программу */
            memcpy(assembler_card.OPERAC, "BCR", 3);
            /* операнды команды и поле построчного коментария */
            memcpy(assembler_card.OPERAND, "15,14", 5);
            memcpy(assembler_card.COMM, "Exit from the program", 21);

            /* запомнить опреацию Ассемблера */
            ZKARD ();

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
                            strcpy(assembler_card.OPERAND, (strcmp(SYM[i].RAZR, "15") <= 0) ? "H\'" : "F\'");
                            strcat(assembler_card.OPERAND, gcvt(VALUE(SYM[i].INIT), 10, &RAB[0]));
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

            return 0;
        }
        default:
        {
            break;
        }
    }

    return 0;
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
static int OPA(int entry, void const *param)
{
    switch (entry)
    {
        case 1:
            return 0;
        case 2:
        {
            int i;
            int dst_index = *((int *)param);

            FORM(dst_index);

            for ( i = 0; i < ISYM; i++ )
            {
                if (!strcmp(SYM[i].NAME, FORMT[0])
                        && strlen(SYM[i].NAME) == strlen(FORMT[0]))
                {
                    if (SYM[i].TYPE == 'B')
                    {
                        if (strcmp(SYM[i].RAZR, "15") <= 0 )
                        {
                            memcpy(assembler_card.OPERAC, "STH", 3);
                        }
                        else
                        {
                            memcpy(assembler_card.OPERAC, "ST", 2);
                            strcpy(assembler_card.OPERAND, "RRAB,");
                            strcat(assembler_card.OPERAND, FORMT[0]);

                            assembler_card.OPERAND[strlen(assembler_card.OPERAND)] = ' ';

                            memcpy(assembler_card.COMM, "Formation of value of the arithmetic expression", 47);

                            ZKARD();

                            return 0;
                        }
                    }
                    else
                    {
                        return 3;
                    }
                }
            }

            return 4;
        }
        default:
        {
            break;
        }
    }
    return 0;
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
static int OPR(int entry, void const *param)
{
    int dst_index = (*(int *)param);
    switch (entry)
    {
        case 1:
        {
            FORM(dst_index);                                        /* форматируем оператор   */
            /* ПЛ1 PROC               */

            strcpy(SYM[ISYM].NAME, FORMT[0]);          /* перепишем имя ПЛ1-прог-*/
            /* раммы в табл. SYM,     */

            SYM[ISYM].TYPE = 'P';                        /* установим тип этого    */
            /* имени = 'P'            */
            SYM[ISYM++].RAZR[0] = '\0';                  /* установим разрядность  */
            /* равной 0               */

                                                   /* успешное завершение    */
            /* программы              */
            break;
        }
        case 2:
        {
            unsigned int i = 0;
            FORM(dst_index);
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
    return 0;
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
static int PRO(int entry, void const *param)
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
                return 7;
            }
            else
            {
                int i;
                for (i = 0; i < IASSTXT; i++)
                {
                    fwrite(ASSTXT[i], 80, 1, p_asm_f);
                    fwrite("\n", 1, 1, p_asm_f);
                }
            }

            fclose(p_asm_f);
            break;
        }
        default:
            break;
    }
    return 0;
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
static int RZR(int entry, void const *param)
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
    return 0;
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

static int TEL(int entry, void const *param)
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
    return 0;
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

static int ZNK(int entry, void const *param)
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
    return 0;
}

/* Function for semantic calculation
 * of the non-ternimal STC on the first
 * and second phases of semantic calculation.
 * STC is "constant string enclosed into single quotation marks " */

static int STC(int entry, void const *param)
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
    return 0;
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
int plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name, int *p_dst_index)                                    
{
    int i1;
    int err_code;

     /* При этом каждая строка воспринимается как команда абстрактной ЭВМ со  
      * следующими полями: 
      * - DST.DST1 - код операции
      * - DST.DST2 - левая граница интерпретируемого фрагмента исх.текста;
      * - DST.DST4 - правая граница интерпретируемого фрагмента исх.текста
      */

    int (*handler[NNETRM])(int, void const*) = {
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
    for (i1 = 1; i1 < 3; i1++)
    {
        int i2;
        for (i2 = 0; i2 < L; i2++)
        {
            int hand_num = numb(DST[i2].DST1, 3);
            switch (hand_num + 1)
            {
                case 1:
                case 9:
                case 10:
                case 11:
                case 12:
                    err_code = handler[hand_num](i1, &i2);
                    break;
                case 13:
                    err_code = handler[hand_num](i1, 1 == i1 ? NULL : p_asm_fp_name);
                    break;
                default:
                    err_code = handler[hand_num](i1, NULL);
                    break;
            }
            
            if (err_code)
            {
                *p_dst_index = i2;
                return err_code;
            }
        }
    }

    return 0;
}