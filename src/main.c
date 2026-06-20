#include "../lib/sigcomm.h"
#include <stdio.h>

int putchar(int c) {
  return (write(STDOUT_FILENO, &c, 1));
}

void router(int signum, siginfo_t *siginfo, void *data) {
	static list_t *clients;
	client_t *client;
	
	list_t *ptr;

	if (!clients)
		clients = list_new(NULL); // NOTE: We're going to assume this succeeds.
	client = get_client(clients, siginfo->si_pid);
	if (client == NULL) {
		ptr = client_add(&clients, siginfo->si_pid, (size_t)siginfo->si_value.sival_ptr);
		printf("to_receive: %zu\n", clients->client->to_receive);
	}
	else {
		if (receive_message(client, (size_t)siginfo->si_value.sival_ptr) == FALSE)
			exit(1); // FIXME: Implement actually error handling.
		if (client->state == INACTIVE) { // FIXME: Temporarily just print the received string.
			char *temp = (char *)client->data;
			printf("Client state INACTIVE\n");
			printf("Received: %zu\n", client->received);
			printf("Received bytes:\n");
			for (size_t i = 0; i < client->received; i++)
				printf("byte %zu: %c (%d)\n", i, temp[i], temp[i]);
		}
		else
			return;
	}
}


int main(int argc, char *argv[]) {
  struct sigaction action;

  memset(&action, 0x0, sizeof(action)); // Clears any wacko memory that may be lurking.
  action.sa_sigaction = router;
  action.sa_flags = SA_SIGINFO;
  if (sigaction(SIGRTMIN, &action, NULL) == -1) {
    return (1); // Trouble setting the signal :0
  }

  printf("PID: %d\n", getpid());
  while (TRUE)
    sleep(10);
}
