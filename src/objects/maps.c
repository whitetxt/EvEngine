#include "../main.h"

size_t mapSize = 0;
struct Tile *map = NULL;

void loadMap(char *path) {
	// Function to load a map from a file.
	// TODO: This is so bad god please help me
	// If you have any better ideas for a map file format
	// Please contact me.

	FILE *fp = fopen(path, "r");

	// If the file couldn't be opened.
	if (!fp) {
		printf("ERROR: Couldn't load map from file %s.\n", path);
		return;
	}

	char line[256];
	int CurrentPart = 0;
	char *texture;
	int xpos;
	int ypos;

	// Read file line by line
	while (fgets(line, sizeof(line), fp)) {
		// Map format seperates texturepath, xpos and ypos by spaces.
		char *sep = strtok(line, " ");
		while(sep != NULL) { 
			// Check CurrentPart to get which part of the tile we are getting.
			switch (CurrentPart) {
				case 0:
					// Set the texture position.
					texture = sep;
					CurrentPart++;
					break;
				case 1:
					// Convert string position to int position.
					xpos = atoi(sep);
					CurrentPart++;
					break;
				case 2:
					// Convert string position to int position.
					ypos = atoi(sep);
					CurrentPart = 0;
					// Increase map array size and create new tile from the gathered data.
					map = realloc(map, ++mapSize * sizeof(*map));
					map[mapSize - 1] = createTile(texture, xpos, ypos);
					break;
			}
			// Get the next part.
			sep = strtok(NULL, " ");
		}
	}
	// Close the file
	fclose(fp);
}