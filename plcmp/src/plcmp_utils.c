/* encoding: UTF-8 */

#include <stdarg.h>
#include <string.h>

#include "plcmp_utils.h"

/* Check equality of two strings */
int streq(char const *restrict str_1, char const *restrict str_2)
{
    return !strcmp(str_1, str_2);
}

/* Subroutine concatenates strings will be passed trought the parameters and
 * puts result to 'str' destination string
 * The last parameter must be a NULL-pointer */
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