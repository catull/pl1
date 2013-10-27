/* encoding: UTF-8 */

#ifndef ASMCMP_COMMON_H
#define ASMCMP_COMMON_H

#include <assert.h>
#include <stdint.h>
#include "utils.h"

#define ASMTEXT_MAX_LEN 50                      /* Max row-length of the source assembler text */ 
#define OBJTEXT_MAX_LEN ASMTEXT_MAX_LEN         /* Length of the object text */
#define NSYM 10                                 /* Size of the table of symbols */
#define NPOP 6                                  /* Size of the table of pseudo operations */
#define NOP 8                                   /* Size of the table of machine operations */

/* The limited length of the line of the assembler source text 
 * It has to be 81 (80 + 1) because 'fread' function reads 
 * newline-symbol too in the end of the each line of assembler code read */
#define LINELEN 81

#define TRUE 1
#define FALSE 0

#endif /* ASMCMP_COMMON_H */
