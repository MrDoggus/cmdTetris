

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wshadow -Werror

SRCS = btetris-demo/main.c btetris-demo/console.c btetris-demo/tfront.c
OBJS = $(SRCS:%.c=%.o)

default: tetrisd

%.o: %.c $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

tetrisd: $(OBJS)
	$(CC) $(OBJS) -Wall -o $@

clean: 
	-rm src/*.o
	-rm btetris-demo/*.o