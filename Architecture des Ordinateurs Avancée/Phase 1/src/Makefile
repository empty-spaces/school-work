CC=gcc
CFLAGS=-O2 -g -Wall
OBJS=driver.o kernel.o rdtsc.o

all: baseline

baseline:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) baseline
