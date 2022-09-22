servermake: server.c
	gcc -o server server.c reversehash.c -lssl -lcrypto -I.

clean:
	rm server

