#define ZED_NET_IMPLEMENTATION
#include <zed_net.h>
#include "include/main.h"

SDL_Color White = {255, 255, 255, 255};
Player MainPlayer;
int PrevTime = 0;
int MaxFPS = 120;
bool paused = false;
double dt = 0;
bool playing;

SDL_Texture *playerTexture;
SDL_Texture *playerTextureCrouch;
struct OtherPlayer *players = NULL;
size_t numPlayers = 0;

SDL_Texture **loadedTextures;
size_t textureSize;

SDL_Texture **loadedInteractablesActive;
SDL_Texture **loadedInteractablesInactive;
size_t interactableSize;

size_t textArrSize = 0;
struct Text *textArr = NULL;

bool onServer = false;

void render() {
	// Main render function. renders the player, all the tiles and all text
	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
	SDL_RenderClear(renderer);
	renderPlayer(&MainPlayer);

	for (size_t x = 0; x < mapSize; x++)
		if (map[x].rect.x < width)
			SDL_RenderCopy(renderer, map[x].tex, NULL, &map[x].rect);

	for (size_t x = 0; x < textArrSize; x++)
		if (textArr[x].rect.x < width)
			SDL_RenderCopy(renderer, textArr[x].tex, NULL, &textArr[x].rect);

	for (size_t x = 0; x < interactableMapSize; x++) {
		if (interactableMap[x].rect.x < width) {
			if (interactableMap[x].active)
				SDL_RenderCopy(renderer, interactableMap[x].activeTex, NULL, &interactableMap[x].rect);
			else
				SDL_RenderCopy(renderer, interactableMap[x].inactiveTex, NULL, &interactableMap[x].rect);
		}
	}

	for (size_t x = 0; x < numPlayers; x++) {
		if (players[x].rect.x < width) {
			if (players[x].crouching) {
				SDL_QueryTexture(playerTextureCrouch, NULL, NULL, &players[x].rect.w, &players[x].rect.h);
				SDL_RenderCopy(renderer, playerTextureCrouch, NULL, &players[x].rect);
			} else {
				SDL_QueryTexture(playerTexture, NULL, NULL, &players[x].rect.w, &players[x].rect.h);
				SDL_RenderCopy(renderer, playerTexture, NULL, &players[x].rect);
			}
		}
	}
}

void calculateDT() {
	int CurrentTime = SDL_GetPerformanceCounter();
	dt = (CurrentTime - PrevTime) * 1000;
	dt = dt / SDL_GetPerformanceFrequency() / 1000;
	PrevTime = CurrentTime;
}

void calculateFPS() {
	// Calculates the FPS from deltatime.
	if (dt != 0) {
		// Before I turned vsync on, dt was sometimes 0, therefore crashing when divide by 0.
		double EstFPS = 1000 / (dt * 1000);
		printf("FPS: %f - %fms taken.\n", EstFPS, dt * 1000);
	} else {
		printf("FPS: INF - 0ms taken.\n");
	}
	return;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	printf("Started.\n");

	loadSettings();

	if (initWindow() != 0) {
		printf("Window or renderer didn't initialise properly");
		return 1;
	}

	initFonts();
	SDL_StartTextInput();

	// Create the player
	MainPlayer = createPlayer("Player.png", "PlayerCrouch.png", 0, 0, 100, 40);

	SDL_Thread *getUpdate;
	SDL_Thread *alive;

	if (mainMenu() == -1) {
		playing = false;
	} else {
		playing = true;
	}
	createMenu("main.menu");
	paused = true;

	// Main event loop.
	while (playing) {
		if (!paused) {
			// EventHandling returns 1 when it wants to quit the game.
			if (eventHandling(&MainPlayer) == 1) {
				break;
			}
			// All of these require passing the player through to allow for potential multiplayer.
			grav(&MainPlayer);
			playerCollision(&MainPlayer);
			updateWorldScroll(&MainPlayer);
			if (onServer) { // If multiplayer
				sendPos();
				updateMultiplayerNames();
			}
			render();
		} else {
			int retVal = pauseEventHandling();
			if (retVal == 1) {
				break;
			} else if (retVal == 2) {
				if (setupClient() != 0) {
					printf("There was an error setting up the client.\n");
					break;
				}
				connectToServer();

				// Recieves info from the server and parses it

				playerTexture = loadImage(renderer, "OtherPlayer.png");
				playerTextureCrouch = loadImage(renderer, "OtherPlayerCrouch.png");

				getUpdate = SDL_CreateThread(getUpdates, "updateThread", NULL);
				if (!getUpdate) {
					printf("Failed to start the update thread: %s\n", SDL_GetError());
					break;
				}
				alive = SDL_CreateThread(stayAlive, "aliveThread", NULL);
				if (!alive) {
					printf("Failed to start the alive thread: %s\n", SDL_GetError());
					break;
				}
				loadMap("levels/level1.map");
				onServer = true;
				paused = false;
			}
			renderMenu();
		}
		renderToasts();
		SDL_RenderPresent(renderer);
		calculateDT();
		//calculateFPS();
	}

	if (onServer) {
		printf("Disconnecting.\n");

		// Disconnect message kills all the threads
		sendMsg("DC");

		int returnVal;
		// Wait for threads to return
		SDL_WaitThread(getUpdate, &returnVal);
		SDL_WaitThread(alive, &returnVal);
		printf("Disconnected.\n");
		zed_net_shutdown();
	}

	printf("Saving settings.\n");
	if (saveSettings() != 0) {
		printf("Failed to save settings. All changes will be lost.\nSorry.");
	}

	printf("Closing.\n");

	// Destroys all the textures
	for (size_t x = 0; x < mapSize; x++) {
		SDL_DestroyTexture(map[x].tex);
	}
	for (size_t x = 0; x < interactableMapSize; x++) {
		SDL_DestroyTexture(interactableMap[x].activeTex);
		SDL_DestroyTexture(interactableMap[x].inactiveTex);
	}
	for (size_t x = 0; x < textArrSize; x++) {
		SDL_DestroyTexture(textArr[x].tex);
	}
	if (onServer) {
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyTexture(playerTextureCrouch);
	}
	SDL_DestroyTexture(MainPlayer.tex);
	SDL_DestroyTexture(MainPlayer.crouchTex);

	// Destroy the window.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0; 
}

