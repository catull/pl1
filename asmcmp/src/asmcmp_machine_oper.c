/* encoding: UTF-8 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "asmcmp_common.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"

static enum asmcmp_machine_oper_error_code_e FRR(int entry);
static enum asmcmp_machine_oper_error_code_e FRX(int entry);
static enum asmcmp_machine_oper_error_code_e FSS(int entry);

/* Table of the machine operations */
machine_operations_table_t T_MOP[NOP] = 
{
    { {'B','A','L','R',' '}, '\x05', 2, FRR },
    { {'B','C','R',' ',' '}, '\x07', 2, FRR },
    { {'S','T',' ',' ',' '}, '\x50', 4, FRX },
    { {'L',' ',' ',' ',' '}, '\x58', 4, FRX },
    { {'A',' ',' ',' ',' '}, '\x5A', 4, FRX },
    { {'S',' ',' ',' ',' '}, '\x5B', 4, FRX },
    { {'L','E','R',' ',' '}, '\x38', 2, FRR },
    { {'L','A',' ',' ',' '}, '\x41', 4, FRX },
    { {'A','R',' ',' ',' '}, '\x1A', 2, FRR },
    { {'M','V','C',' ',' '}, '\xD2', 6, FSS }
};

/* Function handles machine operation with type 'RR'
 * on the first and the second phases */
static enum asmcmp_machine_oper_error_code_e FRR(int entry)
{
    switch (entry)
    {
        case 1:
        {
            size_t oper_len = sizeof(oper_rr_t);
            CHADR += oper_len;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = oper_len;
            }
            break;
        }
        case 2:
        {
            char *METKA;
            char *METKA1;
            char *METKA2;
            int J;

            uint8_t R1R2;

            oper_t oper_rr;
            memset(&oper_rr, 0, sizeof(oper_t));

            oper_rr.oper_type = OPER_RR;
            oper_rr.oper.rr.opcode = T_MOP[I3].CODOP;

            METKA1 = strtok(TEK_ISX_KARTA.OPERAND, ",");
            METKA2 = strtok(NULL, " ");

            if (isalpha(*METKA1))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA1))
                    {
                        R1R2 = T_SYM[J].ZNSYM << 4;
                        goto SRR1;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                R1R2 = atoi(METKA1) << 4;
            }

            SRR1:

            if (isalpha((int)*METKA2))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA2))
                    {
                        R1R2 = R1R2 + T_SYM[J].ZNSYM;
                        goto SRR2;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                R1R2 = R1R2 + atoi(METKA2);
            }

            SRR2:

            oper_rr.oper.rr.R1R2 = R1R2;

            asmcmp_common_save_oper_tex_card(oper_rr);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return ASMCMP_MACHINE_OPER_SUCCESS;
}

/* Function handles machine operation with type 'RX'
 * on the first and the second phases */
static enum asmcmp_machine_oper_error_code_e FRX(int entry)
{
    switch (entry)
    {
        case 1:
        {
            size_t oper_len = sizeof(oper_rx_t);
            CHADR += oper_len;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = oper_len;
            }
            break;
        }
        case 2:
        {
            char *METKA;
            char *METKA1;
            char *METKA2;
            uint8_t *PTR;
            int DELTA;
            int ZNSYM;
            int NBASRG;
            int J;
            int I;

            uint8_t R1X2;
            uint16_t B2D2;

            oper_t oper_rx;
            memset(&oper_rx, 0, sizeof(oper_t));

            oper_rx.oper_type = OPER_RX;
            oper_rx.oper.rx.opcode = T_MOP[I3].CODOP;
            
            METKA1 = strtok(TEK_ISX_KARTA.OPERAND, ",");
            METKA2 = strtok(NULL, " ");
            if (isalpha(*METKA1))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if(!strcmp(METKA, METKA1))
                    {
                        R1X2 = T_SYM[J].ZNSYM << 4;
                        goto SRX1;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                R1X2 = atoi(METKA1) << 4;
            }

            SRX1:

            if (isalpha(*METKA2))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if(!strcmp(METKA, METKA2))
                    {

                        NBASRG = 0;
                        DELTA = 0xfff - 1;
                        ZNSYM = T_SYM[J].ZNSYM;
                        for (I = 0; I < 15; I++)
                        {
                            if ((T_BASR[I].PRDOST == 'Y') &&
                                (ZNSYM - T_BASR[I].SMESH >= 0) &&
                                (ZNSYM - T_BASR[I].SMESH < DELTA))
                            {
                                NBASRG = I + 1;
                                DELTA  = ZNSYM - T_BASR[I].SMESH;
                            }
                        }

                        if (0 == NBASRG || DELTA > 0xfff )
                        {
                            return ASMCMP_MACHINE_OPER_BASING_ERROR;
                        }
                        else
                        {
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

            asmcmp_common_save_oper_tex_card(oper_rx);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return ASMCMP_MACHINE_OPER_SUCCESS;
}

/* Function handles machine operation with type 'SS'
 * on the first and the second phases */
static enum asmcmp_machine_oper_error_code_e FSS(int entry)
{
    switch (entry)
    {
        case 1:
        {
            size_t oper_len = sizeof(oper_ss_t);
            CHADR += oper_len;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = oper_len;
            }
            break;
        }
        case 2:
        {
            uint8_t *PTR;
            int DELTA;
            int ZNSYM;
            int NBASRG;
            int J;
            int I;

            char *symbol;
            char *D1;
            char *B1;
            uint16_t B1D1;

            char *L_str;
            uint8_t L;

            char *D2;
            uint16_t B2D2;

            oper_t oper_ss;
            memset(&oper_ss, 0, sizeof(oper_t));

            oper_ss.oper_type = OPER_SS;
            oper_ss.oper.ss.opcode = T_MOP[I3].CODOP;
            
            /* save D1 */
            D1 = strtok(TEK_ISX_KARTA.OPERAND, "(");
            if (isalpha(*D1))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    symbol = strtok(T_SYM[J].IMSYM, " ");
                    if(!strcmp(symbol, D1))
                    {
                        B1D1 = T_SYM[J].ZNSYM;
                        goto SSS1;
                    }
                }

                return ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                B1D1 = atoi(D1);
            }

            SSS1:

            /* save L */
            L_str = strtok(NULL, ",");
            L = atoi(L_str);

            /* save B1 */
            B1 = strtok(NULL, ")");
            D2 = strtok(NULL, ", ");
            if (isalpha(*B1))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    symbol = strtok(T_SYM[J].IMSYM, " ");
                    if(!strcmp(symbol, B1))
                    {
                        NBASRG = T_SYM[J].ZNSYM;
                        if (0 == NBASRG || DELTA > 0xfff )
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
            if (isalpha(*D2))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    symbol = strtok(T_SYM[J].IMSYM, " ");
                    if(!strcmp(symbol, D2))
                    {
                        NBASRG = 0;
                        DELTA = 0xfff - 1;
                        ZNSYM = T_SYM[J].ZNSYM;
                        for (I = 0; I < 15; I++)
                        {
                            if ((T_BASR[I].PRDOST == 'Y') &&
                                (ZNSYM - T_BASR[I].SMESH >= 0) &&
                                (ZNSYM - T_BASR[I].SMESH < DELTA))
                            {
                                NBASRG = I + 1;
                                DELTA  = ZNSYM - T_BASR[I].SMESH;
                            }
                        }

                        if (0 == NBASRG || DELTA > 0xfff )
                        {
                            return ASMCMP_MACHINE_OPER_BASING_ERROR;
                        }
                        else
                        {
                            B2D2 = NBASRG << 12;
                            B2D2 = B2D2 + DELTA;
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
            oper_ss.oper.ss.B1D1 = B1D1;

            asmcmp_common_save_oper_tex_card(oper_ss);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return ASMCMP_MACHINE_OPER_SUCCESS;
}

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
