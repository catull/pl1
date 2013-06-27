/* encoding: UTF-8 */

#ifndef ASMCMP_MACHINE_OPER_H
#define ASMCMP_MACHINE_OPER_H

#include "asmcmp_common.h"

/* Lengths of the machine operations in bytes */
#define OPER_RR_LEN 2
#define OPER_RX_LEN 4
#define OPER_SS_LEN 6

/* Enumerate defines error codes of machine operations module */
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

/* Enumerate with types of machine operations */
typedef enum oper_type_e {
    /* RR (Register-Register) */
    OPER_RR,
    /* RX (Register-Memory) */
    OPER_RX,
    /* SS (String-String) */
    OPER_SS
} oper_type_t;

/* Structure describes content
 * of the RR-operation type
 * - opcode - operation code (8 bits)
 * - R1R2 - byte containing numbers of 
 * the first destination register (elder tetrad, 4 bits)
 * and the second source register (lower tetrad, 4 bits) => total 8 bits
 *
 *    opcode   R1   R2
 * |xxxx xxxx|xxxx|xxxx|
 *
 * Operation's length is 2 bytes (16 bits)
 */
typedef struct oper_rr_s {
    uint8_t opcode;
    uint8_t R1R2;
} oper_rr_t;

/* Structure describes content
 * of the RX-operation type
 * - opcode - operation code (8 bits)
 * - R1X2 - byte containing number of 
 * the destination register (elder tetrad, 4 bits) as the first operand
 * and index value (lower tetrad, 4 bits) of the second operand => total 8 bits
 * - B2D2 - double-byte containing number of
 * the base register (elder tetrad, 4 bits) of the second operand
 * and 12-bits length value of the address' displacement (12 bits)
 * of the second operand => total 16 bits
 *
 *    opcode   R1   X2   B2        D2
 * |xxxx xxxx|xxxx|xxxx|xxxx|xxxx xxxx xxxx|
 *
 * Operation's length is 4 bytes (32 bits)
 */
typedef struct oper_rx_s {
    uint8_t opcode;
    uint8_t R1X2;
    uint16_t B2D2;
} oper_rx_t;

/* Structure describes content
 * of the SS-operation type
 * - opcode - operation code (8 bits)
 * - L - byte containing length of the operands (8 bits)
 * - B1D1 - double-byte containing number of 
 * the base register (elder tetrad, 4 bits) of the first operand
 * and 12-bit length value of the address' displacement 
 * of the first operand (12 bits) => total 16 bits
 * - B2D2 - double-byte containing number of
 * the base register (elder tetrad, 4 bits) of the second operand
 * and 12-bit length value of the address' displacement 
 * of the second operand (12 bits) => total 16 bits
 *
 *    opcode      L      B1        D2        B2        D2
 * |xxxx xxxx|xxxx xxxx|xxxx|xxxx xxxx xxxx|xxxx|xxxx xxxx xxxx|
 *
 * Operation's length is 6 bytes (48 bits)
 */
typedef struct oper_ss_s {
    uint8_t opcode;
    uint8_t L;
    uint16_t B1D1;
    uint16_t B2D2;
} oper_ss_t;


typedef struct oper_s {
    oper_type_t oper_type;
    union {
        oper_rr_t rr;
        oper_rx_t rx;
        oper_ss_t ss;
    } oper;
} oper_t;


extern machine_operations_table_t T_MOP[NOP];

/* Subroutine constructs error message by error code of 'asmcmp machine operations' module */
char const* asmcmp_machine_oper_errmsg_by_errcode(asmcmp_machine_oper_error_code_t err_code);
/* Subroutine prints transmitted machine operation to STDOUT */
void asmcmp_machine_oper_print_oper(oper_t oper);

#endif /* ASMCMP_MACHINE_OPER_H */