/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_FIRST_HANDLERS_H
#define PLCMP_SEM_CALC_FIRST_HANDLERS_H

#include "plcmp_sem_calc.h"

typedef enum plcmp_sem_calc_error_code_e 
    plcmp_sem_calc_first_handler_t(void const *param);

#define X(s) plcmp_sem_calc_first_handler_t s;

#define SEM_CALC_FIRST_HANDLERS \
    X(FAVI) \
    X(FBUK) \
    X(FCIF) \
    X(FIDE) \
    X(FIPE) \
    X(FIPR) \
    X(FLIT) \
    X(FMAN) \
    X(FODC) \
    X(FOEN) \
    X(FOPA) \
    X(FOPR) \
    X(FPRO) \
    X(FRZR) \
    X(FTEL) \
    X(FZNK) \
    X(FSTC) \
    X(FCON)

SEM_CALC_FIRST_HANDLERS

#undef X
#undef SEM_CALC_FIRST_HANDLERS

#endif /* PLCMP_SEM_CALC_FIRST_HANDLERS_H */