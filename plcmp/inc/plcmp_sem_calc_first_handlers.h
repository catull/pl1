/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_FIRST_HANDLERS_H
#define PLCMP_SEM_CALC_FIRST_HANDLERS_H

#include "plcmp_nterms.h"
#include "plcmp_sem_calc.h"

typedef enum plcmp_sem_calc_error_code_e 
    plcmp_sem_calc_first_handler_t(void const *param);

#define SYM_X(s) plcmp_sem_calc_first_handler_t F##s;
#define SEM_CALC_FIRST_HANDLERS SYM_NTERMS

SEM_CALC_FIRST_HANDLERS

#undef SYM_X
#undef SEM_CALC_FIRST_HANDLERS

#endif /* PLCMP_SEM_CALC_FIRST_HANDLERS_H */