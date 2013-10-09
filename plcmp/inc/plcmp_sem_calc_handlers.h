/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_HADLERS_H
#define PLCMP_SEM_CALC_HADLERS_H

#include "plcmp_sem_calc.h"
#include "handlers.hnd"

typedef enum plcmp_sem_calc_error_code_e 
    plcmp_sem_calc_handler_t(int entry, void const *param);

#define X(s) plcmp_sem_calc_handler_t s;
#define SEM_CALC_HANDLERS HANDLERS_NAMES

SEM_CALC_HANDLERS

#undef X
#undef SEM_CALC_HANDLERS

#endif /* PLCMP_SEM_CALC_HADLERS_H */