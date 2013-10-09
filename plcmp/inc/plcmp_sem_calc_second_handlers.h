/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_SECOND_HANDLERS_H
#define PLCMP_SEM_CALC_SECOND_HANDLERS_H

#include "plcmp_sem_calc.h"
#include "handlers.hnd"

typedef enum plcmp_sem_calc_error_code_e
    plcmp_sem_calc_second_handler_t(void const *param);

#define X(s) plcmp_sem_calc_second_handler_t S##s;
#define SEM_CALC_SECOND_HANDLERS HANDLERS_NAMES

SEM_CALC_SECOND_HANDLERS

#undef X
#undef SEM_CALC_SECOND_HANDLERS

#endif /* PLCMP_SEM_CALC_SECOND_HANDLERS_H */