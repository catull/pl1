/* encoding: UTF-8 */

#include "plcmp_sem_calc.h"
#include "plcmp_sem_calc_first_handlers.h"
#include "plcmp_sem_calc_handlers.h"
#include "plcmp_sem_calc_second_handlers.h"
#include "utils.h"

/* Subroutine is for semantic calculation 
 * non-terminal "AVI" on the first and
 * the second phases of semantic calculation 
 * Here "AVI" is "arithmetic expression"
 */
enum plcmp_sem_calc_error_code_e AVI(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FAVI(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SAVI(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "BUK" on the first and
 * the second phases of semantic calculation 
 * Here "BUK" is "letter"
 */
enum plcmp_sem_calc_error_code_e BUK(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FBUK(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SBUK(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "CIF" on the first and
 * the second phases of semantic calculation 
 * Here "CIF" is "digit" 
 */
enum plcmp_sem_calc_error_code_e CIF(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FCIF(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SCIF(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IDE" on the first and
 * the second phases of semantic calculation 
 * Here "IDE" is "identifier"
 */
enum plcmp_sem_calc_error_code_e IDE(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FIDE(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SIDE(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IPE" on the first and
 * the second phases of semantic calculation 
 * Here "IPE" is "variable name"
 */
enum plcmp_sem_calc_error_code_e IPE(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;
    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FIPE(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SIPE(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IPR" on the first and
 * the second phases of semantic calculation 
 * Here "IPR" is "program name"
 */
enum plcmp_sem_calc_error_code_e IPR(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FIPR(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SIPR(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "LIT" on the first and
 * the second phases of semantic calculation 
 * Here "LIT" is "literal"
 */
enum plcmp_sem_calc_error_code_e LIT(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FLIT(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SLIT(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "MAN" on the first and
 * the second phases of semantic calculation 
 * Here "MAN" is "mantissa"
 */
enum plcmp_sem_calc_error_code_e MAN(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FMAN(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SMAN(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "ODC" on the first and
 * the second phases of semantic calculation 
 * Here "ODC" is "PL1 operator of variable declaration 'DCL' "
 */
enum plcmp_sem_calc_error_code_e ODC(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FODC(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SODC(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OEN" on the first and
 * the second phases of semantic calculation 
 * Here "OEN" is "PL1 operator of epilogue 'END' of program "
 */
enum plcmp_sem_calc_error_code_e OEN(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FOEN(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SOEN(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OPA" on the first and
 * the second phases of semantic calculation 
 * Here "OPA" is "operator of arithmetic assignment"
 */
enum plcmp_sem_calc_error_code_e OPA(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FOPA(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SOPA(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OPR" on the first and
 * the second phases of semantic calculation 
 * Here "OPR" is "PL1 operator of prologue 'PROC' of program "
 */
enum plcmp_sem_calc_error_code_e OPR(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FOPR(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SOPR(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "PRO" on the first and
 * the second phases of semantic calculation 
 * Here "PRO" is "program"
 */
enum plcmp_sem_calc_error_code_e PRO(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch(phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FPRO(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SPRO(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "RZR" on the first and
 * the second phases of semantic calculation 
 * Here "RZR" is "capacity"
 */
enum plcmp_sem_calc_error_code_e RZR(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FRZR(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SRZR(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}


/* Subroutine is for semantic calculation 
 * non-terminal "TEL" on the first and
 * the second phases of semantic calculation 
 * Here "TEL" is "body of the program"
 */
enum plcmp_sem_calc_error_code_e TEL(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FTEL(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = STEL(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "ZNK" on the first and
 * the second phases of semantic calculation 
 * Here "ZNK" is "sign"
 */
enum plcmp_sem_calc_error_code_e ZNK(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FZNK(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SZNK(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine for semantic calculation
 * non-ternimal "STC" on the first and
 * the second phases of semantic calculation.
 * Here "STC" is "constant string enclosed into single quotation marks"
 */
enum plcmp_sem_calc_error_code_e STC(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FSTC(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SSTC(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "CON" on the first and
 * the second phases of semantic calculation 
 * Here "CON" is "concat operation"
 */
enum plcmp_sem_calc_error_code_e CON(plcmp_sem_calc_phase_t phase,
                                     void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (phase)
    {
        case SEM_CALC_FIRST_PHASE:
            errcode = FCON(param);
            break;
        case SEM_CALC_SECOND_PHASE:
            errcode = SCON(param);
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return errcode;
}