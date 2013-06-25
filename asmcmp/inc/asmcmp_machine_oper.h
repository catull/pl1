/* encoding: UTF-8 */

#ifndef ASMCMP_MACHINE_OPER_H
#define ASMCMP_MACHINE_OPER_H

#include "asmcmp_common.h"

/* Lengths of the machine operations in bytes */
#define OPER_RR_LEN 2
#define OPER_RX_LEN 4
#define OPER_SS_LEN 6

typedef enum asmcmp_machine_oper_error_code_e {
    ASMCMP_MACHINE_OPER_SUCCESS = 0,
    ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR,
    ASMCMP_MACHINE_OPER_SECOND_OPERAND_ERROR,
    ASMCMP_MACHINE_OPER_BASING_ERROR
} asmcmp_machine_oper_error_code_t;

/* Structure describes type of 
 * table of machine operations */
typedef struct machine_operations_table_s {
    char MNCOP[5];
    unsigned char opcode;
    enum asmcmp_machine_oper_error_code_e (*BXPROG)(int entry);
} machine_operations_table_t;

extern machine_operations_table_t T_MOP[NOP];

char const* asmcmp_machine_oper_errmsg_by_errcode(asmcmp_machine_oper_error_code_t err_code);

#endif