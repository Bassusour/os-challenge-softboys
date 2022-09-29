servermake: server.c
	gcc -o server server.c reversehash.c -lssl -lcrypto -lpthread -I.

clean:
	rm server

