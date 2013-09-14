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

/* Subroutine creates stack for goals achieved for using later
 * by syntax analyzer (parser) and semantic calculator */
static inline void plcmp_main_create_goals_achieved_stack(dst_t *goals_achieved)
{
    PLCMP_COMMON_ASSERT(NULL != goals_achieved);
    goals_achieved->count = 0;
    PLCMP_COMMON_CALLOCATE_MEM(goals_achieved->p_dst_stack,
                               NDST,
                               sizeof(goals_achieved_stack_t));
}
 
/* Subroutine destroys stack of goals achieved */
static inline void plcmp_main_destroy_goals_achieved_stack(dst_t *goals_achieved)
{
    PLCMP_COMMON_ASSERT(NULL != goals_achieved);
    goals_achieved->count = 0;
    PLCMP_COMMON_RELEASE_MEM(goals_achieved->p_dst_stack);
}

/* Subroutine constructs error message by error code of main module */
static inline char const* errmsg_by_errcode(plcmp_main_error_code_t err_code)
{
    switch (err_code)
    {
        case PLCMP_MAIN_SUCCESS:
            return "No error occured";
        case PLCMP_MAIN_WRONG_NUM_CLI_PAR_ERROR:
            return "Wrong number of command line parameters";
        case PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH_ERROR:
            return "Wrong path to PL1-file with the source text";
        case PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION_ERROR:
            return "Wrong input file extension with the source text";
        case PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE_ERROR:
            return "Couldn't find file with the source text";
        case PLCMP_MAIN_READING_PL1_FILE_ERROR:
            return "Error occured while reading file with the source text";
        case PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW_ERROR:
            return "Overflow of the program buffer while reading file with the source text";
        case PLCMP_MAIN_LEX_ANALYZER_ERROR:
            return "Error in lexical analyzer";
        case PLCMP_MAIN_SYNT_ANALYZER_ERROR:
            return "Error in syntax analyzer";
        case PLCMP_MAIN_SEM_CALCULATOR_ERROR:
            return "Error in semantic calculator";
        default:
            return "Unknown error code for generating error message";
    }
}

/* Subroutine reads PL1-file of the source text with 'p_pl1_fp_name' file path name */
static enum plcmp_main_error_code_e read_pl1_file(char const *p_pl1_fp_name,
                                                  char pl1_src_text[][LINELEN],
                                                  size_t *p_pl1_src_text_len)
{
    FILE *p_pl1_f = NULL;
    size_t pl1_src_text_len = 0;
    plcmp_main_error_code_t err_code = PLCMP_MAIN_SUCCESS;

    p_pl1_f = fopen(p_pl1_fp_name , "rb");
    if (NULL == p_pl1_f)
    {
        err_code = PLCMP_MAIN_NOT_FOUND_INPUT_PL1_FILE_ERROR;
        goto error_file_not_opened;
    }

    /* Write opened file to byte-array */
    for (pl1_src_text_len = 0; pl1_src_text_len < MAXNISXTXT; pl1_src_text_len++)
    {
        if (!fread(pl1_src_text[pl1_src_text_len], 1, LINELEN, p_pl1_f))
        {
            if (!feof(p_pl1_f))
            {
                /* Unsuccessful reading */
                err_code = PLCMP_MAIN_READING_PL1_FILE_ERROR;
                goto error_while_reading_file;
            }
            break;
        }
    }

    if (MAXNISXTXT == pl1_src_text_len)
    {
        /* Buffer is overflowed */
        err_code = PLCMP_MAIN_PROGRAM_BUFFER_OVERFLOW_ERROR;
        goto error_while_reading_file;
    }

    *p_pl1_src_text_len = pl1_src_text_len;

    error_while_reading_file:
    fclose(p_pl1_f);

    error_file_not_opened:
    return err_code;
}

/* Subroutine processes source PL1-text by calling 
 * lexical and syntax analyzers and semantic calculator 
 * which translate PL1-text to text with assembler mnemonic commands */
static struct plcmp_main_error_data_s process_src_text(char pl1_src_text[][LINELEN],
                                                       size_t pl1_src_text_len,
                                                       char const *p_asm_fp_name)
{
    /* Compact source text */
    char compact_pl1_src_text[NSTROKA] = { '\0' };
    /* Stack of goals achieved */
    dst_t goals_achieved;

    plcmp_main_error_data_t err_data;

    /* Clear error data structure and set default successful parameters
     * before lexical, syntax analyzer and semantic calculator will be called */
    memset(&err_data, 0, sizeof(plcmp_main_error_data_t));
    err_data = (plcmp_main_error_data_t) {
        .main_err_code = PLCMP_MAIN_SUCCESS,
        .lex_analyzer_err_data.err_code = PLCMP_LEX_ANALYZER_SUCCESS,
        .synt_analyzer_err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS,
        .sem_calc_err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS,
    };
    memset(&goals_achieved, 0, sizeof(dst_t));

    /* Lexical analysis of the source text */
    err_data.lex_analyzer_err_data =
        plcmp_lex_analyzer_compress_src_text(compact_pl1_src_text, NSTROKA, pl1_src_text, pl1_src_text_len);
    if (PLCMP_LEX_ANALYZER_SUCCESS != err_data.lex_analyzer_err_data.err_code)
    {
        /* Error in lexical analyzer of PL1-text.
         * Error data has already contained into 'err_data.lex_analyzer_err_data' structure
         * because lexical analyzer has already returned its */
        err_data.main_err_code = PLCMP_MAIN_LEX_ANALYZER_ERROR;
        goto error_lex_analyzer;
    }

    /* Create stack of achieved goals */
    plcmp_main_create_goals_achieved_stack(&goals_achieved);

    /* Syntax analysis of the source text and filling stack of goals achived */
    err_data.synt_analyzer_err_data =
        plcmp_synt_analyzer_syntax_analysis(compact_pl1_src_text, &goals_achieved);
    if (PLCMP_SYNT_ANALYZER_SUCCESS != err_data.synt_analyzer_err_data.err_code)
    {
        /* Error in syntax of the source PL1-text.
         * Error data has already contained into 'err_data.synt_analyzer_err_data' structure
         * because syntax analyzer has already returned its */
        err_data.main_err_code = PLCMP_MAIN_SYNT_ANALYZER_ERROR;
        goto error_synt_analyzer;
    }

    /* Semantic calculation */
    err_data.sem_calc_err_data =
        plcmp_sem_calc_gen_asm_code(p_asm_fp_name, compact_pl1_src_text, &goals_achieved);
    if (PLCMP_SEM_CALCULATOR_SUCCESS != err_data.sem_calc_err_data.err_code)
    {
        /* Error in execution logic of the source PL1-text
         * Error data has already contained into 'err_data.sem_calc_err_data' structure
         * because semantic calculator has already returned its */
        err_data.main_err_code = PLCMP_MAIN_SEM_CALCULATOR_ERROR;
        goto error_sem_calculator;
    }

    error_synt_analyzer:
    error_sem_calculator:
    plcmp_main_destroy_goals_achieved_stack(&goals_achieved);

    error_lex_analyzer:
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
 * - 'argv[1]' is the first parameter, it has to be the path to the PL1-file
 */
int main(int const argc, char const *argv[])
{
    char pl1_src_text[MAXNISXTXT][LINELEN] = { {'\0'} };    /* Content of the array of the source PL1-text */
    size_t pl1_src_text_len = 0;                            /* Length of the array of the source PL1-text */
    char *p_pl1_fp_name = NULL, *p_asm_fp_name = NULL;      /* Strings for containing file paths for PL1 and ASM files */
    size_t pl1_fp_len = 0;                                  /* Length of the PL1 file path string */
    plcmp_main_error_data_t err_data;

    /* Clear error data structure and set default 
     * successful parameters before modules call */
    memset(&err_data, 0, sizeof(plcmp_main_error_data_t));
    err_data = (plcmp_main_error_data_t){ 
        .main_err_code = PLCMP_MAIN_SUCCESS,
        .lex_analyzer_err_data.err_code = PLCMP_LEX_ANALYZER_SUCCESS,
        .synt_analyzer_err_data.err_code = PLCMP_SYNT_ANALYZER_SUCCESS,
        .sem_calc_err_data.err_code = PLCMP_SEM_CALCULATOR_SUCCESS,
    };

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_data.main_err_code = PLCMP_MAIN_WRONG_NUM_CLI_PAR_ERROR;
        goto error;
    }

    /* Copy name of translated program from input argument */
    PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_pl1_fp_name, argv[1]);

    /* Length of PL1 file path must be greater than 4 symbols.
     * Minimum file path is extension '.pli' */ 
    pl1_fp_len = strlen(p_pl1_fp_name);
    if (pl1_fp_len < 4)
    {
        err_data.main_err_code = PLCMP_MAIN_WRONG_INPUT_PL1_FILE_PATH_ERROR;
        goto error;
    }

    /* Input file for translation must be with 'pli' extension */
    if (!streq(&p_pl1_fp_name[pl1_fp_len - 4], ".pli"))
    {
        err_data.main_err_code = PLCMP_MAIN_WRONG_INPUT_PL1_FILE_EXTENSION_ERROR;
        goto error;
    }

    /* Clear array for the source PL1-text before getting text from the PL1-file */
    memset(pl1_src_text, '\0', sizeof(char) * MAXNISXTXT * LINELEN);

    err_data.main_err_code = read_pl1_file(p_pl1_fp_name, pl1_src_text, &pl1_src_text_len);
    if (PLCMP_MAIN_SUCCESS != err_data.main_err_code)
    {
        /* Error occured while reading file */
        PLCMP_COMMON_RELEASE_MEM(p_pl1_fp_name); 
        goto error;
    }

    /* After successfully reading file proceed to compression and translation of the source text */
    PLCMP_MAIN_MAKE_ASM_FILE_PATH_BY_PL1_FILE_PATH(p_asm_fp_name, p_pl1_fp_name);
    PLCMP_COMMON_RELEASE_MEM(p_pl1_fp_name);
    err_data = process_src_text(pl1_src_text, pl1_src_text_len, p_asm_fp_name);
    PLCMP_COMMON_RELEASE_MEM(p_asm_fp_name);

    if (PLCMP_MAIN_SUCCESSFUL_TRANSLATION == err_data.main_err_code)
    {
        printf("Translation is finished succesfully\n");
    }
    else
    {
        char errmsg[100] = { '\0' };

        error:

        printf("Translation is interrupted\nReason: %s\n", errmsg_by_errcode(err_data.main_err_code));
        switch(err_data.main_err_code)
        {
            case PLCMP_MAIN_LEX_ANALYZER_ERROR:
                printf("Lexical analyzer error message: %s\n",
                       plcmp_lex_analyzer_errmsg_by_errdata(&err_data.lex_analyzer_err_data, errmsg));
                break;
            case PLCMP_MAIN_SYNT_ANALYZER_ERROR:
                printf("Syntax analyzer error message: %s\n",
                       plcmp_synt_analyzer_errmsg_by_errdata(&err_data.synt_analyzer_err_data, errmsg));
                break;
            case PLCMP_MAIN_SEM_CALCULATOR_ERROR:
                printf("Semantic calculator error message: %s\n",
                       plcmp_sem_calc_errmsg_by_errdata(&err_data.sem_calc_err_data, errmsg));
                break;
            default:
                break;
        }
    }

    return err_data.main_err_code;
}