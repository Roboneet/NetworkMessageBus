server : local_server.c nmb.c utils.h
	gcc local_server.c nmb.c -o server.o

client : driver.c nmb.c utils.h
	gcc driver.c nmb.c -o client.o

