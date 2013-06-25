/* encoding: UTF-8 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "asmcmp_common.h"
#include "asmcmp_global.h"
#include "asmcmp_machine_oper.h"

void asmcmp_common_print_oper(oper_t oper)
{
    switch (oper.oper_type)
    {
        case OPER_RR:
            printf("RR: %#.2x %#.2x\n",
                   oper.oper.rr.opcode,
                   oper.oper.rr.R1R2);
            break;
        case OPER_RX:
            printf("RX: %#.2x %#.2x %#.4x\n",
                   oper.oper.rx.opcode,
                   oper.oper.rx.R1X2,
                   oper.oper.rx.B2D2);
            break;
        case OPER_SS:
            printf("SS: %#.2x %#.2x %#.4x %#.4x\n",
                   oper.oper.ss.opcode,
                   oper.oper.ss.L,
                   oper.oper.ss.B1D1,
                   oper.oper.ss.B2D2);
            break;
    }
}

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
            ASMCMP_COMMON_ASSERT(OPER_RR_LEN == sizeof(oper_rr_t));

            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.rr, OPER_RR_LEN);
            TXT.DLNOP[1] = OPER_RR_LEN;
            oper_len = OPER_RR_LEN;
            break;
        }
        case OPER_RX:
        {
            ASMCMP_COMMON_ASSERT(OPER_RX_LEN == sizeof(oper_rx_t));

            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.rx, OPER_RX_LEN);
            TXT.DLNOP[1] = OPER_RX_LEN;
            oper_len = OPER_RX_LEN;
            break;
        }
        case OPER_SS:
        {
            ASMCMP_COMMON_ASSERT(OPER_SS_LEN == sizeof(oper_ss_t));

            memset(TXT.OPER, 0x40, 56);
            memcpy(TXT.OPER, &oper.oper.ss, OPER_SS_LEN);
            TXT.DLNOP[1] = OPER_SS_LEN;
            oper_len = OPER_SS_LEN;
            break;
        }
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