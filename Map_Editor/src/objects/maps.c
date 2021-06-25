#include "../main.h"

size_t mapSize = 0;
struct Tile *map = NULL;

size_t interactableMapSize = 0;
struct Interactable *interactableMap = NULL;

struct Map m;

void saveMap(char *path) {
	// Function to save the currently rendered map to a file.
	// Get a map name
	printf("Please enter the name for the map (Max 32 chars).\n");
	scanf("%s", m.MapName);

	FILE *outFile = fopen(path, "wb");
	// Open the output file
	if (!outFile) {
		printf("Unable to open output file.\n");
		return;
	}

	fwrite(&m.MapName, sizeof(m.MapName), 1, outFile);
	printf("Written map name\n");
	m.nTexturePaths = textureSize;
	m.nInteractablePaths = interactableSize;
	m.nTiles = mapSize;
	m.nInteractables = interactableMapSize;
	
	m.TexturePaths = malloc(m.nTexturePaths * sizeof(*m.TexturePaths));
	m.InteractableInactivePaths= malloc(m.nInteractablePaths * sizeof(*m.InteractableInactivePaths));
	m.InteractableActivePaths = malloc(m.nInteractablePaths * sizeof(*m.InteractableActivePaths));
	m.Tiles = malloc(m.nTiles * sizeof(*m.Tiles));
	m.Interactables = malloc(m.nInteractables * sizeof(*m.Interactables));
	printf("Malloc'd everything.\n");
	m.maxScrollX = 0;
	for (uint64_t x = 0; x < m.nTiles; x++) {
		printf("inside\n");
		m.Tiles[x].texIndex = map[x].texIndex;
		m.Tiles[x].x = map[x].worldRect.x;
		m.Tiles[x].y = map[x].worldRect.y;
		printf("Tile right side: %d\n", m.Tiles[x].x + map[x].worldRect.w);
		if (m.Tiles[x].x + map[x].worldRect.w > m.maxScrollX) {
			m.maxScrollX = m.Tiles[x].x + map[x].worldRect.w;
			printf("Changed worldscroll to: %lld\n", m.maxScrollX);
		}
	}
	fwrite(&m.maxScrollX, sizeof(m.maxScrollX), 1, outFile);
	printf("Maxscroll: %lld\n", m.maxScrollX);	

	fwrite(&m.nTexturePaths, sizeof(m.nTexturePaths), 1, outFile);
	fwrite(&m.nInteractablePaths, sizeof(m.nInteractablePaths), 1, outFile);
	fwrite(&m.nTiles, sizeof(m.nTiles), 1, outFile);
	fwrite(&m.nInteractables, sizeof(m.nInteractables), 1, outFile);
	printf("Written numbers\n");

	FILE *fp = fopen("textures.list", "r");

	if (!fp) {
		printf("Tile textures couldn't be loaded from file.\n");
		return;
	}

	char buf[1000];
	int BUFSIZE = 1000;
	for (uint64_t i = 0; i < m.nTexturePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		fwrite(buf, strlen(buf), 1, outFile);
		fputc('\n', outFile);
		printf("Written texturepath: %s\n", buf);
	}
	printf("Written all texturepaths.\n");

	fclose(fp);

	fp = fopen("interactables.list", "r");

	if (!fp) {
		printf("Interactables textures couldn't be loaded from file.\n");
		return;
	}

	for (size_t i = 0; i < m.nInteractablePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		char *sep = strtok(buf, ";");
		fwrite(sep, strlen(sep), 1, outFile);
		fputc('\n', outFile);
		printf("Written InteractableInactivePath: %s\n", sep);
	}

	printf("Written all InteractableInactivePaths\n");

	fclose(fp);

	fp = fopen("interactables.list", "r");

	if (!fp) {
		printf("Interactables textures couldn't be loaded from file in the second iteration somehow????\n");
		return;
	}

	for (size_t i = 0; i < m.nInteractablePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		char *sep = strtok(buf, ";");
		sep = strtok(NULL, ";");
		fwrite(sep, strlen(sep), 1, outFile);
		fputc('\n', outFile);
		printf("Written InteractableActivePath: %s\n", sep);
	}

	fclose(fp);
	printf("Written all InteractableActivePaths\n");

	for (uint64_t x = 0; x < m.nTiles; x++) {
		fwrite(&map[x].texIndex, sizeof(map[x].texIndex), 1, outFile);
		fwrite(&map[x].worldRect.x, sizeof(map[x].worldRect.x), 1, outFile);
		fwrite(&map[x].worldRect.y, sizeof(map[x].worldRect.y), 1, outFile);
	}
	printf("Written Tiles\n");
	uint8_t tmp;
	for (uint64_t x = 0; x < m.nInteractables; x++) {
		fwrite(&interactableMap[x].texIndex, sizeof(interactableMap[x].texIndex), 1, outFile);
		fwrite(&interactableMap[x].worldRect.x, sizeof(interactableMap[x].worldRect.x), 1, outFile);
		fwrite(&interactableMap[x].worldRect.y, sizeof(interactableMap[x].worldRect.y), 1, outFile);
		if (interactableMap[x].active)
			tmp = 1;
		else
			tmp = 0;
		fwrite(&tmp, sizeof(tmp), 1, outFile);
	}
	printf("Written interactables.\n");
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

	fread(&m.MapName, sizeof(m.MapName), 1, fp);
	printf("Loading into '%s'\n", m.MapName);
	fread(&m.maxScrollX, sizeof(m.maxScrollX), 1, fp);
	fread(&m.nTexturePaths, sizeof(m.nTexturePaths), 1, fp);
	fread(&m.nInteractablePaths, sizeof(m.nInteractablePaths), 1, fp);
	fread(&m.nTiles, sizeof(m.nTiles), 1, fp);
	fread(&m.nInteractables, sizeof(m.nInteractables), 1, fp);
	char buf[2000];
	int BUFSIZE = 2000;
	for (size_t i = 0; i < m.nTexturePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, gridSize, gridSize, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedTextures = realloc(loadedTextures, ++textureSize * sizeof(loadedTextures));
		loadedTextures[textureSize - 1] = tmpTexture;
	}

	for (size_t i = 0; i < m.nInteractablePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, gridSize, gridSize, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedInteractablesInactive = realloc(loadedInteractablesInactive, ++interactableSize * sizeof(loadedInteractablesInactive));
		loadedInteractablesInactive[interactableSize - 1] = tmpTexture;
	}

	loadedInteractablesActive = realloc(loadedInteractablesActive, interactableSize * sizeof(loadedInteractablesActive));
	for (size_t i = 0; i < m.nInteractablePaths; i++) {
		fgets(buf, BUFSIZE, fp);
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, gridSize, gridSize, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedInteractablesActive[interactableSize - 1] = tmpTexture;
	}
	m.Tiles = malloc(m.nTiles * sizeof(*m.Tiles));
	for (size_t i = 0; i < m.nTiles; i++) {
		fread(&m.Tiles[i].texIndex, sizeof(m.Tiles[i].texIndex), 1, fp);
		fread(&m.Tiles[i].x, sizeof(m.Tiles[i].x), 1, fp);
		fread(&m.Tiles[i].y, sizeof(m.Tiles[i].y), 1, fp);
		map = realloc(map, ++mapSize * sizeof(*map));
		map[mapSize - 1] = createTileFromTexture(loadedTextures[m.Tiles[i].texIndex], m.Tiles[i].x, m.Tiles[i].y);
	}
	m.Interactables = malloc(m.nInteractables * sizeof(*m.Interactables));
	for (size_t i = 0; i < m.nInteractables; i++) {
		fread(&m.Interactables[i].texIndex, sizeof(m.Interactables[i].texIndex), 1, fp);
		fread(&m.Interactables[i].x, sizeof(m.Interactables[i].x), 1, fp);
		fread(&m.Interactables[i].y, sizeof(m.Interactables[i].y), 1, fp);
		fread(&m.Interactables[i].active, sizeof(m.Interactables[i].active), 1, fp);
		interactableMap = realloc(interactableMap, ++interactableMapSize * sizeof(*interactableMap));
		interactableMap[interactableMapSize - 1] = createInteractableFromTextures(m.Interactables[i].texIndex, m.Interactables[i].x, m.Interactables[i].y, m.Interactables[i].active);
	}

	printf("'%s' loaded.\n", m.MapName);
	// Close the file
	fclose(fp);
}

void loadTextures() {
	printf("sdjfia\n");
	FILE *fp = fopen("textures.list", "r");

	if (!fp) {
		printf("Failed to open texture list.\n");
		return;
	}

	char buf[2000];
	int BUFSIZE = 2000;

	while (fgets(buf, BUFSIZE, fp)) {
		buf[strcspn(buf, "\r\n")] = 0;
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, buf);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", buf);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, gridSize, gridSize, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedTextures = realloc(loadedTextures, ++textureSize * sizeof(loadedTextures));
		loadedTextures[textureSize - 1] = tmpTexture;
	}

	fp = fopen("interactables.list", "r");

	if (!fp) {
		printf("Failed to open interactables list.\n");
		return;
	}

	while (fgets(buf, BUFSIZE, fp)) {
		buf[strcspn(buf, "\r\n")] = 0;
		char *sep = strtok(buf, ";");
		SDL_Texture *tmpTexture = IMG_LoadTexture(renderer, sep);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", sep);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, gridSize, gridSize, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedInteractablesInactive = realloc(loadedInteractablesInactive, ++interactableSize * sizeof(loadedInteractablesInactive));
		loadedInteractablesInactive[interactableSize - 1] = tmpTexture;
		sep = strtok(NULL, ";");
		tmpTexture = IMG_LoadTexture(renderer, sep);
		if (!tmpTexture) {
			printf("Failed to load image %s.\n", sep);
			// Create black box to replace image if it could not be loaded.
			SDL_Surface *tmpsurf = SDL_CreateRGBSurface(0, gridSize, gridSize, 32, 0, 0, 0, 0);
			tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
			SDL_FreeSurface(tmpsurf);
		}
		loadedInteractablesActive = realloc(loadedInteractablesActive, interactableSize * sizeof(loadedInteractablesActive));
		loadedInteractablesActive[interactableSize - 1] = tmpTexture;
	}
}