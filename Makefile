

CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wshadow -Werror

SRCS = src/btetris_board.c src/btetris_control.c src/btetris_game.c btetris-demo/main.c btetris-demo/console.c btetris-demo/tdraw.c 
OBJS = $(SRCS:%.c=%.o)

default: tetrisd

%.o: %.c $(SRCS)
	$(CC) $(CFLAGS) -Isrc -Ibtetris-demo -c $< -o $@

tetrisd: $(OBJS)
	$(CC) $(OBJS) -Wall -o $@

clean: 
	-rm src/*.o
	-rm btetris-demo/*.o