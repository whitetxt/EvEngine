#include "main.h"

struct Text createText(char *text, int x, int y) {
	SDL_Color Colour = {255, 255, 255, 255};
	TTF_Font *Font = TTF_OpenFont("fonts/aller.ttf", 20);
	SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, text, Colour);
	if(!TextSurface) {
		printf("Error drawing text: %s\n", TTF_GetError());
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		SDL_FreeSurface(tmpsurf);
		SDL_Rect tmpRect;
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		struct Text tmpText;
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex;
		return tmpText;
	} else {
		SDL_Texture *tmpTex = SDL_CreateTextureFromSurface(renderer, TextSurface);
		SDL_FreeSurface(TextSurface);
		SDL_Rect tmpRect;
		tmpRect.x = x;
		tmpRect.y = y;
		SDL_QueryTexture(tmpTex, NULL, NULL, &tmpRect.w, &tmpRect.h);
		struct Text tmpText;
		tmpText.rect = tmpRect;
		tmpText.tex = tmpTex;
		return tmpText;
	}
}

void removeText(struct Text *text) {
	for (size_t x = 0; x < textArrSize; x++) {
		if (textArr[x].tex == text->tex) {
			SDL_DestroyTexture(textArr[x].tex);
			for(size_t i = x; i < textArrSize - 1; i++)
				textArr[i] = textArr[i + 1];
			textArrSize -= 1;
			break;
		}
	}
}