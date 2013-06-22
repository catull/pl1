/* encoding: UTF-8 */

#ifndef ASMCMP_MACHINE_OPER_H
#define ASMCMP_MACHINE_OPER_H

#include "asmcmp_common.h"

/* Structure describes type of 
 * table of machine operations */
typedef struct machine_operations_table_s {
    char MNCOP[5];
    unsigned char CODOP;
    unsigned char DLOP;
    int (*BXPROG)(int entry);
} machine_operations_table_t;

extern machine_operations_table_t T_MOP[NOP];

#endif