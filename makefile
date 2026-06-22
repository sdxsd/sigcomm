##
# Sigcomm
#
# @file
# @version 0.1

CC = gcc
CFLAGS = -g
#CFLAGS = -Wall -Wextra -Werror -g
NAME_CLIENT = client
NAME_SERVER = server

CFILES_CLIENT = \
		src/client.c \
		src/sigcomm.c

CFILES_SERVER = \
		src/server.c \
		src/sigcomm.c

HEADERS = \
	include/sigcomm.h

OFILES_CLIENT = $(addprefix obj/,$(CFILES_CLIENT:.c=.o))
OFILES_SERVER = $(addprefix obj/,$(CFILES_SERVER:.c=.o))

all: client server

obj/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

client: $(OFILES_CLIENT)
	$(CC) $(CFLAGS) $(OFILES_CLIENT) -o $(NAME_CLIENT)

server: $(OFILES_SERVER)
	$(CC) $(CFLAGS) $(OFILES_SERVER) -o $(NAME_SERVER)

re: fclean all

fclean: clean
	@rm -f $(NAME_CLIENT)
	@rm -f $(NAME_SERVER)
	@echo "DEEP CLEANING"

clean:
	@rm -rf obj/
	@echo "CLEANED UP"

.PHONY: all re fclean clean

# end
