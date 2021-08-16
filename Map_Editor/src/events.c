#include "main.h"

SDL_Event event;
bool grid = true;
bool heldMouse = false;
bool heldLeft = false;
bool heldRight = false;
bool heldI = false;
bool heldSpace = false;
bool interactableMode = false;
bool interactableActive = false;

int gridSize = 64;

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
					worldScrollX = worldScrollX + gridSize;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.x = map[x].worldRect.x - worldScrollX;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_a:
					worldScrollX = worldScrollX - gridSize;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.x = map[x].worldRect.x - worldScrollX;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_w:
					worldScrollY = worldScrollY - gridSize;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_s:
					worldScrollY = worldScrollY + gridSize;
					for (size_t x = 0; x < mapSize; x++) {
						map[x].rect.y = map[x].worldRect.y - worldScrollY;
					}
					currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
					currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
					break;
				case SDLK_RIGHT:
					if (!heldRight) {
						heldRight = true;
						if (!interactableMode) {
							selectedTexture++;
							if (selectedTexture >= textureSize)
								selectedTexture = 0;
							currentTile.tex = loadedTextures[selectedTexture];
						} else {
							selectedInteractable++;
							if (selectedInteractable >= interactableSize)
								selectedInteractable = 0;
							if (interactableActive)
								currentTile.tex = loadedInteractablesActive[selectedInteractable];
							else
								currentTile.tex = loadedInteractablesInactive[selectedInteractable];
						}
					}
					break;
				case SDLK_LEFT:
					if (!heldLeft) {
						heldLeft = true;
						if (!interactableMode) {
							if (selectedTexture == 0)
								selectedTexture = textureSize;
							selectedTexture -= 1;
							currentTile.tex = loadedTextures[selectedTexture];
						} else {
							if (selectedInteractable == 0)
								selectedInteractable = interactableSize;
							selectedInteractable -= 1;
							if (interactableActive)
								currentTile.tex = loadedInteractablesActive[selectedInteractable];
							else
								currentTile.tex = loadedInteractablesInactive[selectedInteractable];
						}
					}
					break;
				case SDLK_i:
					if (!heldI) {
						printf("Interactable: %d\n", interactableMode);
						heldI = true;
						interactableMode = !interactableMode;
						if (interactableMode) {
							if (interactableActive) {
								currentTile.tex = loadedInteractablesActive[selectedInteractable];
								printf("active\n");
							} else {
								currentTile.tex = loadedInteractablesInactive[selectedInteractable];
								printf("Inactive\n");
							}
						} else {
							printf("Tile\n");
							currentTile.tex = loadedTextures[selectedTexture];
						}
					}
					break;
				case SDLK_SPACE:
					if (!heldSpace) {
						heldSpace = true;
						interactableActive = !interactableActive;
						if (interactableActive)
							currentTile.tex = loadedInteractablesActive[selectedInteractable];
						else
							currentTile.tex = loadedInteractablesInactive[selectedInteractable];
					}
					break;
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
				case SDLK_SPACE:
					heldSpace = false;
					break;
				case SDLK_i:
					heldI = false;
					break;
				default:
					break;
			}
			break;
		
		case SDL_MOUSEMOTION:
			currentTile.rect.x = event.motion.x;
			currentTile.rect.y = event.motion.y;
			if (grid) {
				currentTile.rect.x = currentTile.rect.x - (currentTile.rect.x % gridSize);
				currentTile.rect.y = currentTile.rect.y - (currentTile.rect.y % gridSize);
			}
			currentTile.worldRect.x = currentTile.rect.x + worldScrollX;
			currentTile.worldRect.y = currentTile.rect.y + worldScrollY;
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (!heldMouse) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (!interactableMode) {
						map = realloc(map, ++mapSize * sizeof(*map));
						map[mapSize - 1] = createTileFromTexture(loadedTextures[selectedTexture], currentTile.worldRect.x, currentTile.worldRect.y);
						heldMouse = true;
						map[mapSize - 1].rect.x = map[mapSize - 1].worldRect.x - worldScrollX;
						map[mapSize - 1].rect.y = map[mapSize - 1].worldRect.y - worldScrollY;
					} else {
						interactableMap = realloc(interactableMap, ++interactableMapSize * sizeof(*interactableMap));
						if (interactableActive)
							interactableMap[interactableMapSize - 1] = createInteractableFromTextures(selectedInteractable, currentTile.worldRect.x, currentTile.worldRect.y, 1);
						else
							interactableMap[interactableMapSize - 1] = createInteractableFromTextures(selectedInteractable, currentTile.worldRect.x, currentTile.worldRect.y, 0);
						heldMouse = true;
						interactableMap[interactableMapSize - 1].rect.x = interactableMap[interactableMapSize - 1].worldRect.x - worldScrollX;
						interactableMap[interactableMapSize - 1].rect.y = interactableMap[interactableMapSize - 1].worldRect.y - worldScrollY;
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