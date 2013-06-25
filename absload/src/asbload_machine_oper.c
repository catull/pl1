/* encoding: UTF-8 */

#include <curses.h>

#include "absload_common.h"
#include "absload_machine_oper.h"

static enum absload_machine_oper_error_code_e FRR(void);
static enum absload_machine_oper_error_code_e FRX(void);
static enum absload_machine_oper_error_code_e FSS(void);

machine_operations_table_t T_MOP[NOP] = {
    {{'B', 'A', 'L', 'R', ' '}, '\x05', OPER_RR_LEN, FRR},
    {{'B', 'C', 'R', ' ', ' '}, '\x07', OPER_RR_LEN, FRR},
    {{'S', 'T', ' ', ' ', ' '}, '\x50', OPER_RX_LEN, FRX},
    {{'L', ' ', ' ', ' ', ' '}, '\x58', OPER_RX_LEN, FRX},
    {{'A', ' ', ' ', ' ', ' '}, '\x5A', OPER_RX_LEN, FRX},
    {{'S', ' ', ' ', ' ', ' '}, '\x5B', OPER_RX_LEN, FRX},
    {{'L', 'E', 'R', ' ', ' '}, '\x38', OPER_RR_LEN, FRR},
    {{'L', 'A', ' ', ' ', ' '}, '\x41', OPER_RX_LEN, FRX},
    {{'A', 'R', ' ', ' ', ' '}, '\x1A', OPER_RR_LEN, FRR},
    {{'M', 'V', 'C', ' ', ' '}, '\xD2', OPER_SS_LEN, FSS}
};

extern unsigned char INST[6];
extern WINDOW *wgreen;
extern int R1, R2, B, X, D;
extern unsigned long ADDR;
extern unsigned long VR[16];


static enum absload_machine_oper_error_code_e FRR(void)
{
    int i;
  
    for (i = 0; i < NOP; i++)
    {
        if (INST[0] == T_MOP[i].CODOP)
        {
            int j;

            waddstr(wgreen, "      ");
            for (j = 0; j < 5; j++)
            {
                waddch(wgreen, T_MOP[i].MNCOP[j]);
            }
            waddstr(wgreen, " "); 
      
            j = (INST[1] - (INST[1] % 0x10)) / 0x10;
            R1 = j;
            wprintw(wgreen, "%1d, ", j);

            j = INST[1] % 0x10;
            R2 = j;
            wprintw(wgreen, "%1d\n", j);

            break;
        }
    }

    return ABSLOAD_MACHINE_OPER_SUCCESS; 
}


static enum absload_machine_oper_error_code_e FRX(void)
{
    int i, j;
  
    for (i = 0; i < NOP; i++)
    {
        if (INST[0] == T_MOP[i].CODOP)
        {
            waddstr(wgreen, "  ");
            for (j = 0; j < 5; j++)
            {
                waddch(wgreen, T_MOP[i].MNCOP[j]);
            }
            waddstr(wgreen, " ");
          
            j = INST[1] >> 4;
            R1 = j;
            wprintw(wgreen, "%.1d, ", j);
          
            j = INST[2] % 16;
            j = j * 256 + INST[3];
            D = j;
            wprintw(wgreen, "X'%.3X'(", j);
          
            j = INST[1] % 16;
            X = j;
            wprintw(wgreen, "%1d, ", j);
          
            j = INST[2] >> 4;
            B = j;
            wprintw(wgreen, "%1d)", j);
          
            ADDR = VR[B] + VR[X] + D;
            wprintw(wgreen,"        %.06lX       \n", ADDR);
            if (ADDR % 4 != 0)
            {
                return ABSLOAD_MACHINE_OPER_WRONG_ADDR_ALIGMENT_ERROR;
            }
            break;
        }
    }

    return ABSLOAD_MACHINE_OPER_SUCCESS;
}

static enum absload_machine_oper_error_code_e FSS(void)
{
    return ABSLOAD_MACHINE_OPER_SUCCESS;
}