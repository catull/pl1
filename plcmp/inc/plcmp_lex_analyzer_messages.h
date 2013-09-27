/* encoding: UTF-8 */

#ifndef PLCMP_LEX_ANALYZER_MESSAGES_H
#define PLCMP_LEX_ANALYZER_MESSAGES_H

#include "plcmp_lex_analyzer.h"

char* plcmp_lex_analyzer_messages_errmsg_by_errdata(
    plcmp_lex_analyzer_error_data_t const *err_data,
    char *errmsg);

#endif /* PLCMP_LEX_ANALYZER_MESSAGES_H */