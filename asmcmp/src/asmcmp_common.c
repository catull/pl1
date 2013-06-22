#include <string.h>

#include "asmcmp_common.h"
#include "asmcmp_global.h"

void STXT(int ARG)
{
    char *PTR;                                      /*рабоч.переменная-указат.*/

    PTR = (char *)&CHADR;                           /*формирование поля ADOP  */
    TXT.STR_TXT.ADOP[2]  = *PTR;                    /*TXT-карты в формате     */
    TXT.STR_TXT.ADOP[1]  = *(PTR+1);                /*двоичного целого        */
    TXT.STR_TXT.ADOP[0]  = '\x00';                  /*в соглашениях ЕС ЭВМ    */

    if ( ARG == 2 )                                 /*формирование поля OPER  */
    {
        memset ( TXT.STR_TXT.OPER , 64 , 4 );
        memcpy ( TXT.STR_TXT.OPER,RR.BUF_OP_RR , 2 ); /* для RR-формата         */
        TXT.STR_TXT.DLNOP [1] = 2;
    }
    else
    {
        memcpy ( TXT.STR_TXT.OPER , RX.BUF_OP_RX , 4);/* для RX-формата         */
        TXT.STR_TXT.DLNOP [1] = 4;
    }
    memcpy (TXT.STR_TXT.POLE9,ESD.STR_ESD.POLE11,8);/*формиров.идентифик.поля */

    memcpy ( OBJTEXT[ITCARD] , TXT.BUF_TXT , 80 );  /*запись об'ектной карты  */
    ITCARD += 1;                                    /*коррекц.инд-са своб.к-ты*/
    CHADR = CHADR + ARG;                            /*коррекц.счетчика адреса */
}