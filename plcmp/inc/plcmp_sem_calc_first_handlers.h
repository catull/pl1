/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_FIRST_HANDLERS_H
#define PLCMP_SEM_CALC_FIRST_HANDLERS_H

#include "plcmp_sem_calc.h"

typedef enum plcmp_sem_calc_error_code_e 
    plcmp_sem_calc_first_handler_t(void const *param);

plcmp_sem_calc_first_handler_t FAVI;
plcmp_sem_calc_first_handler_t FBUK;
plcmp_sem_calc_first_handler_t FCIF;
plcmp_sem_calc_first_handler_t FIDE;
plcmp_sem_calc_first_handler_t FIPE;
plcmp_sem_calc_first_handler_t FIPR;
plcmp_sem_calc_first_handler_t FLIT;
plcmp_sem_calc_first_handler_t FMAN;
plcmp_sem_calc_first_handler_t FODC;
plcmp_sem_calc_first_handler_t FOEN;
plcmp_sem_calc_first_handler_t FOPA;
plcmp_sem_calc_first_handler_t FOPR;
plcmp_sem_calc_first_handler_t FPRO;
plcmp_sem_calc_first_handler_t FRZR;
plcmp_sem_calc_first_handler_t FTEL;
plcmp_sem_calc_first_handler_t FZNK;
plcmp_sem_calc_first_handler_t FSTC;
plcmp_sem_calc_first_handler_t FCON;

#endif /* PLCMP_SEM_CALC_FIRST_HANDLERS_H */