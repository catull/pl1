/* encoding: UTF-8 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "asmcmp_common.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_global.h"

static enum asmcmp_pseudo_oper_error_code_e FDC(int entry);
static enum asmcmp_pseudo_oper_error_code_e FDS(int entry);
static enum asmcmp_pseudo_oper_error_code_e FEND(int entry);
static enum asmcmp_pseudo_oper_error_code_e FEQU(int entry);
static enum asmcmp_pseudo_oper_error_code_e FSTART(int entry);
static enum asmcmp_pseudo_oper_error_code_e FUSING(int entry);

/* Table of the pseudo operations */
pseudo_operations_table_t T_POP[NPOP] = {
    { {'D','C',' ',' ',' '}, FDC    },
    { {'D','S',' ',' ',' '}, FDS    },
    { {'E','N','D',' ',' '}, FEND   },
    { {'E','Q','U',' ',' '}, FEQU   },
    { {'S','T','A','R','T'}, FSTART },
    { {'U','S','I','N','G'}, FUSING }
};

/* Function handles pseudo operation 'DC'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FDC(int entry)
{
    switch (entry)
    {
        case 1:
            if ('Y' == PRNMET)
            {
                switch (TEK_ISX_KARTA.OPERAND[0])
                {
                    case 'F':
                        T_SYM[ITSYM].DLSYM = 4;
                        T_SYM[ITSYM].PRPER = 'R';

                        if (CHADR % 4)
                        {
                            CHADR = (CHADR / 4 + 1) * 4;
                            T_SYM[ITSYM].ZNSYM = CHADR;
                        }

                        PRNMET = 'N';
                        break;
                    case 'C':
                    {
                        char buffer[2];

                        sprintf(buffer, "%c", TEK_ISX_KARTA.OPERAND[1]);
                        buffer[1] = '\0';
                        T_SYM[ITSYM].DLSYM = atoi(buffer);
                        T_SYM[ITSYM].PRPER = 'R';

                        if (CHADR % 4)
                        {
                            CHADR = (CHADR / 4 + 1) * 4;
                            T_SYM[ITSYM].ZNSYM = CHADR;
                        }

                        PRNMET = 'N';
                        break;
                    }
                    default:
                        return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                }
            }
            else
            {
                if (CHADR % 4)
                {
                    CHADR = (CHADR / 4 + 1) * 4;
                }

                CHADR = CHADR + 4;
            }
            break;
        case 2:
        {
            uint8_t *RAB;

            data_t data;
            memset(&data, 0, sizeof(data_t));

            switch (TEK_ISX_KARTA.OPERAND[0])
            {
                case 'F':
                    data.data_type = DATA_FIXED_BIN;

                    RAB = (uint8_t*)strtok(TEK_ISX_KARTA.OPERAND + 2, "'");
                    data.data.fixed_bin = atoi((char*)RAB);

                    RAB = (uint8_t*)&data.data.fixed_bin;
                    asmcmp_common_swap_bytes(RAB, RAB, 4);

                    asmcmp_common_save_data_tex_card(data);
                    break;
                case 'C':
                {
                    char buffer[2] = {'\0','\0'};
                    size_t str_len;

                    data.data_type = DATA_STRING;

                    buffer[0] = TEK_ISX_KARTA.OPERAND[1];
                    str_len = atoi(buffer);

                    data.data.string_data.str_length = str_len;
                    data.data.string_data.p_string = calloc(str_len + 1, sizeof(uint8_t));

                    RAB = (uint8_t*)strtok(TEK_ISX_KARTA.OPERAND + 3, "'");
                    strcpy((char*)data.data.string_data.p_string, (char*)RAB);

                    RAB = (uint8_t*)data.data.string_data.p_string;
                    asmcmp_common_swap_bytes(RAB, RAB, str_len);

                    asmcmp_common_save_data_tex_card(data);

                    free(data.data.string_data.p_string);
                    data.data.string_data.p_string = NULL;

                    break;
                }
                default:
                    return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
            }
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
   
    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Function handles pseudo operation 'DS'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FDS(int entry)
{
    switch (entry)
    {
        case 1:
            if ('Y' == PRNMET)
            {
                switch (TEK_ISX_KARTA.OPERAND[0])
                {
                    case 'F':
                        T_SYM[ITSYM].DLSYM = 4;
                        T_SYM[ITSYM].PRPER = 'R';

                        if (CHADR % 4)
                        {
                            CHADR = (CHADR / 4 + 1) * 4;
                            T_SYM[ITSYM].ZNSYM = CHADR;
                        }

                        PRNMET = 'N';
                        break;
                    default:
                        return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                }
            } 
            else if (CHADR % 4)
            {
                CHADR = (CHADR / 4 + 1) * 4;
            }

            CHADR = CHADR + 4;
            break;
        case 2:
        /*
            RX.OP = 0;
            RX.R1X2 = 0;
        */
            switch (TEK_ISX_KARTA.OPERAND[0])
            {
                case 'F':
                    //RX.B2D2 = 0;
                    break;
                default:
                    return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;    
            }

            //asmcmp_common_save_data_tex_card(4);
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Function handles pseudo operation 'END'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FEND(int entry)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
            memcpy(END.POLE9, ESD.POLE11, 8);
            memcpy(OBJTEXT[ITCARD], &END, 80);
            ++ITCARD;
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return ASMCMP_PSEUDO_OPER_PHASE_PROCESSING_END;
}

/* Function handles pseudo operation 'EQU'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FEQU(int entry)
{
    switch (entry)
    {
        case 1:
            if ('*' == TEK_ISX_KARTA.OPERAND[0])
            {
                T_SYM[ITSYM].ZNSYM = CHADR;
                T_SYM[ITSYM].DLSYM = 1;
                T_SYM[ITSYM].PRPER = 'R';
            }
            else
            {
                T_SYM[ITSYM].ZNSYM = atoi(TEK_ISX_KARTA.OPERAND);
                T_SYM[ITSYM].DLSYM = 1;
                T_SYM[ITSYM].PRPER = 'A';
            }

            PRNMET = 'N';

            break;
        case 2:
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Function handles pseudo operation 'START'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FSTART(int entry)
{
    switch (entry)
    {
        case 1:
            CHADR = atoi(TEK_ISX_KARTA.OPERAND);

            /* Operand must have a value a multiple of eight */ 
            if (CHADR % 8)
            {
                CHADR += 8 - CHADR % 8;
            }

            T_SYM[ITSYM].ZNSYM = CHADR;
            T_SYM[ITSYM].DLSYM = 1;
            T_SYM[ITSYM].PRPER = 'R';

            PRNMET = 'N';
            break;
        case 2:
        {
            uint8_t *PTR;
            char *METKA;
            char *METKA1;
            int J;
            int RAB;

            METKA1 = strtok(TEK_ISX_KARTA.METKA, " ");
            for (J = 0; J <= ITSYM; J++)
            {
                METKA = strtok(T_SYM[J].IMSYM, " ");

                if (!strcmp(METKA, METKA1))
                {
                    RAB = CHADR - T_SYM[J].ZNSYM;
                    PTR = (uint8_t*)&RAB;
                    asmcmp_common_swap_bytes(PTR, PTR, 2);

                    ESD.DLPRG[0] = 0;
                    ESD.DLPRG[1] = *PTR;
                    ESD.DLPRG[2] = *(PTR + 1);

                    CHADR = T_SYM[J].ZNSYM;
                    PTR = (uint8_t*)&CHADR;

                    ESD.ADPRG[2] = *PTR;
                    ESD.ADPRG[1] = *(PTR + 1);
                    ESD.ADPRG[0] = '\x00';
                    memcpy(ESD.IMPR, METKA, strlen(METKA));
                    memcpy(ESD.POLE11, METKA, strlen(METKA));
                    memcpy(OBJTEXT[ITCARD], &ESD, 80);
                    ++ITCARD; 
                    return ASMCMP_PSEUDO_OPER_SUCCESS;
                }
            }
            return ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR; 
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Subrountine handles pseudo operation 'USING'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FUSING(int entry)
{
    switch (entry)
    {
        case 1:
            break;
        case 2:
        {
            char *METKA;
            char *METKA1;
            char *METKA2;
            int J;
            int NBASRG;

            METKA1 = strtok(TEK_ISX_KARTA.OPERAND, ",");
            METKA2 = strtok(NULL, " ");
            if (isalpha(*METKA2))
            {

                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA2))
                    {
                        if ((NBASRG = T_SYM[J].ZNSYM) <= 0x0f)
                        {
                            goto USING1;            
                        }
                        else
                        {
                            return ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR;
                        }

                    }
                }

                return ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR;
            }
            else
            {
                NBASRG = atoi(METKA2);
                if ((NBASRG = T_SYM[J].ZNSYM) <= 0x0f)
                {
                    goto USING1;
                }
                else
                {
                    return ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR;
                }
            }

            USING1:

            T_BASR[NBASRG - 1].PRDOST = 'Y';
            if ('*' == *METKA1)
            {
                T_BASR[NBASRG - 1].SMESH = CHADR;
            }
            else
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA1))
                    {
                        T_BASR[NBASRG - 1].SMESH = T_SYM[J].ZNSYM;
                    }
                }

                return ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR;
            } 
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }
    return ASMCMP_PSEUDO_OPER_SUCCESS;
}


char const* asmcmp_pseudo_oper_errmsg_by_errcode(asmcmp_pseudo_oper_error_code_t err_code)
{
    switch(err_code)
    {
        case ASMCMP_PSEUDO_OPER_SUCCESS:
            return "No error occured";
        case ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR:
            return "Wrong data format";
        case ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR:
            return "Not declared identifier";
        case ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR:
            return "Illegal register number";
        default:
            return "Unknown error code for generating error message";
    }
}