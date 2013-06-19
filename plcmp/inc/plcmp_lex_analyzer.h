/* encoding: UTF-8 */

#ifndef PLCMP_LEX_ANALYZER_H
#define PLCMP_LEX_ANALYZER_H

#include "plcmp_common.h"

/* Place of storage of compact source text */
extern char compact_pl1_src_text[NSTROKA];

/* Subroutine of primitive lexical analyzer 
 * It compresses the source text by removing all excess spaces */
void plcmp_lex_analyzer_compress_src_text(char *p_compressed_text,
                                          char const p_pl1_src_text[MAXNISXTXT][LINELEN],
                                          size_t pl1_src_text_len);

#endif /* PLCMP_LEX_ANALYZER_H */