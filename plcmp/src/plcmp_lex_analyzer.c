#include <stddef.h>

#include "plcmp_common.h"
#include "plcmp_lex_analyzer.h"

/* Place of storage of compact source text */
char compact_pl1_src_text[NSTROKA];

/* Subroutine of primitive lexical analyzer 
 * It compresses the source text by removing all excess spaces */
void plcmp_lex_analyzer_compress_src_text(char *p_compact_pl1_src_text,
                                          char const pl1_src_text[MAXNISXTXT][LINELEN],
                                          size_t pl1_src_text_len)
{
    int i1, i3 = 0;

    /* Last processed symbol in the compact source PL1-text */
    char prev_processed_symb = '\0';

    for (i1 = 0; i1 < pl1_src_text_len; i1++)
    {
        int i2;
        for (i2 = 0; i2 < LINELEN; i2++)
        {
            if ('\0' != pl1_src_text[i1][i2])
            {
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
                     '!' == prev_processed_symb))
                {
                    /* In accordance with this condition 'pl1_src_text[i1][i2]'
                     * which contains space-symbol will be removed
                     * from the source text.
                     * Compressed text will not contain this space-symbol
                     * after 'prev_processed_symb' which already 
                     * contained in compressed source text
                     * For example:
                     * - "TEXT    TEXT2" source text will be processed to "TEXT TEXT2" compressed source text
                     * - "TEXT: TEXT2" source text will be processed to "TEXT:TEXT2" compressed source text
                     * - "TEXT(    TEXT2" source text will be processed to "TEXT(TEXT2" compressed source text
                     *
                     * In this way accordance with this condition we remove excess
                     * space-symbol after 'prev_processed_symb' symbol
                     */
                     continue; 
                }
                else if (('+' == pl1_src_text[i1][i2] ||
                          '-' == pl1_src_text[i1][i2] ||
                          '=' == pl1_src_text[i1][i2] ||
                          '(' == pl1_src_text[i1][i2] ||
                          ')' == pl1_src_text[i1][i2] ||
                          '*' == pl1_src_text[i1][i2] ||
                          '!' == pl1_src_text[i1][i2]) && 
                           ' ' == prev_processed_symb)
                {
                    /* In accordance with this condition 'prev_processed_symb'
                     * which contains space-symbol will be removed
                     * from the compressed source text, which already contains this space-symbol.
                     * For example:
                     * - "TEXT    TEXT2" source text will be processed to "TEXT TEXT2" compressed source text
                     * - "TEXT + TEXT2" source text will be processed to "TEXT+ TEXT2" compressed source text
                     * - "TEXT  (TEXT2" source text will be processed to "TEXT(TEXT2" compressed source text
                     *
                     * In this way in accordance with this condition we remove excess
                     * space-symbol before 'pl1_src_text[i1][i2]' symbol
                     */
                    i3--;
                }

                prev_processed_symb = pl1_src_text[i1][i2];
                p_compact_pl1_src_text[i3] = prev_processed_symb;
                i3++;

            }
            else
            {
                break;
            }
        }
    }

    p_compact_pl1_src_text[i3] = '\0';
    #if 0
    printf("%s\n", p_compact_pl1_src_text);
    exit(0);
    #endif
}