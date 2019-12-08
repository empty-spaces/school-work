CC=gcc
CFLAGS=-W -Wall
OUT=lexa

all: $(OUT)

afn: afn.o main_afn.o
	$(CC) -o $@ $^

afd: afn.o afd.o main_afd.o
	$(CC) -o $@ $^

lexa: afn.o afd.o main.o
	$(CC) -o $@ $^

afn.o: afn.c
	$(CC) -o $@ -c $< $(CFLAGS)

afd.o: afd.c
	$(CC) -o $@ -c $< $(CFLAGS)

main.o: main.c afn.h afd.h
	$(CC) -o $@ -c $< $(CFLAGS)

main_afn.o: main_afn.c afn.h
	$(CC) -o $@ -c $< $(CFLAGS)

main_afd.o: main_afd.c afn.h afd.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o



