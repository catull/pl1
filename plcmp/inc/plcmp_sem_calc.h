#ifndef PLCMP_SEM_CALC_H
#define PLCMP_SEM_CALC_H

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
extern char FORMT[MAXFORMT][9];
/* formatted array index */
extern int IFORMT;

int plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name, int *p_dst_index);

#endif /* PLCMP_SEM_CALC_H */