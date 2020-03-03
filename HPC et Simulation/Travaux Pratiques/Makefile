CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
EXECS=generator pagerank

generator: src/generator.c
	$(CC) -o generator src/generator.c

pagerank: src/pagerank.c
	$(CC) -o pagerank src/pagerank.c

clean:
	rm -rf $(EXECS)

all: generator pagerank