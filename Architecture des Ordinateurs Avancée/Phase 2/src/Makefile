CC=gcc
CFLAGS=-Ofast -march=native -funroll-loops -g -Wall
OBJS=driver.o kernel.o rdtsc.o

all: baseline

baseline:	$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

kernel.o: kernel.c
	$(CC) $(CFLAGS) -D $(OPT) -c $< -o $@

clean:
	rm -rf $(OBJS) baseline
