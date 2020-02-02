# Target library
lib := libuthread.a
objs := queue.o uthread.o

CC := gcc
CFLAGS := -Wall -Wextra

all: $(lib)

libuthread.a: $(objs)
	ar rcs libuthread.a $(objs)
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ $<
uthread.o: uthread.c uthread.h
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(objs)
