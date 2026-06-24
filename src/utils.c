#include "../include/utils.h"

static int wordcount(const char *s, char c) {
  int wc;

  if (!s)
    return (-1);
  wc = 0;
  while (*s) {
      if (*s == c)
        s++;
      if (*s != c && *s != '\0') {
          wc++;
          while (*s != c && *s)
            s++;
      }
  }
  return (wc);
}

static int gndl(const char *s, char c) {
  int iterator;

  iterator = 0;
  while (s[iterator] != c && s[iterator])
    iterator++;
  return (iterator);
}

static char **clean_split(char **s_array) {
  while (s_array) {
      if (*s_array)
        free(*s_array);
      s_array += 1;
  }
  free(s_array);
  return ((char **)0x0);
}

char **split(char const *s, char c) {
  char **s_array;
  size_t iterator;

  iterator = 0;
  if (!s)
    return (NULL);
  s_array = malloc(sizeof(char *) * (wordcount(s, c) + 1));
  if (!s_array)
    return (NULL);
  while (s_array && iterator < strlen(s)) {
      while (s[iterator] == c && s[iterator] != '\0')
        iterator++;
      if (s[iterator] != c && s[iterator] != '\0') {
          *s_array = strndup(&s[iterator], gndl(&s[iterator], c));
          if (!*s_array)
            return (clean_split(s_array - iterator));
          iterator += gndl(&s[iterator], c);
          s_array += 1;
      }
  }
  *s_array = NULL;
  return (s_array - (wordcount(s, c)));
}

char *read_into_string(int fd) {
  ssize_t bytes_read = 0;
  ssize_t allocated;
  ssize_t total_read = 0;
  char buf[BUF_SIZE];
  char *str = NULL;
  char *tmp;

  bzero(buf, BUF_SIZE);
  str = calloc(BUF_SIZE + 1, sizeof(char));
  allocated = BUF_SIZE;
  if (!str)
    return (NULL);
  bytes_read = read(fd, buf, BUF_SIZE);
  memcpy(str, buf, bytes_read);
  total_read += bytes_read;
  while (bytes_read == BUF_SIZE) {
    bzero(buf, BUF_SIZE);
    bytes_read = read(fd, buf, BUF_SIZE);
    memcpy(str + total_read, buf, bytes_read);
    total_read += bytes_read;
    if ((total_read + BUF_SIZE) >= allocated) {
      allocated *= 2;
      tmp = realloc(str, allocated + 1);
      if (tmp != str) {

      }
    }
  }
  return (str);
}
