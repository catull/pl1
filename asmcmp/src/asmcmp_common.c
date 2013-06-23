/* encoding: UTF-8 */

#include <string.h>

#include "asmcmp_common.h"
#include "asmcmp_global.h"

void STXT(int ARG)
{
    char *PTR;                                      /*рабоч.переменная-указат.*/

    PTR = (char*)&CHADR;                           /*формирование поля ADOP  */
    TXT.ADOP[2] = *PTR;                    /*TXT-карты в формате     */
    TXT.ADOP[1] = *(PTR+1);                /*двоичного целого        */
    TXT.ADOP[0] = '\x00';                  /*в соглашениях ЕС ЭВМ    */

    if (2 == ARG)                                 /*формирование поля OPER  */
    {
        memset(TXT.OPER, 64, 4);
        memcpy(TXT.OPER, &RR, 2); /* для RR-формата         */
        TXT.DLNOP[1] = 2;
    }
    else
    {
        memcpy(TXT.OPER, &RX, 4);/* для RX-формата         */
        TXT.DLNOP[1] = 4;
    }
    memcpy(TXT.POLE9, ESD.POLE11, 8);/*формиров.идентифик.поля */

    memcpy(OBJTEXT[ITCARD], &TXT, 80);  /*запись об'ектной карты  */
    ++ITCARD;                                    /*коррекц.инд-са своб.к-ты*/
    CHADR = CHADR + ARG;                            /*коррекц.счетчика адреса */
}