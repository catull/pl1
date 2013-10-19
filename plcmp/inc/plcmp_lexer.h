/* encoding: UTF-8 */

#ifndef PLCMP_LEXER_H
#define PLCMP_LEXER_H

#include "plcmp_utils.h"

/* Enumerate defines error codes of lexical analyzer module */
typedef enum plcmp_lexer_error_code_e {
    PLCMP_LEXER_SUCCESS = 0,
    PLCMP_LEXER_COMPACT_SRC_TEXT_BUFFER_OVERFLOW
} plcmp_lexer_error_code_t;

/* Structure defines content of the error data of lexical analyzer module */
typedef struct plcmp_lexer_error_data_s {
    plcmp_lexer_error_code_t err_code;
} plcmp_lexer_error_data_t;

/* Subroutine constructs error message 
 * by error code of lexical analyzer module */
char* plcmp_lexer_errmsg_by_errdata(plcmp_lexer_error_data_t const *err_data,
                                    char errmsg[]);

/* Subroutine of primitive lexical analyzer 
 * It compresses the source text by removing 
 * all excess spaces and newline-symbols */
struct plcmp_lexer_error_data_s plcmp_lexer_compress_src_text(
    char compact_pl1_src_text[],
    size_t compact_text_maxlen,
    char pl1_src_text[][LINELEN],
    size_t pl1_src_text_len);

#endif /* PLCMP_LEXER_H */
