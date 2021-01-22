#include "../main.h"

size_t mapSize = 0;
struct Tile *map = NULL;
struct MapHeader m;

void loadMap(char *path) {
	// Function to load a map from a file.

	FILE *fp = fopen(path, "rb");

	if (!fp) {
		printf("Unable to open file to read map.\n");
		return;
	}

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

void loadTextures(char *path) {
	FILE *fp = fopen(path, "r");

	if (!fp) {
		printf("Failed to open texture list.\n");
		return;
	}

	char buf[1000];
	int BUFSIZE = 1000;

	while (fgets(buf, BUFSIZE, fp)) {
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
}