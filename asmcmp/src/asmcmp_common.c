/* encoding: UTF-8 */

#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "asmcmp_common.h"
#include "asmcmp_global.h"

void asmcmp_common_save_oper_tex_card(oper_t oper)
{
    char *PTR;
    size_t oper_len;

    PTR = (char*)&CHADR;
    TXT.ADOP[2] = *PTR;
    TXT.ADOP[1] = *(PTR + 1);
    TXT.ADOP[0] = '\x00';

    switch (oper.oper_type)
    {
        case OPER_RR:
        {
            oper_len = sizeof(oper.oper.rr);
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.rr, oper_len);
            TXT.DLNOP[1] = oper_len;
            break;
        }
        case OPER_RX:
        {
            oper_len = sizeof(oper.oper.rx);
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.rx, oper_len);
            TXT.DLNOP[1] = oper_len;
            break;
        }
        case OPER_SS:
            oper_len = sizeof(oper.oper.ss);
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.ss, oper_len);
            TXT.DLNOP[1] = oper_len;
            break;
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    memcpy(TXT.POLE9, ESD.POLE11, 8);
    memcpy(OBJTEXT[ITCARD], &TXT, 80);
    ++ITCARD;
    CHADR = CHADR + oper_len;
}

void asmcmp_common_save_data_tex_card(data_t data)
{
    char *PTR;
    size_t data_len;

    PTR = (char*)&CHADR;
    TXT.ADOP[2] = *PTR;
    TXT.ADOP[1] = *(PTR + 1);
    TXT.ADOP[0] = '\x00';

    switch (data.data_type)
    {
        case DATA_FIXED_BIN:
        {
            data_len = sizeof(data.data.fixed_bin);
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &data.data.fixed_bin, data_len);
            TXT.DLNOP[1] = data_len;
            break;
        }
        case DATA_STRING:
        {
            data_len = data.data.string_data.str_length;
            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, data.data.string_data.p_string, data_len);
            TXT.DLNOP[1] = data_len;
            break;
        }
        default:
            ASMCMP_COMMON_ASSERT(0);
            break;
    }

    memcpy(TXT.POLE9, ESD.POLE11, 8);
    memcpy(OBJTEXT[ITCARD], &TXT, 80);
    ++ITCARD;
    CHADR = CHADR + data_len;
}

void asmcmp_common_swap_bytes(uint8_t *p_src, uint8_t *p_dst, size_t count_bytes)
{
    uint8_t *p_little_byte_src = p_src, *p_big_byte_src = &p_src[count_bytes - 1];
    uint8_t *p_little_byte_dst = p_dst, *p_big_byte_dst = &p_dst[count_bytes - 1];

    while (p_big_byte_src - p_little_byte_src > 0)
    {
        uint8_t temp_big_byte_src = *p_big_byte_src;
        *p_big_byte_dst = *p_little_byte_src;
        *p_little_byte_dst = temp_big_byte_src;
        ++p_little_byte_src;
        ++p_little_byte_dst;
        --p_big_byte_src;
        --p_big_byte_dst;
    }
}