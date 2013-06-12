#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "pl_common.h"
#include "pl_tables.h"

/*
 * **************************** 
 * Database of the PL1-compiler
 * ****************************
 */

/* 
 * Block of declaration of the array with the source text 
 */

/* Length of the array */
int NISXTXT;
/* Content of the array */
char ISXTXT[MAXNISXTXT][80];


/* 
 * Block of declaration of the working variables 
 */

/* cycles counters */
int I1,I2,I3,I4;
/* Last processed symbol in the compact source text */
char PREDSYM;
/* Place of storage of compact source text */
char STROKA[NSTROKA];

int I; /* current index in compact text */
int J; /* current index in the table of grammar rules */
int K; /* current index in the stack of goals */
int L; /* current index in the stack of achieved goals */

/* template to generate the output file record by IBM 370 assembler */
union {
    char BUFCARD[80];

    struct {
        char METKA[8];
        char PROB1;
        char OPERAC[5];
        char PROB2;
        char OPERAND[12];
        char PROB3;
        char COMM[52];
    } _BUFCARD;
} ASS_CARD;

/* Array for storage of output text written in IBM 370 ASSEMBLER */
char ASSTXT[MAXLTXT][80];
/* output array index */
int IASSTXT;
/* Array for formatted (a sequence of 9-th positional lines-tokens) 
 * representation interpreted fragment for compact source text */
char FORMT[MAXFORMT][9];
/* formatted array index */
int IFORMT;

/* Программа уплотнения исходного текста путем удаления "лишних" */
/* пробелов, выполняющая роль примитивного лексического анализатора */
void compress_ISXTXT(void)
{
    I3 = 0;
    for (I1 = 0 ; I1 < NISXTXT ; I1++)
    {
        for (I2 = 0; I2 < 80; I2++)
        {
            if (ISXTXT[I1][I2] != '\x0')
            {
                if (ISXTXT[I1][I2] == ' '
                    && (PREDSYM == ' '
                        || PREDSYM == ';'
                        || PREDSYM == ')'
                        || PREDSYM == ':'
                        || PREDSYM == '(')
                   )
                {
                    PREDSYM = ISXTXT[I1][I2];
                    goto L2;
                }

                if((ISXTXT[I1][I2] == '+'
                    || ISXTXT[I1][I2] == '-'
                    || ISXTXT[I1][I2] == '='
                    || ISXTXT[I1][I2] == '('
                    || ISXTXT[I1][I2] == ')'
                    || ISXTXT[I1][I2] == '*')
                   && PREDSYM == ' ')
                {
                    I3--;
                    goto L1;
                }

                if (ISXTXT[I1][I2] == ' '
                    && (PREDSYM == '+'
                        || PREDSYM == '-' 
                        || PREDSYM == '='
                        || PREDSYM == '*')
                   )
                {
                    goto L2;
                }

                L1:

                PREDSYM = ISXTXT[I1][I2];
                STROKA[I3] = PREDSYM;
                I3++;

                L2:

                continue;
            }
            else
            {
                break;
            }
        }
    }

    STROKA[I3] = '\x0';
}

/* Построение таблицы  */
/* преемников из матрицы  */
/* смежности по алгоритму */
/* Варшалла */
void build_TPR(void)
{
    for (I1 = 0; I1 < NNETRM; I1++)
    {
        for (I2 = 0; I2 < NVXOD; I2++)
        {
            if (TPR[I2][I1] & (I1 != I2))
            {
                for (I3 = 0; I3 < NNETRM; I3++)
                {
                    TPR[I2][I3] |= TPR[I1][I3];
                }
            }
        }
    }
}

/* программа заполнения ячейки стека поставленных целей */
void mcel(char *T1, int T2, int T3)
{
    strcpy(CEL[K].CEL1, T1);
    CEL[K].CEL2 = T2;
    CEL[K].CEL3 = T3;
    K++;
}

/* программа заполнения ячейки стека достигнутых целей */
void mdst(char *T1, int T2, int T3, int T4, int T5)
{
    strcpy(DST[L].DST1, T1);
    DST[L].DST2 = T2;
    DST[L].DST3 = T3;
    DST[L].DST4 = T4;
    DST[L].DST5 = T5;
    L++;
}

/* п р о г р а м м а      */
/* вычисления порядкового */
/* номера строки в табл.  */
/* VXOD, соответствующей  */
/* строке-параметру функц.*/
int numb(char *T1, int T2)
{
    for (I1 = 0; I1 < NVXOD; I1++)
    {
        int k;
        for (k = 0; k < T2; k++)
        {
            if ((*(T1+k) != VXOD[I1].SYM[k]))
            {
                goto numb1;
            }
        }
        if (VXOD[I1].SYM[k] == '\x0' || VXOD[I1].SYM[k] == ' ')
        {
            return I1;
        }
        numb1:
            continue;
    }
    return -1;
}

/*   п р о г р а м м а    */
/*   построения  дерева   */
/* синтаксического разбора, выполняющая роль синтаксического анализатора */
int sint_ANAL(void)
{
    I4 = 0;

    K = 0;
    L = 0;
    I = 0;
    J = 1;

    mcel("PRO", I, 999);

    if (!TPR[numb(&STROKA[I], 1)][numb("PRO", 3)])
    {
        return 1;
    }

    L2:

    J = VXOD[numb(&STROKA[I],1 )].VX;

    L3:

    J = SINT[J].POSL;

    L31:

    I++;

    if (I > I4)
    {
        I4 = I;
    }

    if (VXOD[numb(SINT[J].DER, 3)].TYP == 'T')
    {

        if (STROKA[I] == SINT[J].DER[0])
        {
            goto L3;    
        }
        else
        {
            goto L8;
        }

    }

    if (SINT[SINT[J].POSL].DER[0] == '*')
    {
        I--;

        if (!strcmp(SINT[J].DER, CEL[K-1].CEL1))
        {
            mdst(CEL[K-1].CEL1, CEL[K-1].CEL2, CEL[K-1].CEL3, I, J);

            if (!strcmp(CEL[K-1].CEL1, "PRO"))
            {
                return 0;
            }

            if (TPR[numb(CEL[K-1].CEL1, 3)][numb(CEL[K-1].CEL1, 3)])
            {
                J = VXOD[numb(CEL[K-1].CEL1, 3 )].VX;
                goto L3;
            }

            L6:

            J = CEL[K-1].CEL3;
            K--;
            goto L3;
        }

        if (!TPR[numb(SINT[J].DER, 3)][numb(CEL[K-1].CEL1, 3)])
        {
            goto L9;    
        }

        mdst(SINT[J].DER, CEL[K-1].CEL2, 0, I, J);
        J = VXOD[numb(SINT[J].DER, 3)].VX;
        goto L3;
    }

    if (!TPR[numb(&STROKA[I], 1)][numb(SINT[J].DER, 3)])
    {
        goto L8;    
    }

    mcel(SINT[J].DER, I, J);
    goto L2;

    L8:

    I--;

    L9:

    if (SINT[J].ALT != 0)
    {
        J = SINT[J].ALT;
        goto L31;
    }

    J = SINT[J].PRED;

    if ((VXOD[numb(SINT[J].DER, 3)].TYP == 'N' ) && (SINT[J].PRED > 0))
    {
        mcel (DST[L-1].DST1, DST[L-1].DST2, DST[L-1].DST3);

    L10:

        J = DST[L-1].DST5;
        L--;
        goto L9;
    }

    if ((VXOD[numb(SINT[J].DER, 3)].TYP == 'N' ) && (SINT[J].PRED == 0))
    {

        if (!strcmp(CEL[K-1].CEL1, DST[L-1].DST1))
        {
            goto L6;    
        }
        else
        {
            goto L10;    
        }
    
    }

    if (SINT[J].PRED > 0)
    {
        goto L8;
    }

    J = CEL[K-1].CEL3;
    K--;

    if (999 == J)
    {
        return 2;
    }
    else
    {
        goto L8;    
    }   
}

/* таблица имен меток и   */
/* переменных, заполняемая*/
/* на первом проходе се-  */
/* мантического вычисления*/
/* и используемая на вто- */
/* ром проходе семантичес-*/
/* кого вычисления        */
struct {
    char NAME[8];
    char TYPE;
    char RAZR[5];
    char INIT[50];
} SYM[NSYM];

/* текущий индекс таблицы имен */
int ISYM = 0;
/* хранилище имени транслируемой программы */
char NFIL[30] = "\x0";                             

/* п р о г р а м м а      */
/* перевода двоичной      */
/* константы из ASCIIz-вида во внутреннее представление типа long int */
long int VALUE(char *s)
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
void FORM(void)
{
    int i, j;

    for (IFORMT = 0; IFORMT < MAXFORMT; IFORMT++)
    {
        memcpy(FORMT[IFORMT], "\x0\x0\x0\x0\x0\x0\x0\x0\x0", 9);    
    }


    IFORMT = 0;
    j = DST [I2].DST2;

    FORM1:

    for ( i = j; i <= DST [I2].DST4+1; i++ )
    {
        if (STROKA[i] == ':' || STROKA[i] == ' '
            || STROKA[i] == '(' || STROKA[i] == ')'
            || STROKA[i] == ';' || STROKA[i] == '+'
            || STROKA[i] == '-' || STROKA[i] == '='
            || STROKA[i] == '*')
        {
            FORMT [IFORMT] [i-j] = '\x0';
            IFORMT ++;
            j = i+1;
            goto FORM1;
        }
        else
        {
            FORMT[IFORMT][i-j] = STROKA[i];
        }
    }

    return;
}

/* п р о г р а м м а      */
/* записи очередной сгене-*/
/* рированной записи вы-  */
/* ходного файла в массив */
/* ASSTXT                 */
void ZKARD(void)
{
    unsigned int i;

    memcpy(ASSTXT[IASSTXT++], ASS_CARD.BUFCARD, 80);

    for(i = 0; i < 79; i++)
    {
        ASS_CARD.BUFCARD[i] = ' ';
    }
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала AVI на пер-*/
/* вом проходе.   Здесь   */
/* AVI -   "арифм.выраж." */
int AVI1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала BUK на пер-*/
/* вом проходе.   Здесь   */
/* BUK -   "буква"        */
int BUK1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала CIF на пер-*/
/* вом проходе.   Здесь   */
/* CIF -   "цифра"        */
int CIF1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IDE на пер-*/
/* вом проходе.   Здесь   */
/* IDE -   "идентификатор"*/
int IDE1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPE на пер-*/
/* вом проходе.   Здесь   */
/* IPE - "имя переменной" */
int IPE1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPR на пер-*/
/* вом проходе.   Здесь   */
/* IPR -   "имя программы" */
int IPR1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала LIT на пер-*/
/* вом проходе.   Здесь   */
/* LIT -   "литерал"      */
int LIT1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала MAN на пер-*/
/* вом проходе.   Здесь   */
/* MAN -   "мантисса"     */
int MAN1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ODC на пер-*/
/* вом проходе.   Здесь   */
/* ODC - "операт.ПЛ1- DCL"*/
int ODC1(void)
{
    int i;
    FORM ();                                        /* форматирование ПЛ1-опе-*/
                          /* ратора DCL             */

    for ( i = 0; i < ISYM; i++ )                    /* если фиксируем повтор- */
    {                                              /* повторное объявление   */
        if (!strcmp(SYM[i].NAME, FORMT[1])      /* второго терма оператора*/ /* DCL, то                */ /* завершение программы   */
            && strlen(SYM[i].NAME) == strlen(FORMT[1])) /* по ошибке              */
        {
            return 6;
        }
    }

    strcpy (SYM[ISYM].NAME, FORMT [1]);          /* при отсутствии повтор- */
    strcpy (SYM[ISYM].RAZR, FORMT [4]);          /* ного объявления иденти-*/
                          /* фикатора запоминаем его*/
                          /* вместе с разрядностью в*/
                          /* табл.SYM               */

    /* если идентификатор оп- */
    /* ределен как bin fixed, */
    if (!strcmp(FORMT[2], "BIN") && !strcmp(FORMT[3], "FIXED"))
    {
        SYM [ISYM].TYPE = 'B';                        /* то устанавливаем тип   */
                          /* идентификатора = 'B' и */
        goto ODC11;                                   /* идем на продолжение об-*/
                          /* работки, а             */
    }
    else                                            /* иначе                  */
    {
        SYM [ISYM].TYPE = 'U';                        /* устанавливаем тип иден-*/
                          /* тификатора = 'U'  и    */
        return 2;                                     /* завершаем программу    */
                          /* по ошибке              */
    }

    ODC11:                                            /* если идентификатор     */
                          /* имеет начальную иници- */
    if (!strcmp(FORMT[5], "INIT" ))           /* ализацию, то запомина- */
    {
        strcpy ( SYM [ISYM++].INIT, FORMT [6] );       /* ем в табл. SYM это на- *//* чальное значение, а    */
    }
    /* иначе                  */
    /* инициализируем иденти- */
    /* фикатор нулем          */
    else
    {
        strcpy ( SYM [ISYM++].INIT, "0B" );
    }

    /* успешное завешение     */
    /* программы              */
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OEN на пер-*/
/* вом проходе.   Здесь   */
/* OEN - "операт.ПЛ1-END" */
int OEN1(void)
{
    unsigned int i;

    FORM(); /* форматирование ПЛ1-оператора END */

    /* если вторй терм оператора END записан в табл */
    for (i = 0; i < ISYM; i++)
    {
        /* SYM и его тип = "P",то:*/
        if (!strcmp(SYM[i].NAME, FORMT[1])
            && (SYM[i].TYPE == 'P') 
            && strlen(SYM[i].NAME) == strlen(FORMT[1]))
        {
            /* успешное завершение программы */
            return 0;        
        }
    }

    /* иначе завершение программы по ошибке */
    return 1;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPA на пер-*/
/* вом проходе.   Здесь   */
/* OPA - "операт.присваи- */
/* вания арифметический   */
int OPA1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPR на пер-*/
/* вом проходе.   Здесь   */
/* OPR - "операт.ПЛ1-PROC"*/
int OPR1(void)
{
    FORM ();                                        /* форматируем оператор   */
                          /* ПЛ1 PROC               */

    strcpy(SYM[ISYM].NAME, FORMT[0]);          /* перепишем имя ПЛ1-прог-*/
                          /* раммы в табл. SYM,     */

    SYM[ISYM].TYPE = 'P';                        /* установим тип этого    */
                          /* имени = 'P'            */
    SYM[ISYM++].RAZR[0] = '\x0';                  /* установим разрядность  */
                          /* равной 0               */

    return 0;                                       /* успешное завершение    */
                          /* программы              */
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала PRO на пер-*/
/* вом проходе.   Здесь   */
/* PRO - "программа"      */
int PRO1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала RZR на пер-*/
/* вом проходе.   Здесь   */
/* RZR - "разрядность"    */
int RZR1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала TEL на пер-*/
/* вом проходе.   Здесь   */
/* TEL - "тело программы" */
int TEL1(void)
{
    return 0;
}


/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ZNK на пер-*/
/* вом проходе.   Здесь   */
/* ZNK - "знак операции"  */
int ZNK1(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала AVI на вто-*/
/* ром проходе.   Здесь   */
/* AVI -   "арифм.выраж." */
int AVI2(void)
{
    unsigned int i;
    FORM();                                        /*форматируем правую часть*/
                          /*арифметического ПЛ1-опе-*/
                          /*ратора присваивания     */

  if ( IFORMT == 1 )                              /* если правая часть одно-*/
     {                                            /* термовая, то:          */
    for ( i = 0; i < ISYM; i++ )                  /* ищем этот терм в табли-*/
     {                                            /* це имен  и             */
      if ( !strcmp ( SYM [i].NAME, FORMT [0] )  &&/* если находим, то:      */
       strlen ( SYM [i].NAME ) ==
                  strlen ( FORMT [0] )
     )
       {
      if ( SYM [i].TYPE == 'B' )              /* в случае типа=bin fixed*/
       {

        if ( strcmp ( SYM [i].RAZR, "15" )    /* и разрядности <= 15    */
                         <= 0 )
         memcpy ( ASS_CARD._BUFCARD.OPERAC,   /* формируем код ассембле-*/
                    "LH", 2 );/* ровской операции LH,   */
        else
         memcpy ( ASS_CARD._BUFCARD.OPERAC,   /* а при разрядности >15  */
                     "L", 1 );/* формируем код ассембле-*/
                          /* ровской операции L     */

        strcpy ( ASS_CARD._BUFCARD.OPERAND,   /*       формируем        */
                    "RRAB," );/*       первый  и        */
        strcat ( ASS_CARD._BUFCARD.OPERAND,   /* второй операнды ассемб-*/
                       FORMT [0]);/* леровской операции     */

        ASS_CARD._BUFCARD.OPERAND [ strlen    /* вставляем разделитель  */
         ( ASS_CARD._BUFCARD.OPERAND ) ] = ' ';

        memcpy ( ASS_CARD._BUFCARD.COMM,      /* и построчный коментарий*/
         "Загрузка переменной в регистр", 29 );

        ZKARD ();                             /* запомнить операцию ас- */
                          /* семблера  и            */
        return 0;                             /* завершить программу    */
       }
      else
       return 3;                              /* если тип терма не bin  */
                          /* fixed,то выход по ошиб-*/
                          /* ке                     */
       }
     }
    return 4;                                     /* если терм-идентификатор*/
                          /* неопределен, то выход  */
                          /* по ошибке              */
   }
  else                                            /* если правая часть ариф-*/
                          /* метического выражения  */
                          /* двухтермовая, то:      */
   {
    for ( i = 0; i < ISYM; i++ )                  /* если правый терм ариф- */
     {                                            /* метического выражения  */
      if ( !strcmp ( SYM [i].NAME,                /*определен в табл.SYM,то:*/
                FORMT [IFORMT-1] )  &&
       strlen ( SYM [i].NAME ) ==
               strlen ( FORMT [IFORMT-1] )
     )
       {
      if ( SYM [i].TYPE == 'B' )              /* если тип правого опе-  */
       {                                      /* ранда bin fixed, то:   */

        if ( STROKA [ DST [I2].DST4 -         /* если знак опер."+",то: */
         strlen( FORMT [IFORMT-1] ) ] == '+' )
         {
          if ( strcmp ( SYM [i].RAZR, "15" )  /* если разрядность прав. */
                        <= 0 )/* операнда <= 15, то:    */
           memcpy ( ASS_CARD._BUFCARD.OPERAC,
                    "AH", 2 );/* формируем код ассембле-*/
          else                                /* ровской операции "AH",а*/
           memcpy ( ASS_CARD._BUFCARD.OPERAC,
                     "A", 1 );/* иначе - "A"            */
         }

        else

         {
          if ( STROKA [ DST [I2].DST4 -       /* если же знак операции  */
         strlen ( FORMT [IFORMT-1] ) ] == /* арифметического выра-  */
                         '-' )/* жения "-", то:         */

           {
        if ( strcmp ( SYM [i].RAZR, "15" )/* при разрядности ариф-  */
                        <= 0 )/* метич.выраж.<= 15      */
         memcpy( ASS_CARD._BUFCARD.OPERAC,/* формируем код ассембле-*/
                    "SH", 2 );/* ровской операции "SH",F*/
        else
         memcpy( ASS_CARD._BUFCARD.OPERAC,/* иначе - "S"            */
                     "S", 1 );
           }

          else

           return 5;                          /* если знак операции не  */
                          /* "+" и не "-", то завер-*/
                          /* шение  программы  по   */
                          /* ошибке                 */
         }
                          /* формируем:             */
        strcpy ( ASS_CARD._BUFCARD.OPERAND,   /* - первый операнд ассем-*/
                    "RRAB," );/*блеровской операции;    */
        strcat ( ASS_CARD._BUFCARD.OPERAND,   /* - второй операнд ассем-*/
                   FORMT [IFORMT-1] );/*блеровской операции;    */
        ASS_CARD._BUFCARD.OPERAND [ strlen
          ( ASS_CARD._BUFCARD.OPERAND )] =/* - разделяющий пробел;  */
                          ' ';
        memcpy ( ASS_CARD._BUFCARD.COMM,
       "Формирование промежуточного значения",/* - построчный коментарий*/
                         36 );
        ZKARD ();                             /* запоминание ассембле-  */
                          /* ровской операции       */

        return 0;                             /* успешное завершение    */
                          /* пограммы               */
       }
      else
       return 3;                              /* если тип правого опе-  */
                          /* ранда арифметического  */
                          /* выражения не bin fixed,*/
                          /* то завершение програм- */
                          /* мы по ошибке           */
       }
     }
    return 4;                                     /* если правый операнд    */
                          /* арифметического выраже-*/
                          /*ния не определен в табл.*/
                          /* SYM, то завершить про- */
                          /* грамму по ошибке       */
   }
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала BUK на вто-*/
/* ром проходе.   Здесь   */
/* BUK -   "буква"        */
int BUK2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала CIF на вто-*/
/* ром проходе.   Здесь   */
/* CIF -   "цифра"        */
int CIF2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IDE на вто-*/
/* ром проходе.   Здесь   */
/* IDE -   "идентификатор"*/
int IDE2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPE на вто-*/
/* ром проходе.   Здесь   */
/* IPE - "имя переменной" */
int IPE2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала IPR на вто-*/
/* ром проходе.   Здесь   */
/* IPR -   "имя программы" */
int IPR2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала LIT на вто-*/
/* ром проходе.   Здесь   */
/* LIT -   "литерал"      */
int LIT2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала MAN на вто-*/
/* ром проходе.   Здесь   */
/* MAN -   "мантисса"     */
int MAN2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ODC на вто-*/
/* ром проходе.   Здесь   */
/* ODC - "операт.ПЛ1- DCL"*/
int ODC2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OEN на вто-*/
/* ром проходе.   Здесь   */
/* OEN - "операт.ПЛ1-END" */
/* программа    формирует */
/* эпилог ассемблеровского*/
/* эквивалента ПЛ1-прог-  */
/* раммы                  */
int OEN2(void)
{
    char RAB[20];
    unsigned int i;
    /* форматируем ПЛ1-оператор END */
    FORM();

    /* формируем код безусловного возврата управления в вызывающую программу */
    memcpy(ASS_CARD._BUFCARD.OPERAC, "BCR", 3);
    /* операнды команды и поле построчного коментария */
    memcpy(ASS_CARD._BUFCARD.OPERAND, "15,14", 5);
    memcpy ( ASS_CARD._BUFCARD.COMM, "Выход из программы", 18);

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
            if ('B' == SYM[i].TYPE)                  
            {
                strcpy(ASS_CARD._BUFCARD.METKA, SYM[i].NAME);         
                ASS_CARD._BUFCARD.METKA[strlen(ASS_CARD._BUFCARD.METKA)] = ' ';
                memcpy(ASS_CARD._BUFCARD.OPERAC, "DC", 2); 

                if (strcmp(SYM[i].RAZR, "15") <= 0)
                {
                    strcpy(ASS_CARD._BUFCARD.OPERAND, "H\'");
                }
                else
                {
                    strcpy(ASS_CARD._BUFCARD.OPERAND, "F\'");               
                }

                strcat(ASS_CARD._BUFCARD.OPERAND, gcvt(VALUE(SYM[i].INIT), 10, &RAB[0]));
                ASS_CARD._BUFCARD.OPERAND[strlen(ASS_CARD._BUFCARD.OPERAND)] = '\'';

                memcpy(ASS_CARD._BUFCARD.COMM, "Определение переменной", 22);
                ZKARD ();
            }
        }
    }

    /* далее идет блок декла- */
    /* ративных ассемблеровс- */
    /* ких EQU-операторов, оп-*/
    /* ределяющих базовый и   */
    /* рабочий регистры общего*/
    /* назначения */

    memcpy(ASS_CARD._BUFCARD.METKA, "RBASE", 5);
    memcpy(ASS_CARD._BUFCARD.OPERAC, "EQU",3);
    memcpy(ASS_CARD._BUFCARD.OPERAND, "15", 2);
                          
    ZKARD();

    memcpy(ASS_CARD._BUFCARD.METKA, "RRAB", 4);
    memcpy(ASS_CARD._BUFCARD.OPERAC, "EQU", 3);
    memcpy(ASS_CARD._BUFCARD.OPERAND, "5", 1);
            
    ZKARD();

    memcpy(ASS_CARD._BUFCARD.OPERAC, "END", 3);

    i = 0;

    while (FORMT[1][i] != '\x0')
    {
        ASS_CARD._BUFCARD.OPERAND[i] = FORMT[1][i];
        ++i;
    }

    memcpy(ASS_CARD._BUFCARD.COMM, "Конец программы", 15);

    ZKARD();

    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPA на вто-*/
/* ром проходе.   Здесь   */
/* OPA - "операт.присваи- */
/* вания арифметический   */
int OPA2(void)
{
    int i;

    FORM ();

    for ( i = 0; i < ISYM; i++ )
    {
        if (!strcmp(SYM[i].NAME, FORMT[0])
            && strlen(SYM[i].NAME) == strlen(FORMT[0]))
        {
            if (SYM[i].TYPE == 'B')
            {
                if (strcmp(SYM[i].RAZR, "15") <= 0 )
                {
                    memcpy(ASS_CARD._BUFCARD.OPERAC, "STH", 3);
                }
                else
                {
                    memcpy(ASS_CARD._BUFCARD.OPERAC, "ST", 2);
                    strcpy(ASS_CARD._BUFCARD.OPERAND, "RRAB,");
                    strcat(ASS_CARD._BUFCARD.OPERAND, FORMT[0]);

                    ASS_CARD._BUFCARD.OPERAND[strlen(ASS_CARD._BUFCARD.OPERAND)] = ' ';

                    memcpy(ASS_CARD._BUFCARD.COMM, "Формирование значения арифм.выражения", 37);
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

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала OPR на вто-*/
/* ром проходе.   Здесь   */
/* OPR - "операт.ПЛ1-PROC"*/
/* программа    формирует */
/* пролог ассемблеровского*/
/* эквивалента  исходной  */
/* ПЛ1-программы          */
int OPR2(void)
{
    unsigned int i;
    FORM();
    /* форматируем оператор   */
    /* ПЛ1 - "начало процедур-*/
    /* ного блока"            */
    while (FORMT[0][i] != '\x0')
    {
        ASS_CARD._BUFCARD.METKA[i] = FORMT[0][i];  /* нулевой терм используем*/
        ++i;
    }

    /* как метку в START-псев-*/
    /* дооперации Ассемблера  */

    /* достраиваем код и операнды в START-псевдооперации Ассемблера */
    memcpy(ASS_CARD._BUFCARD.OPERAC, "START", 5);
    memcpy(ASS_CARD._BUFCARD.OPERAND, "0", 1);
    memcpy(ASS_CARD._BUFCARD.COMM, "Начало программы", 16 );

    /* запоминаем карту Ассемблера */
    ZKARD();

    /* формируем BALR-операцию Ассемблера */
    memcpy(ASS_CARD._BUFCARD.OPERAC, "BALR", 4 ); 
    memcpy(ASS_CARD._BUFCARD.OPERAND, "RBASE,0", 7);
    memcpy(ASS_CARD._BUFCARD.COMM, "Загрузить регистр базы", 22);

    /* и запоминаем ее */
    ZKARD();

    /* формируем USING-псевдооперацию Ассемблера */
    memcpy(ASS_CARD._BUFCARD.OPERAC, "USING", 5);
    memcpy(ASS_CARD._BUFCARD.OPERAND, "*,RBASE", 7);
    memcpy(ASS_CARD._BUFCARD.COMM, "Назначить регистр базой", 23);

    /* и запоминаем ее */
    ZKARD();

    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала PRO на вто-*/
/* ром проходе.   Здесь   */
/* PRO - "программа"      */
/* прогр.формирует выходной файл */
int PRO2(void)
{
    FILE *fp;

    /* сформировать имя выходного файла */
    strcat(NFIL, "ass");

    fp = fopen(NFIL, "wb");
    if (NULL == fp)
    {
        return 7;
    }
    else
    {
        fwrite(ASSTXT, 80, IASSTXT, fp);
    }
    
    fclose(fp);
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала RZR на вто-*/
/* ром проходе.   Здесь   */
/* RZR - "разрядность"    */
int RZR2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала TEL на вто-*/
/* ром проходе.   Здесь   */
/* TEL - "тело программы" */
int TEL2(void)
{
    return 0;
}

/* п р о г р а м м а      */
/* семантич. вычисления   */
/* нетерминала ZNK на вто-*/
/* ром проходе.   Здесь   */
/* ZNK - "знак операции"  */
int ZNK2(void)
{
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
int gen_COD(void)                                    /*интерпретации строк сте-*/
{                                                /*ка достижений  в направ-*/
    int NOSH;                                       /*лении от дна к вершине. */

    int (*FUN[NNETRM][2])(void) =                    /*При этом каждая строка  */
    {                                              /*воспринимается как кома-*/
        {/*    1  */    AVI1, AVI2 },                /*нда абстрактной ЭВМ со  */
        {/*    2  */    BUK1, BUK2 },                /*следующими полями:      */
        {/*    3  */    CIF1, CIF2 },
        {/*    4  */    IDE1, IDE2 },        /* - DST.DST1 - код опера-*/
        {/*    5  */    IPE1, IPE2 },            /*ции;                    */
        {/*    6  */    IPR1, IPR2 },
        {/*    7  */    LIT1, LIT2 },                /* - DST.DST2 - левая гра-*/
        {/*    8  */    MAN1, MAN2 },        /*ница интерпретируемого  */
        {/*    9  */    ODC1, ODC2 },        /*фрагмента исх.текста;   */
        {/*   10  */    OEN1, OEN2 },
        {/*   11  */    OPA1, OPA2 },        /* - DST.DST4 -правая гра-*/
        {/*   12  */    OPR1, OPR2 },        /*ница интерпретируемого  */
        {/*   13  */    PRO1, PRO2 },        /*фрагмента исх.текста.   */
        {/*   14  */    RZR1, RZR2 },
        {/*   15  */    TEL1, TEL2 },
        {/*   16  */    ZNK1, ZNK2 }
    };

    /* организация первого */
    /* прохода семантического */
    /* вычисления             */
    for (I2 = 0; I2 < L; I2++)
    {
        NOSH = FUN[numb(DST[I2].DST1, 3)][0]();
        if (NOSH)
        {
            return NOSH;
        }
    }

    /* организация второго */
    /* прохода семантического */
    /* вычисления */
    for (I2 = 0; I2 < L; I2++)
    {
        NOSH = FUN[numb(DST[I2].DST1, 3)][1]();
        if (NOSH)
        {
            return NOSH;
        }
    }

    return 0;
}

/* п р о г р а м м а, */
/* организующая последова-*/
/* тельную обработку ис-  */
/* ходного текста: */
/* - лексич.анализатором; */
/* - синтаксич.анализат.; */
/* - семантич.вычислителем*/
int main (int argc, char **argv )
{
    PREDSYM = ' ';

    FILE *fp;
    char *ptr = argv[1];

    /* Get name of translated program */
    strcpy(NFIL, ptr);
    /* программы из командной */
    /* строки в рабочее поле */
    /* проверяем корректность */
    /* командной строки */
    if (argc != 2)
    {
        /* по ошибке в командной строке. Выдать диагностику и завершить трансляцию */
        printf ("%s\n", "Ошибка в командной строке"); 
        return 1;                                       
    }

    /* проверка типа исх.файла*/
    /* если тип не "pli", то: */
    if(strcmp(&NFIL[strlen(NFIL)-3], "pli"))
    {
        /* выдать диагностику и завершить трансляцию */
        printf("%s\n", "Неверный тип файла с исходным текстом");
        return 1;                                       
    }
    /* если тип файла "pli" */
    else                    
    {
        /* пытаемся открыть файл */
        fp = fopen(NFIL , "rb");
        /* при неудачн.открыт.ф-ла */
        if (NULL == fp)   
        {
            /* сообщение об ошибке и завершение трансляции */
            printf("%s\n", "Не найден файл с исходным текстом");
            return 1;                                     
        }
        /* иначе: */
        else                      
        {
            /* пишем файл в массив */
            /* ISXTXT */
            for (NISXTXT = 0; NISXTXT <= MAXNISXTXT; NISXTXT++)
            {
                if (!fread(ISXTXT[NISXTXT], 80, 1, fp))
                {
                    /* в конце файла идем на метку main1 */
                    if (feof(fp))
                    {
                        goto main1;
                    }
                }
                /* при сбое чтения выдаем диагностику */
                else
                {
                    /* и завершаем трансляцию */
                    printf("%s\n", "Ошибка при чтении фыйла с исх.текстом");
                    return 1;
                }
            }
        }

        /* при переполнении массива */
        /* ISXTXT выдать диагностику и завершить трансляцию */
        printf ( "%s\n", "Переполнение буфера чтения исх.текста" ); 
        return 1;                                     
    }

    /* по завершении чтения исх.файла формируем префикс имени выходного
     * Ассемблеровского файла */
    main1:

    fclose(fp);
    NFIL[strlen(NFIL)-3] = '\x0';

    /* чистка буфера строки выходного ассемблеровского файла */
    memset(ASS_CARD.BUFCARD, ' ', 80);
                          
    /* лексический анализ исходного текста */
    compress_ISXTXT();
    /* построение матрицы преемников */
    build_TPR();

    /* синтаксический анализ исходного текста */
    if (sint_ANAL())
    {
        /* если найдены ошибки синтаксиса, то выдаем диагностику и завершаем трансляцию */
        STROKA[I4 + 20] = '\x0';
        printf("%s%s%s%s\n", "ошибка синтаксиса в исх.тексте -> ", "\"...", &STROKA[I4], "...\"");
        printf("%s\n", "трансляция прервана");
        return 1;                                      
    }
    /* иначе делаем семантическое вычислен. */
    else                                            
    {
        switch (gen_COD())                         
        {
            case 0:                                       
                printf("%s\n", "трансляция завершена успешно");
                return 1;
            case 1:                                    
                printf ( "%s\n", "несовпадение имени процедуры в прологе-эпилоге" );
                break;                                  
            case 2:
                STROKA[DST[I2].DST2 + 20] = '\x0';
                printf("%s%s\n%s%s%s\n", 
                       "недопустимый тип идентификатора: ",
                       FORMT[1],
                       " в исх.тексте -> \"...",
                       &STROKA[DST[I2].DST2],
                       "...\"");
                break;
            case 3:
                STROKA[DST[I2].DST2 + 20] = '\x0';
                printf("%s%s\n%s%s%s\n",
                       "недопустимый тип идентификатора: ",
                       FORMT[IFORMT - 1],
                       " в исх.тексте -> \"...",
                       &STROKA[DST[I2].DST2],
                       "...\"");
                break;
            case 4:
                STROKA[DST[I2].DST2 + 20] = '\x0';
                printf("%s%s\n%s%s%s\n",
                       "неопределенный идентификатор: ",
                       FORMT[IFORMT - 1],
                       " в исх.тексте -> \"...",
                       &STROKA[DST[I2].DST2],
                       "...\"");
                break;
            case 5:
                STROKA[DST[I2].DST2 + 20] = '\x0';
                printf("%s%c\n%s%s%s\n",
                       "недопустимая операция: ",
                       STROKA[DST[I2].DST4 - strlen(FORMT[IFORMT - 1])],
                       " в исх.тексте -> \"...",
                       &STROKA[DST[I2].DST2],
                       "...\"");
                break;
            case 6:
                STROKA[DST[I2].DST2 + 20] = '\x0';
                printf("%s%s\n%s%s%s\n",
                       "повторное объявление идентификатора: ",
                       FORMT[1],
                       " в исх.тексте -> \"...",
                       &STROKA[DST[I2].DST2],
                       "...\"");
                break;
            default:
                break;
        }

    }

    /* обобщающая диагностика */
    printf ("%s\n", "трансляция прервана");       
    
    return 0;
}