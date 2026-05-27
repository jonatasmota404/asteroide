CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc

FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm
SDL_WIN_INCLUDE = -I/usr/x86_64-w64-mingw32/include/SDL2
SDL_WIN_LIB = -L/usr/x86_64-w64-mingw32/lib

linux:
	$(CC_LINUX) src/asteroids.c -o asteroids $(FLAGS)

windows:
	$(CC_WINDOWS) src/asteroids.c -o asteroids.exe $(SDL_WIN_INCLUDE) $(SDL_WIN_LIB) $(FLAGS) -mwindows

appimage: linux
	cp asteroids AppDir/usr/bin/
	./linuxdeploy-x86_64.AppImage \
		--appdir AppDir \
		--executable AppDir/usr/bin/asteroids \
		--desktop-file AppDir/usr/share/applications/asteroids.desktop \
		--icon-file AppDir/usr/share/icons/hicolor/256x256/apps/asteroids.png \
		--output appimage

all: linux windows

clean:
	rm -f asteroids asteroids.exe