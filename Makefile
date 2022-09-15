servermake: server.c
	gcc -o server.o server.c -I.

clean:
	rm server

