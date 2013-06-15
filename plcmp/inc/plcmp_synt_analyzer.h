#ifndef PLCMP_SYNT_ANALYZER_H
#define PLCMP_SYNT_ANALYZER_H

typedef enum plcmp_synt_analyzer_error_code_e {
    PLCMP_SYNT_ANALYZER_SUCCESS = 0,
    PLCMP_SYNT_ANALYZER_FAILURE = 1
} plcmp_synt_analyzer_error_code_t;

int plcmp_synt_analyzer_syntax_analyzer(void);

#endif /* PLCMP_SYNT_ANALYZER_H */