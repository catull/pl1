/* encoding: UTF-8 */

#ifndef ASMCMP_MACHINE_OPER_H
#define ASMCMP_MACHINE_OPER_H

#include "asmcmp_common.h"

typedef struct machine_operations_table_s {
    char MNCOP[5];
    unsigned char CODOP;
    unsigned char DLOP;
    int (*BXPROG)(void);
} machine_operations_table_t;

extern machine_operations_table_t T_MOP[NOP];

#endif