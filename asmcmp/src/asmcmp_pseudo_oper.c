/* encoding: UTF-8 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "asmcmp_common.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_global.h"
#include "utils.h"

extern assembler_card_t g_current_asm_card;

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

/* Subroutine records the next TXT-card with 
 * new data to OBJTEXT-array */
static void save_data_tex_card(data_t data)
{
    char *PTR = NULL;
    size_t data_len = 0;

    switch (data.data_type)
    {
        case DATA_FIXED_BIN:
        {
            if (CHADR % 4)
            {
                CHADR = (CHADR / 4 + 1) * 4;
            }
            
            data_len = sizeof(data.data.fixed_bin);
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &data.data.fixed_bin, data_len);
            TXT.DLNOP[1] = data_len;
            break;
        }
        case DATA_STRING:
        {
            data_len = data.data.string_data.str_length;
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, data.data.string_data.p_string, data_len);
            TXT.DLNOP[1] = data_len;
            break;
        }
        default:
            UTILS_ASSERT(0);
            break;
    }

    PTR = (char*)&CHADR;
    TXT.ADOP[2] = *PTR;
    TXT.ADOP[1] = *(PTR + 1);
    TXT.ADOP[0] = '\x00';

    memcpy(TXT.POLE9, ESD.POLE11, 8);
    memcpy(OBJTEXT[ITCARD], &TXT, 80);
    
    ++ITCARD;
    CHADR = CHADR + data_len;
}

/* Subroutine handles pseudo operation 'DC'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FDC(int entry)
{
    switch (entry)
    {
        case 1:
        {
            size_t data_len = 4;
            if ('Y' == PRNMET)
            {
                switch (g_current_asm_card.OPERAND[0])
                {
                    case 'F':
                        data_len = 4;
                        T_SYM[ITSYM].DLSYM = data_len;

                        if (CHADR % 4)
                        {
                            CHADR = (CHADR / 4 + 1) * 4;
                            T_SYM[ITSYM].sym_value = CHADR;
                        }
                        PRNMET = 'N';
                        break;
                    case 'C':
                    {
                        char buffer[2] = { '\0' };

                        if ('L' != g_current_asm_card.OPERAND[1]) 
                        {
                            return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                        }

                        buffer[0] = g_current_asm_card.OPERAND[2];
                        buffer[1] = '\0';

                        data_len = atoi(buffer);
                        T_SYM[ITSYM].DLSYM = data_len;

                        PRNMET = 'N';
                        break;
                    }
                    default:
                        return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                }
            }
            else if (CHADR % 4)
            {
                CHADR = (CHADR / 4 + 1) * 4;
            }

            CHADR = CHADR + data_len;
            break;
        }
        case 2:
        {
            uint8_t *RAB = NULL;

            data_t data;
            memset(&data, 0, sizeof(data_t));

            switch (g_current_asm_card.OPERAND[0])
            {
                case 'F':
                {
                    data.data_type = DATA_FIXED_BIN;

                    RAB = (uint8_t*)strtok(g_current_asm_card.OPERAND + 2, "'");
                    data.data.fixed_bin = atoi((char*)RAB);

                    RAB = (uint8_t*)&data.data.fixed_bin;
                    utils_swap_bytes(RAB, RAB, 4);

                    save_data_tex_card(data);
                    break;
                }
                case 'C':
                {
                    char buffer[2] = { '\0' };
                    size_t data_len = 0;

                    data.data_type = DATA_STRING;

                    buffer[0] = g_current_asm_card.OPERAND[2];
                    buffer[1] = '\0';
                    data_len = atoi(buffer);

                    data.data.string_data.str_length = data_len;
                    data.data.string_data.p_string = calloc(data_len + 1, sizeof(uint8_t));

                    if ('\'' == g_current_asm_card.OPERAND[3])
                    {
                        if ('\'' != g_current_asm_card.OPERAND[4])
                        {
                            RAB = (uint8_t*)strtok(g_current_asm_card.OPERAND + 4, "'");
                            strcpy((char*)data.data.string_data.p_string, (char*)RAB);
                        }
                    }
                    else
                    {
                        return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                    }

                    RAB = (uint8_t*)data.data.string_data.p_string;

                    save_data_tex_card(data);

                    UTILS_RELEASE_MEM(data.data.string_data.p_string);

                    break;
                }
                default:
                    return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
            }
            break;
        }
        default:
            UTILS_ASSERT(0);
            break;
    }
   
    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Subroutine handles pseudo operation 'DS'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FDS(int entry)
{
    switch (entry)
    {
        case 1:
        {
            size_t data_len = 4;
            if ('Y' == PRNMET)
            {
                switch (g_current_asm_card.OPERAND[0])
                {
                    case 'F':
                    {
                        data_len = 4;
                        T_SYM[ITSYM].DLSYM = data_len;

                        if (CHADR % 4)
                        {
                            CHADR = (CHADR / 4 + 1) * 4;
                            T_SYM[ITSYM].sym_value = CHADR;
                        }

                        PRNMET = 'N';
                        break;
                    }
                    case 'C':
                    {
                        char buffer[2] = { '\0' };

                        if ('L' != g_current_asm_card.OPERAND[1]) 
                        {
                            return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                        }

                        buffer[0] = g_current_asm_card.OPERAND[2];
                        buffer[1] = '\0';

                        data_len = atoi(buffer);
                        T_SYM[ITSYM].DLSYM = data_len;

                        PRNMET = 'N';
                        break;
                    }
                    default:
                        return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;
                }
            } 
            else if (CHADR % 4)
            {
                CHADR = (CHADR / 4 + 1) * 4;
            }

            CHADR = CHADR + data_len;
            break;
        }
        case 2:
        {
            switch (g_current_asm_card.OPERAND[0])
            {
                case 'F':
                    /* TODO */
                    break;
                case 'C':
                    /* TODO */
                    break;
                default:
                    return ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR;    
            }
            break;
        }
        default:
            UTILS_ASSERT(0);
            break;
    }

    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Subroutine handles pseudo operation 'END'
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
            UTILS_ASSERT(0);
            break;
    }
    return ASMCMP_PSEUDO_OPER_PHASE_PROCESSING_END;
}

/* Subroutine handles pseudo operation 'EQU'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FEQU(int entry)
{
    switch (entry)
    {
        case 1:
            if ('*' == g_current_asm_card.OPERAND[0])
            {
                T_SYM[ITSYM].sym_value = CHADR;
                T_SYM[ITSYM].DLSYM = 1;
            }
            else
            {
                T_SYM[ITSYM].sym_value = atoi(g_current_asm_card.OPERAND);
                T_SYM[ITSYM].DLSYM = 1;
            }

            PRNMET = 'N';

            break;
        case 2:
            break;
        default:
            UTILS_ASSERT(0);
            break;
    }

    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Subroutine handles pseudo operation 'START'
 * on the first and the second phases */
static enum asmcmp_pseudo_oper_error_code_e FSTART(int entry)
{
    switch (entry)
    {
        case 1:
            CHADR = atoi(g_current_asm_card.OPERAND);

            /* Operand must have a value a multiple of eight */ 
            if (CHADR % 8)
            {
                CHADR += 8 - CHADR % 8;
            }

            T_SYM[ITSYM].sym_value = CHADR;
            T_SYM[ITSYM].DLSYM = 1;
            PRNMET = 'N';
            break;
        case 2:
        {
            uint8_t *PTR;
            char *METKA;
            char *METKA1;
            int J;
            int RAB;

            METKA1 = strtok(g_current_asm_card.METKA, " ");
            for (J = 0; J <= ITSYM; J++)
            {
                METKA = strtok(T_SYM[J].IMSYM, " ");

                if (!strcmp(METKA, METKA1))
                {
                    RAB = CHADR - T_SYM[J].sym_value;
                    PTR = (uint8_t*)&RAB;
                    utils_swap_bytes(PTR, PTR, 2);

                    ESD.DLPRG[0] = 0;
                    ESD.DLPRG[1] = *PTR;
                    ESD.DLPRG[2] = *(PTR + 1);

                    CHADR = T_SYM[J].sym_value;
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
            UTILS_ASSERT(0);
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
            int J = 0;
            int NBASRG;

            METKA1 = strtok(g_current_asm_card.OPERAND, ",");
            METKA2 = strtok(NULL, " ");
            if (isalpha(*METKA2))
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA2))
                    {
                        if ((NBASRG = T_SYM[J].sym_value) <= 0x0f)
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
                if ((NBASRG == T_SYM[J].sym_value) <= 0x0f)
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
                T_BASR[NBASRG - 1].offset = CHADR;
            }
            else
            {
                for (J = 0; J <= ITSYM; J++)
                {
                    METKA = strtok(T_SYM[J].IMSYM, " ");
                    if (!strcmp(METKA, METKA1))
                    {
                        T_BASR[NBASRG - 1].offset = T_SYM[J].sym_value;
                    }
                }

                return ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR;
            } 
            break;
        }
        default:
            UTILS_ASSERT(0);
            break;
    }
    return ASMCMP_PSEUDO_OPER_SUCCESS;
}

/* Subroutine constructs error message by error code of 'asmcmp pseudo operations' module */
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