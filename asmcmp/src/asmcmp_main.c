#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "asmcmp_common.h"
#include "asmcmp_main.h"
#include "asmcmp_pseudo_oper.h"
#include "asmcmp_machine_oper.h"
#include "asmcmp_global.h"

static int asmcmp_main_make_tex_file(char const *p_tex_fp_name)
{
    FILE *p_text_fp;
    int RAB2;

    p_text_fp = fopen(p_tex_fp_name , "wb"); 
    if (NULL == p_text_fp)
    {
        return -7; 
    }
    else
    {
        RAB2 = fwrite(OBJTEXT, 80, ITCARD, p_text_fp);
        fclose(p_text_fp);
    }

    return RAB2;
}


static void INITUNION(void)
{
    ESD.STR_ESD.POLE1 = 0x02;
    memcpy(ESD.STR_ESD.POLE2, "ESD", 3);
    memset(ESD.STR_ESD.POLE3, 0x40, 6);
    ESD.STR_ESD.POLE31[0] = 0x00;
    ESD.STR_ESD.POLE31[1] = 0x10;
    memset(ESD.STR_ESD.POLE32, 0x40, 2);
    ESD.STR_ESD.POLE4[0] = 0x00;
    ESD.STR_ESD.POLE4[1] = 0x01;
    memset(ESD.STR_ESD.IMPR, 0x40, 8);
    ESD.STR_ESD.POLE6 = 0x00;
    memset(ESD.STR_ESD.ADPRG, 0x00, 3);
    ESD.STR_ESD.POLE8 = 0x40;
    memset(ESD.STR_ESD.DLPRG, 0x00, 3);
    memset(ESD.STR_ESD.POLE10, 0x40, 40);
    memset(ESD.STR_ESD.POLE11, 0x40, 8);

    TXT.STR_TXT.POLE1 = 0x02;
    memcpy(TXT.STR_TXT.POLE2, "TXT", 3);
    TXT.STR_TXT.POLE3 = 0x40;
    memset(TXT.STR_TXT.ADOP, 0x00, 3);
    memset(TXT.STR_TXT.POLE5, 0x40, 2);
    memset(TXT.STR_TXT.DLNOP, 0X00, 2);
    memset(TXT.STR_TXT.POLE7, 0x40, 2);
    TXT.STR_TXT.POLE71[0] = 0x00;
    TXT.STR_TXT.POLE71[1] = 0x01;
    memset(TXT.STR_TXT.OPER, 0x40, 56);
    memset(TXT.STR_TXT.POLE9, 0x40, 8);

    END.STR_END.POLE1 = 0x02;
    memcpy(END.STR_END.POLE2, "END", 3);
    memset(END.STR_END.POLE3, 0x40, 68);
    memset(END.STR_END.POLE9, 0x40, 8);
}

static char* asmcmp_main_errmsg_by_errdata(asmcmp_main_error_data_t err_data, char *errmsg)
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
        case ASMCMP_MAIN_WRONG_DATA_FORMAT_ERROR:
            strcpy(errmsg, "Wrong data format");
            break;
        case ASMCMP_MAIN_NOT_DECLARED_IDENT_ERROR:
            strcpy(errmsg, "Not declared identifier");
            break;
        case ASMCMP_MAIN_OPERATION_CODE_ERROR:
            strcpy(errmsg, "Error of the operation code");
            break;
        case ASMCMP_MAIN_SECOND_OPERAND_ERROR:
            strcpy(errmsg, "Error of the second operand");
            break;
        case ASMCMP_MAIN_BASING_ERROR:
            strcpy(errmsg, "Basing error");
            break;
        case ASMCMP_MAIN_ILLEGAL_REGISTER_NUMBER_ERROR:
            strcpy(errmsg, "Illegal register number");
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

    strcat(errmsg, ". Error in card ");
    sprintf(&errmsg[strlen(errmsg)], "%d", err_data.card_number);

    return errmsg;
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
    int RAB;
    asmcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters
     * before syntax analyzer and semantic calculator call */
    memset(&err_data, 0, sizeof(asmcmp_main_error_data_t));
    err_data = (asmcmp_main_error_data_t){
        .main_err_code = ASMCMP_MAIN_SUCCESS
    };

    /* The first phase */
    for (i1 = 0; i1 < asm_src_text_len; i1++)
    {
        int i2;

        memcpy(TEK_ISX_KARTA.BUFCARD, asm_src_text[i1], 80);

        if (' ' != TEK_ISX_KARTA.STRUCT_BUFCARD.METKA[0])
        {
            ++ITSYM;
            PRNMET = 'Y';
            memcpy(T_SYM[ITSYM].IMSYM, TEK_ISX_KARTA.STRUCT_BUFCARD.METKA, 8);
            T_SYM[ITSYM].ZNSYM = CHADR;
        }

        /* Check if the current assembler command is one of the pseudo operations */
        for (i2 = 0; i2 < NPOP; i2++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_POP[i2].MNCPOP, 5))
            {
                switch (T_POP[i2].BXPROG(1))
                {
                    case 0:
                        goto CONT2;
                    case 1:
                        err_data.main_err_code = ASMCMP_MAIN_WRONG_DATA_FORMAT_ERROR;
                        goto error;
                    case 100:
                        goto CONT3;
                }
            }
        }

        /* Check if the current assembler command is one of the machine operations */
        for (I3 = 0; I3 < NOP; I3++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_MOP[I3].MNCOP, 5))
            {
                T_MOP[I3].BXPROG(1);
                PRNMET = 'N';
                goto CONT2;
            }
        }

        err_data.main_err_code = ASMCMP_MAIN_OPERATION_CODE_ERROR;
        goto error;

        CONT2:
        
        continue;
    }

    CONT3:

    /* The second phase */
    for (i1 = 0; i1 < asm_src_text_len; i1++)
    {
        int i2;
        memcpy(TEK_ISX_KARTA.BUFCARD, asm_src_text[i1], 80);

        for (i2 = 0; i2 < NPOP; i2++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_POP[i2].MNCPOP, 5))
            {
                switch (T_POP[i2].BXPROG(2))
                {
                    case 0:
                        goto CONT4;
                    case 100:
                        goto CONT5;
                }
            }
        }

        for (I3 = 0; I3 < NOP; I3++)
        {
            if(!memcmp(TEK_ISX_KARTA.STRUCT_BUFCARD.OPERAC, T_MOP[I3].MNCOP, 5))
            {
                switch (T_MOP[I3].BXPROG(2))
                {
                    case 0:
                        goto CONT4;
                    case 2:
                        err_data.main_err_code = ASMCMP_MAIN_NOT_DECLARED_IDENT_ERROR;
                        goto error;
                    case 4:
                        err_data.main_err_code = ASMCMP_MAIN_SECOND_OPERAND_ERROR;
                        goto error;
                    case 5:
                        err_data.main_err_code = ASMCMP_MAIN_BASING_ERROR;
                        goto error;
                    case 6:
                    case 7:
                        err_data.main_err_code = ASMCMP_MAIN_ILLEGAL_REGISTER_NUMBER_ERROR;
                        goto error;
                }
            }
        }

        CONT4:

        continue;
    }

    CONT5:

    RAB = asmcmp_main_make_tex_file(p_tex_fp_name);
    if (ITCARD != RAB)
    {
        err_data.main_err_code = (-7 == RAB) ? ASMCMP_MAIN_CANT_WRITE_TEX_FILE_ERROR : ASMCMP_MAIN_WRONG_WRITE_TEX_FILE_ERROR;
    }

    error:

    if (ASMCMP_MAIN_SUCCESSFUL_TRANSLATION != err_data.main_err_code)
    {
        err_data.card_number = i1 + 1;
    }

    return err_data;
}

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
        .main_err_code = ASMCMP_MAIN_SUCCESS
    };

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_data.main_err_code = ASMCMP_MAIN_WRONG_NUM_CLI_PAR;
        goto error;
    }

    /* Copy name of translated program from input argument */
    ASMCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, argv[1]);

    asm_fp_len = strlen(p_asm_fp_name);
    if (asm_fp_len < 4)
    {
        err_data.main_err_code = ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_PATH;
        goto error;
    }

    /* Input file for translation must be with 'ass' extension */
    if (strcmp(&p_asm_fp_name[asm_fp_len - 4], ".ass"))
    {
        err_data.main_err_code = ASMCMP_MAIN_WRONG_INPUT_ASM_FILE_EXTENSION;
        goto error;
    }
    else
    {
        /* Clear array for the source ASM-text before getting text from the ASM-file */
        memset(asm_src_text, '\0', sizeof(char)*DL_ASSTEXT*LINELEN);

        err_data.main_err_code = asmcmp_main_read_asm_file(p_asm_fp_name, asm_src_text, &asm_src_text_len);
        if (ASMCMP_MAIN_SUCCESS == err_data.main_err_code)
        {
            /* After successfully reading file proceed to translation of the source text */
            INITUNION();
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

    if (ASMCMP_MAIN_SUCCESSFUL_TRANSLATION == err_data.main_err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        char errmsg[100];

        error:

        printf("Translation is interrupted\nReason: %s\n", asmcmp_main_errmsg_by_errdata(err_data, errmsg));
    }

    return err_data.main_err_code;
}

