/* encoding: UTF-8 */

#include <stddef.h>

#include "asmcmp_global.h"

/* Counter of displacement address */
size_t CHADR;
/* Symbols counter */
int ITSYM = -1;
/* Table of symbols */
symbols_table_t T_SYM[NSYM];
/* Template for read line of the assembler code */
assembler_card_t TEK_ISX_KARTA;

/* Indicator of detection of label */
char PRNMET = 'N';
int I3;

/* Table of base registers */
base_registers_table_t T_BASR[BASE_REGISTERS_COUNT] = {
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' },
    { 0x00, 'N' }
};

char OBJTEXT[DL_OBJTEXT][80];
int ITCARD;

esd_card_buffer_t ESD;
txt_card_buffer_t TXT;
end_card_buffer_t END;