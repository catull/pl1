#ifndef PLCMP_SYNT_ANALYZER_H
#define PLCMP_SYNT_ANALYZER_H

#include "plcmp_common.h"

typedef enum plcmp_synt_analyzer_error_code_e {
    PLCMP_SYNT_ANALYZER_SUCCESS = 0,
    PLCMP_SYNT_ANALYZER_FAILURE = 1
} plcmp_synt_analyzer_error_code_t;

plcmp_synt_analyzer_error_code_t plcmp_synt_analyzer_syntax_analyzer(cel_t *p_goals,
                                                                     dst_t *p_goals_achieved);

#endif /* PLCMP_SYNT_ANALYZER_H */