#include <string.h>                               /*вкл.строковые подпрогр. */
#include <stdlib.h>                               /*вкл.подпрогр.преобр.данн*/
#include <stdio.h>                                /*вкл.подпр.станд.вв/выв  */
#include <ctype.h>                                /*вкл.подпр.классиф.симв. */

#include "asmcmp_common.h"
#include "asmcmp_main.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"





/*
***** Б Л О К  об'явлений прототипов обращений к подпрограммам 2-го просмотра
*/

                          /*п р о т о т и п  обращ.к*/
static int SDC(void);                                        /*подпр.обр.пс.опер.DC    */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SDS(void);                                        /*подпр.обр.пс.опер.DS    */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SEND(void);                                       /*подпр.обр.пс.опер.END   */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SEQU(void);                                       /*подпр.обр.пс.опер.EQU   */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SSTART(void);                                     /*подпр.обр.пс.опер.START */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SUSING(void);                                     /*подпр.обр.пс.опер.USING */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SRR(void);                                        /*подпр.обр.опер.RR-форм. */
/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
static int SRX(void);                                        /*подпр.обр.опер.RX-форм. */
/*..........................................................................*/


void STXT( int ARG )                              /*подпр.формир.TXT-карты  */
 {
  char *PTR;                                      /*рабоч.переменная-указат.*/

  PTR = (char *)&CHADR;                           /*формирование поля ADOP  */
  TXT.STR_TXT.ADOP[2]  = *PTR;                    /*TXT-карты в формате     */
  TXT.STR_TXT.ADOP[1]  = *(PTR+1);                /*двоичного целого        */
  TXT.STR_TXT.ADOP[0]  = '\x00';                  /*в соглашениях ЕС ЭВМ    */

  if ( ARG == 2 )                                 /*формирование поля OPER  */
   {
    memset ( TXT.STR_TXT.OPER , 64 , 4 );
    memcpy ( TXT.STR_TXT.OPER,RR.BUF_OP_RR , 2 ); /* для RR-формата         */
    TXT.STR_TXT.DLNOP [1] = 2;
   }
  else
   {
    memcpy ( TXT.STR_TXT.OPER , RX.BUF_OP_RX , 4);/* для RX-формата         */
    TXT.STR_TXT.DLNOP [1] = 4;
   }
  memcpy (TXT.STR_TXT.POLE9,ESD.STR_ESD.POLE11,8);/*формиров.идентифик.поля */

  memcpy ( OBJTEXT[ITCARD] , TXT.BUF_TXT , 80 );  /*запись об'ектной карты  */
  ITCARD += 1;                                    /*коррекц.инд-са своб.к-ты*/
  CHADR = CHADR + ARG;                            /*коррекц.счетчика адреса */
  return;
 }

static int SDC(void)                                         /*подпр.обр.пс.опер.DC    */
 {
  char *RAB;                                      /*рабочая переменная      */

  RX.OP_RX.OP   = 0;                              /*занулим два старших     */
  RX.OP_RX.R1X2 = 0;                              /*байта RX.OP_RX          */
  if
    (                                             /* если операнд начинается*/
     !memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND,/* с комбинации           */
          "F'", 2)                            /* F',                    */
    )                                             /* то                     */
   {
    RAB=strtok                                    /*в перем. c указат.RAB   */
     (                                        /*выбираем первую лексему */
    (char*)TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND+2,/*операнда текущей карты  */
      "'"                                     /*исх.текста АССЕМБЛЕРА   */
     );

    RX.OP_RX.B2D2 = atoi ( RAB );                 /*перевод ASCII-> int     */
    RAB = (char *) &RX.OP_RX.B2D2;                /*приведение к соглашениям*/
    swab ( RAB , RAB , 2 );                       /* ЕС ЭВМ                 */
   }
  else                                            /*иначе                   */
   return (1);                                    /*сообщение об ошибке     */

  STXT (4);                                       /*формирование TXT-карты  */


  return (0);                                     /*успешн.завершение подпр.*/
 }
/*..........................................................................*/
static int SDS(void)                                         /*подпр.обр.пс.опер.DS    */
 {

  RX.OP_RX.OP   = 0;                              /*занулим два старших     */
  RX.OP_RX.R1X2 = 0;                              /*байта RX.OP_RX          */
  if
    (                                             /* если операнд начинается*/
     TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND[0]=='F' /* с комбинации F'        */
    )                                             /* то:                    */
   RX.OP_RX.B2D2 = 0;                             /*занулим RX.OP_RX.B2D2   */
  else                                            /*иначе                   */
   return (1);                                    /*сообщение об ошибке     */

  STXT (4);                                       /*формирование TXT-карты  */

  return (0);                                     /*успешно завершить подпр.*/
 }
/*..........................................................................*/
int SEND()                                        /*подпр.обр.пс.опер.END   */
 {
                          /*формирование            */
  memcpy (                                        /*идентификационнго поля  */
      END.STR_END.POLE9 ,                     /* END - карты            */
      ESD.STR_ESD.POLE11,                     /*                        */
      8                                       /*                        */
     );                                       /*                        */
  memcpy (                                        /*запись об'ектной карты  */
      OBJTEXT[ITCARD],                        /* в                      */
      END.BUF_END,                            /* массив                 */
      80                                      /* об'ектных              */
     );                                       /* карт                   */
  ITCARD += 1;                                    /*коррекц.инд-са своб.к-ты*/
  return (100);                                   /*выход с призн.конца 2-го*/
                          /*просмотра               */
 }
/*..........................................................................*/
static int SEQU(void)                                        /*подпр.обр.пс.опер.EQU   */
 {
  return (0);                                     /*успешное заверш.подпр.  */
 }
/*..........................................................................*/
static int SSTART(void)                                      /*подпр.обр.пс.опер.START */
 {
  char *PTR;                                      /*набор                   */
  char *METKA;                                    /*рабочих                 */
  char *METKA1;                                   /*переменных              */
  int J;                                          /*подпрограммы            */
  int RAB;                                        /*                        */

  METKA1 = strtok                                 /*в перем. c указат.METKA1*/
       (                                      /*выбираем первую лексему */
    (char*) TEK_ISX_KARTA.STRUCT_BUFCARD.METKA,   /*операнда текущей карты  */
        " "                                   /*исх.текста АССЕМБЛЕРА   */
       );
  for ( J=0; J<=ITSYM; J++ )                      /* все метки исх.текста в */
   {                                              /* табл. T_SYM сравниваем */
                          /* со знач.перем. *METKA1 */
    METKA = strtok (
            (char*) T_SYM[J].IMSYM , " "
           );
    if( !strcmp ( METKA , METKA1 ) )              /* и при совпадении:      */
     {                                            /*  берем разность сч.адр.*/
      RAB = CHADR - T_SYM[J].ZNSYM;               /*  знач.этой метки, обра-*/
      PTR = (char *)&RAB;                         /*  зуя длину программы в */
      swab ( PTR , PTR , 2 );                     /*  соглашениях ЕС ЭБМ, и */
      ESD.STR_ESD.DLPRG [0] = 0;                  /*  записыв.ее в ESD-карту*/
      ESD.STR_ESD.DLPRG [1] = *PTR;               /*  побайтно              */
      ESD.STR_ESD.DLPRG [2] = *(PTR + 1);         /*                        */
      CHADR = T_SYM[J].ZNSYM;                     /*устанавл.CHADR, равным  */
                          /*операнду операт.START   */
                          /*исходного текста        */
      PTR = (char *)&CHADR;                       /*формирование поля ADPRG */
      ESD.STR_ESD.ADPRG[2]  = *PTR;               /*ESD-карты в формате     */
      ESD.STR_ESD.ADPRG[1]  = *(PTR+1);           /*двоичного целого        */
      ESD.STR_ESD.ADPRG[0]  = '\x00';             /*в соглашениях ЕС ЭВМ    */
      memcpy (                                    /*формирование            */
           ESD.STR_ESD.IMPR ,                 /* имени программы        */
           METKA ,                            /*  и                     */
           strlen ( METKA )                   /*                        */
         );                                   /*                        */
      memcpy (                                    /*идентификационнго поля  */
           ESD.STR_ESD.POLE11 ,               /* ESD - карты            */
           METKA ,                            /*                        */
           strlen ( METKA )                   /*                        */
         );                                   /*                        */
      memcpy (                                    /*запись об'ектной карты  */
           OBJTEXT[ITCARD] ,                  /* в                      */
           ESD.BUF_ESD ,                      /* массив                 */
           80                                 /* об'ектных              */
         );                                   /* карт                   */
      ITCARD += 1;                                /*коррекц.инд-са своб.к-ты*/
      return (0);                                 /*успешное заверш.подпрогр*/
     }
   }
  return (2);                                     /*ошибочное заверш.прогр. */
 }
/*..........................................................................*/
static int SUSING(void)                                      /*подпр.обр.пс.опер.USING */
 {
                          /*набор                   */
  char *METKA;                                    /*рабочих                 */
  char *METKA1;                                   /*переменных              */
  char *METKA2;                                   /*                        */
  int J;                                          /*                        */
  int NBASRG;                                     /*                        */
  METKA1 = strtok                                 /*в перем. c указат.METKA1*/
       (                                      /*выбираем первую лексему */
    (char*) TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, /*операнда текущей карты  */
        ","                                   /*исх.текста АССЕМБЛЕРА   */
       );
  METKA2 = strtok                                 /*в перем. c указат.METKA2*/
       (                                      /*выбираем вторую лексему */
        NULL,                                 /*операнда текущей карты  */
        " "                                   /*исх.текста АССЕМБЛЕРА   */
       );
  if ( isalpha ( (int) *METKA2 ) )                /*если лексема начинается */
   {                                              /*с буквы, то:            */

    for ( J=0; J<=ITSYM; J++ )                    /* все метки исх.текста в */
     {                                            /* табл. T_SYM сравниваем */
                          /* со знач.перем. *METKA2 */
      METKA = strtok (
              (char*) T_SYM[J].IMSYM , " "
             );
      if( !strcmp ( METKA , METKA2 ) )            /* и при совпадении:      */
       {                                          /*  запоминаем значение   */
     if ( (NBASRG = T_SYM[J].ZNSYM) <= 0x0f ) /*  метки в NBASRG и в сл.*/
      goto SUSING1;                           /*  NBASRG <= 0x0f идем на*/
                          /*  устан.регистра базы   */
     else                                     /* иначе:                 */
      return (6);                             /*  сообщение об ошибке   */

       }
     }
    return (2);                                   /*заверш.подпр.по ошибке  */
   }
  else                                            /*иначе, если второй опер.*/
   {                                              /*начинается с цифры, то: */
    NBASRG = atoi ( METKA2 );                     /* запомним его в NBASRG  */
    if ( (NBASRG = T_SYM[J].ZNSYM) <= 0x0f )      /* и,если он <= 0x0f,то:  */
     goto SUSING1;                                /* идем на устан.рег.базы */
    else                                          /*иначе:                  */
     return (6);                                  /* сообщение об ошибке    */
   }

 SUSING1:                                         /*установить базовый рег. */

  T_BASR [NBASRG - 1].PRDOST = 'Y';               /* взвести призн.активн.  */
  if ( *METKA1 == '*' )                           /* если перв.оп-нд == '*',*/
   {
    T_BASR[NBASRG-1].SMESH = CHADR;               /* выбир.знач.базы ==CHADR*/
   }
  else                                            /*иначе:                  */
   {
    for ( J=0; J<=ITSYM; J++ )                    /* все метки исх.текста в */
     {                                            /* табл. T_SYM сравниваем */
                          /* со знач.перем. *METKA1 */
      METKA = strtok (
              (char*) T_SYM[J].IMSYM , " "
             );
      if( !strcmp ( METKA , METKA1 ) )            /* и при совпадении:      */
       {                                          /*  берем значение этой   */
    T_BASR[NBASRG-1].SMESH = T_SYM[J].ZNSYM;  /*  этой метки как базу   */
       }
     }
    return (2);                                   /*завершение прогр.по ошиб*/
   }
  return (0);                                     /*успешное заверш.подпрогр*/
 }
/*..........................................................................*/
static int SRR(void)                                         /*подпр.обр.опер.RR-форм. */
 {
  char *METKA;                                    /*набор                   */
  char *METKA1;                                   /*рабочих                 */
  char *METKA2;                                   /*переменных              */
  unsigned char R1R2;                             /*                        */
  int J;                                          /*                        */
  RR.OP_RR.OP = T_MOP[I3].CODOP;                  /*формирование кода операц*/

  METKA1 = strtok                                 /*в перем. c указат.METKA1*/
       (                                      /*выбираем первую лексему */
    (char*) TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, /*операнда текущей карты  */
        ","                                   /*исх.текста АССЕМБЛЕРА   */
       );

  METKA2 = strtok                                 /*в перем. c указат.METKA2*/
       (                                      /*выбираем вторую лексему */
        NULL,                                 /*операнда текущей карты  */
        " "                                   /*исх.текста АССЕМБЛЕРА   */
       );

  if ( isalpha ( (int) *METKA1 ) )                /*если лексема начинается */
   {                                              /*с буквы, то:            */
    for ( J=0; J<=ITSYM; J++ )                    /* все метки исх.текста в */
     {                                            /* табл. T_SYM сравниваем */
                          /* со знач.перем. *METKA1 */
      METKA = strtok (
              (char*) T_SYM[J].IMSYM , " "
             );
      if( !strcmp ( METKA , METKA1 ) )            /* и при совпадении:      */
       {                                          /*  берем значение этой   */
     R1R2 = T_SYM[J].ZNSYM << 4;              /*  метки в качестве перв.*/
     goto SRR1;
       }                                          /*  опреранда машинной ком*/
     }
    return(2);                                    /*сообщ."необ'явл.идентиф"*/
   }
  else                                            /*иначе, берем в качестве */
   {                                              /*перв.операнда машинн.ком*/
     R1R2 = atoi ( METKA1 ) << 4;                 /*значен.выбр.   лексемы  */
   }


 SRR1:


  if ( isalpha ( (int) *METKA2 ) )                /*если лексема начинается */
   {                                              /*с буквы, то:            */
    for ( J=0; J<=ITSYM; J++ )                    /* все метки исх.текста в */
     {                                            /* табл. T_SYM сравниваем */
                          /* со знач.перем. *МЕТКА2 */
      METKA = strtok (
              (char*) T_SYM[J].IMSYM , " "
             );
      if( !strcmp ( METKA , METKA2 ) )            /* и при совпадении:      */
       {                                          /*  берем значение этой   */
     R1R2 = R1R2 + T_SYM[J].ZNSYM;            /*  метки в качестве втор.*/
     goto SRR2;                               /*                        */
       }                                          /*  опреранда машинной ком*/
     }                                            /*                        */
    return(2);                                    /*сообщ."необ'явл.идентиф"*/
   }
  else                                            /*иначе, берем в качестве */
   {                                              /*втор.операнда машинн.ком*/
     R1R2 = R1R2 + atoi ( METKA2 );               /*значен.выбр.   лексемы  */
   }

 SRR2:

  RR.OP_RR.R1R2 = R1R2;                           /*формируем опер-ды маш-ой*/
                          /*команды                 */

  STXT(2);
  return(0);                                      /*выйти из подпрограммы   */
 }

/*..........................................................................*/
static int SRX(void)                                         /*подпр.обр.опер.RX-форм. */
 {
  char *METKA;                                    /*набор                   */
  char *METKA1;                                   /*рабочих                 */
  char *METKA2;                                   /*переменных              */
  char *PTR;                                      /*                        */
  int  DELTA;                                     /*                        */
  int  ZNSYM;                                     /*                        */
  int  NBASRG;                                    /*                        */
  int J;                                          /*                        */
  int I;                                          /*                        */
  unsigned char R1X2;                             /*                        */
  int B2D2;                                       /*                        */
  RX.OP_RX.OP = T_MOP[I3].CODOP;                  /*формирование кода операц*/
  METKA1 = strtok                                 /*в перем. c указат.METKA1*/
       (                                      /*выбираем первую лексему */
    (char*) TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, /*операнда текущей карты  */
        ","                                   /*исх.текста АССЕМБЛЕРА   */
       );

  METKA2 = strtok                                 /*в перем. c указат.METKA2*/
       (                                      /*выбираем вторую лексему */
        NULL,                                 /*операнда текущей карты  */
        " "                                   /*исх.текста АССЕМБЛЕРА   */
       );

  if ( isalpha ( (int) *METKA1 ) )                /*если лексема начинается */
   {                                              /*с буквы, то:            */
    for ( J=0; J<=ITSYM; J++ )                    /* все метки исх.текста в */
     {                                            /* табл. T_SYM сравниваем */
                          /* со знач.перем. *METKA  */
      METKA = strtok (
              (char*) T_SYM[J].IMSYM , " "
             );
      if( !strcmp ( METKA , METKA1 ) )            /* и при совпадении:      */

       {                                          /*  берем значение этой   */
     R1X2 = T_SYM[J].ZNSYM << 4;              /*  метки в качестве перв.*/
     goto SRX1;
       }                                          /*  опреранда машинной ком*/
     }
    return(2);                                    /*сообщ."необ'явл.идентиф"*/
   }
  else                                            /*иначе, берем в качестве */
   {                                              /*перв.операнда машинн.ком*/
     R1X2 = atoi ( METKA1 ) << 4;                 /*значен.выбр.   лексемы  */
   }


 SRX1:


  if ( isalpha ( (int) *METKA2 ) )                /*если лексема начинается */
   {                                              /*с буквы, то:            */
    for ( J=0; J<=ITSYM; J++ )                    /* все метки исх.текста в */
     {                                            /* табл. T_SYM сравниваем */
                          /* со знач.перем. *МЕТКА  */
      METKA = strtok (
              (char*) T_SYM[J].IMSYM , " "
             );
      if( !strcmp ( METKA , METKA2 ) )            /* и при совпадении:      */
       {                                          /*  установить нач.знач.: */
    NBASRG = 0;                               /*   номера базов.регистра*/
    DELTA  = 0xfff - 1;                       /*   и его значен.,а также*/
    ZNSYM  = T_SYM[J].ZNSYM;                  /*   смещен.втор.операнда */
    for ( I=0; I<15; I++ )                    /*далее в цикле из всех   */
     {                                        /*рег-ров выберем базовым */
      if (                                    /*тот, который имеет:     */
           T_BASR[I].PRDOST == 'Y'            /* призн.активности,      */
          &&                                  /*  и                     */
           ZNSYM - T_BASR[I].SMESH >= 0       /* значенение, меньшее по */
          &&                                  /* величине,но наиболее   */
           ZNSYM - T_BASR[I].SMESH < DELTA    /* близкое к смещению вто-*/
         )                                    /* рого операнда          */
       {
        NBASRG = I + 1;
        DELTA  = ZNSYM - T_BASR[I].SMESH;
       }
     }
    if ( NBASRG == 0 || DELTA > 0xfff )       /*если баз.рег.не выбр.,то*/
     return(5);                               /* заверш.подпр.по ошибке */
    else                                      /*иначе                   */
     {                                        /* сформировыать машинное */
      B2D2 = NBASRG << 12;                    /* представление второго  */
      B2D2 = B2D2 + DELTA;                    /* операнда в виде B2D2   */
      PTR = (char *)&B2D2;                    /* и в соглашениях ЕС ЭВМ */
      swab ( PTR , PTR , 2 );                 /* с записью в тело ком-ды*/
      RX.OP_RX.B2D2 = B2D2;
     }
    goto SRX2;                                /*перех.на форм.первого   */
       }                                          /*  опреранда машинной ком*/
     }
    return(2);                                    /*сообщ."необ'явл.идентиф"*/
   }
  else                                            /*иначе, берем в качестве */
   {                                              /*втор.операнда машинн.ком*/
    return(4);                                    /*значен.выбр.   лексемы  */
   }

 SRX2:

  RX.OP_RX.R1X2 = R1X2;                           /*дозапись перв.операнда  */

  STXT(4);                                        /*формирование TXT-карты  */
  return(0);                                      /*выйти из подпрограммы   */
 }


static int SOBJFILE(char const *p_tex_fp_name)
{
    FILE *p_text_fp;
    int RAB2;

    p_text_fp = fopen(p_tex_fp_name , "wb"); 
    if (NULL == p_text_fp)
    {
        return -7; 
    }
    else
    {
        RAB2 = fwrite(OBJTEXT, 80, ITCARD, p_text_fp);
        fclose(p_text_fp);
    }

    return RAB2;
}


static void INITUNION(void)
{
    ESD.STR_ESD.POLE1 = 0x02;
    memcpy(ESD.STR_ESD.POLE2, "ESD", 3);
    memset(ESD.STR_ESD.POLE3, 0x40, 6);
    ESD.STR_ESD.POLE31[0] = 0x00;
    ESD.STR_ESD.POLE31[1] = 0x10;
    memset(ESD.STR_ESD.POLE32, 0x40, 2);
    ESD.STR_ESD.POLE4[0] = 0x00;
    ESD.STR_ESD.POLE4[1] = 0x01;
    memset(ESD.STR_ESD.IMPR, 0x40, 8);
    ESD.STR_ESD.POLE6 = 0x00;
    memset(ESD.STR_ESD.ADPRG, 0x00, 3);
    ESD.STR_ESD.POLE8 = 0x40;
    memset(ESD.STR_ESD.DLPRG, 0x00, 3);
    memset(ESD.STR_ESD.POLE10, 0x40, 40);
    memset(ESD.STR_ESD.POLE11, 0x40, 8);

    TXT.STR_TXT.POLE1 = 0x02;
    memcpy(TXT.STR_TXT.POLE2, "TXT", 3);
    TXT.STR_TXT.POLE3 = 0x40;
    memset(TXT.STR_TXT.ADOP, 0x00, 3);
    memset(TXT.STR_TXT.POLE5, 0x40, 2);
    memset(TXT.STR_TXT.DLNOP, 0X00, 2);
    memset(TXT.STR_TXT.POLE7, 0x40, 2);
    TXT.STR_TXT.POLE71[0] = 0x00;
    TXT.STR_TXT.POLE71[1] = 0x01;
    memset(TXT.STR_TXT.OPER, 0x40, 56);
    memset(TXT.STR_TXT.POLE9, 0x40, 8);

    END.STR_END.POLE1 = 0x02;
    memcpy(END.STR_END.POLE2, "END", 3);
    memset(END.STR_END.POLE3, 0x40, 68);
    memset(END.STR_END.POLE9, 0x40, 8);
}

static char* asmcmp_main_errmsg_by_errdata(asmcmp_main_error_data_t err_data, char *errmsg)
{
    switch (err_data.main_err_code)
    {
        case ASMCMP_MAIN_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case ASMCMP_MAIN_WRONG_NUM_CLI_PAR:
            strcpy(errmsg, "Wrong number of command line parameters");
            break;
        case ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH:
            strcpy(errmsg, "Wrong path to ASM-file with the source text");
            break;
        case ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION:
            strcpy(errmsg, "Wrong input file extension with the source text");
            break;
        case ASMCMP_MAIN_NOT_FOUND_INPUT_ASM_FILE:
            strcpy(errmsg, "Couldn't find file with the source text");
            break;
        case ASMCMP_MAIN_ERROR_READING_ASM_FILE:
            strcpy(errmsg, "Error occured while reading file with the source text");
            break;
        case ASMCMP_MAIN_PROGRAM_BUFFER_OVERFLOW:
            strcpy(errmsg, "Overflow of the program buffer while reading file with the source text");
            break;
        case ASMCMP_MAIN_WRONG_DATA_FORMAT_ERROR:
            strcpy(errmsg, "Wrong data format");
            break;
        case ASMCMP_MAIN_NOT_DECLARED_IDENT_ERROR:
            strcpy(errmsg, "Not declared identifier");
            break;
        case ASMCMP_MAIN_OPERATION_CODE_ERROR:
            strcpy(errmsg, "Error of the operation code");
            break;
        case ASMCMP_MAIN_SECOND_OPERAND_ERROR:
            strcpy(errmsg, "Error of the second operand");
            break;
        case ASMCMP_MAIN_BASING_ERROR:
            strcpy(errmsg, "Basing error");
            break;
        case ASMCMP_MAIN_ILLEGAL_REGISTER_NUMBER_ERROR:
            strcpy(errmsg, "Illegal register number");
            break;
        case ASMCMP_MAIN_CANT_WRITE_TEX_FILE_ERROR:
            strcpy(errmsg, "Can't write to object file");
            break;
        case ASMCMP_MAIN_WRONG_WRITE_TEX_FILE_ERROR:
            strcpy(errmsg, "Wrong writing to object file");
            break;
        default:
            return "Unknown error code for generating error message";
    }

    strcat(errmsg, ". Error in card ");
    sprintf(&errmsg[strlen(errmsg)], "%d", err_data.card_number);

    return errmsg;
}


/* Function of reading ASM-file of the source text with 'p_asm_fp_name' file path name */
static enum asmcmp_main_error_code_e asmcmp_main_read_asm_file(char const *p_asm_fp_name,
                                                               char asm_src_text[][LINELEN],
                                                               size_t *p_asm_src_text_len)
{
    FILE *p_asm_f;
    asmcmp_main_error_code_t err_code = ASMCMP_MAIN_SUCCESS;

    p_asm_f = fopen(p_asm_fp_name , "rb");
    if (NULL == p_asm_f)
    {
        err_code = ASMCMP_MAIN_NOT_FOUND_INPUT_ASM_FILE;
    }
    else
    {
        size_t asm_src_text_len;
        /* Write opened file to byte-array */
        for (asm_src_text_len = 0; asm_src_text_len < DL_ASSTEXT; asm_src_text_len++)
        {
            if (!fread(asm_src_text[asm_src_text_len], 1, LINELEN, p_asm_f))
            {
                if (feof(p_asm_f))
                {   
                    /* Successful reading */
                    break;
                }
                else
                {
                    err_code = ASMCMP_MAIN_ERROR_READING_ASM_FILE;
                    break;
                }
            }
        }

        if (DL_ASSTEXT == asm_src_text_len)
        {
            /* Buffer is overflowed */
            err_code = ASMCMP_MAIN_PROGRAM_BUFFER_OVERFLOW;
        }

        *p_asm_src_text_len = asm_src_text_len;
        fclose(p_asm_f);
    }

    return err_code;
}

/* Function processes source ASM-text */
static struct asmcmp_main_error_data_s asmcmp_main_process_src_text(char asm_src_text[][LINELEN],
                                                                    size_t asm_src_text_len,
                                                                    char const *p_tex_fp_name)
{
    int i1;
    int RAB;
    asmcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters
     * before syntax analyzer and semantic calculator call */
    memset(&err_data, 0, sizeof(asmcmp_main_error_data_t));
    err_data = (asmcmp_main_error_data_t){
        .main_err_code = ASMCMP_MAIN_SUCCESS
    };

    /* The first phase */
    for (i1 = 0; i1 < asm_src_text_len; i1++)
    {
        int i2;

        memcpy(TEK_ISX_KARTA.BUFCARD, asm_src_text[i1], 80);

        if (' ' != TEK_ISX_KARTA.STRUCT_BUFCARD.METKA[0])
        {
            ITSYM += 1;
            PRNMET = 'Y';
            memcpy(T_SYM[ITSYM].IMSYM, TEK_ISX_KARTA.STRUCT_BUFCARD.METKA, 8);
            T_SYM[ITSYM].ZNSYM = CHADR;
        }

        /* Check if the current assembler command is one of the pseudo commands */
        for (i2 = 0; i2 < NPOP; i2++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_POP[i2].MNCPOP, 5))
            {
                switch (T_POP[i2].BXPROG())
                {
                    case 0:
                        goto CONT2;
                    case 1:
                        err_data.main_err_code = ASMCMP_MAIN_WRONG_DATA_FORMAT_ERROR;
                        goto error;
                    case 100:
                        goto CONT3;
                }
            }
        }

        for (I3 = 0; I3 < NOP; I3++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_MOP[I3].MNCOP, 5))
            {
                T_MOP[I3].BXPROG();
                PRNMET = 'N';
                goto CONT2;
            }
        }

        err_data.main_err_code = ASMCMP_MAIN_OPERATION_CODE_ERROR;
        goto error;

        CONT2:
        
        continue;
    }

    CONT3:

    T_MOP[0].BXPROG = SRR;
    T_MOP[1].BXPROG = SRR;
    T_MOP[2].BXPROG = SRX;
    T_MOP[3].BXPROG = SRX;
    T_MOP[4].BXPROG = SRX;
    T_MOP[5].BXPROG = SRX;

    T_POP[0].BXPROG = SDC;
    T_POP[1].BXPROG = SDS;
    T_POP[2].BXPROG = SEND;
    T_POP[3].BXPROG = SEQU;
    T_POP[4].BXPROG = SSTART;
    T_POP[5].BXPROG = SUSING;

    /* The second phase */
    for (i1 = 0; i1 < asm_src_text_len; i1++)
    {
        int i2;
        memcpy(TEK_ISX_KARTA.BUFCARD, asm_src_text[i1], 80);

        for (i2 = 0; i2 < NPOP; i2++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_POP[i2].MNCPOP, 5))
            {
                switch (T_POP[i2].BXPROG())
                {
                    case 0:
                        goto CONT4;
                    case 100:
                        goto CONT5;
                }
            }
        }

        for (I3 = 0; I3 < NOP; I3++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_MOP[I3].MNCOP , 5))
            {
                switch (T_MOP[I3].BXPROG())
                {
                    case 0:
                        goto CONT4;
                    case 2:
                        err_data.main_err_code = ASMCMP_MAIN_NOT_DECLARED_IDENT_ERROR;
                        goto error;
                    case 4:
                        err_data.main_err_code = ASMCMP_MAIN_SECOND_OPERAND_ERROR;
                        goto error;
                    case 5:
                        err_data.main_err_code = ASMCMP_MAIN_BASING_ERROR;
                        goto error;
                    case 6:
                    case 7:
                        err_data.main_err_code = ASMCMP_MAIN_ILLEGAL_REGISTER_NUMBER_ERROR;
                        goto error;
                }
            }
        }

        CONT4:

        continue;
    }

    CONT5:

    RAB = SOBJFILE(p_tex_fp_name);
    if (ITCARD != RAB)
    {
        err_data.main_err_code = (-7 == RAB) ? ASMCMP_MAIN_CANT_WRITE_TEX_FILE_ERROR : ASMCMP_MAIN_WRONG_WRITE_TEX_FILE_ERROR;
    }

    error:

    if (ASMCMP_MAIN_SUCCESSFUL_TRANSLATION != err_data.main_err_code)
    {
        err_data.card_number = i1 + 1;
    }

    return err_data;
}

int main(int const argc, char const *argv[])
{
    char asm_src_text[DL_ASSTEXT][LINELEN];                  /* Content of the array of the source ASM-text */
    size_t asm_src_text_len = 0;                             /* Length of the array of the source ASM-text */
    char *p_asm_fp_name = NULL, *p_tex_fp_name = NULL;
    size_t asm_fp_len;
    asmcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters */
    memset(&err_data, 0, sizeof(asmcmp_main_error_data_t));
    err_data = (asmcmp_main_error_data_t){ 
        .main_err_code = ASMCMP_MAIN_SUCCESS
    };

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_data.main_err_code = ASMCMP_MAIN_WRONG_NUM_CLI_PAR;
        goto error;
    }

    /* Copy name of translated program from input argument */
    ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, argv[1]);

    asm_fp_len = strlen(p_asm_fp_name);
    if (asm_fp_len < 4)
    {
        err_data.main_err_code = ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH;
        goto error;
    }

    /* Input file for translation must be with 'ass' extension */
    if (strcmp(&p_asm_fp_name[asm_fp_len - 4], ".ass"))
    {
        err_data.main_err_code = ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION;
        goto error;
    }
    else
    {
        /* Clear array for the source ASM-text before getting text from the ASM-file */
        memset(asm_src_text, '\0', sizeof(char)*DL_ASSTEXT*LINELEN);

        err_data.main_err_code = asmcmp_main_read_asm_file(p_asm_fp_name, asm_src_text, &asm_src_text_len);
        if (ASMCMP_MAIN_SUCCESS == err_data.main_err_code)
        {
            /* After successfully reading file proceed to translation of the source text */
            INITUNION();
            ASMCMP_MAIN_MAKE_TEX_FILE_PATH_BY_ASM_FILE_PATH(p_tex_fp_name, p_asm_fp_name);
            ASMCMP_COMMON_RELEASE_MEM(p_asm_fp_name);
            err_data = asmcmp_main_process_src_text(asm_src_text, asm_src_text_len, p_tex_fp_name);
            ASMCMP_COMMON_RELEASE_MEM(p_tex_fp_name);
        }
        else
        {
            /* Error occured while reading file */
            ASMCMP_COMMON_RELEASE_MEM(p_asm_fp_name);
            goto error;
        }
    }

    if (ASMCMP_MAIN_SUCCESSFUL_TRANSLATION == err_data.main_err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        char errmsg[100];

        error:

        printf("Translation is interrupted\nReason: %s\n", asmcmp_main_errmsg_by_errdata(err_data, errmsg));
    }

    return err_data.main_err_code;
}

