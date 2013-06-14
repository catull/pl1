#include "plcmp_common.h"
#include "plcmp_tables.h"

/*
 * ****************************
 * Database of the PL1-compiler
 * ****************************
 */

/*
 * Block of declaration of the working variables
 */

/* cycles counters */
int I4;

int I; /* current index in compact text */
int J; /* current index in the table of grammar rules */
int K; /* current index in the stack of goals */
int L; /* current index in the stack of achieved goals */