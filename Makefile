servermake: server.c
	gcc -o server server.c -I.

clean:
	rm server

