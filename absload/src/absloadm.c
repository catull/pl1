/* encoding: UTF-8 */
/* russian letters: KOI8 (Cyrillic) */

/* for compilation use key '-lncurses'
 * for example, gcc absgraph.c -o absgraph -lncurses */

/* if you use pl1-project, it has already added for compilation
 * simply start command 'make absload' */

#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>

#define NSPIS 5                                 /*����.������ ����.�����. */
#define NOBJ 50                                /*����.����.��'������ ����*/
#define DOBLZ 1024                              /*����� ������� ��������  */
#define NOP 6                                    /*���-�� ��������������   */
                                                  /* ������                 */
char NFIL[30] = "\x0";
int IOBJC = 0;                                 /*���.�������.���. OBJCARD*/
char OBJCARD[NOBJ][80];                          /*����.������.��'����.����*/
int ISPIS = 0;                                 /*���.�������.���. SPISOK */
char SPISOK[NSPIS][80];                         /*����.������.������ �����*/

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

unsigned char INST[6];                           /*������, ������. �������.*/
                          /*�������                 */


/*..........................................................................*/
                          /*� � � � � � � �  �����.�*/
int FRR();                                        /*�����.���.����.RR-����. */

/*..........................................................................*/

                          /*� � � � � � � �  �����.�*/
int FRX();                                        /*�����.���.����.RX-����. */
/*..........................................................................*/


int X1 = 1;                                       /* ������������� �����.   */
int Y1 = 15;                                      /* �� ������              */
int R1,                                           /*����� 1-�� ��������-���-*/
                          /*����� � �������� RR � RX*/
    R2,                                           /*����� 2-�� ��������-���-*/
                          /*����� � ������� RX      */
    D,                                            /*�������� � ������� RX   */
    X,                                            /*����� �������. �������� */
                          /*� ������� RX            */
    B;                                            /*����� �������� �������� */
                          /*� ������� RX            */
unsigned long I,                                  /*������� ���.���.���-��  */
          BAS_ADDR,                           /*����� ������ ���.������.*/
          I1,ADDR,ARG,VS;                     /*��������������� �����.  */
unsigned long VR[16],                             /*������,������.����.���. */
          LIGHTPTR;                           /*����� ������ ���.������.*/


int x,y,i,j,k,kk;                                 /*������� ����������      */

int CUR_IND;                                      /*������ ����.���.����.,  */
                          /*�����.�����.���-�� �����*/
int BAS_IND;                                      /*������ ����.���.����.,  */
                          /*�����.������ ���-�� ����*/

union U1                                        /*���������� ����� �����- */
{                                              /*�� �������� ��������� ��*/
    struct                                        /*������ �������          */
    {
        char NOMREG  [ 3];
        char ZNEQUEL [1];
    } REGS_ASC [16];
    char BUFR [16][4];
}R_ASC;

union u2 {                                       /*������ ��� �������      */
                                              /*��������� �����������   */
    struct {
        unsigned int SMESH;
        unsigned int SEGM;
    } VAL_P;
    unsigned char *P_OBLZ ;
} POINT;

unsigned char OBLZ [DOBLZ] ;                    /*������� �������� ����-  */
                          /*�������� ���������      */
/*
***** ������� �������� ��������
*/

struct TMOP {                                      /*������.���.����.���.����*/
    unsigned char MNCOP [5];                       /*�������� ��������       */
    unsigned char CODOP    ;                       /*�������� ��� ��������   */
    unsigned char DLOP     ;                       /*����� �������� � ������ */
    int (*BXPROG)()        ;                       /*��������� �� �����.�����*/
} T_MOP [NOP] = {                               /*��'������� ����.���.����*/
    {{'B' , 'A' , 'L' , 'R' , ' '} , '\x05', 2 , FRR},/*�������������           */
    {{'B' , 'C' , 'R' , ' ' , ' '} , '\x07', 2 , FRR}, /*�����                   */
    {{'S' , 'T' , ' ' , ' ' , ' '} , '\x50', 4 , FRX}, /*�������                 */
    {{'L' , ' ' , ' ' , ' ' , ' '} , '\x58', 4 , FRX}, /*��������                */
    {{'A' , ' ' , ' ' , ' ' , ' '} , '\x5A', 4 , FRX}, /*��������                */
    {{'S' , ' ' , ' ' , ' ' , ' '} , '\x5B', 4 , FRX}, /*                        */
};

//..........................................................................
//� � � � � � � � � ���������� ��������� ������� BALR
int P_BALR(void)
{
  if (R2 != 0)
    I = VR[R2];
  if (R1 != 0)
    VR[R1] = I;
    
  return 0;
}

//..........................................................................
//� � � � � � � � � ���������� ��������� ������� BCR � ������ 15
int P_BCR(void)
{
  int ret;
  
  ret = 1;
  if (R1 == 15)
  {
    ret = 0;
    if ((VR[R2] != 0) && (R2 != 0))
      I = VR[R2];
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

/*..........................................................................*/

int P_ST()                                        /*  � � � � � � � � �     */
                          /*���������� ���������    */
 {                                                /*������� ST              */
  int sm,i;                                       /*�������                 */
  char bytes[4];                                  /*����������              */

  ADDR = VR[B] + VR[X] + D;                       /*���������� ���.������ � */
  sm = (int) (ADDR -I);                           /*��������                */

  bytes[0] = (VR[R1] -                            /*�������������� ��������.*/
        VR[R1]% 0x1000000L)/0x1000000L;   /*���, ��������������� �  */
  bytes[1] = ((VR[R1] -                           /*�������� ������� ��-��, */
        VR[R1]%0x10000L)/0x10000L)%0x100; /*� ����, ��������� �     */
  bytes[2] = ((VR[R1] % 0x10000L) -               /*�� ���                  */
        ((VR[R1]%0x10000L)%0x100))/0x100; /*                        */
  bytes[3] = (VR[R1] % 0x10000L) % 0x100;         /*                        */

  for (i=0; i<4; i++)                             /*������ ���������������  */
   OBLZ[BAS_IND + CUR_IND + sm + i] = bytes[i];   /*�������� �� ������ 2-�  */
                          /*��������                */
  return 0;                                       /*�������� ������.�����.  */
 }

/*..........................................................................*/

int P_L()                                         /*  � � � � � � � � �     */
                          /*���������� ���������    */
 {                                                /*������� L               */
   int sm;                                        /*������� ����������      */

   ADDR = VR[B] + VR[X] + D;                      /*���������� ���.������ � */
   sm = (int) ( ADDR - I );                       /*��������                */
   VR[R1] =                                       /*�������������� ��������.*/
    OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L +   /*������� �������� � ����,*/
    OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L + /*��������� � IBM PC, �   */
    OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +    /*������ � ���, �������.� */
    OBLZ[BAS_IND + CUR_IND + sm + 3];             /*�������� 1-�� ��������  */

   return 0;                                      /*�������� ������.�����.  */
 }

/*..........................................................................*/
int P_A()                                         /*  � � � � � � � � �     */
                          /*���������� ���������    */
 {                                                /*������� A               */
  int sm;                                         /*������� ����������      */

  ADDR = VR[B] + VR[X] + D;                       /*���������� ���.������ � */
  sm = ( int ) ( ADDR -I );                       /*��������                */
  ARG = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L+/*������������ �����������*/
   OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L +  /*������� �������� � ���- */
   OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +     /*�������� �� ���         */
   OBLZ[BAS_IND + CUR_IND + sm + 3];              /*                        */
                          /*�                       */
  VR[R1] = VR[R1] + ARG;                          /*�������� � 1-� ���������*/

  return 0;                                       /*�������� ������. �����. */
 }

/*..........................................................................*/

int P_S()                                         /* � � � � � � � � �      */
                                                  /* ���������� ���������   */
{                                                /* ������� S              */
  int sm;                                         /*������� ����������      */

  ADDR = VR[B] + VR[X] + D;                       /*���������� ��������     */
  sm = ( int ) ( ADDR - I );                      /*������ � ��������       */

    ARG = OBLZ[BAS_IND + CUR_IND + sm] * 0x1000000L+/*������������ �����������*/
     OBLZ[BAS_IND + CUR_IND + sm + 1] * 0x10000L +/*������� �������� � ���- */
     OBLZ[BAS_IND + CUR_IND + sm + 2] * 0x100 +   /*�������� �� ���         */
     OBLZ[BAS_IND + CUR_IND + sm + 3];            /*                        */
                          /* �                      */
  VR[R1] = VR[R1] - ARG;                          /*���-�� �� 1-�� �������� */

  return 0;                                       /*�������� ������.�����.  */
 }


//..........................................................................
int FRR(void)
{
  int i, j;
  
  for (i = 0; i < NOP; i++)
  {
    if (INST[0] == T_MOP[i].CODOP)
    {
      waddstr(wgreen, "      ");
      for (j = 0; j < 5; j++)
        waddch(wgreen, T_MOP[i].MNCOP[j]);
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

//...........................................................................   
//---------------------------------------------------------------------------
int wind(void)
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
    wprintw(wred, "%.02X", OBLZ[BAS_IND + kk + j * 4 + k]);
      waddstr(wred, " ");
    }

    waddstr(wred, "/* ");
    for (j = 0; j < 16; j++)
    {
      if (isprint (OBLZ[BAS_IND + kk]) )  
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
  wrefresh(wred);           //����� �� �����
  wclear(wred);             //������� ����������� ���� �����
  
  return 0;
}
//---------------------------------------------------------------------------
//��������� ����������� �������������(�������)
// ����������� ���������
int sys(void)           
{
  int res, temp;
  int ch;
  int gr_pos_x, gr_pos_y;
  int ii = 0, jj = 0;
  int gr_y;
  char wstr[80];
  
  I = BAS_ADDR;         //���������� ������� �����
                //������ ����������
//������ ����    

  wmargenta = newwin(1, 80, 24, 0);
  wbkgd(wmargenta, COLOR_PAIR(COLOR_MAGENTA));
  waddstr(wmargenta, "\"PgUp\",\"PgDn\",\"Up\",\"Down\"->view dump; \"Enter\"->execute the next command");
      
//������ ���������
  wcyan = newwin(1, 80, 23, 0);
  wbkgd(wcyan, COLOR_PAIR(COLOR_CYAN));
  
//���� ������� ��������
  wred = newwin(8, 67, 15, 0);
  wbkgd(wred, COLOR_PAIR(COLOR_RED));
  
//���� ���������
  wblue = newwin(16, 12, 0, 68);
  wbkgd(wblue, COLOR_PAIR(COLOR_BLUE));
  
//�����  
  gr_pos_x = 0;
  gr_pos_y = 14; 
  gr_y = 11;
  wgreen = newwin(gr_y, 67, gr_pos_y, gr_pos_x);    //�������� ����� ����
  wbkgd(wgreen, COLOR_PAIR(COLOR_GREEN));   //����� �������� ����
  
 
  keypad(wmargenta, TRUE);              //��������� �������������� ����� ����������

BEGIN:  

//��� ���������� ���� �-�� �������� � ������� � ��� 
//������������������� ����� ������ ������� ��������
  for (i = 0; i < NOP; i++)
  {
    if (OBLZ[BAS_IND + CUR_IND] == T_MOP[i].CODOP)
    {
      k = i;
      wprintw(wgreen, "%.06lX: ", I);
//������ ����, ������� �����
      for (j = 0; j < 6; j++)                     /*                        */
      {                                        /*                        */
        if (j < T_MOP[i].DLOP)                  /*                        */
        {                                      /* ������ ���������������.*/
         wprintw(wgreen, "%.02X", OBLZ[BAS_IND + CUR_IND + j]);
                          /* ��������� ��� �� �     */
        INST[j] =                                   /* ���������� INST,       */
             OBLZ [BAS_IND + CUR_IND + j];/*                        */
        }                                      /*                        */
        else INST [j] = '\x00';                     /*                        */
      }
      if ((res = T_MOP[i].BXPROG()) != 0)    /* ���� � ��������� �����.*/
    return (res);                 /* ������������� ��������-*/
                          /* ��� ������� �������,   */
                          /*                        */
    goto l0;                                  /* ������� � ����������  */
    }                  
  }
  return (6);
  
l0:
//����� ���� �����
  wrefresh(wgreen);
  ii++;
  if (gr_pos_y > 14 - gr_y + 1)
    mvwin(wgreen, gr_pos_y--, gr_pos_x);
//��� ���������� ���������� ���������, �������� ���������������, � ������������ 
//��������� ����
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
  
  I += T_MOP[k].DLOP;                            /*��������� ����-��.������*/
  CUR_IND = ( int ) ( I - BAS_ADDR );            /*���-�� �����. �������   */
                          /*� ������� OBLZ          */
  I1 = I;                                        /*��������� ������ ������ */
                          /*������� ��������        */
      
  for ( i = 0; i < 16; i++)
  {
    if (i < 10)
      waddstr(wblue, "R0");
    else 
      waddstr(wblue, "R");
    wprintw(wblue, "%d:", i);
    wprintw(wblue, "%.08lX", VR[i]);
  }      
  wrefresh(wblue);          //����� �� �����          
  wclear(wblue);            //������� ���� ���������
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
    
    case  KEY_UP:
    {
      I1 = LIGHTPTR - 16;
      CUR_IND = (int)(I1 - BAS_ADDR);
      wind();
      goto WAIT;
    }
    
    case  KEY_DOWN:
    {
      I1 = LIGHTPTR + 16;
      CUR_IND = (int)(I1 - BAS_ADDR);
      wind();
      goto WAIT;      
    }
    
    case  KEY_PPAGE:
    {
      I1 = LIGHTPTR - 128;
      CUR_IND = (int)(I1 - BAS_ADDR);
      wind();
      goto WAIT;
    }
    
    case  KEY_NPAGE:
    {
      I1 = LIGHTPTR + 128 ;
      CUR_IND = (int)(I1 - BAS_ADDR);
      wind();
      goto WAIT;
    }
  }
goto WAIT;

SKIP:

   switch (T_MOP[k].CODOP)                        //��������  ���� �������, 
   {                                              //������������� ��.������ 
                          //������� ��������.�����- 
    case '\x05' : P_BALR();                       //�������� ���������      
           break;                         //������� �������        
    case '\x07' : { i = P_BCR();
            getch();
            if (i == 1)
             return 8;
          }
           break;
    case '\x50' : P_ST();
           break;
    case '\x58' : P_L();
           break;
    case '\x5A' : P_A();
           break;
    case '\x5B' : P_S();
   }
   
   goto BEGIN;  
   
   delwin(wblue);             
   delwin(wred);
   delwin(wgreen);
   delwin(wmargenta);

  return 0;
}
//...........................................................................
//..........................������������� curses..............................
int InitCurses(void)
{

  initscr();                    //������������� ���������� curses
  curs_set(0);
  noecho();                 //�� ���������� ����
  cbreak();                 //������ ���� ������ 
                                                //�� ���, �� ����� \n
  keypad(stdscr, TRUE);             //��������� �������������� ����� ����������
  start_color(); 

  
  if (has_colors())
  {
    init_pair(COLOR_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(COLOR_RED, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_CYAN, COLOR_BLACK, COLOR_CYAN);
    init_pair(COLOR_MAGENTA, COLOR_WHITE, COLOR_MAGENTA);
  }

  
  return 0;
}



int main(int const argc, char const *argv[])
{
    int I, K, N, J0, res;
    unsigned long J;
    FILE *fp;
    char *ptr;

    if (argc != 2)
    {
        printf ( "%s\n", "Error in command-line" );
        return -1;
    }
 
    ptr = argv[1];
    strcpy(NFIL, ptr);

    if (strcmp(&NFIL[strlen(NFIL) - 3], "mod"))
    {
        goto ERR9;
        return -1;
    }
  
    if ((fp = fopen(NFIL,"rt")) == NULL)
    {
        goto ERR1;    
    }
    else
    {
        while (!feof(fp))
        {
            fgets(SPISOK[ISPIS], 80, fp);
            ISPIS++;

            if (ISPIS == NSPIS)
            {
                fclose(fp);
                goto ERR4;
            }
        }
        fclose(fp);

        if (0 == ISPIS)
        {
            goto ERR2;
        }
        else
        {
            goto CONT1;
        }
    }

    CONT1:

    for (I = 0; I < ISPIS; I++)
    {      
        if ((fp = fopen(SPISOK[I], "rb" )) ==  NULL)
        {
            goto ERR3;
        }
        else
        {
            while (!feof(fp))
            {
                fread(OBJCARD[IOBJC], 80, 1, fp);
                IOBJC++;
                if (IOBJC == NOBJ)
                {
                    fclose(fp);
                    goto ERR5;
                }
            }
            fclose(fp);

            goto CONT2;
        }
    }

    CONT2:

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
            J = TXT.ADOP [0];
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

    res = sys(); 
  
    switch (res)
    {
        case 6: 
            endwin();
            goto ERR6;
        case 7:
            endwin();
            goto ERR7;
        case 8:
            endwin();
            goto ERR8;
        default:
            assert(0);
    }
  
    endwin(); 

    END:

    printf ("\n%s\n", "end processing");

    return 0;

    ERR1:
        printf("%s%s\n", "error opening of the files with list of the assembled ", "modules");
        goto END;

    ERR2:
        printf("%s\n", "empty file with list of the assembled modules");
        goto END;

    ERR3:
        printf("%s: %s\n", "error opening of the file", SPISOK[I]);
        goto END;

    ERR4:
        printf("%s\n", "overflow list of the assembled modules");
        goto END;

    ERR5:
        printf("%s\n", "overflow buffer of storage of object cards");
        goto END;

    ERR6:
        printf("%s\n", "illegal command's code");
        goto END;

    ERR7:
        printf("interruption - error of addressing\n");
        goto END;

    ERR8:
        goto END;

    ERR9:
        printf("%s\n", "Wrong type of file with the source text");
        goto END;
}
