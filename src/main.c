#include "main.h"

SDL_Color Black = {255, 255, 255, 255};
struct Player MainPlayer;
int PrevTime = 0;
int MaxFPS = 120;
int dt = 0;

size_t textArrSize = 0;
struct Text *textArr = NULL;

void render() {
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

void calculateFPS() {
	int CurrentTime = SDL_GetTicks();
	dt = CurrentTime - PrevTime;
	if (dt != 0) {
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

	MainPlayer = createPlayer("Player.png", "PlayerCrouch.png", 0, 0, 0.4, 0.15);

	loadMap("1.map");

	textArr = realloc(textArr, ++textArrSize * sizeof(*textArr));
	textArr[0] = createText("Hello, this is a test", 200, 200);

	while (1) {
		if (eventHandling(&MainPlayer) == 1)
			break;
		grav(&MainPlayer);
		playerCollision(&MainPlayer);
		render();
		calculateFPS();
	}

	printf("Closing.\n");

	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0; 
}

