/* encoding: UTF-8 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "plcmp_common.h"
#include "plcmp_tables.h"
#include "plcmp_main.h"
#include "plcmp_lex_analyzer.h"
#include "plcmp_synt_analyzer.h"
#include "plcmp_sem_calc.h"

static const char* plcmp_main_errmsg_by_errcode(plcmp_main_error_code_t err_code)
{
    switch (err_code)
    {
        case PLCMP_MAIN_SUCCESS:
            return "No error occured";
        case PLCMP_MAIN_WRONG_NUM_CLI_PAR:
            return "Wrong number of command line parameters";
        case PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH:
            return "Wrong path to PL1-file with the source text";
        case PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION:
            return "Wrong input file extension with the source text";
        case PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE:
            return "Couldn't find file with the source text";
        case PLCMP_MAIN_ERROR_READING_PL1_FILE:
            return "Error occured while reading file with the source text";
        case PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW:
            return "Overflow of the program buffer while reading file with the source text";
        default:
            return "Unknown error code for generating error message";
    }
}

/* Построение таблицы  */
/* преемников из матрицы  */
/* смежности по алгоритму */
/* Варшалла */
void build_TPR(void)
{
    int i1;
    for (i1 = 0; i1 < NNETRM; i1++)
    {
        int i2;
        for (i2 = 0; i2 < NVXOD; i2++)
        {
            if (TPR[i2][i1] & (i1 != i2))
            {
                int i3;
                for (i3 = 0; i3 < NNETRM; i3++)
                {
                    TPR[i2][i3] |= TPR[i1][i3];
                }
            }
        }
    }
}

/* Function of reading PL1-file of the source text with 'p_pl1_fp_name' file path name */
static enum plcmp_main_error_code_e plcmp_main_read_pl1_file(char const *p_pl1_fp_name,
                                                             char pl1_src_text[MAXNISXTXT][80],
                                                             size_t *p_pl1_src_text_len)
{
    FILE *p_pl1_f;
    plcmp_main_error_code_t err_code = PLCMP_MAIN_SUCCESS;

    p_pl1_f = fopen(p_pl1_fp_name , "rb");
    if (NULL == p_pl1_f)
    {
        err_code = PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE;
    }
    else
    {
        size_t pl1_src_text_len;
        /* Write opened file to byte-array */
        for (pl1_src_text_len = 0; pl1_src_text_len < MAXNISXTXT; pl1_src_text_len++)
        {
            if (!fread(pl1_src_text[pl1_src_text_len], 80, 1, p_pl1_f))
            {
                if (feof(p_pl1_f))
                {   
                    /* Successful reading */
                    break;
                }
            }
            else
            {
                err_code = PLCMP_MAIN_ERROR_READING_PL1_FILE;
                break;
            }
        }

        if (MAXNISXTXT == pl1_src_text_len)
        {
            /* Buffer is overflowed */
            err_code = PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW;
        }

        *p_pl1_src_text_len = pl1_src_text_len;
        fclose(p_pl1_f);
    }

    return err_code;
}

/* This program organizes the sequential processing of the source text:
 * - the lexical analyzer
 * - the syntax analyzer
 * - the semantic calculator
 * Requirements of the input parameters:
 * - 'argc' must be equal '2'
 * - 'argv[0]' as always is path to current program
 * - 'argv[1]' is the first parameter, it has to be the path of the PL1-file
 */
int main(int const argc, char const *argv[])
{
    /* Content of the array of the source PL1-text */
    char pl1_src_text[MAXNISXTXT][80];
    /* Length of the array of the source PL1-text */
    size_t pl1_src_text_len;

    char *p_pl1_fp_name = NULL, *p_asm_fp_name = NULL;
    size_t pl1_fp_len, asm_fp_len;

    plcmp_main_error_code_t err_code = PLCMP_MAIN_SUCCESS;

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_code = PLCMP_MAIN_WRONG_NUM_CLI_PAR;
        goto error;
    }

    /* Copy name of translated program from input argument */
    PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_pl1_fp_name, argv[1]);
    pl1_fp_len = strlen(p_pl1_fp_name);

    if (pl1_fp_len < 4)
    {
        err_code = PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH;
        goto error;
    }

    /* Input file for translation must be with 'pli' extension */
    if (strcmp(&p_pl1_fp_name[pl1_fp_len - 4], ".pli"))
    {
        err_code = PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION;
        goto error;
    }
    else
    {
        err_code = plcmp_main_read_pl1_file(p_pl1_fp_name, pl1_src_text, &pl1_src_text_len);
        if (PLCMP_MAIN_SUCCESS == err_code)
        {
            /* After successfully reading file proceed to translation of the source text */
            goto process_source_text;
        }
        else
        {
            /* Error occured while reading file */
            goto error;
        }

    }

    process_source_text:

    /* Make file path name for assembler */
    PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_asm_fp_name, p_pl1_fp_name);
    asm_fp_len = strlen(p_asm_fp_name);
    p_asm_fp_name[asm_fp_len - 4] = '\0';
    strcat(p_asm_fp_name, ".ass");

    PLCMP_COMMON_DEALLOC_MEM(p_pl1_fp_name);

    /* Lexical analysis of the source text */
    plcmp_lex_analyzer_compress_src_text(compact_pl1_src_text, pl1_src_text, pl1_src_text_len);
    /* построение матрицы преемников */
    build_TPR();

    /* Syntax analysis of the source text */
    if (plcmp_synt_analyzer_syntax_analyzer())
    {
        /* Error in syntax */
        compact_pl1_src_text[I4 + 20] = '\0';
        printf("Error in syntax of the source text: %s.\n"
               "Traslation is interrupted\n",
               &compact_pl1_src_text[I4]);
        PLCMP_COMMON_DEALLOC_MEM(p_asm_fp_name);
        goto error;
    }
    /* Semantic calculation */
    else
    {
        int dst_index = 0;

        switch (plcmp_sem_calc_gen_asm_code(p_asm_fp_name, &dst_index))
        {
            case 0:
                break;
            case 1:
                printf("Mismatch of the name of the procedure "
                       "in prologue-epilogue\n");
                break;
            case 2:
                compact_pl1_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not allowed indentifier type '%s' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       FORMT[1],
                       &compact_pl1_src_text[DST[dst_index].DST2]);
                break;
            case 3:
                compact_pl1_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not allowed indentifier type '%s' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       FORMT[IFORMT - 1],
                       &compact_pl1_src_text[DST[dst_index].DST2]);
                break;
            case 4:
                compact_pl1_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not determined identifier '%s' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       FORMT[IFORMT - 1],
                       &compact_pl1_src_text[DST[dst_index].DST2]);
                break;
            case 5:
                compact_pl1_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not allowed operation '%c' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       compact_pl1_src_text[DST[dst_index].DST4 - strlen(FORMT[IFORMT - 1])],
                       &compact_pl1_src_text[DST[dst_index].DST2]);
                break;
            case 6:
                compact_pl1_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Repeated declaration of the identifier '%c' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       compact_pl1_src_text[DST[dst_index].DST4 - strlen(FORMT[IFORMT - 1])],
                       &compact_pl1_src_text[DST[dst_index].DST2]);
                break;
            default:
                break;
        }
    }

    error:

    if (PLCMP_MAIN_SUCCESSFUL_TRANSLATION == err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        printf("Translation is interrupted\nReason: %s\n", plcmp_main_errmsg_by_errcode(err_code));
    }

    return err_code;
}