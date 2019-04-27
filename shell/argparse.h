#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define STRLEN_INC_NULL(str) (strlen(str) + 1)
#define LAST_INDEX(arr_len) (arr_len - 1)

char* get_arg(char**);
char** get_argv(char*);
void clean_argv(char**);
size_t arrlen(void**);
bool check_last_char(char**, char);
char** trim_last_arg(char**);

#endif /* ARGPARSE_H */
