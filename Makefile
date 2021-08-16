CC = gcc

CFLAGS = -g -Wall -Wextra -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lws2_32 -I../GCC/include -L../GCC/lib

SRCS = src/main.c src/objects/gameObj.c src/game.c src/events.c src/objects/player.c src/objects/maps.c src/menu/text.c src/settings.c src/client.c ../GCC/include/ini.c src/menu/menu.c src/notif.c src/menu/mainMenu.c src/pvp/pvp.c

OBJS = $(SRCS:.c=.o)

EXEC = EvEngine.exe

all: $(EXEC)

new: clean $(EXEC)

$(EXEC): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)