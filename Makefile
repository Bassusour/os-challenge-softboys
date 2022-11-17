servermake: server.c
	gcc -o3 -o server server.c reversehash.c spinlock.c hashcache.c  linkedlist.c -lssl -lcrypto -lpthread -I.

clean:
	rm server

