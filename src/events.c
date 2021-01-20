#include "main.h"

SDL_Event event;

int eventHandling(struct Player *Player) {
	// Using SDL_GetKeyboardState because that allows for held characters easily. SDL_KEYDOWN event does not work like this.
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	// Handling movement
	if (keys[SDL_SCANCODE_W]) 
		playerJump(Player);
	
	if (keys[SDL_SCANCODE_A]) 
		movePlayer(Player, 3);
	
	if (keys[SDL_SCANCODE_S]) {
		if (!Player->isCrouching)
			startCrouch(Player);
	} else {
		if (Player->isCrouching)
			endCrouch(Player);
	}
	
	if (keys[SDL_SCANCODE_D])
		movePlayer(Player, 1);

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
				default:
					break;
			}
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