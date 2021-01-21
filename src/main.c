#include "main.h"

SDL_Color Black = {255, 255, 255, 255};
struct Player MainPlayer;
int PrevTime = 0;
int MaxFPS = 120;
int dt = 0;

SDL_Texture **loadedTextures;
size_t textureSize;

struct Tile currentTile;

void render() {
	// Main render function. renders the player, all the tiles and all text
	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
	SDL_RenderClear(renderer);

	for (size_t x = 0; x < mapSize; x++) {
		SDL_RenderCopy(renderer, map[x].tex, NULL, &map[x].rect);
	}
	
	SDL_RenderCopy(renderer, currentTile.tex, NULL, &currentTile.rect);

	SDL_RenderPresent(renderer);
}

void calculateFPS() {
	/* 	Calculates how long a frame took to do the game loop
		and works out an approx framerate from that */
	int CurrentTime = SDL_GetTicks();
	dt = CurrentTime - PrevTime;
	// dt is used in other functions to allow physics to be framerate-independent
	if (dt != 0) {
		// Before I turned vsync on, dt was sometimes 0, therefore crashing when divide by 0.
		float EstFPS = 1000 / dt;
		PrevTime = CurrentTime;
		printf("FPS: %f - %dms taken.\n", EstFPS, dt);
	} else {
		printf("FPS: INF - 0ms taken.\n");
	}
	return;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	printf("Started.\n");

	if (initWindow() != 0) {
		printf("Window or renderer didn't initialise properly");
		return 1;
	}

	loadMap("out.map");

	// Create the first tile
	currentTile = createTile("ground.png", 0, 0);
	printf("Created tile.\n");

	// Main event loop.
	while (1) {
		// EventHandling returns 1 when it wants to quit the game.
		if (eventHandling() == 1)
			break;
		// All of these require passing the player through to allow for potential multiplayer.
		render();
		//calculateFPS();
	}

	printf("Closing.\n");

	// Destroys all the textures
	for (size_t x = 0; x < mapSize; x++) {
		SDL_DestroyTexture(map[x].tex);
	}

	// Destroy the window.
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0; 
}

