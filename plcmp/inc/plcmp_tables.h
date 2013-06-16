#ifndef PLCMP_TABLES_H
#define PLCMP_TABLES_H

#include "plcmp_common.h"

typedef struct sint_s {
    int POSL;
    int PRED;
    char DER[4];
    int ALT;
} sint_t;

typedef struct dst_s {
    char DST1[4];
    int DST2;
    int DST3;
    int DST4;
    int DST5;
} dst_t;

typedef struct cel_s {
    char CEL1[4];
    int CEL2;
    int CEL3;
} cel_t;

typedef struct vxod_s {
    char SYM[4];
    int VX;
    char TYP;
} vxod_t;

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

/* Table of the syntax rules that is written in the form 
 * of recognition, grouped in "bushes" and represented 
 * as bidirectional list with alternate branching */
extern sint_t SINT[NSINT];

/* Table that is used as a stack of achievements */
extern dst_t DST[NDST];

/* Table that is used as a stack of goals */
extern cel_t CEL[NCEL];

/* Table of inputs in "bushes" (roots) of the grammar rules.
 * This table contains root symbols type (terminal or non-terminal property) */
extern vxod_t VXOD[NVXOD];

extern char TPR[NVXOD][NNETRM];

/* таблица имен меток и   */
/* переменных, заполняемая*/
/* на первом проходе се-  */
/* мантического вычисления*/
/* и используемая на вто- */
/* ром проходе семантичес-*/
/* кого вычисления        */
extern sym_t SYM[NSYM];

/* текущий индекс таблицы имен */
extern int ISYM;

/* Function finds necessary string with sought-for symbol in the VXOD-table 
 * and returns string-index of VXOD-table.
 * 
 * @param1: const char *p_str_symbol
 * C-string containing name of sought-for symbol
 *
 * @param2: unsigned int symbol_str_len
 * Length of the string of sought-for symbol
 */
unsigned int numb(const char *p_str_symbol, unsigned int symbol_str_len);

#endif /* PLCMP_TABLES_H */

