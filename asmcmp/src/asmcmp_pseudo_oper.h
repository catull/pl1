/* encoding: UTF-8 */

#ifndef ASMCMP_PSEUDO_OPER_H
#define ASMCMP_PSEUDO_OPER_H

#include "asmcmp_common.h"

/* Enumerate defines error codes of pseudo operations module */
typedef enum asmcmp_pseudo_oper_error_code_e {
    ASMCMP_PSEUDO_OPER_SUCCESS = 0,
    ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR,
    ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR,
    ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR,
    ASMCMP_PSEUDO_OPER_PHASE_PROCESSING_END
} asmcmp_pseudo_oper_error_code_t;

/* Structure describes type of 
 * table of pseudo operations */
typedef struct pseudo_operations_table_s
{
    unsigned char MNCPOP[5];
    enum asmcmp_pseudo_oper_error_code_e (*BXPROG)(int entry);
} pseudo_operations_table_t;


typedef enum data_type_e {
    DATA_FIXED_BIN,
    DATA_STRING
} data_type_t;

typedef struct string_data_s {
    size_t str_length;
    uint8_t *p_string;
} string_data_t;

typedef struct data_s {
    data_type_t data_type;
    union {
        uint32_t fixed_bin;
        string_data_t string_data;
    } data;
} data_t;


extern pseudo_operations_table_t T_POP[NPOP];

/* Subroutine constructs error message by error code of 'asmcmp pseudo operations' module */
char const* asmcmp_pseudo_oper_errmsg_by_errcode(asmcmp_pseudo_oper_error_code_t err_code);

#endif /* ASMCMP_PSEUDO_OPER_H */