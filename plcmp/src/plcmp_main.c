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

/* Length of the array of the source PL1-text */
static int NISXTXT;
/* Content of the array of the source PL1-text */
static char ISXTXT[MAXNISXTXT][80];

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
    char *p_pl1_fp_name = NULL, *p_asm_fp_name = NULL;
    size_t pl1_fp_len, asm_fp_len;
    FILE *p_pl1_f;
    int err_code = 1;

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        printf("Wrong number of command line parameters\n");
        goto error;
    }

    /* Get name of translated program */
    PLCMP_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_pl1_fp_name, argv[1]);
    pl1_fp_len = strlen(p_pl1_fp_name);

    if (pl1_fp_len < 4)
    {
        printf("Wrong path to file with the source text\n");
        goto error;
    }

    /* Input file for translation must be with 'pli' extension */
    if (strcmp(&p_pl1_fp_name[pl1_fp_len - 4], ".pli"))
    {
        printf("Wrong file type with the source text\n");
        goto error;
    }
    else
    {
        p_pl1_f = fopen(p_pl1_fp_name , "rb");
        if (NULL == p_pl1_f)
        {
            printf("Couldn't find file with the source text\n");
            goto error;
        }
        else
        {
            /* Write opened file to byte-array */
            for (NISXTXT = 0; NISXTXT < MAXNISXTXT; NISXTXT++)
            {
                if (!fread(ISXTXT[NISXTXT], 80, 1, p_pl1_f))
                {
                    if (feof(p_pl1_f))
                    {   
                        fclose(p_pl1_f);
                        /* After reading file proceed to translating of the source text */
                        goto process_source_text;
                    }
                }
                else
                {
                    printf("Error occured while reading file with the source text\n");
                    fclose(p_pl1_f);
                    goto error;
                }
            }

            /* Buffer is overflowed */
            printf("Overflow of the program buffer while reading file with the source text\n");
            fclose(p_pl1_f);
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

    /* Clear buffer string of the assembler output file */
    memset(&assembler_card, '\0', sizeof(assembler_card));

    /* Lexical analysis of the source text */
    plcmp_lex_analyzer_compress_src_text(compact_src_text, ISXTXT, NISXTXT);
    /* построение матрицы преемников */
    build_TPR();

    /* Syntax analysis of the source text */
    if (plcmp_synt_analyzer_syntax_analyzer())
    {
        /* Error in syntax */
        compact_src_text[I4 + 20] = '\0';
        printf("Error in syntax of the source text: %s.\n"
               "Traslation is interrupted\n",
               &compact_src_text[I4]);
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
                printf("Translation is finished succesfully\n");
                return 0;
            case 1:
                printf("Mismatch of the name of the procedure "
                       "in prologue-epilogue\n");
                break;
            case 2:
                compact_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not allowed indentifier type '%s' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       FORMT[1],
                       &compact_src_text[DST[dst_index].DST2]);
                break;
            case 3:
                compact_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not allowed indentifier type '%s' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       FORMT[IFORMT - 1],
                       &compact_src_text[DST[dst_index].DST2]);
                break;
            case 4:
                compact_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not determined identifier '%s' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       FORMT[IFORMT - 1],
                       &compact_src_text[DST[dst_index].DST2]);
                break;
            case 5:
                compact_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Not allowed operation '%c' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       compact_src_text[DST[dst_index].DST4 - strlen(FORMT[IFORMT - 1])],
                       &compact_src_text[DST[dst_index].DST2]);
                break;
            case 6:
                compact_src_text[DST[dst_index].DST2 + 20] = '\0';
                printf("Repeated declaration of the identifier '%c' "
                       "in the source text: %s.\n"
                       "Traslation is interrupted\n",
                       compact_src_text[DST[dst_index].DST4 - strlen(FORMT[IFORMT - 1])],
                       &compact_src_text[DST[dst_index].DST2]);
                break;
            default:
                break;
        }
    }

    error:

    printf("Translation is interrupted\n");
    return err_code;
}