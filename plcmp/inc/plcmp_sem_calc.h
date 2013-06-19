/* encoding: UTF-8 */

#ifndef PLCMP_SEM_CALC_H
#define PLCMP_SEM_CALC_H

/* Array for formatted (a sequence of 9-th positional lines-tokens)
 * representation interpreted fragment for compact source text */
extern char FORMT[MAXFORMT][9];
/* Formatted array index */
extern int IFORMT;

/* This struct is type of the table of labels' names of variables
 * which is being filled on the first process of semantic calculation
 * and is being used on the second process of semantic calculation 
 */
typedef struct sym_s
{
    char NAME[8];
    char TYPE;
    char RAZR[5];
    char INIT[50];
} sym_t;

int plcmp_sem_calc_gen_asm_code(char const *p_asm_fp_name,
                                dst_t const *p_goals_achieved,
                                int *p_dst_index);

#endif /* PLCMP_SEM_CALC_H */