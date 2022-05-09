CFLAGS = -Wall -g -O2 -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS = -lm
SRCS = *.c *.h
CC = gcc
build: tema3

tema3: main.c
	$(CC) $(CFLAGS) -o tema3 $(SRCS) $(LDFLAGS)

clean: 
	rm -f *.o tema3
