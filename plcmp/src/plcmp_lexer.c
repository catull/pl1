/* encoding: UTF-8 */

#include <stddef.h>
#include <string.h>

#include "plcmp_common.h"
#include "plcmp_lexer.h"
#include "utils.h"

#define NUL '\0'

static char const list_1[] = {
    ' ',
    ';',
    ')',
    ':',
    '(',
    '+',
    '-',
    '=',
    '*',
    '!',
    '\'',
    NUL
};

static char const list_2[] = {
    ')',
    '(',
    '+',
    '-',
    '=',
    '*',
    '!',
    '\'',
    NUL
};

static int symbol_into_list(char const list[], char sym)
{
    unsigned int i = 0;
    for (i = 0; NUL != list[i]; i++)
    {
        if (list[i] == sym)
        {
            return 1;
        }
    }
    return 0;
}

char const* plcmp_lexer_errmsg_by_errdata(
    plcmp_lexer_error_data_t const *err_data,
    char errmsg[])
{
    switch (err_data->err_code)
    {
        case PLCMP_LEXER_SUCCESS:
            (void)strcpy(errmsg, "No error occured");
            break;
        case PLCMP_LEXER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW:
            (void)strcpy(errmsg,
                         "Overflow of the compact text buffer "
                         "while lexical analysis was processing");
            break;
        default:
            (void)strcpy(errmsg,
                         "Unknown error code for generating error message");
            break;
    }
    return errmsg;
}

struct plcmp_lexer_error_data_s plcmp_lexer_compress_src_text(
    char compact_pl1_src_text[],
    size_t compact_text_maxlen,
    char pl1_src_text[][LINELEN],
    size_t pl1_src_text_len)
{
    unsigned int i1 = 0, i3 = 0;
    /* Last processed symbol in the compact source PL1-text */
    char prev_processed_symb = '\0';

    plcmp_lexer_error_data_t err_data;

    memset(&err_data, 0, sizeof(plcmp_lexer_error_data_t));
    err_data.err_code = PLCMP_LEXER_SUCCESS;

    for (i1 = 0; i1 < pl1_src_text_len; i1++)
    {
        unsigned int i2 = 0;
        for (i2 = 0; i2 < LINELEN; i2++)
        {
            if ('\0' == pl1_src_text[i1][i2])
            {
                break;
            }

            if ('\n' == pl1_src_text[i1][i2])
            {
                /* Remove newline-symbol from the source text */
                continue;
            }

            if (' ' == pl1_src_text[i1][i2] &&
                symbol_into_list(list_1, prev_processed_symb))
            {
                /* In accordance with this condition 'pl1_src_text[i1][i2]'
                 * which contains whitespace-symbol will be removed
                 * from the source text.
                 * Compressed text will not contain this whitespace-symbol
                 * after 'prev_processed_symb' which is being already 
                 * contained in compact source text
                 * For example:
                 * - "TEXT    TEXT2" source text will be processed to 
                 * "TEXT TEXT2" compact source text
                 * - "TEXT: TEXT2" source text will be processed to 
                 * "TEXT:TEXT2" compact source text
                 * - "TEXT(    TEXT2" source text will be processed to 
                 * "TEXT(TEXT2" compact source text
                 *
                 * In this way accordance with this condition 
                 * we remove excess whitespace-symbol after 
                 * 'prev_processed_symb' symbol
                 */
                continue; 
            }
            else if (symbol_into_list(list_2, pl1_src_text[i1][i2]) &&
                     ' ' == prev_processed_symb)
            {
                /* In accordance with this condition 'prev_processed_symb'
                 * which contains space-symbol will be removed
                 * from the compact source text, which already contains 
                 * this space-symbol.
                 * For example:
                 * - "TEXT - TEXT2" source text will be processed to 
                 * "TEXT- TEXT2" compact source text
                 * - "TEXT + TEXT2" source text will be processed to 
                 * "TEXT+ TEXT2" compact source text
                 * - "TEXT (TEXT2" source text will be processed to 
                 * "TEXT(TEXT2" compact source text
                 *
                 * In this way in accordance with this condition 
                 * we remove excess whitespace-symbol before 
                 * 'pl1_src_text[i1][i2]' symbol
                 */
                i3--;
            }

            prev_processed_symb = pl1_src_text[i1][i2];
            compact_pl1_src_text[i3] = prev_processed_symb;
            i3++;
            if (i3 == compact_text_maxlen)
            {
                err_data.err_code =
                    PLCMP_LEXER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW;
                goto error;
            }
        }
    }

    error:
    compact_pl1_src_text[i3] = '\0';

    return err_data;
}