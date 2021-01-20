struct Tile {
	SDL_Texture *tex;
	SDL_Rect rect;
	SDL_Rect worldRect;
};

SDL_Rect createRect(SDL_Texture *tex, int x, int y);

SDL_Texture *loadImage(SDL_Renderer *renderer, char *fp);

struct Tile almostCreateTile(SDL_Texture *tex, SDL_Rect rect);

struct Tile createTile(char *fp, int x, int y);