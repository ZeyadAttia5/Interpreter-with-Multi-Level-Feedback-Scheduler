//
// Created by amr on 5/18/24.
//

#ifndef OS_PROJECT_STRING_OPERATIONS_H
#define OS_PROJECT_STRING_OPERATIONS_H

char **str_split(char *a_str, char a_delim, int *length);

char *trimwhitespace(char *str);

void itoa(int val, char *buf, int base);


#endif //OS_PROJECT_STRING_OPERATIONS_H
