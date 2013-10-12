/* encoding: UTF-8 */

#include "plcmp_utils.h"
#include "plcmp_sem_calc.h"
#include "plcmp_sem_calc_first_handlers.h"
#include "plcmp_sem_calc_handlers.h"
#include "plcmp_sem_calc_second_handlers.h"

/* Subroutine is for semantic calculation 
 * non-terminal "AVI" on the first and
 * the second phases of semantic calculation 
 * Here "AVI" is "arithmetic expression"
 */
enum plcmp_sem_calc_error_code_e AVI(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FAVI(param);
            break;
        case 2:
            errcode = SAVI(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "BUK" on the first and
 * the second phases of semantic calculation 
 * Here "BUK" is "letter"
 */
enum plcmp_sem_calc_error_code_e BUK(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FBUK(param);
            break;
        case 2:
            errcode = SBUK(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "CIF" on the first and
 * the second phases of semantic calculation 
 * Here "CIF" is "digit" 
 */
enum plcmp_sem_calc_error_code_e CIF(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FCIF(param);
            break;
        case 2:
            errcode = SCIF(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IDE" on the first and
 * the second phases of semantic calculation 
 * Here "IDE" is "identifier"
 */
enum plcmp_sem_calc_error_code_e IDE(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FIDE(param);
            break;
        case 2:
            errcode = SIDE(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IPE" on the first and
 * the second phases of semantic calculation 
 * Here "IPE" is "variable name"
 */
enum plcmp_sem_calc_error_code_e IPE(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;
    switch (entry)
    {
        case 1:
            errcode = FIPE(param);
            break;
        case 2:
            errcode = SIPE(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "IPR" on the first and
 * the second phases of semantic calculation 
 * Here "IPR" is "program name"
 */
enum plcmp_sem_calc_error_code_e IPR(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FIPR(param);
            break;
        case 2:
            errcode = SIPR(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "LIT" on the first and
 * the second phases of semantic calculation 
 * Here "LIT" is "literal"
 */
enum plcmp_sem_calc_error_code_e LIT(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FLIT(param);
            break;
        case 2:
            errcode = SLIT(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "MAN" on the first and
 * the second phases of semantic calculation 
 * Here "MAN" is "mantissa"
 */
enum plcmp_sem_calc_error_code_e MAN(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FMAN(param);
            break;
        case 2:
            errcode = SMAN(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "ODC" on the first and
 * the second phases of semantic calculation 
 * Here "ODC" is "PL1 operator of variable declaration 'DCL' "
 */
enum plcmp_sem_calc_error_code_e ODC(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FODC(param);
            break;
        case 2:
            errcode = SODC(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OEN" on the first and
 * the second phases of semantic calculation 
 * Here "OEN" is "PL1 operator of epilogue 'END' of program "
 */
enum plcmp_sem_calc_error_code_e OEN(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FOEN(param);
            break;
        case 2:
            errcode = SOEN(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OPA" on the first and
 * the second phases of semantic calculation 
 * Here "OPA" is "operator of arithmetic assignment"
 */
enum plcmp_sem_calc_error_code_e OPA(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FOPA(param);
            break;
        case 2:
            errcode = SOPA(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "OPR" on the first and
 * the second phases of semantic calculation 
 * Here "OPR" is "PL1 operator of prologue 'PROC' of program "
 */
enum plcmp_sem_calc_error_code_e OPR(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FOPR(param);
            break;
        case 2:
            errcode = SOPR(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "PRO" on the first and
 * the second phases of semantic calculation 
 * Here "PRO" is "program"
 */
enum plcmp_sem_calc_error_code_e PRO(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch(entry)
    {
        case 1:
            errcode = FPRO(param);
            break;
        case 2:
            errcode = SPRO(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "RZR" on the first and
 * the second phases of semantic calculation 
 * Here "RZR" is "capacity"
 */
enum plcmp_sem_calc_error_code_e RZR(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FRZR(param);
            break;
        case 2:
            errcode = SRZR(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}


/* Subroutine is for semantic calculation 
 * non-terminal "TEL" on the first and
 * the second phases of semantic calculation 
 * Here "TEL" is "body of the program"
 */
enum plcmp_sem_calc_error_code_e TEL(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FTEL(param);
            break;
        case 2:
            errcode = STEL(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "ZNK" on the first and
 * the second phases of semantic calculation 
 * Here "ZNK" is "sign"
 */
enum plcmp_sem_calc_error_code_e ZNK(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FZNK(param);
            break;
        case 2:
            errcode = SZNK(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine for semantic calculation
 * non-ternimal "STC" on the first and
 * the second phases of semantic calculation.
 * Here "STC" is "constant string enclosed into single quotation marks"
 */
enum plcmp_sem_calc_error_code_e STC(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FSTC(param);
            break;
        case 2:
            errcode = SSTC(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}

/* Subroutine is for semantic calculation 
 * non-terminal "CON" on the first and
 * the second phases of semantic calculation 
 * Here "CON" is "concat operation"
 */
enum plcmp_sem_calc_error_code_e CON(int entry, void const *param)
{
    plcmp_sem_calc_error_code_t errcode = PLCMP_SEM_CALCULATOR_SUCCESS;

    switch (entry)
    {
        case 1:
            errcode = FCON(param);
            break;
        case 2:
            errcode = SCON(param);
            break;
        default:
            PLCMP_UTILS_ASSERT(0, "");
            break;
    }

    return errcode;
}