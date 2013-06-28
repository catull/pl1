/* encoding: UTF-8 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "asmcmp_common.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"

extern assembler_card_t g_current_asm_card;

static enum asmcmp_machine_oper_error_code_e FRR(int entry);
static enum asmcmp_machine_oper_error_code_e FRX(int entry);
static enum asmcmp_machine_oper_error_code_e FSS(int entry);

/* Table of the machine operations */
machine_operations_table_t T_MOP[NOP] = {
    { {'B','A','L','R',' '}, 0x05, FRR },
    { {'B','C','R',' ',' '}, 0x07, FRR },
    { {'S','T',' ',' ',' '}, 0x50, FRX },
    { {'L',' ',' ',' ',' '}, 0x58, FRX },
    { {'A',' ',' ',' ',' '}, 0x5A, FRX },
    { {'S',' ',' ',' ',' '}, 0x5B, FRX },
    { {'L','R',' ',' ',' '}, 0x18, FRR },
    { {'L','A',' ',' ',' '}, 0x41, FRX },
    { {'A','R',' ',' ',' '}, 0x1A, FRR },
    { {'M','V','C',' ',' '}, 0xD2, FSS }
};

/* Subroutine records the next TXT-card with 
 * new machine operation to OBJTEXT-array */
static void save_oper_tex_card(oper_t oper)
{
    char *PTR;
    size_t oper_len;

    PTR = (char*)&CHADR;
    TXT.ADOP[2] = *PTR;
    TXT.ADOP[1] = *(PTR + 1);
    TXT.ADOP[0] = '\x00';

    switch (oper.oper_type)
    {
        case OPER_RR:
        {
            ASMCMP_COMMON_ASSERT(OPER_RR_LEN == sizeof(oper_rr_t));

            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.rr, OPER_RR_LEN);
            TXT.DLNOP[1] = OPER_RR_LEN;
            oper_len = OPER_RR_LEN;
            break;
        }
        case OPER_RX:
        {
            ASMCMP_COMMON_ASSERT(OPER_RX_LEN == sizeof(oper_rx_t));

            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.rx, OPER_RX_LEN);
            TXT.DLNOP[1] = OPER_RX_LEN;
            oper_len = OPER_RX_LEN;
            break;
        }
        case OPER_SS:
        {
            ASMCMP_COMMON_ASSERT(OPER_SS_LEN == sizeof(oper_ss_t));

            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.ss, OPER_SS_LEN);
            TXT.DLNOP[1] = OPER_SS_LEN;
            oper_len = OPER_SS_LEN;
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    memcpy(TXT.POLE9, ESD.POLE11, 8);
    memcpy(OBJTEXT[ITCARD], &TXT, 80);

    ++ITCARD;
    CHADR = CHADR + oper_len;
}

/* Subroutine handles machine operation with type 'RR'
 * on the first and the second phases */
static enum asmcmp_machine_oper_error_code_e FRR(int entry)
{
    switch (entry)
    {
        case 1:
        {
            CHADR += OPER_RR_LEN;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = OPER_RR_LEN;
            }
            break;
        }
        case 2:
        {
            ASMCMP_COMMON_ASSERT(OPER_RR_LEN == sizeof(oper_rr_t));

            char *REG_str[2];
            int i;

            uint8_t R1R2;

            oper_t oper_rr;
            memset(&oper_rr, 0, sizeof(oper_t));

            oper_rr.oper_type = OPER_RR;
            oper_rr.oper.rr.opcode = T_MOP[I3].opcode;

            /* Get labels or direct values from the OPERAND-field of assembler code line */
            REG_str[0] = strtok(g_current_asm_card.OPERAND, ",");
            REG_str[1] = strtok(NULL, " ");

            for (i = 0; i < 2; i++)
            {
                /* REG_str[i] is label */
                if (isalpha(REG_str[i][0]))
                {
                    int j;
                    for (j = 0; j <= ITSYM; j++)
                    {
                        char *symbol = strtok(T_SYM[j].IMSYM, " ");
                        if (!strcmp(symbol, REG_str[i]))
                        {
                            R1R2 = (0 == i) ? T_SYM[j].sym_value << 4 : R1R2 + T_SYM[j].sym_value;
                            goto SRR1;
                        }
                    }

                    return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
                }
                /* It is direct value */
                else
                {
                    R1R2 = (0 == i) ? atoi(REG_str[i]) << 4 : R1R2 + atoi(REG_str[i]);
                }

                SRR1:

                continue;
            }

            oper_rr.oper.rr.R1R2 = R1R2;

            #ifdef DEBUG_MODE
            asmcmp_machine_oper_print_oper(oper_rr);
            #endif

            save_oper_tex_card(oper_rr);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return ASMCMP_MACHINE_OPER_SUCCESS;
}

/* Subroutine handles machine operation with type 'RX'
 * on the first and the second phases */
static enum asmcmp_machine_oper_error_code_e FRX(int entry)
{
    switch (entry)
    {
        case 1:
        {
            CHADR += OPER_RX_LEN;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = OPER_RX_LEN;
            }
            break;
        }
        case 2:
        {
            ASMCMP_COMMON_ASSERT(OPER_RX_LEN == sizeof(oper_rx_t));

            char *symbol;
            char *REG_str;
            char *OPER_str;
            int j;

            uint8_t R1X2;
            uint16_t B2D2;

            oper_t oper_rx;
            memset(&oper_rx, 0, sizeof(oper_t));

            oper_rx.oper_type = OPER_RX;
            oper_rx.oper.rx.opcode = T_MOP[I3].opcode;
            
            REG_str = strtok(g_current_asm_card.OPERAND, ",");
            OPER_str = strtok(NULL, " ");

            if (isalpha(REG_str[0]))
            {
                for (j = 0; j <= ITSYM; j++)
                {
                    symbol = strtok(T_SYM[j].IMSYM, " ");
                    if(!strcmp(symbol, REG_str))
                    {
                        R1X2 = T_SYM[j].sym_value << 4;
                        goto SRX1;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                R1X2 = atoi(REG_str) << 4;
            }

            SRX1:

            if (isalpha(OPER_str[0]))
            {
                for (j = 0; j <= ITSYM; j++)
                {
                    symbol = strtok(T_SYM[j].IMSYM, " ");
                    if(!strcmp(symbol, OPER_str))
                    {
                        int NBASRG = 0;
                        int DELTA = 0xfff - 1;
                        int sym_value = T_SYM[j].sym_value;
                        int i;

                        for (i = 0; i < BASE_REGISTERS_COUNT; i++)
                        {
                            if ((T_BASR[i].PRDOST == 'Y') &&
                                (sym_value - T_BASR[i].offset >= 0) &&
                                (sym_value - T_BASR[i].offset < DELTA))
                            {
                                NBASRG = i + 1;
                                DELTA  = sym_value - T_BASR[i].offset;
                            }
                        }

                        if (0 == NBASRG || DELTA > 0xfff )
                        {
                            return ASMCMP_MACHINE_OPER_BASING_ERROR;
                        }
                        else
                        {
                            uint8_t *PTR;

                            B2D2 = NBASRG << 12;
                            B2D2 = B2D2 + DELTA;
                            PTR = (uint8_t*)&B2D2;

                            asmcmp_common_swap_bytes(PTR, PTR, 2);

                            oper_rx.oper.rx.B2D2 = B2D2;
                        }

                        goto SRX2;

                    }

                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                return ASMCMP_MACHINE_OPER_SECOND_OPERAND_ERROR;
            }

            SRX2:

            oper_rx.oper.rx.R1X2 = R1X2;

            #ifdef DEBUG_MODE
            asmcmp_machine_oper_print_oper(oper_rx);
            #endif

            save_oper_tex_card(oper_rx);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return ASMCMP_MACHINE_OPER_SUCCESS;
}

/* Subroutine handles machine operation with type 'SS'
 * on the first and the second phases */
static enum asmcmp_machine_oper_error_code_e FSS(int entry)
{
    switch (entry)
    {
        case 1:
        {
            CHADR += OPER_SS_LEN;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = OPER_SS_LEN;
            }
            break;
        }
        case 2:
        {
            ASMCMP_COMMON_ASSERT(OPER_SS_LEN == sizeof(oper_ss_t));

            uint8_t *PTR;
            int NBASRG;
            int j;

            char *symbol;
            char *D1_str;
            char *L_str;
            char *B1_str;
            char *D2_str;

            uint8_t L;
            uint16_t B2D2;
            uint16_t B1D1;

            oper_t oper_ss;
            memset(&oper_ss, 0, sizeof(oper_t));

            oper_ss.oper_type = OPER_SS;
            oper_ss.oper.ss.opcode = T_MOP[I3].opcode;
            
            /* save D1 */
            D1_str = strtok(g_current_asm_card.OPERAND, "(");
            /* D1 is label */
            if (isalpha(D1_str[0]))
            {
                for (j = 0; j <= ITSYM; j++)
                {
                    symbol = strtok(T_SYM[j].IMSYM, " ");
                    if(!strcmp(symbol, D1_str))
                    {
                        B1D1 = T_SYM[j].sym_value;
                        goto SSS1;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            /* D1 is direct value */
            else
            {
                B1D1 = atoi(D1_str);
            }

            SSS1:

            /* save L */
            L_str = strtok(NULL, ",");
            L = atoi(L_str);

            /* save B1 */
            B1_str = strtok(NULL, ")");
            D2_str = strtok(NULL, ", ");
            if (isalpha(B1_str[0]))
            {
                for (j = 0; j <= ITSYM; j++)
                {
                    symbol = strtok(T_SYM[j].IMSYM, " ");
                    if(!strcmp(symbol, B1_str))
                    {
                        NBASRG = T_SYM[j].sym_value;
                        if (0 == NBASRG)
                        {
                            return ASMCMP_MACHINE_OPER_BASING_ERROR;
                        }
                        else
                        {
                            B1D1 += NBASRG << 12;
                            PTR = (uint8_t*)&B1D1;

                            asmcmp_common_swap_bytes(PTR, PTR, 2);

                            oper_ss.oper.ss.B1D1 = B1D1;
                        }
                        goto SSS2;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                return ASMCMP_MACHINE_OPER_SECOND_OPERAND_ERROR;
            }

            SSS2:

            /* save B2 and D2 */
            if (isalpha(D2_str[0]))
            {
                for (j = 0; j <= ITSYM; j++)
                {
                    symbol = strtok(T_SYM[j].IMSYM, " ");
                    if(!strcmp(symbol, D2_str))
                    {
                        int DELTA = 0xfff - 1;
                        int sym_value = T_SYM[j].sym_value;
                        int i;

                        NBASRG = 0;

                        for (i = 0; i < BASE_REGISTERS_COUNT; i++)
                        {
                            if ((T_BASR[i].PRDOST == 'Y') &&
                                (sym_value - T_BASR[i].offset >= 0) &&
                                (sym_value - T_BASR[i].offset < DELTA))
                            {
                                NBASRG = i + 1;
                                DELTA  = sym_value - T_BASR[i].offset;
                            }
                        }

                        if (0 == NBASRG || DELTA > 0xfff )
                        {
                            return ASMCMP_MACHINE_OPER_BASING_ERROR;
                        }
                        else
                        {
                            B2D2 = NBASRG << 12;
                            B2D2 += DELTA;
                            PTR = (uint8_t*)&B2D2;

                            asmcmp_common_swap_bytes(PTR, PTR, 2);

                            oper_ss.oper.ss.B2D2 = B2D2;
                        }
                        goto SSS3;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                return ASMCMP_MACHINE_OPER_SECOND_OPERAND_ERROR;
            }

            SSS3:
            
            oper_ss.oper.ss.L = L;

            #ifdef DEBUG_MODE
            asmcmp_machine_oper_print_oper(oper_ss);
            #endif

            save_oper_tex_card(oper_ss);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return ASMCMP_MACHINE_OPER_SUCCESS;
}

/* Subroutine constructs error message by error code of 'asmcmp machine operations' module */
char const* asmcmp_machine_oper_errmsg_by_errcode(asmcmp_machine_oper_error_code_t err_code)
{
    switch (err_code)
    {
        case ASMCMP_MACHINE_OPER_SUCCESS:
            return "No error occured";
        case ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR:
            return "Not declared identifier";
        case ASMCMP_MACHINE_OPER_SECOND_OPERAND_ERROR:
            return "Error of the second operand";
        case ASMCMP_MACHINE_OPER_BASING_ERROR:
            return "Basing error";
        default:
            return "Unknown error code for generating error message";
    }
}

/* Subroutine prints transmitted machine operation to STDOUT */
void asmcmp_machine_oper_print_oper(oper_t oper)
{
    switch (oper.oper_type)
    {
        case OPER_RR:
            printf("RR: %#.2x %#.2x\n",
                   oper.oper.rr.opcode,
                   oper.oper.rr.R1R2);
            break;
        case OPER_RX:
            printf("RX: %#.2x %#.2x %#.4x\n",
                   oper.oper.rx.opcode,
                   oper.oper.rx.R1X2,
                   oper.oper.rx.B2D2);
            break;
        case OPER_SS:
            printf("SS: %#.2x %#.2x %#.4x %#.4x\n",
                   oper.oper.ss.opcode,
                   oper.oper.ss.L,
                   oper.oper.ss.B1D1,
                   oper.oper.ss.B2D2);
            break;
    }
}
