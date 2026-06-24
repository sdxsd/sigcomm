#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLKSIZE 1024

char **split(char const *s, char c);
char *read_into_str(int fd);

#endif // UTILS_H
