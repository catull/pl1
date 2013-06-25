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

extern unsigned char OBLZ[DOBLZ];

extern unsigned long I;
extern unsigned char INST[6];
extern WINDOW *wblue, *wgreen, *wred, *wcyan, *wmargenta;
extern int R1, R2;
extern int B, X, D;
extern unsigned long ADDR;
extern unsigned long VR[16];

extern unsigned long ARG;

extern int CUR_IND;                        /* индекс масс.обл.загр., соотв.текущ.ком-де прогр */
extern int BAS_IND;                        /* индекс масс.обл.загр., соотв.первой ком-ды прог */


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
          
            j = INST[1] >> 0x4;
            R1 = j;
            wprintw(wgreen, "%.1d, ", j);
          
            j = INST[2] % 0x10;
            j = j * 256 + INST[3];
            D = j;
            wprintw(wgreen, "X'%.3X'(", j);
          
            j = INST[1] % 0x10;
            X = j;
            wprintw(wgreen, "%1d, ", j);
          
            j = INST[2] >> 0x4;
            B = j;
            wprintw(wgreen, "%1d)", j);
          
            ADDR = VR[B] + VR[X] + D;
            wprintw(wgreen,"        %.06lX       \n", ADDR);
            if (ADDR % 0x4 != 0)
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

/* Subroutine realizes semantic of the machine operation 'BALR' */
int P_BALR(void)
{
    if (R2 != 0)
    {
        I = VR[R2];
    }

    if (R1 != 0)
    {
        VR[R1] = I;
    }
    
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'BCR' with mask value '15' */
int P_BCR(void)
{
    int ret = 1;
  
    if (15 == R1)
    {
        ret = 0;
        if (!(0 == VR[R2] || 0 == R2))
        {
            I = VR[R2];
        }
        else
        {
            if (0 != R2)
            {
                waddstr(wcyan, "jump to address = 0 or finishing trace of the program after key press");
                wrefresh(wcyan);
                ret = 1;
            }
        }
    }  

    return ret;
}

/* Subroutine realizes semantic of the machine operation 'ST' */
int P_ST(void)
{
    int sm, i;
    char bytes[4];

    ADDR = VR[B] + VR[X] + D;   /* вычисление абс. адреса */
    sm = (int)(ADDR - I);       /* вычисление смещения */

    /* преобразование содержим. РОН, использованного в качестве первого оп-да 
     * к виду, принятому в ЕС ЭВМ */
    bytes[0] = (VR[R1] - VR[R1] % 0x1000000L) / 0x1000000L;
    bytes[1] = ((VR[R1] - VR[R1] % 0x10000L) / 0x10000L) % 0x100; 
    bytes[2] = ((VR[R1] % 0x10000L) - ((VR[R1] % 0x10000L) % 0x100)) / 0x100;
    bytes[3] = (VR[R1] % 0x10000L) % 0x100;

    /* запись преобразованого значения по адресу 2-го операнда */
    for (i = 0; i < 4; i++)
    {
        OBLZ[BAS_IND + CUR_IND + sm + i] = bytes[i];
    }

    return 0;
}

/* Subroutine realizes semantic of the machine operation 'L' */
int P_L(void)
{
    int sm;

    ADDR = VR[B] + VR[X] + D;  /* вычисление абс.адреса */
    sm = (int)(ADDR - I);      /* вычисление смещения */

    /* преобразование содержим. второго операнда, использованного в качестве первого оп-да 
     * к виду, принятому в ЕС ЭВМ. Запись в РОН, использованного в качестве первого операнда */
    VR[R1] = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L +
        OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L +
        OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +
        OBLZ[BAS_IND + CUR_IND + sm + 3];

    return 0;
 }

/* Subroutine realizes semantic of the machine operation 'A' */
int P_A(void)
{
    int sm;

    ADDR = VR[B] + VR[X] + D;   /* вычисление абс.адреса */
    sm = (int)(ADDR - I);       /* вычисление смещения */

    /* формирование содержимого второго операнда в соглашениях ЕС ЭВМ 
     * и сложение с 1-м операндом */
    ARG = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L +
    OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L +
    OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +
    OBLZ[BAS_IND + CUR_IND + sm + 3];

    VR[R1] = VR[R1] + ARG;

    return 0;
}

/* Subroutine realizes semantic of the machine operation 'S' */
int P_S(void)
{
    int sm;

    ADDR = VR[B] + VR[X] + D;   /* вычисление абс.адреса */
    sm = (int)(ADDR - I);       /* вычисление смещения   */

    /* формирование содержимого второго операнда в соглашениях ЕС ЭВМ 
     * и вычитание из 1-го операнда */
    ARG = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L + 
    OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L + 
    OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 + 
    OBLZ[BAS_IND + CUR_IND + sm + 3];

    VR[R1] = VR[R1] - ARG;

    return 0;
}

/* Subroutine realizes semantic of the machine operation 'LER' */
int P_LER(void)
{
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'P_LA' */
int P_LA(void)
{
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'AR' */
int P_AR(void)
{
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'MVC' */
int P_MVC(void)
{
    return 0;
}