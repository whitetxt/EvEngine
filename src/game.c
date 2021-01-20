#include "main.h"

SDL_Window *win;
SDL_Renderer *renderer;
int height = 768;
int width = 768;

int initWindow() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	if(TTF_Init() == -1) {
		printf("Error initialising SDL_ttf: %s\n", TTF_GetError());
		return 1;
	}

	Uint32 winFlags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI;

	win = SDL_CreateWindow("Unfedora", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, height, width, winFlags);

	if (win == NULL) {
		printf("SDL had an error creating the window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	
	printf("Initialised Window.\n");

	Uint32 rendFlags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
	
	renderer = SDL_CreateRenderer(win, -1, rendFlags);

	if (renderer == NULL) {
		printf("SDL had an error creating the renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	return 0;
}