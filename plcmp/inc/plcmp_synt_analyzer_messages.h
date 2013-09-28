/* encoding: UTF-8 */

#ifndef PLCMP_SYNT_ANALYZER_MESSAGES_H
#define PLCMP_SYNT_ANALYZER_MESSAGES_H

/* Subroutine constructs error message 
 * by error data of syntax analyzer module */
char* plcmp_synt_analyzer_messages_errmsg_by_errdata(
    plcmp_synt_analyzer_error_data_t const *err_data,
    char errmsg[]);

#endif /* PLCMP_SYNT_ANALYZER_MESSAGES_H */