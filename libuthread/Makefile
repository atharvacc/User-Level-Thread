# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o preempt.o

CC := gcc
CFLAGS := -Wall -Wextra

all: $(lib)

libuthread.a: $(objs)
	ar rcs libuthread.a $(objs)
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ $<
uthread.o: uthread.c uthread.h
	$(CC) $(CFLAGS) -c -o $@ $<
context.o: context.c context.h
	$(CC) $(CFLAGS) -c -o $@ $<
preempt.o: preempt.c preempt.h
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(objs)
