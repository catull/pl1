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

/* � � � � � � � � �      */
/* ������ ��������� �����-*/
/* ���������� ������ ��-  */
/* ������� ����� � ������ */
/* ASSTXT                 */
static void ZKARD(void)
{
    memcpy(ASSTXT[IASSTXT], &assembler_card, 80);
    ++IASSTXT;
    plcmp_sem_calc_clear_assembler_card();
}

/* � � � � � � � � �      */
/* �������� ��������      */
/* ��������� �� ASCIIz-���� �� ���������� ������������� ���� long int */
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

/* � � � � � � � � � */
/* ������������� ���������*/
/* �������� ������ � ���� */
/* ������� 9-�� ����������*/
/* ������ */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� AVI �� ���-*/
/* ��� �������.   �����   */
/* AVI -   "�����.�����." */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� AVI �� ���-*/
/* ��� �������.   �����   */
/* AVI -   "�����.�����." */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� BUK �� ���-*/
/* ��� �������.   �����   */
/* BUK -   "�����"        */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� BUK �� ���-*/
/* ��� �������.   �����   */
/* BUK -   "�����"        */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� CIF �� ���-*/
/* ��� �������.   �����   */
/* CIF -   "�����"        */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� CIF �� ���-*/
/* ��� �������.   �����   */
/* CIF -   "�����"        */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� IDE �� ���-*/
/* ��� �������.   �����   */
/* IDE -   "�������������"*/

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� IDE �� ���-*/
/* ��� �������.   �����   */
/* IDE -   "�������������"*/
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� IPE �� ���-*/
/* ��� �������.   �����   */
/* IPE - "��� ����������" */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� IPE �� ���-*/
/* ��� �������.   �����   */
/* IPE - "��� ����������" */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� IPR �� ���-*/
/* ��� �������.   �����   */
/* IPR -   "��� ���������" */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� IPR �� ���-*/
/* ��� �������.   �����   */
/* IPR -   "��� ���������" */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� LIT �� ���-*/
/* ��� �������.   �����   */
/* LIT -   "�������"      */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� LIT �� ���-*/
/* ��� �������.   �����   */
/* LIT -   "�������"      */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� MAN �� ���-*/
/* ��� �������.   �����   */
/* MAN -   "��������"     */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� MAN �� ���-*/
/* ��� �������.   �����   */
/* MAN -   "��������"     */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� ODC �� ���-*/
/* ��� �������.   �����   */
/* ODC - "������.��1- DCL"*/

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� ODC �� ���-*/
/* ��� �������.   �����   */
/* ODC - "������.��1- DCL"*/
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� OEN �� ���-*/
/* ��� �������.   �����   */
/* OEN - "������.��1-END" */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� OEN �� ���-*/
/* ��� �������.   �����   */
/* OEN - "������.��1-END" */
/* ���������    ��������� */
/* ������ ����������������*/
/* ����������� ��1-����-  */
/* �����                  */

static int OEN(int entry, void const *param)
{
    int dst_index = *((int *)param);
    switch (entry)
    {
        case 1:
        {
            unsigned int i;

            FORM(dst_index); /* �������������� ��1-��������� END */

            /* ���� ����� ���� ��������� END ������� � ���� */
            for (i = 0; i < ISYM; i++)
            {
                /* SYM � ��� ��� = "P",��:*/
                if (!strcmp(SYM[i].NAME, FORMT[1]) && 
                    (SYM[i].TYPE == 'P') && 
                    (strlen(SYM[i].NAME) == strlen(FORMT[1])))
                {
                    /* Successful completion of the function */
                    return 0;
                }
            }

            /* ����� ���������� ��������� �� ������ */
            return 1;
        }
        case 2:
        {
            char RAB[20];
            unsigned int i;
            /* ����������� ��1-�������� END */
            FORM(dst_index);

            /* ��������� ��� ������������ �������� ���������� � ���������� ��������� */
            memcpy(assembler_card.OPERAC, "BCR", 3);
            /* �������� ������� � ���� ����������� ���������� */
            memcpy(assembler_card.OPERAND, "15,14", 5);
            memcpy(assembler_card.COMM, "Exit from the program", 21);

            /* ��������� �������� ���������� */
            ZKARD ();

            /* ����� ���� ���� ������������ �������������
             * �������������� DC ��� ������� ��������������
             * ��������� � ����.SYM
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

            /* ����� ���� ���� �����- */
            /* �������� ������������- */
            /* ��� EQU-����������, ��-*/
            /* ���������� ������� �   */
            /* ������� �������� ������*/
            /* ���������� */

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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� OPA �� ���-*/
/* ��� �������.   �����   */
/* OPA - "������.�������- */
/* ����� ��������������   */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� OPA �� ���-*/
/* ��� �������.   �����   */
/* OPA - "������.�������- */
/* ����� ��������������   */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� OPR �� ���-*/
/* ��� �������.   �����   */
/* OPR - "������.��1-PROC"*/

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� OPR �� ���-*/
/* ��� �������.   �����   */
/* OPR - "������.��1-PROC"*/
/* ���������    ��������� */
/* ������ ����������������*/
/* �����������  ��������  */
/* ��1-���������          */
static int OPR(int entry, void const *param)
{
    int dst_index = (*(int *)param);
    switch (entry)
    {
        case 1:
        {
            FORM(dst_index);                                        /* ����������� ��������   */
            /* ��1 PROC               */

            strcpy(SYM[ISYM].NAME, FORMT[0]);          /* ��������� ��� ��1-����-*/
            /* ����� � ����. SYM,     */

            SYM[ISYM].TYPE = 'P';                        /* ��������� ��� �����    */
            /* ����� = 'P'            */
            SYM[ISYM++].RAZR[0] = '\0';                  /* ��������� �����������  */
            /* ������ 0               */

                                                   /* �������� ����������    */
            /* ���������              */
            break;
        }
        case 2:
        {
            unsigned int i = 0;
            FORM(dst_index);
            /* ����������� ��������   */
            /* ��1 - "������ ��������-*/
            /* ���� �����"            */

            while ('\0' != FORMT[0][i])
            {
                assembler_card.METKA[i] = FORMT[0][i];  /* ������� ���� ���������� */
                ++i;
            }


            /* ��� ����� � START-����-*/
            /* ���������� ����������  */

            /* ����������� ��� � �������� � START-�������������� ���������� */
            memcpy(assembler_card.OPERAC, "START", 5);
            memcpy(assembler_card.OPERAND, "0", 1);
            memcpy(assembler_card.COMM, "Start of the program", 20);

            /* ���������� ����� ���������� */
            ZKARD();

            /* ��������� BALR-�������� ���������� */
            memcpy(assembler_card.OPERAC, "BALR", 4 );
            memcpy(assembler_card.OPERAND, "RBASE,0", 7);
            memcpy(assembler_card.COMM, "Load base register", 18);

            /* � ���������� �� */
            ZKARD();

            /* ��������� USING-�������������� ���������� */
            memcpy(assembler_card.OPERAC, "USING", 5);
            memcpy(assembler_card.OPERAND, "*,RBASE", 7);
            memcpy(assembler_card.COMM, "Assign base register", 20);

            /* � ���������� �� */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� PRO �� ���-*/
/* ��� �������.   �����   */
/* PRO - "���������"      */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� PRO �� ���-*/
/* ��� �������.   �����   */
/* PRO - "���������"      */
/* �����.��������� �������� ���� */
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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� RZR �� ���-*/
/* ��� �������.   �����   */
/* RZR - "�����������"    */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� RZR �� ���-*/
/* ��� �������.   �����   */
/* RZR - "�����������"    */
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


/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� TEL �� ���-*/
/* ��� �������.   �����   */
/* TEL - "���� ���������" */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� TEL �� ���-*/
/* ��� �������.   �����   */
/* TEL - "���� ���������" */

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

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� ZNK �� ���-*/
/* ��� �������.   �����   */
/* ZNK - "���� ��������"  */

/* � � � � � � � � �      */
/* ��������. ����������   */
/* ����������� ZNK �� ���-*/
/* ��� �������.   �����   */
/* ZNK - "���� ��������"  */

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

/*  � � � � � � � � �     */
/* ���������� ����������� */
/* ���  -  �������������� */
/* �����������, ��������- */
/* ��������� �����������  */
/* ���������, �����������-*/
/* ��� �������������� ���-*/
/* ��������� � ����� ���- */
/* �������� �����.        */

/* ���� ��������� ���������� � ���������������� */
/* ������������� ����� ����� ���������� � ����������� �� ��� � ������� */
int plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name, int *p_dst_index)                                    
{
    int i1;
    int err_code;

     /* ��� ���� ������ ������ �������������� ��� ������� ����������� ��� ��  
      * ���������� ������: 
      * - DST.DST1 - ��� ��������
      * - DST.DST2 - ����� ������� ����������������� ��������� ���.������;
      * - DST.DST4 - ������ ������� ����������������� ��������� ���.������
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