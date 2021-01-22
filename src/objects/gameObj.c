#include "../main.h"

SDL_Rect createRect(SDL_Texture *tex, int x, int y) {
	// Function to create a SDL_Rect from a texture and position.
	SDL_Rect rect;
	SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
	rect.x = x;
	rect.y = y;
	return rect;
}

SDL_Texture *loadImage(SDL_Renderer *renderer, char *fp) {
	// Function to create a texture from an image location.
	SDL_Texture *tmptex = IMG_LoadTexture(renderer, fp);
	// Check if the image was loaded correctly
	if (!tmptex) {
		printf("Failed to load image %s.\n", fp);
		// Create black box to replace image if it could not be loaded.
		SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
		tmptex = SDL_CreateTextureFromSurface(renderer, tmpsurf);
		SDL_FreeSurface(tmpsurf);
	}
	return tmptex;
}

struct Tile almostCreateTile(SDL_Texture *tex, SDL_Rect rect) {
	// Function to create a struct Tile from a SDL_Texture and SDL_Rect.
	struct Tile tmpTile;
	tmpTile.tex = tex;
	tmpTile.rect = rect;
	tmpTile.worldRect = rect;
	tmpTile.texIndex = selectedTexture;
	return tmpTile;
}

struct Tile createTile(char *fp, int x, int y) {
	// Wraps the 3 previous functions together for ease of use.
	SDL_Texture *tmpTex = loadImage(renderer, fp);
	SDL_Rect tmpRect = createRect(tmpTex, x, y);
	struct Tile tmpTile = almostCreateTile(tmpTex, tmpRect);
	return tmpTile;
}

struct Tile createTileFromTexture(SDL_Texture *tmpTex, int x, int y) {
	// Function to create a Tile from an already loaded texture.
	SDL_Rect tmpRect = createRect(tmpTex, x, y);
	struct Tile tmpTile = almostCreateTile(tmpTex, tmpRect);
	return tmpTile;
}