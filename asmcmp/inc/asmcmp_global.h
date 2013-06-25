/* encoding: UTF-8 */

#ifndef ASMCMP_GLOBAL_H
#define ASMCMP_GLOBAL_H

#include <stddef.h>

#include "asmcmp_common.h"


#define BASE_REGISTERS_COUNT 15

/* This struct is type of assembler card
 * It is template to read the input file written by IBM 370 assembler */
typedef struct assembler_card_s {
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
    char IMSYM[8];          /* symbol's name */
    int sym_value;          /* symbol's value */
    int DLSYM;              /* length in bytes of symbol's value */
} symbols_table_t;

typedef struct base_registers_table_s {
    int offset;
    char PRDOST;
} base_registers_table_t;

/* Structure describes content of the ESD-card */
typedef struct esd_card_buffer_s {
    unsigned char POLE1;
    unsigned char POLE2[3];
    unsigned char POLE3[6];
    unsigned char POLE31[2];
    unsigned char POLE32[2];
    unsigned char POLE4[2];
    unsigned char IMPR[8];
    unsigned char POLE6;
    unsigned char ADPRG[3];
    unsigned char POLE8;
    unsigned char DLPRG[3];
    unsigned char POLE10[40];
    unsigned char POLE11[8];
} esd_card_buffer_t;

/* Structure describes content of the TXT-card */
typedef struct txt_card_buffer_s {
    unsigned char POLE1;
    unsigned char POLE2[3];
    unsigned char POLE3;
    unsigned char ADOP[3];
    unsigned char POLE5[2];
    unsigned char DLNOP[2];
    unsigned char POLE7[2];
    unsigned char POLE71[2];
    unsigned char OPER[56];
    unsigned char POLE9[8];
} txt_card_buffer_t;

/* Structure describes content of the END-card */
typedef struct end_card_buffer_s {
    unsigned char POLE1;
    unsigned char POLE2[3];
    unsigned char POLE3[68];
    unsigned char POLE9[8];
} end_card_buffer_t;

extern size_t CHADR;
extern int ITSYM;
extern symbols_table_t T_SYM[NSYM];
extern assembler_card_t TEK_ISX_KARTA;
extern int I3;
extern char PRNMET;
extern base_registers_table_t T_BASR[BASE_REGISTERS_COUNT];

extern char OBJTEXT[DL_OBJTEXT][80];
extern int ITCARD;

extern esd_card_buffer_t ESD;
extern txt_card_buffer_t TXT;
extern end_card_buffer_t END;

#endif /* ASMCMP_GLOBAL_H */
 

