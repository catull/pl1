/* encoding: UTF-8 */

#include <stdarg.h>
#include <string.h>

#include "utils.h"

int utils_streq(char const *restrict str_1, char const *restrict str_2)
{
    return (str_1 && str_2) ? 
           ((*(str_1) == *(str_2)) && !strcmp(str_1, str_2)) : -1;
}

char* utils_strcats(char *restrict str, ...)
{
    va_list strings;
    char *str_to_concat = NULL;

    *str = '\0';

    va_start(strings, str);
    str_to_concat = va_arg(strings, char*);
    while (str_to_concat)
    {
        strcat(str, str_to_concat);
        str_to_concat = va_arg(strings, char*);
    }
    va_end(strings);

    return str;
}

void utils_swap_bytes(uint8_t *p_src, uint8_t *p_dst, size_t count_bytes)
{
    uint8_t *p_little_byte_src = p_src,
            *p_big_byte_src = &p_src[count_bytes - 1];
    uint8_t *p_little_byte_dst = p_dst,
            *p_big_byte_dst = &p_dst[count_bytes - 1];

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

char* utils_copy_file_path_and_change_extension(char const *fp,
                                                char const *new_ext)
{
    char *p_new_fp_name = NULL;
    char *p_new_fp = NULL;

    UTILS_ASSERT(fp);

    p_new_fp_name = strdup((fp));
    p_new_fp = UTILS_MALLOC_MEM(strlen(p_new_fp_name));
    p_new_fp_name[strlen((p_new_fp_name)) - 3] = '\0';

    utils_strcats(p_new_fp, p_new_fp_name, new_ext, NULL);
    UTILS_RELEASE_MEM(p_new_fp_name);

    return p_new_fp;
}