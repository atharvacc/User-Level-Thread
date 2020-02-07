# Target library
lib := libuthread.a

objs := queue.o uthread.o context.o preempt.o libuthread.a

CC := gcc
CFLAGS := -Wall -Wextra -Werror

all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a: $(objs)
	ar rcs libuthread.a $(objs)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(DEFFLAGS)
clean:
	rm -f $(objs)
