/* encoding: UTF-8 */

#include <stddef.h>

#include "plcmp_common.h"
#include "plcmp_lex_analyzer.h"

/* Subroutine of primitive lexical analyzer 
 * It compresses the source text by removing all excess spaces and newline-symbols */
enum plcmp_lex_analyzer_error_code_e plcmp_lex_analyzer_compress_src_text(char compact_pl1_src_text[],
                                                                          size_t compact_text_maxlen,
                                                                          char pl1_src_text[][LINELEN],
                                                                          size_t pl1_src_text_len)
{
    unsigned int i1, i3 = 0;
    /* Last processed symbol in the compact source PL1-text */
    char prev_processed_symb = '\0';

    for (i1 = 0; i1 < pl1_src_text_len; i1++)
    {
        unsigned int i2;
        for (i2 = 0; i2 < LINELEN; i2++)
        {
            if ('\0' != pl1_src_text[i1][i2])
            {
                if ('\n' == pl1_src_text[i1][i2])
                {
                    /* Remove newline-symbol from the source text */
                    continue;
                }

                if (' ' == pl1_src_text[i1][i2] &&
                    (' ' == prev_processed_symb ||
                     ';' == prev_processed_symb ||
                     ')' == prev_processed_symb ||
                     ':' == prev_processed_symb ||
                     '(' == prev_processed_symb ||
                     '+' == prev_processed_symb ||
                     '-' == prev_processed_symb ||
                     '=' == prev_processed_symb ||
                     '*' == prev_processed_symb ||
                     '!' == prev_processed_symb ||
                     '\''== prev_processed_symb))
                {
                    /* In accordance with this condition 'pl1_src_text[i1][i2]'
                     * which contains whitespace-symbol will be removed
                     * from the source text.
                     * Compressed text will not contain this whitespace-symbol
                     * after 'prev_processed_symb' which is being already 
                     * contained in compact source text
                     * For example:
                     * - "TEXT    TEXT2" source text will be processed to "TEXT TEXT2" compact source text
                     * - "TEXT: TEXT2" source text will be processed to "TEXT:TEXT2" compact source text
                     * - "TEXT(    TEXT2" source text will be processed to "TEXT(TEXT2" compact source text
                     *
                     * In this way accordance with this condition we remove excess
                     * whitespace-symbol after 'prev_processed_symb' symbol
                     */
                     continue; 
                }
                else if (('+'   == pl1_src_text[i1][i2] ||
                          '-'   == pl1_src_text[i1][i2] ||
                          '='   == pl1_src_text[i1][i2] ||
                          '('   == pl1_src_text[i1][i2] ||
                          ')'   == pl1_src_text[i1][i2] ||
                          '*'   == pl1_src_text[i1][i2] ||
                          '\''  == pl1_src_text[i1][i2] ||
                          '!'   == pl1_src_text[i1][i2]) && 
                           ' '  == prev_processed_symb)
                {
                    /* In accordance with this condition 'prev_processed_symb'
                     * which contains space-symbol will be removed
                     * from the compact source text, which already contains this space-symbol.
                     * For example:
                     * - "TEXT    TEXT2" source text will be processed to "TEXT TEXT2" compact source text
                     * - "TEXT + TEXT2" source text will be processed to "TEXT+ TEXT2" compact source text
                     * - "TEXT  (TEXT2" source text will be processed to "TEXT(TEXT2" compact source text
                     *
                     * In this way in accordance with this condition we remove excess
                     * whitespace-symbol before 'pl1_src_text[i1][i2]' symbol
                     */
                    i3--;
                }

                prev_processed_symb = pl1_src_text[i1][i2];
                compact_pl1_src_text[i3] = prev_processed_symb;
                i3++;
                if (i3 == compact_text_maxlen)
                {
                    return PLCMP_LEX_ANALYZER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW;
                }
            }
            else
            {
                break;
            }
        }
    }

    compact_pl1_src_text[i3] = '\0';

    return PLCMP_LEX_ANALYZER_SUCCESS;
}

/* Subroutine constructs error message by error code of lexical analyzer module */
char const* plcmp_lex_analyzer_errmsg_by_errcode(plcmp_lex_analyzer_error_code_t err_code)
{
    switch (err_code)
    {
        case PLCMP_LEX_ANALYZER_SUCCESS:
            return "No error occured";
        case PLCMP_LEX_ANALYZER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW:
            return "Overflow of the compact text buffer while lexical analysis was processing";
        default:
            return "Unknown error code for generating error message";
    }
}