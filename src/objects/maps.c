#include "../main.h"

size_t mapSize = 0;
struct Tile *map = NULL;

void saveMap(char *path) {
	// Function to save the currently rendered map to a file.
	struct MapHeader m;
	// Get a map name
	printf("Please enter the name for the map (Max 32 chars).\n");
	scanf("%s", m.MapName);

	m.nTiles = mapSize;
	m.nTexturePaths = 1;
	m.Tiles = malloc(m.nTiles * sizeof(*m.Tiles));
	m.maxScrollX = 0;
	m.maxScrollY = 0;
	for (uint64_t x = 0; x < m.nTiles; x++) {
		m.Tiles[x].texIndex = map[x].texIndex;
		m.Tiles[x].x = map[x].worldRect.x;
		m.Tiles[x].y = map[x].worldRect.y;
		if (m.Tiles[x].x + map[x].worldRect.w > m.maxScrollX)
			m.maxScrollX = m.Tiles[x].x + map[x].worldRect.w;
		if (m.Tiles[x].y + map[x].worldRect.h > m.maxScrollY)
			m.maxScrollY = m.Tiles[x].y + map[x].worldRect.h;
	}
	printf("Created tiles\n");
	m.TexturePaths = malloc(m.nTexturePaths * sizeof(*m.TexturePaths));
	m.TexturePaths[0] = "ground.png";
	printf("Set texture path\n");
	FILE *outFile;
	printf("Declared file.\n");
	// Open the output file
	outFile = fopen(path, "wb");
	printf("Opened file.\n");
	if (!outFile) {
		printf("Unable to open output file.\n");
		return;
	}
	// Write the header followed by the map itself.
	fwrite(&m.MapName, sizeof(m.MapName), 1, outFile);
	fwrite(&m.maxScrollX, sizeof(m.maxScrollX), 1, outFile);
	fwrite(&m.maxScrollY, sizeof(m.maxScrollY), 1, outFile);
	fwrite(&m.nTexturePaths, sizeof(m.nTexturePaths), 1, outFile);
	printf("Written ntexturepaths\n");
	fwrite(&m.nTiles, sizeof(m.nTiles), 1, outFile);
	char **curpath = m.TexturePaths;
	for (size_t x = 0; x < m.nTexturePaths; x++) {
		fwrite(*curpath, strlen(*curpath), 1, outFile);
		fputc('\n', outFile);
		curpath++;
	}
	printf("written texturepaths\n");
	for (uint64_t x = 0; x < m.nTiles; x++) {
		fwrite(&m.Tiles[x].texIndex, sizeof(m.Tiles[x].texIndex), 1, outFile);
		fwrite(&m.Tiles[x].x, sizeof(m.Tiles[x].x), 1, outFile);
		fwrite(&m.Tiles[x].y, sizeof(m.Tiles[x].y), 1, outFile);
	}
	printf("Written to file.\n");
	// Close the file
	fclose(outFile);
	return;
}

void loadMap(char *path) {
	// Function to load a map from a file.

	FILE *fp = fopen(path, "rb");

	if (!fp) {
		printf("Unable to open file to read map.\n");
		return;
	}

	struct MapHeader m;

	fread(&m.MapName, sizeof(m.MapName), 1, fp);
	printf("Loading into '%s'\n", m.MapName);
	fread(&m.maxScrollX, sizeof(m.maxScrollX), 1, fp);
	fread(&m.maxScrollY, sizeof(m.maxScrollY), 1, fp);
	fread(&m.nTexturePaths, sizeof(m.nTexturePaths), 1, fp);
	fread(&m.nTiles, sizeof(m.nTiles), 1, fp);
	char buf[1000];
	int BUFSIZE = 1000;
	for (size_t i = 0; i < m.nTexturePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		printf("%s\n", buf);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 70, 70, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedTextures = realloc(loadedTextures, ++textureSize * sizeof(loadedTextures));
		loadedTextures[textureSize - 1] = tmpTexture;
	}
	m.Tiles = malloc(m.nTiles * sizeof(*m.Tiles));
	for (size_t i = 0; i < m.nTiles; i++) {
		fread(&m.Tiles[i].texIndex, sizeof(m.Tiles[i].texIndex), 1, fp);
		fread(&m.Tiles[i].x, sizeof(m.Tiles[i].x), 1, fp);
		fread(&m.Tiles[i].y, sizeof(m.Tiles[i].y), 1, fp);
		map = realloc(map, ++mapSize * sizeof(*map));
		map[mapSize - 1] = createTileFromTexture(loadedTextures[m.Tiles[i].texIndex], m.Tiles[i].x, m.Tiles[i].y);
	}

	printf("'%s' loaded.\n", m.MapName);
	// Close the file
	fclose(fp);
}

void loadTextures() {
	FILE *fp = fopen("textures.list", "r");

	if (!fp) {
		printf("Failed to open texture list.\n");
		return;
	}

	char buf[1000];
	int BUFSIZE = 1000;

	while (fgets(buf, BUFSIZE, fp)) {
		buf[strcspn(buf, "\r\n")] = 0;
		printf("%s\n", buf);
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, 70, 70, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedTextures = realloc(loadedTextures, ++textureSize * sizeof(loadedTextures));
		printf("TextureSize: %I64d\n", textureSize - 1);
		loadedTextures[textureSize - 1] = tmpTexture;
	}
}