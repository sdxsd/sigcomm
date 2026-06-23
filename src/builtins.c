#include "../include/builtins.h"
#include "../include/sigcomm.h"
#include "../include/utils.h"

void *print_message(client_t *client, void *data) {
  if (!client || !data)
    return (NULL);
  char *str = (char *)data;
  printf("Client state: MESSAGE_RECEIVED\n");
  printf("Received: %zu\n", client->received);
  printf("Received bytes:\n");
  for (size_t i = 0; i < client->received; i++)
    printf("%c", str[i]);
  printf("\n");
  return (NULL);
}

void *simple_exec(client_t *client, void *data) {
  int tube[2];
  pid_t pid;
  char buf[1024];
  char *str = (char *)data;
  char **cmd;

  if (!client || !data)
    return (NULL);

  if (pipe(tube) == -1)
    return (NULL);
  pid = fork();
  if (pid == -1) {
    close(tube[READ]);
    close(tube[WRITE]);
    return (NULL);
  }
  if (pid == 0) { // Child Process
    dup2(tube[WRITE], STDOUT_FILENO);
    close(tube[READ]);
    cmd = split(str, ' ');
    if (!cmd) {
      close(tube[READ]);
      close(tube[WRITE]);
      return (NULL);
    }
    execvp(cmd[0], cmd);
  }
  else {
    close(tube[WRITE]);

  }
}
