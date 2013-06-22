/* encoding: UTF-8 */

#include "asmcmp_common.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"

static int FRR(void);
static int FRX(void);

/* Table of the machine operations */
machine_operations_table_t T_MOP[NOP] = 
{
    { {'B','A','L','R',' '}, '\x05', 2, FRR },
    { {'B','C','R',' ',' '}, '\x07', 2, FRR },
    { {'S','T',' ',' ',' '}, '\x50', 4, FRX },
    { {'L',' ',' ',' ',' '}, '\x58', 4, FRX },
    { {'A',' ',' ',' ',' '}, '\x5A', 4, FRX },
    { {'S',' ',' ',' ',' '}, '\x5B', 4, FRX }
};

static int FRR(void)
{
    CHADR = CHADR + 2;

    if ('Y' == PRNMET)
    {
        T_SYM[ITSYM].DLSYM = 2;
        T_SYM[ITSYM].PRPER = 'R';
    }

    return 0;
}

static int FRX(void)
{
    CHADR = CHADR + 4;

    if ('Y' == PRNMET)
    {
        T_SYM[ITSYM].DLSYM = 4;
        T_SYM[ITSYM].PRPER = 'R';
    }

    return 0;
}