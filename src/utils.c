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

char *read_into_str(int fd) {
  char *temp;
  char *buf;
  ssize_t bufsize;
  ssize_t bytes_read;
  ssize_t bytes;

  bufsize = BLKSIZE;
  bytes_read = 0;
  bytes = 0;
  buf = calloc(bufsize + 1, sizeof(char));
  if (!buf)
    return (NULL);
  for (;;) {
    bytes_read = read(fd, &buf[bytes], BLKSIZE);
    bytes += bytes_read;
    if (bytes_read < BLKSIZE)
      break;
    if (bytes_read == 0)
      break;
    if (bytes == bufsize) {
      bufsize *= 2;
      temp = realloc(buf, bufsize + 1);
      if (!temp) {
        free(buf);
        return (NULL);
      }
      buf = temp;
    }
  }
  if (bytes_read == 0 && bytes == 0) {
    free(buf);
    return (NULL);
  }
  buf[bufsize] = '\0';
  return (buf);
}
