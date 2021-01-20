#include "../main.h"

SDL_Rect createRect(SDL_Texture *tex, int x, int y) {
	SDL_Rect rect;
	SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
	rect.x = x;
	rect.y = y;
	return rect;
}

SDL_Texture *loadImage(SDL_Renderer *renderer, char *fp) {
	SDL_Texture *tmptex = IMG_LoadTexture(renderer, fp);
	if (!tmptex) {
		printf("Failed to load image %s.\n", fp);
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		tmptex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		SDL_FreeSurface(tmpsurf);
	}
	return tmptex;
}

struct Tile almostCreateTile(SDL_Texture *tex, SDL_Rect rect) {
	struct Tile tmpTile;
	tmpTile.tex = tex;
	tmpTile.rect = rect;
	tmpTile.worldRect = rect;
	return tmpTile;
}

struct Tile createTile(char *fp, int x, int y) {
	SDL_Texture *tmpTex = loadImage(renderer, fp);
	SDL_Rect tmpRect = createRect(tmpTex, x, y);
	struct Tile tmpTile = almostCreateTile(tmpTex, tmpRect);
	return tmpTile;
}