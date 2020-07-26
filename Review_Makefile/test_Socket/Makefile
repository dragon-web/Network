CC := gcc
TAR = Sever Client
all : $(TAR)
.PHONY: all
Server:Server.c
	$(CC) -o Sever Sever.c
Client:Client.c
	$(CC) -o Client Client.c
.PHONY:
clear:
	rm -f $(TAR)

