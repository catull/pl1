/* encoding: UTF-8 */

#ifndef PLCMP_COMMON_H
#define PLCMP_COMMON_H

/* The limited size of the source text */
#define MAXNISXTXT 50
/* The limited size of the table of the syntax rules */
#define NSINT 233
/* The limited size of the stack of the goals */
#define NCEL 20
/* The limited size of the stack of the goals achieved */
#define NDST 500
/* The limited size of the string of compact text */
#define NSTROKA 200
/* The limited size of the output text */
#define MAXLTXT 50
/* The limited size of formatted interpreted fragment of the source text */
#define MAXFORMT 30
/* The limited size of the tables of names and labels */
#define NSYM 100
/* The limited length of the line of 
 * the source text (80 symbols +1 symbol for endline)*/
#define LINELEN 81

typedef unsigned int index_t;
typedef _Bool bool_t;

#define INCORRECT_INDEX (~(index_t)0)

#endif