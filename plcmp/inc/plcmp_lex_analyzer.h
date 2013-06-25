/* encoding: UTF-8 */

#ifndef PLCMP_LEX_ANALYZER_H
#define PLCMP_LEX_ANALYZER_H

#include "plcmp_common.h"

typedef enum plcmp_lex_analyzer_error_code_e {
    PLCMP_LEX_ANALYZER_SUCCESS = 0,
    PLCMP_LEX_ANALYZER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW
} plcmp_lex_analyzer_error_code_t;

/* Subroutine of primitive lexical analyzer 
 * It compresses the source text by removing all excess spaces and newline-symbols */
enum plcmp_lex_analyzer_error_code_e plcmp_lex_analyzer_compress_src_text(char compact_pl1_src_text[],
                                                                          size_t compact_text_maxlen,
                                                                          char pl1_src_text[][LINELEN],
                                                                          size_t pl1_src_text_len);

char const* plcmp_lex_analyzer_errmsg_by_errcode(plcmp_lex_analyzer_error_code_t err_code);

#endif /* PLCMP_LEX_ANALYZER_H */
