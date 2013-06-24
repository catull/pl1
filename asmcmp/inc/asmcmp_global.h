/* encoding: UTF-8 */

#ifndef ASMCMP_GLOBAL_H
#define ASMCMP_GLOBAL_H

#include <ctype.h>

#include "asmcmp_common.h"

/*
******* НАЛОЖЕНИЕ структуры карты исх. текста на входной буфер
*/

typedef struct assembler_card_un {
    char METKA[8];
    char PROBEL1[1];
    char OPERAC[5];
    char PROBEL2[1];
    char OPERAND[12];
    char PROBEL3[1];
    char COMM[52];
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

typedef union esd_card_buffer_s {
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
} esd_card_buffer_t;

typedef union txt_card_buffer_s {
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
} txt_card_buffer_t;

typedef union end_card_buffer_s {
    unsigned char POLE1;                    /*место для кода 0x02     */
    unsigned char POLE2[3];                    /*поле типа об'ектн.карты */
    unsigned char POLE3[68];                    /*пробелы                 */
    unsigned char POLE9[8];                    /*идентификационное поле  */
} end_card_buffer_t;

extern size_t CHADR;
extern int ITSYM;
extern symbols_table_t T_SYM[NSYM];
extern assembler_card_t TEK_ISX_KARTA;
extern int I3;
extern char PRNMET;
extern base_registers_table_t T_BASR[15];

extern char OBJTEXT[DL_OBJTEXT][80];
extern int ITCARD;

extern esd_card_buffer_t ESD;
extern txt_card_buffer_t TXT;
extern end_card_buffer_t END;

#endif
 

