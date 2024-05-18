#include "strings.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "ctype.h"
#include <stddef.h>
#include <bits/types/FILE.h>
#include <dirent.h>

/**
 * Splits a string into an array of substrings based on a delimiter.
 *
 * @param a_str The string to be split.
 * @param a_delim The delimiter character used to split the string.
 * @return A dynamically allocated array of strings representing the substrings.
 *         The last element of the array is set to NULL to indicate the end of the list.
 *         The caller is responsible for freeing the memory allocated for the array and its elements.
 */
char **str_split(char *a_str, char a_delim)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result)
    {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


/**
 * Trims leading and trailing whitespace from a string.
 *
 * @param str The string to be trimmed.
 * @return A pointer to the trimmed string.
 */
char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

void itoa(int val, char *buf, int base)
{

    char *p = buf;
    char *p1, *p2;
    int dig;
    unsigned int uval;

    uval = (unsigned int)val;
    if (val < 0)
    {
        *p++ = '-';
        uval = (unsigned int)(-val);
    }

    p1 = p;

    do
    {
        dig = uval % base;
        uval = uval / base;

        if (dig < 10)
            *p++ = (char)(48 + dig);
        else
            *p++ = (char)(97 + (dig - 10));
    } while (uval > 0);

    *p-- = 0;

    p2 = p;
    while (p1 < p2)
    {
        dig = *p1;
        *p1++ = *p2;
        *p2-- = (char)dig;
    }
}

