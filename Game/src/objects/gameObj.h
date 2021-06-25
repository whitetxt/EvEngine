struct Tile {
	SDL_Texture *tex;
	SDL_Rect rect;
	SDL_Rect worldRect;
};

struct TileSerialisation {
	uint64_t texIndex;
	uint32_t x, y;
};

struct Interactable {
	SDL_Texture *activeTex;
	SDL_Texture *inactiveTex;
	SDL_Rect rect;
	SDL_Rect worldRect;
	bool active;
};

struct InteractableSerialisation {
	uint64_t texIndex;
	uint32_t x, y;
	uint8_t active;
};

struct OtherPlayer {
	char *ID;
	char *name;
	bool crouching;
	SDL_Rect rect;
	SDL_Rect worldRect;
};

SDL_Rect createRect(SDL_Texture *tex, int x, int y);

SDL_Texture *loadImage(SDL_Renderer *renderer, char *fp);

struct Tile almostCreateTile(SDL_Texture *tex, SDL_Rect rect);

struct Tile createTile(char *fp, int x, int y);

struct Tile createTileFromTexture(SDL_Texture *tmpTex, int x, int y);

struct Interactable createInteractableFromTextures(int texIndex, int x, int y, uint8_t active);

void interactables(struct Player *Player);