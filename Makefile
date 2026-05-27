CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc

FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm
SDL_WIN_INCLUDE = -I/usr/x86_64-w64-mingw32/include/SDL2
SDL_WIN_LIB = -L/usr/x86_64-w64-mingw32/lib

linux:
	$(CC_LINUX) src/asteroids.c -o asteroids $(FLAGS)

windows:
	$(CC_WINDOWS) src/asteroids.c -o asteroids.exe $(SDL_WIN_INCLUDE) $(SDL_WIN_LIB) $(FLAGS) -mwindows

all: linux windows

clean:
	rm -f asteroids asteroids.exe