/* encoding: UTF-8 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "asmcmp_common.h"
#include "asmcmp_global.h"
#include "asmcmp_machine_oper.h"

/* Subroutine swaps bytes in input data by 'p_src' pointer 
 * and puts result into data by 'p_dst' pointer 
 * Number of swapped bytes is 'count_bytes' 
 */
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