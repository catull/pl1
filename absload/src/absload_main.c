/* encoding: UTF-8 */
/* russian letters: KOI8 (Cyrillic) */

/* for compilation use key '-lncurses'
 * for example, gcc absloadm.c -o absloadm -lncurses */

/* if you use mine pl1-project, key has already added for compilation;
 * simply start command 'make absload' from the root of the project */

#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>

#include "absload_common.h"
#include "absload_main.h"


char NFIL[30] = "\x0";
int IOBJC = 0;                                 /*инд.вакантн.стр. OBJCARD*/
char OBJCARD[NOBJ][80];                          /*масс.хранен.об'ектн.карт*/
int ISPIS = 0;                                 /*инд.вакантн.стр. SPISOK */
char SPISOK[NSPIS][80];                         /*масс.хранен.списка прогр*/

WINDOW *wblue, *wgreen, *wred, *wcyan, *wmargenta;

/* Structure describes content of the TXT-card */
typedef struct txt_card_buffer_s {
    unsigned char POLE1;
    unsigned char POLE2[3];
    unsigned char POLE3;
    unsigned char ADOP[3];
    unsigned char POLE5[2];
    unsigned char DLNOP[2];
    unsigned char POLE7[2];
    unsigned char POLE71[2];
    unsigned char OPER[56];
    unsigned char POLE9[8];
} txt_card_buffer_t;

txt_card_buffer_t TXT;

unsigned char INST[6];                           /*массив, содерж. обрабат.*/
                          /*команду                 */


/*..........................................................................*/
                          /*п р о т о т и п  обращ.к*/
int FRR();                                        /*подпр.обр.опер.RR-форм. */

/*..........................................................................*/

                          /*п р о т о т и п  обращ.к*/
int FRX();                                        /*подпр.обр.опер.RX-форм. */
/*..........................................................................*/


int X1 = 1;                                       /* инициализация коорд.   */
int Y1 = 15;                                      /* на экране              */
int R1,                                           /*номер 1-го регистра-опе-*/
                          /*ранда в форматах RR и RX*/
    R2,                                           /*номер 2-го регистра-опе-*/
                          /*ранда в формате RX      */
    D,                                            /*смещение в формате RX   */
    X,                                            /*номер индексн. регистра */
                          /*в формате RX            */
    B;                                            /*номер базового регистра */
                          /*в формате RX            */
unsigned long I,                                  /*счетчик адр.тек.ком-ды  */
          BAS_ADDR,                           /*адрес начала обл.загруз.*/
          I1,ADDR,ARG,VS;                     /*вспомогательные перем.  */
unsigned long VR[16],                             /*массив,содерж.знач.рег. */
          LIGHTPTR;                           /*адрес начала обл.отсвет.*/


int x,y,i,j,k,kk;                                 /*рабочие переменные      */

int CUR_IND;                                      /*индекс масс.обл.загр.,  */
                          /*соотв.текущ.ком-де прогр*/
int BAS_IND;                                      /*индекс масс.обл.загр.,  */
                          /*соотв.первой ком-ды прог*/

union U1                                        /*постоянная часть шабло- */
{                                              /*на отсветки регистров на*/
    struct                                        /*экране консоли          */
    {
        char NOMREG  [ 3];
        char ZNEQUEL [1];
    } REGS_ASC [16];
    char BUFR [16][4];
}R_ASC;

union u2 {                                       /*шаблон для расчета      */
                                              /*элементов абсолютного   */
    struct {
        unsigned int SMESH;
        unsigned int SEGM;
    } VAL_P;
    unsigned char *P_OBLZ ;
} POINT;

unsigned char OBLZ [DOBLZ] ;                    /*область загрузки трас-  */
                          /*сируемой программы      */
/*
***** ТАБЛИЦА машинных операций
*/

struct TMOP {                                      /*структ.стр.табл.маш.опер*/
    unsigned char MNCOP[5];                       /*мнемокод операции       */
    unsigned char CODOP;                       /*машинный код операции   */
    unsigned char DLOP;                       /*длина операции в байтах */
    int (*BXPROG)(void);                       /*указатель на подпр.обраб*/
} T_MOP [NOP] = {                               /*об'явление табл.маш.опер*/
    {{'B' , 'A' , 'L' , 'R' , ' '} , '\x05', 2 , FRR},/*инициализация           */
    {{'B' , 'C' , 'R' , ' ' , ' '} , '\x07', 2 , FRR}, /*строк                   */
    {{'S' , 'T' , ' ' , ' ' , ' '} , '\x50', 4 , FRX}, /*таблицы                 */
    {{'L' , ' ' , ' ' , ' ' , ' '} , '\x58', 4 , FRX}, /*машинных                */
    {{'A' , ' ' , ' ' , ' ' , ' '} , '\x5A', 4 , FRX}, /*операций                */
    {{'S' , ' ' , ' ' , ' ' , ' '} , '\x5B', 4 , FRX}, /*                        */
};

//п р о г р а м м а реализации семантики команды BALR
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

//п р о г р а м м а реализации семантики команды BCR с маской 15
int P_BCR(void)
{
    int ret;
  
    ret = 1;
    if (R1 == 15)
    {
        ret = 0;
        if ((VR[R2] != 0) && (R2 != 0))
        {
            I = VR[R2];
        }
        else
        {
            if (R2 != 0)
            {
                waddstr(wcyan, "jump to address = 0 or finishing trace of the program after key press");
                wrefresh(wcyan);
                ret = 1;
            }
        }
    }  

    return ret;
}

int P_ST(void)                                        /*  п р о г р а м м а     */
                          /*реализации семантики    */
{                                                /*команды ST              */
    int sm,i;                                       /*рабочие                 */
    char bytes[4];                                  /*переменные              */

    ADDR = VR[B] + VR[X] + D;                       /*вычисление абс.адреса и */
    sm = (int)(ADDR - I);                           /*смещения                */

    bytes[0] = (VR[R1] -                            /*преобразование содержим.*/
        VR[R1]% 0x1000000L)/0x1000000L;   /*РОН, использованного в  */
    bytes[1] = ((VR[R1] -                           /*качестве первого оп-да, */
        VR[R1]%0x10000L)/0x10000L)%0x100; /*к виду, принятому в     */
    bytes[2] = ((VR[R1] % 0x10000L) -               /*ЕС ЭВМ                  */
        ((VR[R1]%0x10000L)%0x100))/0x100; /*                        */
    bytes[3] = (VR[R1] % 0x10000L) % 0x100;         /*                        */

    for (i = 0; i < 4; i++)                             /*запись преобразованого  */
    {
        OBLZ[BAS_IND + CUR_IND + sm + i] = bytes[i];   /*значения по адресу 2-г  */
    }
                          /*операнда                */
    return 0;                                       /*успешное заверш.прогр.  */
}

/*..........................................................................*/

int P_L(void)                                         /*  п р о г р а м м а     */
                          /*реализации семантики    */
{                                                /*команды L               */
    int sm;                                        /*рабочая переменная      */

    ADDR = VR[B] + VR[X] + D;                      /*вычисление абс.адреса и */
    sm = (int) ( ADDR - I );                       /*смещения                */
    VR[R1] =                                       /*преобразование содержим.*/
        OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L +   /*второго операнда к виду,*/
        OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L + /*принятому в IBM PC, и   */
        OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +    /*запись в РОН, использ.в */
        OBLZ[BAS_IND + CUR_IND + sm + 3];             /*качестве 1-го операнда  */

    return 0;                                      /*успешное заверш.прогр.  */
 }


int P_A(void)                                         /*  п р о г р а м м а     */
                          /*реализации семантики    */
{                                                /*команды A               */
    int sm;                                         /*рабочая переменная      */

    ADDR = VR[B] + VR[X] + D;                       /*вычисление абс.адреса и */
    sm = ( int ) ( ADDR -I );                       /*смещения                */
    ARG = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L+/*формирование содержимого*/
    OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L +  /*второго операнда в сог- */
    OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +     /*лашениях ЕС ЭВМ         */
    OBLZ[BAS_IND + CUR_IND + sm + 3];              /*                        */
                          /*и                       */
    VR[R1] = VR[R1] + ARG;                          /*сложение с 1-м операндом*/

    return 0;                                       /*успешное заверш. прогр. */
}

/*..........................................................................*/

int P_S(void)                                         /* п р о г р а м м а      */
                                                  /* реализации семантики   */
{                                                /* команды S              */
    int sm;                                         /*рабочая переменная      */

    ADDR = VR[B] + VR[X] + D;                       /*вычисление рабочего     */
    sm = (int)(ADDR - I);                      /*адреса и смещения       */

    ARG = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L+/*формирование содержимого*/
    OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L +/*второго операнда в сог- */
    OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +   /*лашениях ЕС ЭВМ         */
    OBLZ[BAS_IND + CUR_IND + sm + 3];            /*                        */
                          /* и                      */
    VR[R1] = VR[R1] - ARG;                          /*выч-ие из 1-го операнда */

    return 0;                                       /*успешное заверш.прогр.  */
}


int FRR(void)
{
    int i, j;
  
    for (i = 0; i < NOP; i++)
    {
        if (INST[0] == T_MOP[i].CODOP)
        {
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

    return 0; 
}
//...........................................................................
int FRX(void)
{
  int i, j;
  
  for (i = 0; i < NOP; i++)
  {
    if (INST[0] == T_MOP[i].CODOP)
    {
      waddstr(wgreen, "  ");
      for (j = 0; j < 5; j++)
        waddch(wgreen, T_MOP[i].MNCOP[j]);
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
        return (7);
      break;
    }
  }

  return 0;
} 


void wind(void)
{
    int j1, k;
  
    x = 0;
    y = 16;
   
    kk = CUR_IND;
    LIGHTPTR = I1;
  
    for (j1 = 0; j1 < 8; j1++)
    {
        wprintw(wred, "%.06lX: ", I1);  
        for (j = 0; j < 4; j++)
        {
            for (k = 0; k < 4; k++)
            {
                wprintw(wred, "%.02X", OBLZ[BAS_IND + kk + j * 4 + k]);    
            }
            waddstr(wred, " ");
        }

        waddstr(wred, "/* ");
        for (j = 0; j < 16; j++)
        {
            if (isprint(OBLZ[BAS_IND + kk]))  
            {
                waddch(wred, OBLZ[BAS_IND + kk++]);
                wrefresh(wred);
            } 
            else 
            {
                waddstr(wred, ".");
                kk++;
            }
        }

        waddstr(wred, " */");
        I1 += 16;
    }

    wrefresh(wred);           //вывод на экран
    wclear(wred);             //очистка содержимого окна дампа
}

//---------------------------------------------------------------------------
//программа покомандной интерпретпции(отладки)
// загруженной программы
static enum absload_main_error_code_e sys(void)           
{
    int res, temp;
    int ch;
    int gr_pos_x, gr_pos_y;
    int ii = 0, jj = 0;
    int gr_y;
    char wstr[80];
  
    I = BAS_ADDR;         //установить текущий адрес
                //равный начальному
//нижнее поле    

    wmargenta = newwin(1, 80, 24, 0);
    wbkgd(wmargenta, COLOR_PAIR(COLOR_MAGENTA));
    waddstr(wmargenta, "\"PgUp\",\"PgDn\",\"Up\",\"Down\"->view dump; \"Enter\"->execute the next command");
      
    //строка состояния
    wcyan = newwin(1, 80, 23, 0);
    wbkgd(wcyan, COLOR_PAIR(COLOR_CYAN));
  
    //дамп области загрузки
    wred = newwin(8, 67, 15, 0);
    wbkgd(wred, COLOR_PAIR(COLOR_RED));
  
    //поле регистров
    wblue = newwin(16, 12, 0, 68);
    wbkgd(wblue, COLOR_PAIR(COLOR_BLUE));
  
    //текст  
    gr_pos_x = 0;
    gr_pos_y = 14; 
    gr_y = 11;
    wgreen = newwin(gr_y, 67, gr_pos_y, gr_pos_x);    //создадим новое окно
    wbkgd(wgreen, COLOR_PAIR(COLOR_GREEN));   //выбор цветовой пары
  
 
    keypad(wmargenta, TRUE);              //разрешить преобразование кодов клавиатуры

    BEGIN:  

    //все допустимые коды к-нд сравнить с текущей и при 
    //совпадениизапомнить номер строки таблицы операций
    for (i = 0; i < NOP; i++)
    {
        if (OBLZ[BAS_IND + CUR_IND] == T_MOP[i].CODOP)
        {
            k = i;
            wprintw(wgreen, "%.06lX: ", I);
            //рисуем окно, выводим текст
            for (j = 0; j < 6; j++)                     /*                        */
            {                                        /*                        */
                if (j < T_MOP[i].DLOP)                  /*                        */
                {                                      /* выдать шестнадцатеричн.*/
                    wprintw(wgreen, "%.02X", OBLZ[BAS_IND + CUR_IND + j]);
                                                        /* запомнить его же в     */
                    INST[j] = OBLZ [BAS_IND + CUR_IND + j];/*                        */
                }                                      /*                        */
                else 
                {
                    INST [j] = '\x00';
                }
            }

            if ((res = T_MOP[i].BXPROG()) != 0)
            {
                return (res);
            }

            goto l0;

        }                  
    }

    return 6;
  
    l0:
    //сдвиг окна вверх
    wrefresh(wgreen);
    ii++;
    if (gr_pos_y > 14 - gr_y + 1)
    {
        mvwin(wgreen, gr_pos_y--, gr_pos_x);
    }
    //при достижении некоторого положения, движение останавливается, и производится 
    //прокрутка окна
    else
    {
        for (jj = 0; jj < gr_y - 1; jj++)
        {
            temp = mvwinnstr(wgreen, jj + 1, 0, wstr, 67);    
            mvwaddnstr(wgreen, jj, 0, wstr, 67);
            wrefresh(wgreen);
        }
    }

    wrefresh(wgreen);
  
    I += T_MOP[k].DLOP;                            /*коррекция счет-ка.адреса*/
    CUR_IND = (int)( I - BAS_ADDR );            /*уст-ка текущ. индекса   */
                          /*в массиве OBLZ          */
    I1 = I;                                        /*установка адреса начала */
                          /*области отсветки        */
      
    for ( i = 0; i < 16; i++)
    {
        if (i < 10)
        {
            waddstr(wblue, "R0");
        }
        else
        {
            waddstr(wblue, "R");
        }

        wprintw(wblue, "%d:", i);
        wprintw(wblue, "%.08lX", VR[i]);
    }

    wrefresh(wblue);          //вывод на экран          
    wclear(wblue);            //очистка окна регистров
    wind();   

    waddstr(wcyan, "ready to execution the next command with address ");
    wprintw(wcyan, "%.06lX", I - T_MOP[k].DLOP);
    waddstr(wcyan, "\n");                 
    wrefresh(wcyan);
    wclear(wcyan);

    WAIT:
  
    CUR_IND = (int)(I - BAS_ADDR);

    ch = wgetch(wmargenta);

    switch (ch)
    {
        case 10:
        {
          goto SKIP;
        }
    
        case KEY_UP:
        {
            I1 = LIGHTPTR - 16;
            CUR_IND = (int)(I1 - BAS_ADDR);
            wind();
            goto WAIT;
        }
    
        case KEY_DOWN:
        {
            I1 = LIGHTPTR + 16;
            CUR_IND = (int)(I1 - BAS_ADDR);
            wind();
            goto WAIT;      
        }
    
        case KEY_PPAGE:
        {
            I1 = LIGHTPTR - 128;
            CUR_IND = (int)(I1 - BAS_ADDR);
            wind();
            goto WAIT;
        }
    
        case KEY_NPAGE:
        {
            I1 = LIGHTPTR + 128 ;
            CUR_IND = (int)(I1 - BAS_ADDR);
            wind();
            goto WAIT;
        }
  }

    goto WAIT;

    SKIP:

    switch (T_MOP[k].CODOP)                        //согласно  коду команды, 
    {                                              //селектируемой сч.адреса 
                          //выбрать подпрогр.интер- 
        case '\x05':
            P_BALR();                       //претации семантики      
            break;                         //текущей команды        
        case '\x07': 
        { 
            i = P_BCR();
            getch();
            if (i == 1)
            {
                return 8;
            }
            break;
        }
        case '\x50':
            P_ST();
            break;
        case '\x58':
            P_L();
            break;
        case '\x5A':
            P_A();
            break;
        case '\x5B':
            P_S();
            break;
    }
   
    goto BEGIN;  
   
    delwin(wblue);             
    delwin(wred);
    delwin(wgreen);
    delwin(wmargenta);

    return ABSLOAD_MAIN_SUCCESS;
}

static void InitCurses(void)
{
    initscr();                    //инициализация библиотеки curses
    curs_set(0);
    noecho();                 //не показывать ввод
    cbreak();                 //читать один символ 
                                                //за раз, не ждать \n
    keypad(stdscr, TRUE);             //разрешить преобразование кодов клавиатуры
    start_color(); 

  
    if (has_colors())
    {
        init_pair(COLOR_BLUE, COLOR_WHITE, COLOR_BLUE);
        init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
        init_pair(COLOR_RED, COLOR_WHITE, COLOR_RED);
        init_pair(COLOR_CYAN, COLOR_BLACK, COLOR_CYAN);
        init_pair(COLOR_MAGENTA, COLOR_WHITE, COLOR_MAGENTA);
    }
}

static char* absload_main_errmsg_by_errdata(absload_main_error_data_t err_data, char *errmsg)
{
    switch (err_data.main_err_code)
    {
        case ABSLOAD_MAIN_SUCCESS:
            strcpy(errmsg, "No error occured");
            break;
        case ABSLOAD_MAIN_WRONG_NUM_CLI_PAR:
            strcpy(errmsg, "Wrong number of command line parameters");
            break;
        case ABSLOAD_MAIN_WRONG_INPUT_MOD_FILE_PATH:
            strcpy(errmsg, "Wrong path to MOD-file with list of the modules");
            break;
        case ABSLOAD_MAIN_WRONG_INPUT_MOD_FILE_EXTENSION:
            strcpy(errmsg, "Wrong input file extension with list of the modules");
            break;
        case ABSLOAD_MAIN_NOT_FOUND_INPUT_MOD_FILE:
            strcpy(errmsg, "Couldn't find mod-file with the list of the modules");
            break;
        case ABSLOAD_MAIN_LIST_MODULES_OVERFLOW:
            strcpy(errmsg, "Overflow list of the assembled modules");
            break;
        case ABSLOAD_MAIN_EMPTY_LIST_MODULES:
            strcpy(errmsg, "Empty file with list of the assembled modules");
            break;
        case ABSLOAD_MAIN_NOT_FOUND_READ_TEX_FILE:
            strcpy(errmsg, "Couldn't find tex-file with the module binery code");
            break;
        case ABSLOAD_MAIN_OBJ_CARDS_BUFFER_OVERFLOW:
            strcpy(errmsg, "Overflow buffer of storage of object cards");
            break;
        case ABSLOAD_MAIN_ILLEGAL_OPERATION_CODE:
            strcpy(errmsg, "Illegal operation code");
            break;
        case ABSLOAD_MAIN_ADDRESSING_ERROR:
            strcpy(errmsg, "Error of addressing");
            break;
        case ABSLOAD_MAIN_FAILURE:
            strcpy(errmsg, "Unknown failure");
        default:
            strcpy(errmsg, "Unknown error code for generating error message");
            break;
    }

    return errmsg;
}

static struct absload_main_error_data_s absload_main_read_tex_file(char const p_tex_fp_name[])
{
    FILE *p_tex_f;
    absload_main_error_data_t err_data;

    memset(&err_data, 0, sizeof(absload_main_error_data_t));
    err_data = (absload_main_error_data_t){
        .main_err_code = ABSLOAD_MAIN_SUCCESS
    };

    p_tex_f = fopen(SPISOK[I], "rb" );
    if (NULL == p_tex_f)
    {
        err_data.main_err_code = ABSLOAD_MAIN_NOT_FOUND_READ_TEX_FILE;
    }
    else
    {
        while (!feof(p_tex_f))
        {
            fread(OBJCARD[IOBJC], 80, 1, p_tex_f);
            IOBJC++;

            if (IOBJC == NOBJ)
            {
                err_data.main_err_code = ABSLOAD_MAIN_OBJ_CARDS_BUFFER_OVERFLOW;
                goto error2;
            }
        }

        error2:

        fclose(p_tex_f);
    }

    return err_data;
}

static enum absload_main_error_code_e absload_main_read_mod_file(char const *p_mod_fp_name)
{
    FILE *p_mod_f;
    absload_main_error_code_t err_code = ABSLOAD_MAIN_SUCCESS;

    p_mod_f = fopen(p_mod_fp_name,"rt");
    if (NULL == p_mod_f)
    {
        err_code = ABSLOAD_MAIN_NOT_FOUND_INPUT_MOD_FILE;
    }
    else
    {
        while (!feof(p_mod_f))
        {
            fgets(SPISOK[ISPIS], 80, p_mod_f);
            ++ISPIS;

            if (ISPIS == NSPIS)
            {
                err_code = ABSLOAD_MAIN_LIST_MODULES_OVERFLOW;
                goto error1;
            }
        }

        if (0 == ISPIS)
        {
            err_code = ABSLOAD_MAIN_EMPTY_LIST_MODULES;
        }

        error1:

        fclose(p_mod_f);
    }

    return err_code;
}

/* Requirements of the input parameters:
 * - 'argc' must be equal '2'
 * - 'argv[0]' as always is path to current program
 * - 'argv[1]' is the first parameter, it has to be the path of the '.mod'-file
 * '.mod'-file must contains list of the loaded binary '.tex'-modules
 */
int main(int const argc, char const *argv[])
{
    int I, K, N, J0;
    unsigned long J;
    char *p_mod_fp_name = NULL;
    size_t mod_fp_len;
    absload_main_error_data_t err_data;

    memset(&err_data, 0, sizeof(absload_main_error_data_t));
    err_data = (absload_main_error_data_t){
        .main_err_code = ABSLOAD_MAIN_SUCCESS
    };

    /* Current program must contains one real parameter */
    if (argc != 2)
    {
        err_data.main_err_code = ABSLOAD_MAIN_WRONG_NUM_CLI_PAR;
        goto error;
    }

    /* Copy name of '.mod'-file from input argument */
    ABSLOAD_COMMON_ALLOC_MEM_AND_COPY_FP_STR(p_mod_fp_name, argv[1]);

    mod_fp_len = strlen(p_mod_fp_name);
    if (mod_fp_len < 4)
    {
        err_data.main_err_code = ABSLOAD_MAIN_WRONG_INPUT_MOD_FILE_PATH;
        goto error;
    }

    /* Input file for reading list of modules must be with 'mod' extension */
    if (strcmp(&p_mod_fp_name[mod_fp_len - 4], ".mod"))
    {
        err_data.main_err_code = ABSLOAD_MAIN_WRONG_INPUT_MOD_FILE_EXTENSION;
        goto error;
    }
    else
    {
        err_data.main_err_code = absload_main_read_mod_file(p_mod_fp_name);
        ABSLOAD_COMMON_RELEASE_MEM(p_mod_fp_name);
        if (ABSLOAD_MAIN_SUCCESS == err_data.main_err_code)
        {
            for (I = 0; I < ISPIS; I++)
            {
                err_data = absload_main_read_tex_file(SPISOK[I]);
                if (ABSLOAD_MAIN_SUCCESS == err_data.main_err_code)
                {
                    POINT.P_OBLZ = OBLZ;
                    J = POINT.VAL_P.SEGM;
                    J = J << 4;
                    J += POINT.VAL_P.SMESH;

                    if ((J0 = (int)J % 8 ) == 0)
                    {
                        BAS_ADDR = J;
                        BAS_IND = 0;
                    }
                    else
                    {
                        BAS_ADDR = ((J >> 3) + 1) << 3;
                        BAS_IND = 8 - J0;
                    }

                    for (I = 0; I < IOBJC; I++)
                    {
                        if (!memcmp(&OBJCARD[I][1], "TXT", 3))
                        {
                            memcpy(&TXT, OBJCARD[I], 80);
                            J = TXT.ADOP[0];
                            J = (J << 8) + TXT.ADOP[1];
                            J = (J << 8) + TXT.ADOP[2];
                            J += BAS_IND;
                            K = TXT.DLNOP[0];
                            K = (K << 8) + TXT.DLNOP[1];

                            for (N = 0; N < K; N++)
                            {
                                OBLZ[(int)J] = TXT.OPER[N];
                                J++;
                            }
                        }
                    }

                    InitCurses();

                    err_data.main_err_code = sys(); 
                  
                    endwin();


                    /* I dunno but the first version of this 
                     * 'wonderful' absolute loader 
                     * was processing only the first file in the SPISOK.
                     * Therefore I made 'break' statement
                     * after processing the first file */
                    break;
                }
            }
        }
        else
        {
            goto error;
        }
    }

    if (ABSLOAD_MAIN_SUCCESS == err_data.main_err_code)
    {
        printf("Absolute loader has processed successfully");
    }
    else
    {
        char errmsg[100];

        error:

        printf("Absolute loader process was interrupted\nReason: %s\n", absload_main_errmsg_by_errdata(err_data, errmsg));
    }

    return err_data.main_err_code;
}
