/* encoding: UTF-8 */

#include <ctype.h>
#include <string.h>

#include "asmcmp_common.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_global.h"

static int FDC(int entry);
static int FDS(int entry);
static int FEND(int entry);
static int FEQU(int entry);
static int FSTART(int entry);
static int FUSING(int entry);

/* Table of the pseudo operations */
pseudo_operations_table_t T_POP[NPOP] = {

    { {'D','C',' ',' ',' '}, FDC    },
    { {'D','S',' ',' ',' '}, FDS    },
    { {'E','N','D',' ',' '}, FEND   },
    { {'E','Q','U',' ',' '}, FEQU   },
    { {'S','T','A','R','T'}, FSTART },
    { {'U','S','I','N','G'}, FUSING }

};

static int FDC(int entry)
{
    switch (entry)
    {
        case 1:
            if ('Y' == PRNMET)
            {
                if ('F' == TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND[0])
                {
                    T_SYM[ITSYM].DLSYM = 4;
                    T_SYM[ITSYM].PRPER = 'R';

                    if (CHADR % 4)
                    {
                        CHADR = (CHADR/4 + 1)*4;
                        T_SYM[ITSYM].ZNSYM = CHADR;
                    }

                    PRNMET = 'N';
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if (CHADR % 4)
                {
                    CHADR = (CHADR /4 + 1) * 4;
                }

                CHADR = CHADR + 4;
            }
            break;
        case 2:
        {
            char *RAB;

            RX.OP_RX.OP = 0;
            RX.OP_RX.R1X2 = 0;

            if (!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, "F'", 2))
            {
                RAB = strtok(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND + 2, "'");
                RX.OP_RX.B2D2 = atoi(RAB);
                RAB = (char*)&RX.OP_RX.B2D2;
                swab(RAB, RAB, 2);
            }
            else
            {
                return 1;
            }

            STXT(4);
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
   
    return 0;
}

static int FDS(int entry)
{
    switch (entry) 
    {
        case 1:
            if ('Y' == PRNMET)
            {
                if ('F' == TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND[0])
                {
                    T_SYM[ITSYM].DLSYM = 4;
                    T_SYM[ITSYM].PRPER = 'R';

                    if (CHADR % 4)
                    {
                        CHADR = (CHADR / 4 + 1)*4;
                        T_SYM[ITSYM].ZNSYM = CHADR;
                    }

                    PRNMET = 'N';
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                ;
            }
            if (CHADR % 4)
            {
                CHADR = (CHADR / 4 + 1) * 4;
            }

            CHADR = CHADR + 4;
            break;
        case 2:
            RX.OP_RX.OP = 0;
            RX.OP_RX.R1X2 = 0;

            if ('F' == TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND[0])
            {
                RX.OP_RX.B2D2 = 0;
            }
            else
            {
                return 1;
            }

            STXT(4);
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return 0;
}

static int FEND(int entry)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            memcpy(END.STR_END.POLE9, ESD.STR_ESD.POLE11, 8);
            memcpy(OBJTEXT[ITCARD], END.BUF_END, 80);
            ITCARD += 1;
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return 100;
}

static int FEQU(int entry)
{
    switch (entry)
    {
        case 1:
            if ('*' == TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND[0])
            {
                T_SYM[ITSYM].ZNSYM = CHADR;
                T_SYM[ITSYM].DLSYM = 1;
                T_SYM[ITSYM].PRPER = 'R';
            }
            else
            {
                T_SYM[ITSYM].ZNSYM = atoi(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND);
                T_SYM[ITSYM].DLSYM = 1;
                T_SYM[ITSYM].PRPER = 'A';
            }

            PRNMET = 'N';

            break;
        case 2:
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return 0;
}

static int FSTART(int entry)
{
    switch (entry)
    {
        case 1:
            CHADR = atoi(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND);

            if (CHADR % 8)
            {
                CHADR = (CHADR + (8 - CHADR % 8));
            }

            T_SYM[ITSYM].ZNSYM = CHADR;
            T_SYM[ITSYM].DLSYM = 1;
            T_SYM[ITSYM].PRPER = 'R';
            PRNMET = 'N';
            break;
        case 2:
        {
            char *PTR;
            char *METKA;
            char *METKA1;
            int J;
            int RAB;

            METKA1 = strtok(TEK_ISX_KARTA.STRUCT_BUFCARD.METKA, " ");
            for (J = 0; J <= ITSYM; J++)
            {
                METKA = strtok(T_SYM[J].IMSYM, " ");

                if (!strcmp(METKA, METKA1))
                {
                    RAB = CHADR - T_SYM[J].ZNSYM;
                    PTR = (char *)&RAB;
                    swab(PTR, PTR, 2);

                    ESD.STR_ESD.DLPRG[0] = 0;
                    ESD.STR_ESD.DLPRG[1] = *PTR;
                    ESD.STR_ESD.DLPRG[2] = *(PTR + 1);

                    CHADR = T_SYM[J].ZNSYM;
                    PTR = (char*)&CHADR;

                    ESD.STR_ESD.ADPRG[2] = *PTR;
                    ESD.STR_ESD.ADPRG[1] = *(PTR+1);
                    ESD.STR_ESD.ADPRG[0] = '\x00';
                    memcpy(ESD.STR_ESD.IMPR, METKA, strlen(METKA));
                    memcpy(ESD.STR_ESD.POLE11, METKA, strlen(METKA));
                    memcpy(OBJTEXT[ITCARD], ESD.BUF_ESD, 80);
                    ITCARD += 1; 
                    return 0;
                }
            }
            return 2; 
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return 0;
}

static int FUSING(int entry)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
        {
            char *METKA;
            char *METKA1;
            char *METKA2;
            int J;
            int NBASRG;

            METKA1 = strtok(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND, ",");
            METKA2 = strtok(NULL, " ");
            if (isalpha(*METKA2))
            {

                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA2))
                    {
                        if ((NBASRG = T_SYM[J].ZNSYM) <= 0x0f)
                        {
                            goto USING1;            
                        }
                        else
                        {
                            return 6;
                        }

                    }
                }

                return 2;
            }
            else
            {
                NBASRG = atoi(METKA2);
                if ((NBASRG = T_SYM[J].ZNSYM) <= 0x0f)
                {
                    goto USING1;
                }
                else
                {
                    return 6;
                }
            }

            USING1:

            T_BASR[NBASRG - 1].PRDOST = 'Y';
            if ('*' == *METKA1)
            {
                T_BASR[NBASRG-1].SMESH = CHADR;
            }
            else
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA1))
                    {
                        T_BASR[NBASRG-1].SMESH = T_SYM[J].ZNSYM;
                    }
                }

                return 2;
            } 
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return 0;
}