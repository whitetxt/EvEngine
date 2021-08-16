#include "../main.h"

struct Text bottomText;
struct Text mainTitle;

SDL_Event mainMenuEv;

double frame = 0;

int mainMenu() {
	createMainMenu();
	while (1) {
		renderMainMenu();
		int retVal = mainMenuEventHandling();
		if (retVal == 1) {
			return -1;
		} else if (retVal == 2) {
			return 0;
		}
		calculateDT();
	}
}

void renderMainMenu() {
	// Special function for rendering the menu.
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderCopyEx(renderer, mainTitle.tex, NULL, &mainTitle.rect, sin(frame) * 10, NULL, SDL_FLIP_NONE);
	frame += dt;

	SDL_RenderCopy(renderer, bottomText.tex, NULL, &bottomText.rect);
	SDL_RenderPresent(renderer);
}

void createMainMenu() {
	// Create text, could have made it read from a text file but thats too much effort.
	bottomText = createColouredText("Press any key to continue", width / 2, height - 40, menuFont, Black);
	bottomText.rect.x = width / 2 - (bottomText.rect.w / 2);
	mainTitle = createColouredText("EvEngine", width / 2, 20, mainMenuFont, Black);
	mainTitle.rect.x = width / 2 - (mainTitle.rect.w / 2);
}

int mainMenuEventHandling() {
	// Polling events
	SDL_PollEvent(&mainMenuEv);

	switch (mainMenuEv.type) {
		// If the window has been closed
		case SDL_QUIT:
			return 1;
		
		// If key pressed: Used here for single press keys.
		case SDL_KEYDOWN:
			return 2;
	}
	return 0;
}

void destroyMainMenu() {
	// Cleanup
	SDL_DestroyTexture(bottomText.tex);
	SDL_DestroyTexture(mainTitle.tex);
}