#include "main.h"

struct Text createText(char *text, int x, int y) {
	// Creates the colour, loads the font and creates the text surface.
	SDL_Color Colour = {255, 255, 255, 255};
	TTF_Font *Font = TTF_OpenFont("fonts/aller.ttf", 20);
	SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, text, Colour);
	// Checks if the surface was created correctly.
	if(!TextSurface) {
		// Display error if it was not.
		printf("Error drawing text: %s\n", TTF_GetError());
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		// Create temporary black box to replace it.
		SDL_FreeSurface(tmpsurf);
		SDL_Rect tmpRect;
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		struct Text tmpText;
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex; 
		// Return the black box as a struct Text.
		return tmpText;
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
		struct Text tmpText;
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex;
		return tmpText;
	}
}

void removeText(struct Text *text) {
	// Function to remove text from the drawing array, and move all other objects.
	for (size_t x = 0; x < textArrSize; x++) {
		// Check if the texture is the one specified. TODO: Add names to the text,
		// to allow checking for the name because this method will currently delete
		// the first occurance of a texture, which may not be the one intended to be
		// deleted.
		if (textArr[x].tex == text->tex) {
			SDL_DestroyTexture(textArr[x].tex);
			for(size_t i = x; i < textArrSize - 1; i++)
				textArr[i] = textArr[i + 1];
			textArrSize -= 1;
			break;
		}
	}
}