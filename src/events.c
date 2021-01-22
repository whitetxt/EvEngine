#include "main.h"

SDL_Event event;
bool grid = true;
bool heldMouse = false;
bool heldLeft = false;
bool heldRight = false;

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
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_a:
					worldScrollX = worldScrollX - 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.x = map[x].worldRect.x - worldScrollX;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_w:
					worldScrollY = worldScrollY - 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_s:
					worldScrollY = worldScrollY + 70;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_RIGHT:
					if (!heldRight) {
						heldRight = true;
						selectedTexture++;
						if (selectedTexture >= textureSize)
							selectedTexture = 0;
						currentTile.tex = loadedTextures[selectedTexture];
					}
					break;
				case SDLK_LEFT:
					if (!heldLeft) {
						heldLeft = true;
						if (selectedTexture == 0)
							selectedTexture = textureSize;
						selectedTexture -= 1;
						currentTile.tex = loadedTextures[selectedTexture];
					}
				default:
					break;
			}
			break;
		
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					heldRight = false;
					break;
				case SDLK_LEFT:
					heldLeft = false;
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
			currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (!heldMouse) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					map = realloc(map, ++mapSize * sizeof(*map));
					printf("%I64d\n", mapSize);
					map[mapSize - 1] = createTileFromTexture(loadedTextures[selectedTexture], currentTile.worldRect.x, currentTile.worldRect.y);
					printf("created tile\n");
					heldMouse = true;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.x = map[x].worldRect.x - worldScrollX;
					}
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					SDL_Rect mouseRect;
					mouseRect.x = currentTile.rect.x;
					mouseRect.y = currentTile.rect.y;
					mouseRect.w = 5;
					mouseRect.h = 5;
					SDL_Rect returnRect;
					for (size_t x = 0; x < mapSize; x++) {
						if (SDL_IntersectRect(&mouseRect, &map[x].rect, &returnRect)) {
							for(size_t i = x; i < mapSize - 1; i++)
								map[i] = map[i + 1];
							mapSize -= 1;
						}
					}
				}
			}
			break;
		
		case SDL_MOUSEBUTTONUP:
			heldMouse = false;
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