/* encoding: UTF-8 */

#include <stdarg.h>
#include <string.h>

#include "plcmp_utils.h"

int streq(char const *restrict str_1, char const *restrict str_2)
{
    return (str_1 && str_2) ? 
           ((*(str_1) == *(str_2)) && !strcmp(str_1, str_2)) : -1;
}

char* strcats(char *restrict str, ...)
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