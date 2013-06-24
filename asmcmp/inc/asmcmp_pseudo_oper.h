/* encoding: UTF-8 */

#ifndef ASMCMP_PSEUDO_OPER_H
#define ASMCMP_PSEUDO_OPER_H

#include "asmcmp_common.h"

typedef enum asmcmp_pseudo_oper_error_code_e {
    ASMCMP_PSEUDO_OPER_SUCCESS = 0,
    ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR,
    ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR,
    ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR,
    ASMCMP_PSEUDO_OPER_PHASE_PROCESSING_END,

} asmcmp_pseudo_oper_error_code_t;

/* Structure describes type of 
 * table of pseudo operations */
typedef struct pseudo_operations_table_s
{
    unsigned char MNCPOP[5];
    enum asmcmp_pseudo_oper_error_code_e (*BXPROG)(int entry);
} pseudo_operations_table_t;

extern pseudo_operations_table_t T_POP[NPOP];

char const* asmcmp_pseudo_oper_errmsg_by_errcode(asmcmp_pseudo_oper_error_code_t err_code);

#endif