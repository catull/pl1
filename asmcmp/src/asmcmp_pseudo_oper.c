/* encoding: UTF-8 */

#include "asmcmp_common.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_global.h"

static int FDC(void);
static int FDS(void);
static int FEND(void);
static int FEQU(void);
static int FSTART(void);
static int FUSING(void);

/* Table of the pseudo operations */
pseudo_operations_table_t T_POP[NPOP] = {

    { {'D','C',' ',' ',' '}, FDC    },
    { {'D','S',' ',' ',' '}, FDS    },
    { {'E','N','D',' ',' '}, FEND   },
    { {'E','Q','U',' ',' '}, FEQU   },
    { {'S','T','A','R','T'}, FSTART },
    { {'U','S','I','N','G'}, FUSING }

};

static int FDC(void)
{
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
   
    return 0;
}

static int FDS(void)
{
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

    }
    if (CHADR % 4)
    {
        CHADR = (CHADR / 4 + 1) * 4;
    }

    CHADR = CHADR + 4;
    return 0;
}

static int FEND(void)
{
  return 100;
}

static int FEQU(void)
{
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
    return 0;
}

static int FSTART(void)
{
    CHADR = atoi(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAND);

    if (CHADR % 8)
    {
        CHADR = (CHADR + (8 - CHADR % 8));
    }

    T_SYM[ITSYM].ZNSYM = CHADR;
    T_SYM[ITSYM].DLSYM = 1;
    T_SYM[ITSYM].PRPER = 'R';
    PRNMET = 'N';

    return 0;
}

static int FUSING(void)
{
    return 0;
}