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
#include <signal.h>
#include <stdint.h>

list_t *list_new(client_t *client) {
  list_t *element;

  element = calloc(1, sizeof(list_t));
  if (!element)
    return (NULL);
  element->next = NULL;
  element->client = client;
  return (element);
}

void list_add_front(list_t **list, list_t *new) {
  new->next = *list;
  *list = new;
}

client_t *get_client(list_t *list_clients, pid_t pid) {
  if (!list_clients)
    return (NULL);
  if (!list_clients->client)
    return (NULL);
  if (list_clients->client->pid != pid) {
    if (list_clients->next)
      return (get_client(list_clients->next, pid));
    else
      return (NULL);
  }
  else
    return (list_clients->client);
}

client_t *new_client(pid_t pid, size_t to_receive) {
  client_t *client = calloc(1, sizeof(client_t));
  if (!client)
    return (NULL);
  client->pid = pid;
  client->state = RECEIVING;
  client->to_receive = to_receive;
  client->received = 0;
  client->allocated = INITIAL_ALLOC;
  client->data = calloc(1, INITIAL_ALLOC);
  if (!client->data) {
    free(client);
    return (NULL);
  }
  return (client);
}

int send_message(char *data, size_t bytes, pid_t receiver, int sig) {
	union sigval sigval;
	size_t buf;
	size_t sent = 0;
	size_t to_fill = sizeof(sigval.sival_ptr);

	if (!data || bytes == 0 || receiver < 0 || receiver > PID_MAX)
		return (FALSE);
	sigval.sival_ptr = (void *)bytes;
	if (sigqueue(receiver, sig, sigval) == -1)
		return (FALSE);

	while (sent < bytes) {
		buf = 0;
		if ((bytes - sent) < sizeof(sigval.sival_ptr))
			to_fill = bytes - sent;
		for (size_t i = 0; i < to_fill; i++)
			buf = (buf << 8) | (size_t)data[sent + i];
		sigval.sival_ptr = (void *)buf;
		if (sigqueue(receiver, sig, sigval) == -1)
			return (FALSE);
		sent += to_fill;
	}
	return (TRUE);
}

int receive_message(client_t *client, size_t data) {
	size_t to_fill = sizeof(void *);
	char *buf = (char *)client->data;

	if (!client || !client->data)
		return (FALSE);
	if ((client->to_receive - client->received) < to_fill)
		to_fill = (client->to_receive - client->received);
	if (client->received >= client->allocated) {
		if ((client->allocated * 2) > client->to_receive) {
			client->data = realloc(client->data, client->to_receive);
			client->allocated = client->to_receive;
		}
		else {
			client->data = realloc(client->data, (client->allocated * 2));
			client->allocated *= 2;
		}
		if (client->data == NULL) {
			client->data = buf;
			return (FALSE);
		}
		buf = (char *)client->data;
	}
	for (int i = to_fill - 1; i >= 0; i--) {
		buf[client->received + i] = (char)(data & 0xFFu);
		data >>= 8;
	}
	client->received += to_fill;
	if (client->received == client->to_receive)
		client->state = MESSAGE_RECEIVED;
	return (TRUE);
}

list_t *client_add(list_t **client_list, pid_t pid, size_t to_receive) {
  client_t *client;
  list_t *new_element;

  client = new_client(pid, to_receive);
  if (!client)
    return (NULL);
  new_element = list_new(client);
  if (!new_element) {
    free(client);
    return (NULL);
  }
  list_add_front(client_list, new_element);
  return (new_element);
}
