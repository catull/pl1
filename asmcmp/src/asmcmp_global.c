#include "asmcmp_global.h"

/*
***** СЧЕТЧИК относительного адреса (смещешия относительно базы )
*/

int CHADR;
int ITSYM = -1;                                  /*инд.своб.стр. табл.симв.*/
symbols_table_t T_SYM[NSYM];                        /*определение табл.симв.  */
assembler_card_t TEK_ISX_KARTA;

/* Indicator of detection of label */
char PRNMET = 'N';
int I3;

/* Table of base registers */
base_registers_table_t T_BASR[15] = {
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'},
    {0x00,'N'}
};

/*
***** Б Л О К   об'явления массива с об'ектным текстом
*/

char OBJTEXT[DL_OBJTEXT][80];
int ITCARD = 0;
oper_rr_t RR;
oper_rx_t RX;

esd_card_buffer_t ESD;
txt_card_buffer_t TXT;
end_card_buffer_t END;