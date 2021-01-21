#include "main.h"

SDL_Event event;
bool grid = true;
bool held = true;

int eventHandling() {
	// Polling events
	SDL_PollEvent(&event);

	switch (event.type) {
		// If the window has been closed
		case SDL_QUIT:
			return 1;
		
		// If key pressed: Used here for single press keys.
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_q:
					return 1;
				case SDLK_ESCAPE:
					return 1;
				case SDLK_RETURN:
					printf("Saving map.\n");
					saveMap("out.map");
					printf("Saved map to 'out.map'\n");
					break;
				case SDLK_g:
					grid = !grid;
					break;
				case SDLK_d:
					worldScrollX = worldScrollX + 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.x = map[x].worldRect.x - worldScrollX;
					}
					break;
				case SDLK_a:
					worldScrollX = worldScrollX - 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.x = map[x].worldRect.x - worldScrollX;
					}
					break;
				case SDLK_w:
					worldScrollY = worldScrollY - 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
					break;
				case SDLK_s:
					worldScrollY = worldScrollY + 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
					break;
				default:
					break;
			}
			break;
		
		case SDL_MOUSEMOTION:
			currentTile.rect.x = event.motion.x;
			currentTile.rect.y = event.motion.y;
			if (grid) {
				currentTile.rect.x = currentTile.rect.x - (currentTile.rect.x % 70);
				currentTile.rect.y = currentTile.rect.y - (currentTile.rect.y % 70);
			}
			currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
			currentTile.worldRect.y = currentTile.rect.y + worldScrollX;
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (!held) {
				map = realloc(map, ++mapSize * sizeof(*map));
				printf("%I64d\n", mapSize);
				map[mapSize - 1] = createTile("ground.png", currentTile.rect.x, currentTile.rect.y);
				held = true;
			}
			break;
		
		case SDL_MOUSEBUTTONUP:
			held = false;
			break;
		
		// Allows for the window to be resized, and to still function correctly; although resizing isnt recommended.
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					width = event.window.data1;
					height = event.window.data2;
					printf("Width: %d\nHeight:%d\n", width, height);
					break;
			}
	}
	return 0;
}