/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_SECOND_HANDLERS_H
#define PLCMP_SEM_CALC_SECOND_HANDLERS_H

#include "plcmp_sem_calc.h"

typedef enum plcmp_sem_calc_error_code_e
    plcmp_sem_calc_second_handler_t(void const *param);

#define X(s) plcmp_sem_calc_second_handler_t s;

#define SEM_CALC_SECOND_HANDLERS \
    X(SAVI) \
    X(SBUK) \
    X(SCIF) \
    X(SIDE) \
    X(SIPE) \
    X(SIPR) \
    X(SLIT) \
    X(SMAN) \
    X(SODC) \
    X(SOEN) \
    X(SOPA) \
    X(SOPR) \
    X(SPRO) \
    X(SRZR) \
    X(STEL) \
    X(SZNK) \
    X(SSTC) \
    X(SCON)

SEM_CALC_SECOND_HANDLERS

#undef X
#undef SEM_CALC_SECOND_HANDLERS

#endif /* PLCMP_SEM_CALC_SECOND_HANDLERS_H */