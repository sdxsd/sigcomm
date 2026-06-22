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
#include <stdio.h>

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
		if (client->state == MESSAGE_RECEIVED) { // FIXME: Temporarily just print the received string.
			char *temp = (char *)client->data;
			printf("Client state: MESSAGE_RECEIVED\n");
			printf("Received: %zu\n", client->received);
			printf("Received bytes:\n");
			for (size_t i = 0; i < client->received; i++)
				printf("%c", temp[i]);
			printf("\n");
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
