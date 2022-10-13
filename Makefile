servermake: server.c
	gcc -g -o server server.c reversehash.c spinlock.c linkedlist.c -lssl -lcrypto -lpthread -I.

clean:
	rm server

