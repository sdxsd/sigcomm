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

#ifndef SIGCOMM_H
#define SIGCOMM_H

#define TRUE 1
#define FALSE 0
#define PID_MAX 4194304 // FIXME: Find a consistent way of getting this.
#define RECEIVING 1
#define MESSAGE_RECEIVED 0
#define INITIAL_ALLOC 8 * 4 // 8 bytes * 4 = 4 signals.

#include <signal.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> // FIXME: Temp

typedef struct client_s {
  pid_t pid; // PID of sender.
  int state; // RECEIVING / MESSAGE_RECEIVED
  size_t to_receive; // Bytes to receive.
  size_t received; // Bytes received.
  size_t allocated; // Bytes already allocated in *data
  void *data; // Raw data.
} client_t;

typedef struct list_s {
  struct list_s *next;
  client_t *client;
} list_t;

// Client Functions:
client_t *get_client(list_t *list_clients, pid_t pid);
client_t *new_client(pid_t pid, size_t to_receive);

// List Functions:
list_t *list_new(client_t *client);
void list_add_front(list_t **list, list_t *new);
list_t *client_add(list_t **client_list, pid_t pid, size_t to_receive);

// Client Functions:
int send_message(char *data, size_t bytes, pid_t receiver, int sig);

// Server Functions:
int receive_message(client_t *client, size_t data);

#endif // SIGCOMM_H
