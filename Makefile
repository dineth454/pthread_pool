CC = gcc
CFLAGS = -Wall -pthread

all: thread_pool_example

thread_pool_example: main.o thread_pool.o
	$(CC) $(CFLAGS) -o thread_pool_example main.o thread_pool.o

main.o: main.c thread_pool.h
	$(CC) $(CFLAGS) -c main.c

thread_pool.o: thread_pool.c thread_pool.h
	$(CC) $(CFLAGS) -c thread_pool.c

clean:
	rm -f *.o thread_pool_example
