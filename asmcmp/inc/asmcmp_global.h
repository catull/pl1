/* encoding: UTF-8 */

#ifndef ASMCMP_GLOBAL_H
#define ASMCMP_GLOBAL_H


#include "asmcmp_common.h"

typedef struct __assembler_card_s
{
    char METKA[8];                  /*поле метки              */
    char PROBEL1[1];                  /*пробел-разделитель      */
    char OPERAC[5];                  /*поле операции           */
    char PROBEL2[1];                  /*пробел-разделитель      */
    char OPERAND[12];                  /*поле операнда           */
    char PROBEL3[1];                  /*пробел разделитель      */
    char COMM[52];                  /*поле комментария        */
} __assembler_card_t;

/*
******* НАЛОЖЕНИЕ структуры карты исх. текста на входной буфер
*/

typedef union assembler_card_un {
    unsigned char BUFCARD[80];                    /*буфер карты.исх.текста  */
    __assembler_card_t STRUCT_BUFCARD;                /*наложение шабл.на буфер */
} assembler_card_t;


/* Structure describes type of symbols table */
typedef struct symbols_table_s {
    /* symbol's name */
    char IMSYM[8];
    /* symbol's value */
    int ZNSYM;
    /* length in bytes of symbol's value */ 
    int DLSYM;
    char PRPER;
} symbols_table_t;

typedef struct base_registers_table_s {
    int SMESH;
    char PRDOST;
} base_registers_table_t;


typedef struct __oper_rr_s {
    unsigned char OP;
    unsigned char R1R2;
} __oper_rr_t;

typedef union oper_rr_un {
    char BUF_OP_RR[2];
    __oper_rr_t OP_RR;
} oper_rr_t;


typedef struct __oper_rx_s {
    unsigned char OP;
    unsigned char R1X2;
    short int B2D2;
} __oper_rx_t;

typedef union oper_rx_un {
    char BUF_OP_RX[4];
    __oper_rx_t OP_RX;
} oper_rx_t;

typedef struct __esd_card_buffer_s {
    unsigned char POLE1;                    /*место для кода 0x02     */
    unsigned char POLE2[3];                    /*поле типа об'ектн.карты */
    unsigned char POLE3[6];                    /*пробелы                 */
    unsigned char POLE31[2];                    /*длина данных на карте   */
    unsigned char POLE32[2];                    /*пробелы                 */
    unsigned char POLE4[2];                    /*внутр.ид-р имени прогр. */
    unsigned char IMPR[8];                    /*имя программы           */
    unsigned char POLE6;                    /*код типа ESD-имени      */
    unsigned char ADPRG[3];                    /*относит.адрес программы */
    unsigned char POLE8;                    /*пробелы                 */
    unsigned char DLPRG[3];                    /*длина программы         */
    unsigned char POLE10[40];                    /*пробелы                 */
    unsigned char POLE11[8];                    /*идентификационное поле  */
} __esd_card_buffer_t;

typedef union esd_card_buffer_un {
    __esd_card_buffer_t STR_ESD;
    unsigned char BUF_ESD[80];                   /*буфер карты ESD         */
} esd_card_buffer_t;

typedef struct __txt_card_buffer_s {
    unsigned char POLE1;                    /*место для кода 0x02     */
    unsigned char POLE2[3];                    /*поле типа об'ектн.карты */
    unsigned char POLE3;                    /*пробел                  */
    unsigned char ADOP[3];                    /*относит.адрес опреации  */
    unsigned char POLE5[2];                    /*пробелы                 */
    unsigned char DLNOP[2];                    /*длина операции          */
    unsigned char POLE7[2];                    /*пробелы                 */
    unsigned char POLE71[2];                    /*внутренний идент.прогр. */
    unsigned char OPER[56];                    /*тело операции           */
    unsigned char POLE9[8];                    /*идентификационное поле  */
} __txt_card_buffer_t;

typedef union txt_card_buffer_un {
    __txt_card_buffer_t STR_TXT;
    unsigned char BUF_TXT[80];                   /*буфер карты TXT         */
} txt_card_buffer_t;

typedef struct __end_card_buffer_s {
    unsigned char POLE1      ;                    /*место для кода 0x02     */
    unsigned char POLE2  [ 3];                    /*поле типа об'ектн.карты */
    unsigned char POLE3  [68];                    /*пробелы                 */
    unsigned char POLE9  [ 8];                    /*идентификационное поле  */
} __end_card_buffer_t;


typedef union end_card_buffer_s {
    __end_card_buffer_t STR_END;
    unsigned char BUF_END [80];                   /*буфер карты ESD         */
} end_card_buffer_t;

extern int CHADR;
extern int ITSYM;
extern symbols_table_t T_SYM[NSYM];
extern assembler_card_t TEK_ISX_KARTA;
extern int I3;
extern char PRNMET;
extern base_registers_table_t T_BASR[15];

extern char OBJTEXT[DL_OBJTEXT][80];
extern int ITCARD;

extern oper_rr_t RR;
extern oper_rx_t RX;

extern esd_card_buffer_t ESD;
extern txt_card_buffer_t TXT;
extern end_card_buffer_t END;

#endif
 

