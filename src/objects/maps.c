#include "../main.h"

size_t mapSize = 0;
struct Tile *map = NULL;

void loadMap(char *path) {

	FILE *fp = fopen(path, "r");

	if (!fp) {
		printf("ERROR: Couldn't load map from file %s.\n", path);
		return;
	}

	char line[256];
	int CurrentPart = 0;
	char *texture;
	int xpos;
	int ypos;

	while (fgets(line, sizeof(line), fp)) {
		char *sep = strtok(line, " ");
		while(sep != NULL) {
			switch (CurrentPart) {
				case 0:
					texture = sep;
					CurrentPart++;
					break;
				case 1:
					xpos = atoi(sep);
					CurrentPart++;
					break;
				case 2:
					ypos = atoi(sep);
					CurrentPart = 0;
					map = realloc(map, ++mapSize * sizeof(*map));
					map[mapSize - 1] = createTile(texture, xpos, ypos);
					break;
			}
			sep = strtok(NULL, " ");
		}
	}
	fclose(fp);
}