#include "../lib/sigcomm.h"
#include <ctype.h>

static char *validate_string(char *str) {
  if (!str)
    return (NULL);
  size_t len = strlen(str);

  for (size_t i = 0; i < len; i++) {
    if (!isascii(str[i]))
      return (NULL);
  }
  return (str);
}

static pid_t validate_pid(char *str) {
  if (!str)
    return (FALSE);
  pid_t pid = atoi(str);
  if (pid > 0 && pid < PID_MAX)
    return (pid);
  return (FALSE);
}

// Takes as argument string to send and pid.
int main(int argc, char *argv[]) {
  char *str = NULL;
  pid_t pid = 0;
  size_t len;

  if (argc != 3)
    return (1);
  pid = validate_pid(argv[2]);
  if (pid == FALSE)
    return (1);
  str = validate_string(argv[1]);
  if (!str)
    return (1);
  len = strlen(str);

  send_message(str, len, pid, SIGRTMIN);
}
