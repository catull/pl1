/* encoding: UTF-8 */

#include <ctype.h>
#include <string.h>

#include "asmcmp_common.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"

static int FRR(int entry);
static int FRX(int entry);
static int FSS(int entry);

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
static int FRR(int entry)
{
    switch (entry)
    {
        case 1:
            CHADR = CHADR + 2;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = 2;
                T_SYM[ITSYM].PRPER = 'R';
            }
            break;
        case 2:
        {
            char *METKA;
            char *METKA1;
            char *METKA2;
            unsigned char R1R2;
            int J;
            RR.OP_RR.OP = T_MOP[I3].CODOP;

            METKA1 = strtok(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, ",");
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

                return 2;
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

                return 2;
            }
            else
            {
                R1R2 = R1R2 + atoi(METKA2);
            }

            SRR2:

            RR.OP_RR.R1R2 = R1R2;

            STXT(2);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return 0;
}

/* Function handles machine operation with type 'RX'
 * on the first and the second phases */
static int FRX(int entry)
{
    switch (entry)
    {
        case 1:
            CHADR = CHADR + 4;

            if ('Y' == PRNMET)
            {
                T_SYM[ITSYM].DLSYM = 4;
                T_SYM[ITSYM].PRPER = 'R';
            }
            break;
        case 2:
        {
            char *METKA;
            char *METKA1;
            char *METKA2;
            char *PTR;
            int DELTA;
            int ZNSYM;
            int NBASRG;
            int J;
            int I;
            unsigned char R1X2;
            int B2D2;
            RX.OP_RX.OP = T_MOP[I3].CODOP;
            METKA1 = strtok(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, ",");
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

                return 2;
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
                            return 5;
                        }
                        else
                        {
                            B2D2 = NBASRG << 12;
                            B2D2 = B2D2 + DELTA;
                            PTR = (char *)&B2D2;
                            swab(PTR, PTR, 2);
                            RX.OP_RX.B2D2 = B2D2;
                        }
                        goto SRX2;
                    }
                }

                return 2;
            }
            else
            {
                return 4;
            }

            SRX2:

            RX.OP_RX.R1X2 = R1X2;

            STXT(4);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return 0;
}

int FSS(int entry)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return 0;
}