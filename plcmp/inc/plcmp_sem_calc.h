#ifndef PLCMP_SEM_CALC_H
#define PLCMP_SEM_CALC_H

/* This union is type of assembler card
 * It is template to generate the output file record by IBM 370 assembler */
union assembler_card_un {
    char BUFCARD[80];
    struct
    {
        char METKA[8];
        char PROB1;
        char OPERAC[5];
        char PROB2;
        char OPERAND[12];
        char PROB3;
        char COMM[52];
    } _BUFCARD;
};

extern union assembler_card_un assembler_card;

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
extern char FORMT[MAXFORMT][9];
/* formatted array index */
extern int IFORMT;

int plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name, int *p_dst_index);

#endif /* PLCMP_SEM_CALC_H */