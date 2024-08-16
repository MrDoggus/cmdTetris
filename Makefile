

CC = gcc
CCWIN = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wshadow -Werror

TSRCS = btetris_control.c btetris_game.c btetris_board.c
SRCS = main.c tdraw.c

TOBJS = $(TSRCS:%.c=btetris-demo/binaries/%.o)
TOBJS_WIN = $(TSRCS:%.c=btetris-demo/winexe/%.o)
OBJS = $(SRCS:%.c=btetris-demo/binaries/%.o) 
OBJS_WIN = $(SRCS:%.c=btetris-demo/winexe/%.o) 

OBJS_DIR = btetris-demo/binaries
OBJS_WINDIR = btetris-demo/winexe

default: tetrisd tetrisd-win

$(OBJS): $(OBJS_DIR)/%.o: btetris-demo/%.c
	$(CC) $(CFLAGS) -Isrc -Ibtetris-demo -c $^ -o $@ -lncurses

$(TOBJS): $(OBJS_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -Isrc -Ibtetris-demo -c $^ -o $@ -lncurses

$(OBJS_WIN): $(OBJS_WINDIR)/%.o: btetris-demo/%.c
	$(CCWIN) $(CFLAGS) -Isrc -Ibtetris-demo -c $^ -o $@

$(TOBJS_WIN): $(OBJS_WINDIR)/%.o: src/%.c
	$(CCWIN) $(CFLAGS) -Isrc -Ibtetris-demo -c $^ -o $@


tetrisd: $(OBJS) $(TOBJS)
	$(CC) -Wall $(OBJS) $(TOBJS) -o $@ -lncurses
	strip $@

tetrisd-win: $(OBJS_WIN) $(TOBJS_WIN) 
	$(CCWIN) $(OBJS_WIN) $(TOBJS_WIN)  btetris-demo/pdcurses/pdcurses.a -Wall -o $@
	strip $@.exe

clean: 
	-rm src/*.o
	-rm btetris-demo/winexe/*.o
	-rm btetris-demo/binaries/*.o
	-rm tetrisd
	-rm tetrisd-win.exe