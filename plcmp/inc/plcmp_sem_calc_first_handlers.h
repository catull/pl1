/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_FIRST_HANDLERS_H
#define PLCMP_SEM_CALC_FIRST_HANDLERS_H

#include "plcmp_sem_calc.h"
#include "plcmp_handlers.h"

typedef enum plcmp_sem_calc_error_code_e 
    plcmp_sem_calc_first_handler_t(void const *param);

#define X(s) plcmp_sem_calc_first_handler_t F##s;
#define SEM_CALC_FIRST_HANDLERS HANDLERS_NAMES

SEM_CALC_FIRST_HANDLERS

#undef X
#undef SEM_CALC_FIRST_HANDLERS

#endif /* PLCMP_SEM_CALC_FIRST_HANDLERS_H */