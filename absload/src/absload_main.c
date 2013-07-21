/* encoding: UTF-8 */
/* russian letters: KOI8 (Cyrillic) */

/* for compilation use key '-lncurses'
 * for example, gcc absloadm.c -o absloadm -lncurses */

/* if you use mine pl1-project, key has already added for compilation;
 * simply start command 'make absload' from the root of the project */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>

#include "absload_common.h"
#include "absload_main.h"
#include "absload_machine_oper.h"

extern machine_operations_table_t T_MOP[NOP];

int IOBJC = 0;                      /* инд.вакантн.стр. OBJCARD */
char OBJCARD[NOBJ][80];             /* масс.хранен.об'ектн.карт */
int ISPIS = 0;                      /* инд.вакантн.стр. SPISOK */
char SPISOK[NSPIS][80];             /* масс.хранен.списка прогр */

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

/* Current processing command */
unsigned char INST[6];

int X1 = 1;     /* X-coordinate init */
int Y1 = 15;    /* Y-coordinate init */
int R1;         /* Number of the first register in 'RR' and 'RX' operations */
int R2;         /* Number of the second register in 'RR' operations */
int D;          /* Displacement of the second operand in 'RX' operations */
int X;          /* Number of the register which contains index for 'RX' oeprations */
int B;          /* Number of the base register for 'RX' operations */
int L;          /* Length of the operands in 'SS' operations */
int D1, D2;
int B1, B2;

unsigned long I;                    /* Counter of the address of the current operation */
unsigned long BAS_ADDR;             /* адрес начала обл.загруз. */
unsigned long I1, ADDR, ARG, VS;    /* вспомогательные перем. */
unsigned long ADDR1, ADDR2;
unsigned long VR[16];               /* Array contaning values of the registers */ 
unsigned long LIGHTPTR;             /* адрес начала обл.отсвет. */

int x, y, i, j, k, kk;              /* рабочие переменные */
int CUR_IND;                        /* индекс масс.обл.загр., соотв.текущ.ком-де прогр */
int BAS_IND;                        /* индекс масс.обл.загр., соотв.первой ком-ды прог */

/* постоянная часть шаблона отсветки регистров на экране консоли */
union U1 {
    struct {
        char NOMREG[3];
        char ZNEQUEL[1];
    } REGS_ASC[16];
    char BUFR[16][4];
}R_ASC;

/* шаблон для расчета элементов абсолютного */
union u2 {
    struct {
        unsigned int SMESH;
        unsigned int SEGM;
    } VAL_P;
    unsigned char *P_OBLZ;
} POINT;

unsigned char OBLZ[DOBLZ];          /* область загрузки трассируемой программы */

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
            break;
        default:
            strcpy(errmsg, "Unknown error code for generating error message");
            break;
    }

    return errmsg;
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

        waddstr(wred, "....................... */");
        I1 += 16;
    }

    wrefresh(wred);           //вывод на экран
    wclear(wred);             //очистка содержимого окна дампа
}

//программа покомандной интерпретпции(отладки)
//загруженной программы
static enum absload_main_error_code_e sys(void)           
{
    int res;
    int ch;
    int gr_pos_x, gr_pos_y;
    int ii = 0, jj = 0;
    int gr_y;
    char wstr[80];
  
    /* Set current address to starting address */
    I = BAS_ADDR;

    //window below  
    wmargenta = newwin(1, 90, 24, 0);
    wbkgd(wmargenta, COLOR_PAIR(COLOR_MAGENTA));
    waddstr(wmargenta, "\"PgUp\", \"PgDn\", \"Up\", \"Down\" -> view dump; \"Enter\" -> execute the next command");
      
    //status bar window
    wcyan = newwin(1, 90, 23, 0);
    wbkgd(wcyan, COLOR_PAIR(COLOR_CYAN));
  
    //boot area dump window
    wred = newwin(8, 90, 15, 0);
    wbkgd(wred, COLOR_PAIR(COLOR_RED));
  
    //registers' window
    wblue = newwin(16, 12, 0, 95);
    wbkgd(wblue, COLOR_PAIR(COLOR_BLUE));
  
    //current command window
    gr_pos_x = 0;
    gr_pos_y = 14; 
    gr_y = 11;
    wgreen = newwin(gr_y, 90, gr_pos_y, gr_pos_x);      //создадим новое окно
    wbkgd(wgreen, COLOR_PAIR(COLOR_GREEN));             //выбор цветовой пары
  
    keypad(wmargenta, TRUE);                            //разрешить преобразование кодов клавиатуры

    BEGIN:  

    //все допустимые коды к-нд сравнить с текущей, и при 
    //совпадении запомнить номер строки таблицы операций
    for (i = 0; i < NOP; i++)
    {
        if (OBLZ[BAS_IND + CUR_IND] == T_MOP[i].CODOP)
        {
            k = i;
            wprintw(wgreen, "%.06lX: ", I);
            
            //рисуем окно, выводим текст
            for (j = 0; j < OPER_MAX_LEN; j++)
            {
                if (j < T_MOP[i].DLOP)
                {
                    wprintw(wgreen, "%.02X", OBLZ[BAS_IND + CUR_IND + j]);
                    INST[j] = OBLZ[BAS_IND + CUR_IND + j];
                }
                else 
                {
                    wprintw(wgreen, "  ");
                    INST[j] = '\x00';
                }
            }

            res = T_MOP[i].BXPROG();
            if (0 != res)
            {
                return res;
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
            mvwinnstr(wgreen, jj + 1, 0, wstr, 67);    
            mvwaddnstr(wgreen, jj, 0, wstr, 67);
            wrefresh(wgreen);
        }
    }

    wrefresh(wgreen);
  
    I += T_MOP[k].DLOP;                            /*коррекция счет-ка.адреса*/
    CUR_IND = (int)(I - BAS_ADDR);            /*уст-ка текущ. индекса   */
                          /*в массиве OBLZ          */
    I1 = I;                                        /*установка адреса начала */
                          /*области отсветки        */
      
    for (i = 0; i < 16; i++)
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

    /* Look up current operation code 
     * Choose appropriate handle */
    switch (T_MOP[k].CODOP)
    {
        case 0x05:
            P_BALR();
            break;
        case 0x07: 
        { 
            i = P_BCR();
            getch();
            if (i == 1)
            {
                return ABSLOAD_MAIN_SUCCESS;
            }
            break;
        }
        case 0x50:
            P_ST();
            break;
        case 0x58:
            P_L();
            break;
        case 0x5A:
            P_A();
            break;
        case 0x5B:
            P_S();
            break;
        case 0x38:
            P_LR();
            break;
        case 0x41:
            P_LA();
            break;
        case 0x1A:
            P_AR();
            break;
        case 0xD2:
            P_MVC();
            break;
        default:
            ABSLOAD_COMMON_ASSERT(0);
            break;
    }
   
    goto BEGIN;  
   
    delwin(wblue);             
    delwin(wred);
    delwin(wgreen);
    delwin(wmargenta);

    return ABSLOAD_MAIN_SUCCESS;
}

static void absload_main_init_curses(void)
{
    initscr();                      /* Init curses library */
    curs_set(0);
    noecho();                       /* Don't show input */
    cbreak();                       /* Read one symbol, за раз, не ждать \n */
    keypad(stdscr, TRUE);           /* allow transforming of keybord codes */
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


/* 
 *
 * Requirements of the input parameters:
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
                                ++J;
                            }
                        }
                    }

                    absload_main_init_curses();

                    err_data.main_err_code = sys(); 
                  
                    endwin();


                    /* I dunno why but the first version of this 
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
        printf("Absolute loader has processed successfully\n");
    }
    else
    {
        char errmsg[100];

        error:

        printf("Absolute loader process was interrupted\nReason: %s\n", absload_main_errmsg_by_errdata(err_data, errmsg));
    }

    return err_data.main_err_code;
}
