#ifndef BUILTINS_H
#define BUILTINS_H

#define WRITE 1
#define READ 0

#include <stdio.h>
#include "../include/sigcomm.h"

void *simple_exec(client_t *client, void *data);
void *print_message(client_t *client, void *data);

#endif // BUILTINS_H
