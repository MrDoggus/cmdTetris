CC = gcc
CCWIN = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wshadow -Werror

TSRCS = btetris_control.c btetris_game.c btetris_board.c
SRCS = main.c tdraw.c

TOBJS = $(TSRCS:%.c=btetris-demo/binaries/%.o)
OBJS = $(SRCS:%.c=btetris-demo/binaries/%.o) 

OBJS_DIR = btetris-demo/binaries

TETRIS_WIDTH = 10
TETRIS_HEIGHT = 20
TETRIS_PP_SIZE = 2

DEFINES = -DTETRIS_WIDTH=$(TETRIS_WIDTH) -DTETRIS_HEIGHT=$(TETRIS_HEIGHT) -DTETRIS_PP_SIZE=$(TETRIS_PP_SIZE)

default: tetrisd

$(OBJS): $(OBJS_DIR)/%.o: btetris-demo/%.c
	$(CC) $(CFLAGS) -g -Isrc -Ibtetris-demo $(DEFINES) -c $^ -o $@ -lncurses

$(TOBJS): $(OBJS_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -g -Isrc -Ibtetris-demo $(DEFINES) -c $^ -o $@ -lncurses

tetrisd: $(OBJS) $(TOBJS)
	$(CC) -Wall $(OBJS) $(TOBJS) -o $@ -lncurses
	# strip $@

clean: 
	-rm src/*.o
	-rm btetris-demo/binaries/*.o
	-rm tetrisd