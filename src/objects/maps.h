struct MapHeader {
	uint8_t MapName[32];
	uint64_t maxScrollX;
	uint64_t maxScrollY;
	uint64_t nTexturePaths;
	uint64_t nTiles;
	char **TexturePaths;
	struct TileSerialisation *Tiles;
};

void saveMap(char *path);
void loadMap(char *path);
void loadTextures();

extern size_t mapSize;
extern struct Tile *map;