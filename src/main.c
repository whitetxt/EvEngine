#include "main.h"

SDL_Color Black = {255, 255, 255, 255};
struct Player MainPlayer;
int PrevTime = 0;
int MaxFPS = 120;
int dt = 0;

SDL_Texture **loadedTextures;
size_t textureSize;

SDL_Texture **loadedInteractablesActive;
SDL_Texture **loadedInteractablesInactive;
size_t interactableSize;

size_t textArrSize = 0;
struct Text *textArr = NULL;

void render() {
	// Main render function. renders the player, all the tiles and all text
	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
	SDL_RenderClear(renderer);
	if (MainPlayer.isCrouching)
		SDL_RenderCopy(renderer, MainPlayer.crouchTex, NULL, &MainPlayer.rect);
	else
		SDL_RenderCopy(renderer, MainPlayer.tex, NULL, &MainPlayer.rect);
	for (size_t x = 0; x < mapSize; x++)
		SDL_RenderCopy(renderer, map[x].tex, NULL, &map[x].rect);

	for (size_t x = 0; x < textArrSize; x++) 
		SDL_RenderCopy(renderer, textArr[x].tex, NULL, &textArr[x].rect);
	
	SDL_RenderPresent(renderer);
}

void calculateDT() {
	// Calculates the deltatime (Time to perform all actions and render.)
	int CurrentTime = SDL_GetTicks();
	dt = CurrentTime - PrevTime;
	PrevTime = CurrentTime;
	// dt is used in other functions to allow physics to be framerate-independent
}

void calculateFPS() {
	// Calculates the FPS from deltatime.
	if (dt != 0) {
		// Before I turned vsync on, dt was sometimes 0, therefore crashing when divide by 0.
		float EstFPS = 1000 / dt;
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

	// Create the player
	MainPlayer = createPlayer("Player.png", "PlayerCrouch.png", 0, 0, 0.1, 0.0375);

	// Load the first map
	loadMap("out.map");

	// Resize the text array and create new text.
	textArr = realloc(textArr, ++textArrSize * sizeof(*textArr));
	textArr[0] = createText("Hello, this is a test", 200, 200);

	// Main event loop.
	while (1) {
		// EventHandling returns 1 when it wants to quit the game.
		if (eventHandling(&MainPlayer) == 1)
			break;
		// All of these require passing the player through to allow for potential multiplayer.
		grav(&MainPlayer);
		playerCollision(&MainPlayer);
		render();
		calculateDT();
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

