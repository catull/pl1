#include <stddef.h>

#include "plcmp_common.h"
#include "plcmp_lex_analyzer.h"

/* Place of storage of compact source text */
char compact_src_text[NSTROKA];

/* Subroutine of primitive lexical analyzer 
 * It compresses the source text by removing all excess spaces */
void plcmp_lex_analyzer_compress_src_text(char *p_compact_src_text,
                                          char p_src_text[MAXNISXTXT][80],
                                          size_t src_text_lines)
{
    int i1, i2, i3 = 0;

    /* Last processed symbol in the compact source PL1-text */
    char prev_processed_symb = '\0';

    for (i1 = 0; i1 < src_text_lines; i1++)
    {
        for (i2 = 0; i2 < 80; i2++)
        {
            if ('\0' != p_src_text[i1][i2])
            {
                if (' ' == p_src_text[i1][i2] &&
                    ('(' == prev_processed_symb ||
                     ' ' == prev_processed_symb ||
                     ';' == prev_processed_symb ||
                     ')' == prev_processed_symb ||
                     ':' == prev_processed_symb))
                {
                    prev_processed_symb = p_src_text[i1][i2];
                    goto L2;
                }

                if ((' ' == p_src_text[i1][i2] ||
                     '+' == p_src_text[i1][i2] ||
                     '-' == p_src_text[i1][i2] ||
                     '=' == p_src_text[i1][i2] ||
                     '(' == p_src_text[i1][i2] ||
                     ')' == p_src_text[i1][i2]) && 
                      '*' == prev_processed_symb)
                {
                    i3--;
                    goto L1;
                }

                if (' ' == p_src_text[i1][i2] && 
                    ('+' == prev_processed_symb ||
                     '-' == prev_processed_symb ||
                     '=' == prev_processed_symb ||
                     '*' == prev_processed_symb))
                {
                    goto L2;
                }

                L1:

                prev_processed_symb = p_src_text[i1][i2];
                p_compact_src_text[i3] = prev_processed_symb;
                i3++;

                L2:

                continue;
            }
            else
            {
                break;
            }
        }
    }

    p_compact_src_text[i3] = '\0';
}