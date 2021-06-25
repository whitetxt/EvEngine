#include "../main.h"

size_t selectedMenu = 0;
size_t menuSize = 0;
size_t inputNum;

struct MenuItem *menu = NULL;
struct MenuItem menuTitle;

SDL_Color Selected = {49, 127, 67, 255};
SDL_Color Black = {0, 0, 0, 255};

char *menuFrom;
char *currentMenu;

SDL_Event menuEv;
bool inputting = false;
char finalText[512];
bool pressed = false;
char *lastKey = "¬";

int createMenu(char *path) {
	// Open a menu file by path
	char outpath[256];
	sprintf(outpath, "menus/%s", path); // Format the file to the correct directory
	FILE *fp = fopen(outpath, "r"); // Open the file
	menuSize = 0;
	menuFrom = strdup(currentMenu);
	currentMenu = strdup(path);

	if (!fp) { // If the file doesnt exist
		printf("Failed to open menu file: %s\n", outpath);
		return 1;
	}

	char buf[2048];
	bool found = false;

	while (fgets(buf, sizeof(buf), fp)) { // While there is a line
		buf[strcspn(buf, "\n")] = 0;
		menu = realloc(menu, ++menuSize * sizeof(*menu)); // Increase the size of the menu array
		struct MenuItem tmpItem;
		tmpItem.name = strdup(strtok(buf, ":"));
		tmpItem.nextmenu = strdup(strtok(NULL, ":"));
		tmpItem.textInput = false;
		if (strcmp(tmpItem.name, "UNPAUSABLE") == 0) { // If this menu is unpausable
			unpausable = true;
			menuSize--;
			continue;
		} else if (strcmp(tmpItem.name, "NOT_UNPAUSABLE") == 0) { // If this menu is not unpausable
			unpausable = false;
			menuSize--;
			continue;
		}
		if (strcmp(tmpItem.nextmenu, "NULL") == 0) { // If the destination is set to have no destination
			printf("WARN: No destination menu for option %s\n", tmpItem.name);
		}
		if (strcmp(tmpItem.name, "TITLE") == 0) { // If the text found is a title
			if (found) {
				printf("Multiple titles found. Existing title: %s\nNew title: %s\nUsing newer title.", menuTitle.nextmenu, tmpItem.nextmenu);
			}
			// Revert the changes made
			menuSize--;
			// Set the title
			menuTitle = tmpItem;
			found = true;
			continue;
		} else if (strcmp(tmpItem.nextmenu, "LoadLevel") == 0) { // If the item loads a map
			tmpItem.levelName = strdup(strtok(NULL, ":")); // Set the map to load
		} else if (strcmp(tmpItem.nextmenu, "TextInput") == 0) { // If its an input box
			tmpItem.textInput = true;
			tmpItem.text = '\0';
			tmpItem.textBuf[0] = '\0';
		}
		menu[menuSize - 1] = tmpItem;
	}
	if (!found) {
		// If there is no title text
		printf("Title for menu not found.\n");
		menuTitle.nextmenu = "TITLE NOT FOUND";
	}
	selectedMenu = 0;
	return 0;
}

void renderMenu() {
	// Special function for rendering the menu.
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	struct Text tmpText;

	// Render the title
	tmpText = createColouredText(menuTitle.nextmenu, width / 2, 20, titleFont, Black);
	tmpText.rect.x -= tmpText.rect.w / 2;
	SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
	int offset = tmpText.rect.h + 20;
	SDL_DestroyTexture(tmpText.tex);

	// Render the seperation bar
	SDL_Rect tmpRect;
	tmpRect.x = 0;
	tmpRect.y = offset + 5;
	tmpRect.w = width;
	tmpRect.h = 2;
	SDL_RenderDrawRect(renderer, &tmpRect);

	for (size_t x = 0; x < menuSize; x++) {
		// Create the text for this screen
		// NOTE: Could be optimised to have these texts created when the menu is loaded
		// and store it in the MenuItem struct. I will probably implement that later today.
		if (x == selectedMenu) {
			tmpText = createColouredText(menu[x].name, 20, (Settings.menuSize + 8) * (x + 1) + offset, menuFont, Selected);
		} else {
			tmpText = createColouredText(menu[x].name, 20, (Settings.menuSize + 8) * (x + 1) + offset, menuFont, Black);
		}
		SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
		SDL_DestroyTexture(tmpText.tex);
		if (menu[x].textInput) { // If the text is an input area
			// Create the text input box
			tmpRect.x = tmpText.rect.x + tmpText.rect.w + 10;
			tmpRect.y = tmpText.rect.y - 1;
			tmpRect.w = width - tmpText.rect.x - tmpText.rect.w - 20;
			tmpRect.h = tmpText.rect.h + 2;
			// Render the box
			SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
			SDL_RenderDrawRect(renderer, &tmpRect);
			if (menu[x].text != NULL && strcmp(menu[x].text, "") != 0) { // If there is text in the box
				// Render the text
				tmpText = createColouredText(menu[x].text, tmpRect.x + 5, tmpRect.y + 1, menuFont, Black);
				SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
				SDL_DestroyTexture(tmpText.tex);
			}
			if (x == selectedMenu) {
				// Var used to track keyboard inputs
				inputting = true;
			}
		}
	}
	return;
}

int selectOption() {
	// Get the menu to go to or whatever else is in that field.
	char *menuToGo = menu[selectedMenu].nextmenu;
	if (strcmp(menuToGo, "ResumeGame") == 0) { // Special command to resume the game
		paused = false;
		return 0;
	} else if (strcmp(menuToGo, "ExitGame") == 0) { // Special command to quit the game
		return 1;
	} else if (strcmp(menuToGo, "NULL") == 0) { // If it has no destination
		printf("%s has no menu destination.\n", menu[selectedMenu].name);
		return 0;
	} else if (strcmp(menuToGo, "LoadLevel") == 0) { // Special command to load a level instead of a menu
		if (loadMap(menu[selectedMenu].levelName) != 0) {
			printf("Failed to load into level: %s\n", menu[selectedMenu].levelName);
			return 0;
		} else {
			paused = false;
			unpausable = true;
			return 0;
		}
	} else if (strcmp(menuToGo, "MenuBack") == 0) { // Special command to allow for menus to be used in multiple places, while returning to different menus
		menuToGo = strdup(menuFrom);
	} else if (strcmp(menuToGo, "TextInput") == 0) { // Special command to input into text field
		inputting = true;
		return 0;
	} else if (strcmp(strtok(menuToGo, ","), "ServerConnect") == 0) { // Special command to connect to server
		return 2;
	}
	createMenu(menuToGo);
	return 0;
}

void menuInputHandling() {
	// Special event handling to handle key inputs
	switch (event.type) {
		case SDL_TEXTINPUT:
			// Handle text entering
			if (inputting && (event.text.text != lastKey) && (strcmp(event.text.text, " ") != 0)) {
				strcat(menu[selectedMenu].textBuf, event.text.text);
				menu[selectedMenu].text = strdup(menu[selectedMenu].textBuf);
				lastKey = strdup(event.text.text);
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				// Handle backspace
				case SDLK_BACKSPACE:
					if (strlen(menu[selectedMenu].text) != 0)
						menu[selectedMenu].text[strlen(menu[selectedMenu].text) - 1] = '\0';
					if (strlen(menu[selectedMenu].textBuf) != 0)
						menu[selectedMenu].textBuf[strlen(menu[selectedMenu].textBuf) - 1] = '\0';
			}
	}
}