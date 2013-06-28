/* encoding: UTF-8 */

#include <curses.h>
#include <stdint.h>
#include <stdlib.h>

#include "absload_common.h"
#include "absload_machine_oper.h"

static enum absload_machine_oper_error_code_e FRR(void);
static enum absload_machine_oper_error_code_e FRX(void);
static enum absload_machine_oper_error_code_e FSS(void);

machine_operations_table_t T_MOP[NOP] = {
    {{'B', 'A', 'L', 'R', ' '}, 0x05, OPER_RR_LEN, FRR},
    {{'B', 'C', 'R', ' ', ' '}, 0x07, OPER_RR_LEN, FRR},
    {{'S', 'T', ' ', ' ', ' '}, 0x50, OPER_RX_LEN, FRX},
    {{'L', ' ', ' ', ' ', ' '}, 0x58, OPER_RX_LEN, FRX},
    {{'A', ' ', ' ', ' ', ' '}, 0x5A, OPER_RX_LEN, FRX},
    {{'S', ' ', ' ', ' ', ' '}, 0x5B, OPER_RX_LEN, FRX},
    {{'L', 'R', ' ', ' ', ' '}, 0x18, OPER_RR_LEN, FRR},
    {{'L', 'A', ' ', ' ', ' '}, 0x41, OPER_RX_LEN, FRX},
    {{'A', 'R', ' ', ' ', ' '}, 0x1A, OPER_RR_LEN, FRR},
    {{'M', 'V', 'C', ' ', ' '}, 0xD2, OPER_SS_LEN, FSS}
};

extern unsigned char OBLZ[DOBLZ];

extern unsigned long I;
extern unsigned char INST[6];
extern WINDOW *wblue, *wgreen, *wred, *wcyan, *wmargenta;
extern int R1, R2;
extern int B, X, D, L;
extern unsigned long ADDR;
extern unsigned long VR[16];
extern int B1, B2, D1, D2;
extern unsigned long ADDR1, ADDR2;

extern unsigned long ARG;

extern int CUR_IND;                        /* индекс масс.обл.загр., соотв.текущ.ком-де прогр */
extern int BAS_IND;                        /* индекс масс.обл.загр., соотв.первой ком-ды прог */


static enum absload_machine_oper_error_code_e FRR(void)
{
    /* RR operation has the next format on the
     * IBM 370 side in accordance with Unified Agreement:
     * 
     *    opcode   R1   R2
     * |xxxx xxxx|xxxx|xxxx|
     *
     * - opcode - operation code (8 bits)
     * - R1R2 - byte containing numbers of 
     * the first destination register (elder tetrad, 4 bits)
     * and the second source register (lower tetrad, 4 bits) => total 8 bits
     *
     * Operation's length is 2 bytes (16 bits)
     */
    int i;
  
    for (i = 0; i < NOP; i++)
    {
        /*               opcode
         * INST[0] => |xxxx xxxx|
         */
        if (INST[0] == T_MOP[i].CODOP)
        {
            int j;

            waddstr(wgreen, "      ");
            for (j = 0; j < 5; j++)
            {
                waddch(wgreen, T_MOP[i].MNCOP[j]);
            }
            waddstr(wgreen, " ");

            /*               R1   R2
             * INST[1] ==> |xxxx|xxxx|
             */

            /* get R1 using bitwise right shift (get elder tetrad of INST[1])*/
            R1 = INST[1] >> 4;
            wprintw(wgreen, "%1d, ", R1);

            /* get R2 using bitwise multiply on 0x0F (get lower tetrad of INST[1])*/
            R2 = INST[1] & 0x0F;
            wprintw(wgreen, "%1d\n", R2);

            break;
        }
    }

    return ABSLOAD_MACHINE_OPER_SUCCESS; 
}


static enum absload_machine_oper_error_code_e FRX(void)
{
    /* RX operation has the next format on the 
     * IBM 370 side in accordance with Unified Agreement:
     * 
     *    opcode   R1   X2   B2        D2
     * |xxxx xxxx|xxxx|xxxx|xxxx|xxxx xxxx xxxx|
     *
     * - opcode - operation code (8 bits)
     *
     * - R1X2 - byte containing number of 
     * the destination register (elder tetrad, 4 bits) as the first operand
     * and index value (lower tetrad, 4 bits) of the second operand => total 8 bits
     *
     * - B2D2 - double-byte containing number of
     * the base register (4 bits) of the second operand
     * and 12-bits length value of the address' displacement (12 bits)
     * of the second operand => total 16 bits
     *
     * Operation's length is 4 bytes (32 bits)
     */
    int i;
  
    for (i = 0; i < NOP; i++)
    {
        /*               opcode
         * INST[0] => |xxxx xxxx|
         */
        if (INST[0] == T_MOP[i].CODOP)
        {
            int j;

            waddstr(wgreen, "      ");
            for (j = 0; j < 5; j++)
            {
                waddch(wgreen, T_MOP[i].MNCOP[j]);
            }
            waddstr(wgreen, " ");

            /*              R1   X2
             * INST[1] => |xxxx|xxxx|
             */
          
            /* get R1 using bitwise right shift (get elder tetrad of INST[1]) */
            R1 = INST[1] >> 4;
            wprintw(wgreen, "%.1d, ", R1);
          
            /*
             *              B2  D2(4) (4 elder bits of D2 value)
             * INST[2] => |xxxx|xxxx| 
             *               D2(8)    (8 lower bits of D2 value)
             * INST[3] => |xxxx xxxx|
             */

            /* Make D value: transmit 4 elder bits to the place before 8 lower bits */
            D = (((int)INST[2] << 8) & 0x0F00) + INST[3];
            wprintw(wgreen, "X'%.3X'(", D);
          
            /* get X using multyply on 0x0F (get lower tetrad of INST[1]) */
            X = INST[1] & 0x0F;
            wprintw(wgreen, "%1d, ", X);
          
            /* get B using bitwise right shift (get elder tetrad of INST[2]) */
            B = INST[2] >> 4;
            wprintw(wgreen, "%1d)", B);
          
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
    /* SS operation has the next format on the 
     * IBM 370 side in accordance with Unified Agreement:
     *
     *    opcode      L      B1        D2        B2        D2
     * |xxxx xxxx|xxxx xxxx|xxxx|xxxx xxxx xxxx|xxxx|xxxx xxxx xxxx|
     *
     * - opcode - operation code (8 bits)
     *
     * - L - byte containing length of the operands (8 bits)
     *
     * - B1D1 - double-byte containing number of 
     * the base register (elder tetrad, 4 bits) of the first operand
     * and 12-bit length value of the address' displacement 
     * of the first operand (12 bits) => total 16 bits
     *
     * - B2D2 - double-byte containing number of
     * the base register (elder tetrad, 4 bits) of the second operand
     * and 12-bit length value of the address' displacement 
     * of the second operand (12 bits) => total 16 bits
     *
     * Operation's length is 6 bytes (48 bits)
     */

    int i;
  
    for (i = 0; i < NOP; i++)
    {
        /*               opcode
         * INST[0] => |xxxx xxxx|
         */
        if (INST[0] == T_MOP[i].CODOP)
        {
            int j;

            waddstr(wgreen, "      ");
            for (j = 0; j < 5; j++)
            {
                waddch(wgreen, T_MOP[i].MNCOP[j]);
            }
            waddstr(wgreen, " ");

            /*
             *              B1  D1(4) (4 elder bits of D1 value)
             * INST[2] => |xxxx|xxxx| 
             *               D1(8)    (8 lower bits of D1 value)
             * INST[3] => |xxxx xxxx|
             */

            /* Make D1 value: transmit 4 elder bits to the place before 8 lower bits */
            D1 = (((int)INST[2] << 8) & 0x0F00) + INST[3];
            wprintw(wgreen, "X'%.3X'(", D1);

            /*                 L
             * INST[1] => |xxxx xxxx|
             *
             * IBM370 architecture demands 
             * to specify in SS operations 
             * length of the operands one less. (see assembler compiler)
             * 
             * Therefore we restore real value of length
             * of the operands by adding one 
             */
            L = INST[1] + 0x1;
            wprintw(wgreen, "%.1d, ", L);
          
            B1 = INST[2] >> 4;
            wprintw(wgreen, "%1d),", B1);

            ADDR1 = VR[B1] + D1;

            /*
             *              B2  D2(4) (4 elder bits of D2 value)
             * INST[4] => |xxxx|xxxx| 
             *               D2(8)    (8 lower bits of D2 value)
             * INST[5] => |xxxx xxxx|
             */

            /* Make D1 value: transmit 4 elder bits to the place before 8 lower bits */
            D2 = (((int)INST[4] << 8) & 0x0F00) + INST[5];
            wprintw(wgreen, "X'%.3X'(", D2);
          
            B2 = INST[4] >> 4;
            wprintw(wgreen, "%1d)", B2);
          
            ADDR2 = VR[B2] + D2;
            wprintw(wgreen, " dst %.06lX src %.06lX \n", ADDR1, ADDR2);

            if (ADDR2 % 0x4 != 0)
            {
                return ABSLOAD_MACHINE_OPER_WRONG_ADDR_ALIGMENT_ERROR;
            }

            break;
        }
    }

    return ABSLOAD_MACHINE_OPER_SUCCESS;
}

/* Subroutine realizes semantic of the machine operation 'BALR' */
int P_BALR(void)
{
    if (0 != R2)
    {
        I = VR[R2];
    }

    if (0 != R1)
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

    VR[R1] += ARG;

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

    VR[R1] -= ARG;

    return 0;
}

/* Subroutine realizes semantic of the machine operation 'LER' */
int P_LR(void)
{
    VR[R1] = VR[R2];
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'P_LA' */
int P_LA(void)
{
    VR[R1] = VR[B] + VR[X] + D;
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'AR' */
int P_AR(void)
{
    VR[R1] += VR[R2];
    return 0;
}

/* Subroutine realizes semantic of the machine operation 'MVC' */
int P_MVC(void)
{
    int sm1, sm2, ind;

    ADDR1 = VR[B1] + D1;
    ADDR2 = VR[B2] + D2;

    sm1 = (int)(ADDR1 - I);
    sm2 = (int)(ADDR2 - I);

    for (ind = 0; ind < L; ind++)
    {
        OBLZ[BAS_IND + CUR_IND + sm1 + ind] = OBLZ[BAS_IND + CUR_IND + sm2 + ind];
    }

    return 0;
}