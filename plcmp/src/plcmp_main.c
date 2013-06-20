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

static char const* plcmp_main_errmsg_by_errcode(plcmp_main_error_code_t err_code)
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
        case PLCMP_MAIN_SYNT_ANALYZER_ERROR:
            return "Error in syntax analyzer";
        case PLCMP_MAIN_SEM_CALCULATOR_ERROR:
            return "Error in semantic calculator";
        default:
            return "Unknown error code for generating error message";
    }
}

/* Subroutine constructs table of the
 * successors of the adjacency matrix
 * of the Uorshell's algorithm
 */
static void plcmp_main_build_tpr(void)
{
    int i1;
    for (i1 = 0; i1 < NNETRM; i1++)
    {
        int i2;
        for (i2 = 0; i2 < NVXOD; i2++)
        {
            if (TPR[i2][i1] && (i1 != i2))
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
static enum plcmp_main_error_code_e plcmp_main_read_pl1_file(char const pl1_fp_name[],
                                                             char pl1_src_text[][LINELEN],
                                                             size_t *p_pl1_src_text_len)
{
    FILE *p_pl1_f;
    plcmp_main_error_code_t err_code = PLCMP_MAIN_SUCCESS;

    p_pl1_f = fopen(pl1_fp_name , "rb");
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
            if (!fread(pl1_src_text[pl1_src_text_len], 1, LINELEN, p_pl1_f))
            {
                if (feof(p_pl1_f))
                {   
                    /* Successful reading */
                    break;
                }
                else
                {
                    err_code = PLCMP_MAIN_ERROR_READING_PL1_FILE;
                    break;
                }
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

/* Function processes source PL1-text by calling 
 * lexical and syntax analyzers and semantic calculator 
 * which translate PL1-text to text with assembler mnemonic commands */
static struct plcmp_main_error_data_s plcmp_main_process_src_text(char pl1_src_text[][LINELEN],
                                                                  size_t pl1_src_text_len,
                                                                  char const p_asm_fp_name[])
{
    /* It's stack of goals achieved. Later it will be created by macro */
    dst_t goals_achieved;

    plcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters
     * before syntax analyzer and semantic calculator call */
    memset(&err_data, 0, sizeof(plcmp_main_error_data_t));
    err_data = (plcmp_main_error_data_t){
        .main_err_code = PLCMP_MAIN_SUCCESS,
        .synt_analyzer_err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS,
        .sem_calc_err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS,
    };

    /* Lexical analysis of the source text */
    plcmp_lex_analyzer_compress_src_text(compact_pl1_src_text, pl1_src_text, pl1_src_text_len);
    /* Construct adjacency matrix */
    plcmp_main_build_tpr();

    /* Create stack of achieved goals */
    PLCMP_MAIN_CREATE_GOALS_ACHIEVED_STACK(goals_achieved);

    /* Syntax analysis of the source text */
    err_data.synt_analyzer_err_data = plcmp_synt_analyzer_syntax_analyzer(&goals_achieved);
    if (PLCMP_SYNT_ANALYZER_SUCCESS != err_data.synt_analyzer_err_data.err_code)
    {
        /* Error in syntax of the source PL1-text.
         * Error data has already contained into 'err_data.synt_analyzer_err_data' structure
         * because syntax analyzer has already returned its */
        err_data.main_err_code = PLCMP_MAIN_SYNT_ANALYZER_ERROR;
    }
    /* Semantic calculation */
    else
    {
        err_data.sem_calc_err_data = plcmp_sem_calc_gen_asm_code(p_asm_fp_name, &goals_achieved);
        if (PLCMP_SEM_CALCULATOR_SUCCESS != err_data.sem_calc_err_data.err_code)
        {
            /* Error in execution logic of the source PL1-text
             * Error data has already contained into 'err_data.sem_calc_err_data' structure
             * because semantic calculator has already returned its */
            err_data.main_err_code = PLCMP_MAIN_SEM_CALCULATOR_ERROR;
        }
    }

    PLCMP_MAIN_DESTROY_GOALS_ACHIEVED_STACK(goals_achieved);

    return err_data;
}

/* This program organizes the sequential processing of the source text:
 * - the lexical analyzer
 * - the syntax analyzer
 * - the semantic calculator 
 * Successful result of this PL1 high level compiler is '.ass' assembler file
 * Unsuccessful result is an error message with reason of interruption of translation
 *
 * Requirements of the input parameters:
 * - 'argc' must be equal '2'
 * - 'argv[0]' as always is path to current program
 * - 'argv[1]' is the first parameter, it has to be the path of the PL1-file
 */
int main(int const argc, char const *argv[])
{
    char pl1_src_text[MAXNISXTXT][LINELEN];             /* Content of the array of the source PL1-text */
    size_t pl1_src_text_len = 0;                        /* Length of the array of the source PL1-text */
    char *p_pl1_fp_name = NULL, *p_asm_fp_name = NULL;
    size_t pl1_fp_len;
    plcmp_main_error_data_t err_data;

    /* Clear error data structure and set default 
     * successful parameters before modules call */
    memset(&err_data, 0, sizeof(plcmp_main_error_data_t));
    err_data = (plcmp_main_error_data_t){ 
        .main_err_code = PLCMP_MAIN_SUCCESS,
        .synt_analyzer_err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS,
        .sem_calc_err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS,
    };

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_data.main_err_code = PLCMP_MAIN_WRONG_NUM_CLI_PAR;
        goto error;
    }

    /* Copy name of translated program from input argument */
    PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_pl1_fp_name, argv[1]);

    pl1_fp_len = strlen(p_pl1_fp_name);
    if (pl1_fp_len < 4)
    {
        err_data.main_err_code = PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH;
        goto error;
    }

    /* Input file for translation must be with 'pli' extension */
    if (strcmp(&p_pl1_fp_name[pl1_fp_len - 4], ".pli"))
    {
        err_data.main_err_code = PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION;
        goto error;
    }
    else
    {
        /* Clear array for the source PL1-text before getting text from the PL1-file */
        memset(pl1_src_text, '\0', sizeof(char)*MAXNISXTXT*LINELEN);

        err_data.main_err_code = plcmp_main_read_pl1_file(p_pl1_fp_name, pl1_src_text, &pl1_src_text_len);
        if (PLCMP_MAIN_SUCCESS == err_data.main_err_code)
        {
            /* After successfully reading file proceed to compression and translation of the source text */
            PLCMP_MAIN_MAKE_ASM_FILE_PATH_BY_PL1_FILE_PATH(p_asm_fp_name, p_pl1_fp_name);
            PLCMP_COMMON_RELEASE_MEM(p_pl1_fp_name);
            err_data = plcmp_main_process_src_text(pl1_src_text, pl1_src_text_len, p_asm_fp_name);
            PLCMP_COMMON_RELEASE_MEM(p_asm_fp_name);
        }
        else
        {
            /* Error occured while reading file */
            PLCMP_COMMON_RELEASE_MEM(p_pl1_fp_name); 
        }
    }

    if (PLCMP_MAIN_SUCCESSFUL_TRANSLATION == err_data.main_err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        error:

        printf("Translation is interrupted\nReason: %s\n", plcmp_main_errmsg_by_errcode(err_data.main_err_code));
        switch(err_data.main_err_code)
        {
            case PLCMP_MAIN_SYNT_ANALYZER_ERROR:
            {
                char errmsg[100];
                printf("Syntax analyzer error message: %s\n",
                       plcmp_synt_analyzer_errmsg_by_errdata(&err_data.synt_analyzer_err_data, errmsg));
                break;
            }
            case PLCMP_MAIN_SEM_CALCULATOR_ERROR:
            {
                char errmsg[100];
                printf("Semantic calculator error message: %s\n",
                       plcmp_sem_calc_errmsg_by_errdata(&err_data.sem_calc_err_data, errmsg));
                break;
            }
            default:
                break;
        }
    }

    return err_data.main_err_code;
}