/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_MESSAGES_H
#define PLCMP_SEM_CALC_MESSAGES_H

#include "plcmp_sem_calc.h"

/* Subroutine constructs error message 
 * by error data of semantic calculator module */
char* plcmp_sem_calc_messages_errmsg_by_errdata(
    plcmp_sem_calc_error_data_t const *err_data,
    char errmsg[]);

#endif /* PLCMP_SEM_CALC_MESSAGES_H */