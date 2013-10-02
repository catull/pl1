/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_HADLERS_H
#define PLCMP_SEM_CALC_HADLERS_H

#include "plcmp_sem_calc.h"

typedef enum plcmp_sem_calc_error_code_e 
    plcmp_sem_calc_handler_t(int entry, void const *param);

plcmp_sem_calc_handler_t AVI;
plcmp_sem_calc_handler_t BUK;
plcmp_sem_calc_handler_t CIF;
plcmp_sem_calc_handler_t IDE;
plcmp_sem_calc_handler_t IPE;
plcmp_sem_calc_handler_t IPR;
plcmp_sem_calc_handler_t LIT;
plcmp_sem_calc_handler_t MAN;
plcmp_sem_calc_handler_t ODC;
plcmp_sem_calc_handler_t OEN;
plcmp_sem_calc_handler_t OPA;
plcmp_sem_calc_handler_t OPR;
plcmp_sem_calc_handler_t PRO;
plcmp_sem_calc_handler_t RZR;
plcmp_sem_calc_handler_t TEL;
plcmp_sem_calc_handler_t ZNK;
plcmp_sem_calc_handler_t STC;
plcmp_sem_calc_handler_t CON;

#endif /* PLCMP_SEM_CALC_HADLERS_H */