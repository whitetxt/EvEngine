#include "main.h"

SDL_Event event;

int eventHandling(struct Player *Player) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

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

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			return 1;
		
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_q:
					return 1;
				case SDLK_ESCAPE:
					return 1;
			}
	}
	return 0;
}