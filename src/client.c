/*
THIS FILE IS LICENSED UNDER THE GNU GPLv3
Copyright (C) 2026 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>

The definition of Free Software is as follows:
				- The freedom to run the program, for any purpose.
				- The freedom to study how the program works, and adapt it to your needs.
				- The freedom to redistribute copies so you can help your neighbor.
				- The freedom to improve the program, and release
				your improvements to the public, so that the whole community benefits.

A program is free software if users have all of these freedoms.
*/

#include "../include/sigcomm.h"
#include "../include/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static int validate_signal(char *str) {
  if (!str)
    return (FALSE);
  int sig = atoi(str);
  if (sig >= SIGRTMIN && sig <= SIGRTMAX)
    return (sig);
  return (FALSE);
}

static char *validate_string(char *str) {
  if (!str)
    return (NULL);
  size_t len = strlen(str);

  for (size_t i = 0; i < len; i++)
    if (!isascii(str[i]))
      return (NULL);
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
  char *line = NULL;
  char **cmd;
  pid_t pid = 0;
  int sig = 0;

  rl_bind_key('\t', rl_complete);
  using_history();

  while (TRUE) {
    line = readline("sigshell> ");
    if (!line)
      break;
    add_history(line);
    cmd = split(line, ' ');
    if (!cmd)
      exit(1); // FIXME: Add real error handling.

    str = validate_string(cmd[0]);
    if (!str)
      return (1);
    sig = validate_signal(cmd[1]);
    if (!sig)
      return (1);
    pid = validate_pid(cmd[2]);
    if (pid == FALSE)
      return (1);
    send_message(str, strlen(str), pid, sig);
  }
}
