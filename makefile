src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

agrs = -Wall -g

ALL:server client

server: server.o wrap.o
	gcc server.o wrap.o -o server -Wall

client: client.o wrap.o
	gcc client.o wrap.o -o client -Wall

%.o:%.c
	gcc -c $< -Wall

clean:
	-rm -rf server client

.PHONY: ALL clean

