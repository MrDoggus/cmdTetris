

CC = gcc
CCWIN = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wshadow -Werror

TSRCS = btetris_control.c btetris_game.c btetris_board.c
SRCS = main.c tdraw.c

TOBJS = $(TSRCS:%.c=btetris-demo/binaries/%.o)
OBJS = $(SRCS:%.c=btetris-demo/binaries/%.o) 

OBJS_DIR = btetris-demo/binaries

default: tetrisd

$(OBJS): $(OBJS_DIR)/%.o: btetris-demo/%.c
	$(CC) $(CFLAGS) -Isrc -Ibtetris-demo -c $^ -o $@ -lncurses

$(TOBJS): $(OBJS_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -Isrc -Ibtetris-demo -c $^ -o $@ -lncurses

tetrisd: $(OBJS) $(TOBJS)
	$(CC) -Wall $(OBJS) $(TOBJS) -o $@ -lncurses
	strip $@

clean: 
	-rm src/*.o
	-rm btetris-demo/binaries/*.o
	-rm tetrisd