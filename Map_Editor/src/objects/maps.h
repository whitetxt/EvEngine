struct Map {
	uint8_t MapName[32];
	int64_t maxScrollX;
	uint64_t nTexturePaths;
	uint64_t nInteractablePaths;
	uint64_t nTiles;
	uint64_t nInteractables;
	char **TexturePaths;
	char **InteractableInactivePaths;
	char **InteractableActivePaths;
	struct TileSerialisation *Tiles;
	struct InteractableSerialisation *Interactables;
};

void saveMap(char *path);
void loadMap(char *path);
void loadTextures();

extern size_t mapSize;
extern struct Tile *map;

extern size_t interactableMapSize;
extern struct Interactable *interactableMap;