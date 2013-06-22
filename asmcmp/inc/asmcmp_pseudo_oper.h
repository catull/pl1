/* encoding: UTF-8 */

#ifndef ASMCMP_PSEUDO_OPER_H
#define ASMCMP_PSEUDO_OPER_H

#include "asmcmp_common.h"


/* Structure describes type of 
 * table of pseudo operations */
typedef struct pseudo_operations_table_s
{
    unsigned char MNCPOP[5];
    int (*BXPROG)(int entry);
} pseudo_operations_table_t;

extern pseudo_operations_table_t T_POP[NPOP];

#endif