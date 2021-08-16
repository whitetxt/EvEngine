#include "include/main.h"

TTF_Font *menuFont;
TTF_Font *playerFont;
TTF_Font *titleFont;
TTF_Font *notifFont;
TTF_Font *mainMenuFont;

void initFonts() {
	menuFont = TTF_OpenFont("fonts/aller.ttf", Settings.menuSize);
	playerFont = TTF_OpenFont("fonts/aller.ttf", Settings.playerTextSize);
	titleFont = TTF_OpenFont("fonts/aller.ttf", Settings.titleSize);
	notifFont = TTF_OpenFont("fonts/aller.ttf", Settings.notifSize);
	mainMenuFont = TTF_OpenFont("fonts/aller.ttf", Settings.mainMenuSize);
}

// Creates a text object for the given text, font and position.
// @param text The text to be displayed.
// @param x The x position of the text.
// @param y The y position of the text.
// @param Font The font to be used.
// @return The text object.
Text createText(char *text, int x, int y, TTF_Font *Font) {
	// Creates the colour, loads the font and creates the text surface.
	SDL_Color Colour = {255, 255, 255, 255};
	SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, text, Colour);
	// Checks if the surface was created correctly.
	struct Text tmpText;
	SDL_Rect tmpRect;
	if(!TextSurface) {
		// Display error if it was not.
		printf("Error drawing text: %s\n", TTF_GetError());
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		// Create temporary black box to replace it.
		SDL_FreeSurface(tmpsurf);
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex;
	} else { 
		// Create the texture if the text was make correctly.
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, TextSurface);
		SDL_FreeSurface(TextSurface);
		// Prepare the SDL_Rect
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		// Prepare the struct Text
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex;
	}
	return tmpText;
}

// Creates a text object for the given text, font, colour and position.
// @param text The text to be displayed.
// @param x The x position of the text.
// @param y The y position of the text.
// @param Font The font to be used.
// @param Colour The colour to be used.
// @return The text object.
Text createColouredText(char *text, int x, int y, TTF_Font *Font, SDL_Color Colour) {
	// Loads the font and creates the text surface.
	SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, text, Colour);
	// Checks if the surface was created correctly.
	struct Text tmpText;
	SDL_Rect tmpRect;
	if(!TextSurface) {
		// Display error if it was not.
		printf("Error drawing text: %s\n", TTF_GetError());
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		// Create temporary black box to replace it.
		SDL_FreeSurface(tmpsurf);
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex; 
	} else { 
		// Create the texture if the text was make correctly.
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, TextSurface);
		SDL_FreeSurface(TextSurface);
		// Prepare the SDL_Rect
		SDL_Rect tmpRect;
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		// Prepare the struct Text
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex;
	}
	return tmpText;
}

// Frees a text object.
// @param Text The text object to be freed.
void freeText(struct Text text) {
	SDL_DestroyTexture(text.tex);
	return;
}