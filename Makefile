filename = build/EvEngine.exe

CC = gcc
CFLAGS = -g -Wall -Wextra -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lws2_32 -I../GCC/include -L../GCC/lib

files = ${wildcard src/*.c}
objFiles = ${files:.c=.o}

all:
	${CC} -o ${filename} ${files} ${CFLAGS} 

clean:
	-rm -rf ${filename}
	-rm -rf ${objFiles}

run:
	${CC} -o ${filename} ${files} ${CFLAGS}
	cd build && ../${filename}