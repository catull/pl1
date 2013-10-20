#ifndef PLCMP_SYMBOLS_H
#define PLCMP_SYMBOLS_H

#include "plcmp_nterms.h"

#define SYM_X(s) SYM_##s,

#define SYM_TERMS               \
    SYM_X(A)                    \
    SYM_X(B)                    \
    SYM_X(C)                    \
    SYM_X(D)                    \
    SYM_X(E)                    \
    SYM_X(M)                    \
    SYM_X(P)                    \
    SYM_X(X)                    \
    SYM_X(0)                    \
    SYM_X(1)                    \
    SYM_X(2)                    \
    SYM_X(3)                    \
    SYM_X(4)                    \
    SYM_X(5)                    \
    SYM_X(6)                    \
    SYM_X(7)                    \
    SYM_X(8)                    \
    SYM_X(9)                    \
    SYM_X(PLUS)                 \
    SYM_X(MINUS)                \
    SYM_X(COLON)                \
    SYM_X(I)                    \
    SYM_X(R)                    \
    SYM_X(N)                    \
    SYM_X(O)                    \
    SYM_X(T)                    \
    SYM_X(S)                    \
    SYM_X(LEFT_PARENTHESIS)     \
    SYM_X(RIGHT_PARENTHESIS)    \
    SYM_X(SPACE)                \
    SYM_X(SEMICOLON)            \
    SYM_X(L)                    \
    SYM_X(F)                    \
    SYM_X(ASSIGN)               \
    SYM_X(H)                    \
    SYM_X(MUL)                  \
    SYM_X(SINGLE_QUOTE)         \
    SYM_X(EXCL_POINT)

typedef enum sym_e {
    SYM_NTERMS
#define SYM_NTERMS_COUNT (SYM_CON + 1)
    SYM_TERMS
#undef SYM_TERMS
#define SYM_COUNT (SYM_EXCL_POINT + 1)
#define SYM_TERMS_COUNT (SYM_COUNT - SYM_NTERMS_COUNT)
    SYM_END_RULE = SYM_COUNT,
    SYM_INCORRECT = SYM_END_RULE + 1
} sym_t;

#undef SYM_X

typedef enum sym_type_e {
    SYM_NON_TERM,
    SYM_TERM,
    SYM_UNDEFINED_TYPE
} sym_type_t;

#endif /* PLCMP_SYMBOLS_H */