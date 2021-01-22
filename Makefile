CC := x86_64-w64-mingw32-gcc

CFLAGS := -g -Wall -Wextra -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -I/home/evelyn/C/GCC/include -L/home/evelyn/C/GCC/lib

SRCS := src/main.c src/objects/gameObj.c src/game.c src/events.c src/objects/player.c src/objects/maps.c src/text.c

OBJS := $(SRCS:.c=.o)

EXEC := EvEngine.exe

all: $(EXEC)
	 cp $(EXEC) /mnt/hgfs/Ubuntu_20.04/EvEngine/$(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)