/* encoding: UTF-8 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "asmcmp_common.h"
#include "asmcmp_main.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"

/* Function constructs error message by error data of main module */
static char* asmcmp_main_errmsg_by_errdata(__asmcmp_main_error_data_t err_data, char *errmsg)
{
    switch (err_data.main_err_code)
    {
        case ASMCMP_MAIN_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case ASMCMP_MAIN_WRONG_NUM_CLI_PAR:
            strcpy(errmsg, "Wrong number of command line parameters");
            break;
        case ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH:
            strcpy(errmsg, "Wrong path to ASM-file with the source text");
            break;
        case ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION:
            strcpy(errmsg, "Wrong input file extension with the source text");
            break;
        case ASMCMP_MAIN_NOT_FOUND_INPUT_ASM_FILE:
            strcpy(errmsg, "Couldn't find file with the source text");
            break;
        case ASMCMP_MAIN_ERROR_READING_ASM_FILE:
            strcpy(errmsg, "Error occured while reading file with the source text");
            break;
        case ASMCMP_MAIN_PROGRAM_BUFFER_OVERFLOW:
            strcpy(errmsg, "Overflow of the program buffer while reading file with the source text");
            break;
        case ASMCMP_MAIN_PSEUDO_OPER_ERROR:
            strcpy(errmsg, "Pseudo operations module error");
            break;
        case ASMCMP_MAIN_MACHINE_OPER_ERROR:
            strcpy(errmsg, "Machine operations module error");
            break;
        case ASMCMP_MAIN_OPERATION_CODE_ERROR:
            strcpy(errmsg, "Error of the operation code");
            break;
        case ASMCMP_MAIN_CANT_WRITE_TEX_FILE_ERROR:
            strcpy(errmsg, "Can't write to object file");
            break;
        case ASMCMP_MAIN_WRONG_WRITE_TEX_FILE_ERROR:
            strcpy(errmsg, "Wrong writing to object file");
            break;
        default:
            return "Unknown error code for generating error message";
    }

    if (-1 != err_data.card_number)
    {
        strcat(errmsg, ". Error in card ");
        sprintf(&errmsg[strlen(errmsg)], "%d", err_data.card_number);
    }

    return errmsg;
}

/* Function constructs final '.tex' file. 
 * 'p_tex_fp_name' is path to this file */
static enum asmcmp_main_error_code_e asmcmp_main_make_tex_file(char const *p_tex_fp_name)
{
    FILE *p_text_fp;
    unsigned int cards_written;

    p_text_fp = fopen(p_tex_fp_name , "wb"); 
    if (NULL == p_text_fp)
    {
        return ASMCMP_MAIN_CANT_WRITE_TEX_FILE_ERROR;
    }
    else
    {
        cards_written = fwrite(OBJTEXT, 80, ITCARD, p_text_fp);
        fclose(p_text_fp);
    }

    return (cards_written == ITCARD) ? ASMCMP_MAIN_SUCCESSFUL_TRANSLATION : ASMCMP_MAIN_WRONG_WRITE_TEX_FILE_ERROR;
}


static void plcmp_main_init_cards(void)
{
    ESD.POLE1 = 0x02;
    memcpy(ESD.POLE2, "ESD", 3);
    memset(ESD.POLE3, 0x40, 6);
    ESD.POLE31[0] = 0x00;
    ESD.POLE31[1] = 0x10;
    memset(ESD.POLE32, 0x40, 2);
    ESD.POLE4[0] = 0x00;
    ESD.POLE4[1] = 0x01;
    memset(ESD.IMPR, 0x40, 8);
    ESD.POLE6 = 0x00;
    memset(ESD.ADPRG, 0x00, 3);
    ESD.POLE8 = 0x40;
    memset(ESD.DLPRG, 0x00, 3);
    memset(ESD.POLE10, 0x40, 40);
    memset(ESD.POLE11, 0x40, 8);

    TXT.POLE1 = 0x02;
    memcpy(TXT.POLE2, "TXT", 3);
    TXT.POLE3 = 0x40;
    memset(TXT.ADOP, 0x00, 3);
    memset(TXT.POLE5, 0x40, 2);
    memset(TXT.DLNOP, 0x00, 2);
    memset(TXT.POLE7, 0x40, 2);
    TXT.POLE71[0] = 0x00;
    TXT.POLE71[1] = 0x01;
    memset(TXT.OPER, 0x40, 56);
    memset(TXT.POLE9, 0x40, 8);

    END.POLE1 = 0x02;
    memcpy(END.POLE2, "END", 3);
    memset(END.POLE3, 0x40, 68);
    memset(END.POLE9, 0x40, 8);
}

/* Function of reading ASM-file of the source text with 'p_asm_fp_name' file path name */
static enum asmcmp_main_error_code_e asmcmp_main_read_asm_file(char const *p_asm_fp_name,
                                                               char asm_src_text[][LINELEN],
                                                               size_t *p_asm_src_text_len)
{
    FILE *p_asm_f;
    asmcmp_main_error_code_t err_code = ASMCMP_MAIN_SUCCESS;

    p_asm_f = fopen(p_asm_fp_name , "rb");
    if (NULL == p_asm_f)
    {
        err_code = ASMCMP_MAIN_NOT_FOUND_INPUT_ASM_FILE;
    }
    else
    {
        size_t asm_src_text_len;
        /* Write opened file to byte-array */
        for (asm_src_text_len = 0; asm_src_text_len < DL_ASSTEXT; asm_src_text_len++)
        {
            if (!fread(asm_src_text[asm_src_text_len], 1, LINELEN, p_asm_f))
            {
                if (feof(p_asm_f))
                {   
                    /* Successful reading */
                    break;
                }
                else
                {
                    err_code = ASMCMP_MAIN_ERROR_READING_ASM_FILE;
                    break;
                }
            }
        }

        if (DL_ASSTEXT == asm_src_text_len)
        {
            /* Buffer is overflowed */
            err_code = ASMCMP_MAIN_PROGRAM_BUFFER_OVERFLOW;
        }

        *p_asm_src_text_len = asm_src_text_len;
        fclose(p_asm_f);
    }

    return err_code;
}

/* Function processes source ASM-text */
static struct asmcmp_main_error_data_s asmcmp_main_process_src_text(char asm_src_text[][LINELEN],
                                                                    size_t asm_src_text_len,
                                                                    char const *p_tex_fp_name)
{
    int i1;
    asmcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters
     * before syntax analyzer and semantic calculator call */
    memset(&err_data, 0, sizeof(asmcmp_main_error_data_t));
    err_data = (asmcmp_main_error_data_t){
        .main_err_data = {
            .main_err_code = ASMCMP_MAIN_SUCCESS,
            .card_number = -1
        },
        .pseudo_oper_err_code = ASMCMP_PSEUDO_OPER_SUCCESS,
        .machine_oper_err_code = ASMCMP_MACHINE_OPER_SUCCESS
    };

    /* The first phase */
    for (i1 = 0; i1 < asm_src_text_len; i1++)
    {
        int i2;

        memcpy(&TEK_ISX_KARTA, asm_src_text[i1], 80);

        if (' ' != TEK_ISX_KARTA.METKA[0])
        {
            ++ITSYM;
            PRNMET = 'Y';
            memcpy(T_SYM[ITSYM].IMSYM, TEK_ISX_KARTA.METKA, 8);
            T_SYM[ITSYM].ZNSYM = CHADR;
        }

        /* Check if the current assembler command is one of the pseudo operations */
        for (i2 = 0; i2 < NPOP; i2++)
        {
            if(!memcmp(TEK_ISX_KARTA.OPERAC, T_POP[i2].MNCPOP, 5))
            {
                err_data.pseudo_oper_err_code = T_POP[i2].BXPROG(1);
                switch (err_data.pseudo_oper_err_code)
                {
                    case ASMCMP_PSEUDO_OPER_SUCCESS:
                        goto CONT2;
                    case ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR:
                    case ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR:
                    case ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR:
                        err_data.main_err_data.main_err_code = ASMCMP_MAIN_PSEUDO_OPER_ERROR;
                        goto error;
                    case ASMCMP_PSEUDO_OPER_PHASE_PROCESSING_END:
                        goto CONT3;
                }
            }
        }

        /* Check if the current assembler command is one of the machine operations */
        for (I3 = 0; I3 < NOP; I3++)
        {
            if(!memcmp(TEK_ISX_KARTA.OPERAC, T_MOP[I3].MNCOP, 5))
            {
                err_data.machine_oper_err_code = T_MOP[I3].BXPROG(1);
                if (ASMCMP_MACHINE_OPER_SUCCESS != err_data.machine_oper_err_code)
                {
                    ASMCMP_COMMON_ASSERT(0);
                }

                PRNMET = 'N';
                goto CONT2;
            }
        }

        err_data.main_err_data.main_err_code = ASMCMP_MAIN_OPERATION_CODE_ERROR;
        goto error;

        CONT2:
        
        continue;
    }

    CONT3:

    /* The second phase */
    for (i1 = 0; i1 < asm_src_text_len; i1++)
    {
        int i2;
        memcpy(&TEK_ISX_KARTA, asm_src_text[i1], 80);

        for (i2 = 0; i2 < NPOP; i2++)
        {
            if(!memcmp(TEK_ISX_KARTA.OPERAC, T_POP[i2].MNCPOP, 5))
            {
                err_data.pseudo_oper_err_code = T_POP[i2].BXPROG(2);
                switch (err_data.pseudo_oper_err_code)
                {
                    case ASMCMP_PSEUDO_OPER_SUCCESS:
                        goto CONT4;
                    case ASMCMP_PSEUDO_OPER_WRONG_DATA_FORMAT_ERROR:
                    case ASMCMP_PSEUDO_OPER_NOT_DECLARED_IDENT_ERROR:
                    case ASMCMP_PSEUDO_OPER_ILLEGAL_REGISTER_NUMBER_ERROR:
                        err_data.main_err_data.main_err_code = ASMCMP_MAIN_PSEUDO_OPER_ERROR;
                        goto error;
                    case ASMCMP_PSEUDO_OPER_PHASE_PROCESSING_END:
                        goto CONT5;
                }
            }
        }

        for (I3 = 0; I3 < NOP; I3++)
        {
            if(!memcmp(TEK_ISX_KARTA.OPERAC, T_MOP[I3].MNCOP, 5))
            {
                switch (T_MOP[I3].BXPROG(2))
                {
                    case ASMCMP_MACHINE_OPER_SUCCESS:
                        goto CONT4;
                    case ASMCMP_MACHINE_OPER_NOT_DECLARED_IDENT_ERROR:
                    case ASMCMP_MACHINE_OPER_SECOND_OPERAND_ERROR:
                    case ASMCMP_MACHINE_OPER_BASING_ERROR:
                        err_data.main_err_data.main_err_code = ASMCMP_MAIN_MACHINE_OPER_ERROR;
                        goto error;
                }
            }
        }

        CONT4:

        continue;
    }

    CONT5:

    err_data.main_err_data.main_err_code = asmcmp_main_make_tex_file(p_tex_fp_name);

    error:

    if (ASMCMP_MAIN_SUCCESSFUL_TRANSLATION != err_data.main_err_data.main_err_code)
    {
        err_data.main_err_data.card_number = i1 + 1;
    }

    return err_data;
}

/* This program organizes the sequential processing of the source text:
 * - the first phase of processing assembler text
 * - the second phase of processing assembler text, translate its to object text
 * - recording object text to '.tex' file
 * Successful result of this Assemler compiler is '.tex' object file
 * Unsuccessful result is an error message with reason of interruption of translation
 *
 * Requirements of the input parameters:
 * - 'argc' must be equal '2'
 * - 'argv[0]' as always is path to current program
 * - 'argv[1]' is the first parameter, it has to be the path of the Assembler-file
 */
int main(int const argc, char const *argv[])
{
    char asm_src_text[DL_ASSTEXT][LINELEN];                  /* Content of the array of the source ASM-text */
    size_t asm_src_text_len = 0;                             /* Length of the array of the source ASM-text */
    char *p_asm_fp_name = NULL, *p_tex_fp_name = NULL;
    size_t asm_fp_len;
    asmcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters */
    memset(&err_data, 0, sizeof(asmcmp_main_error_data_t));
    err_data = (asmcmp_main_error_data_t){ 
        .main_err_data = {
            .main_err_code = ASMCMP_MAIN_SUCCESS,
            .card_number = -1
        },
        .pseudo_oper_err_code = ASMCMP_PSEUDO_OPER_SUCCESS,
        .machine_oper_err_code = ASMCMP_MACHINE_OPER_SUCCESS
    };

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_data.main_err_data.main_err_code = ASMCMP_MAIN_WRONG_NUM_CLI_PAR;
        goto error;
    }

    /* Copy name of translated program from input argument */
    ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, argv[1]);

    asm_fp_len = strlen(p_asm_fp_name);
    if (asm_fp_len < 4)
    {
        err_data.main_err_data.main_err_code = ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH;
        goto error;
    }

    /* Input file for translation must be with 'ass' extension */
    if (strcmp(&p_asm_fp_name[asm_fp_len - 4], ".ass"))
    {
        err_data.main_err_data.main_err_code = ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION;
        goto error;
    }
    else
    {
        /* Clear array for the source ASM-text before getting text from the ASM-file */
        memset(asm_src_text, '\0', sizeof(char)*DL_ASSTEXT*LINELEN);

        err_data.main_err_data.main_err_code = asmcmp_main_read_asm_file(p_asm_fp_name, asm_src_text, &asm_src_text_len);
        if (ASMCMP_MAIN_SUCCESS == err_data.main_err_data.main_err_code)
        {
            /* After successfully reading file proceed to translation of the source text */
            plcmp_main_init_cards();
            ASMCMP_MAIN_MAKE_TEX_FILE_PATH_BY_ASM_FILE_PATH(p_tex_fp_name, p_asm_fp_name);
            ASMCMP_COMMON_RELEASE_MEM(p_asm_fp_name);
            err_data = asmcmp_main_process_src_text(asm_src_text, asm_src_text_len, p_tex_fp_name);
            ASMCMP_COMMON_RELEASE_MEM(p_tex_fp_name);
        }
        else
        {
            /* Error occured while reading file */
            ASMCMP_COMMON_RELEASE_MEM(p_asm_fp_name);
            goto error;
        }
    }

    if (ASMCMP_MAIN_SUCCESSFUL_TRANSLATION == err_data.main_err_data.main_err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        char errmsg[100];

        error:

        printf("Translation is interrupted\nReason: %s\n", asmcmp_main_errmsg_by_errdata(err_data.main_err_data, errmsg));

        switch (err_data.main_err_data.main_err_code)
        {
            case ASMCMP_MAIN_PSEUDO_OPER_ERROR:
                printf("Pseudo operations module error message: %s\n",
                       asmcmp_pseudo_oper_errmsg_by_errcode(err_data.pseudo_oper_err_code));
                break;
            case ASMCMP_MAIN_MACHINE_OPER_ERROR:
                printf("Machine operations module error message: %s\n",
                       asmcmp_machine_oper_errmsg_by_errcode(err_data.machine_oper_err_code));
                break;
            default:
                break;
        }
    }

    return err_data.main_err_data.main_err_code;
}

