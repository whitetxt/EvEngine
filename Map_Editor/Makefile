CC := gcc

CFLAGS := -g -Wall -Wextra -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -I/home/jrblo/src/GCC/include -L/home/jrblo/src/GCC/lib

SRCS := src/main.c src/objects/gameObj.c src/game.c src/events.c src/objects/player.c src/objects/maps.c

OBJS := $(SRCS:.c=.o)

EXEC := EvEngineEditor.exe

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)