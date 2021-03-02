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
char *lastKey = "£";

int createMenu(char *path) {
	char outpath[256];
	sprintf(outpath, "menus/%s", path);
	FILE *fp = fopen(outpath, "r");
	menuSize = 0;
	menuFrom = strdup(currentMenu);
	currentMenu = strdup(path);

	if (!fp) {
		printf("Failed to open menu file: %s\n", outpath);
		return 1;
	}

	char buf[2048];
	bool found = false;

	while (fgets(buf, sizeof(buf), fp)) {
		buf[strcspn(buf, "\n")] = 0;
		menu = realloc(menu, ++menuSize * sizeof(*menu));
		struct MenuItem tmpItem;
		tmpItem.name = strdup(strtok(buf, ":"));
		tmpItem.nextmenu = strdup(strtok(NULL, ":"));
		tmpItem.textInput = false;
		if (strcmp(tmpItem.name, "UNPAUSABLE") == 0) {
			unpausable = true;
			menuSize--;
			continue;
		} else if (strcmp(tmpItem.name, "NOT_UNPAUSABLE") == 0) {
			unpausable = false;
			menuSize--;
			continue;
		}
		if (strcmp(tmpItem.nextmenu, "NULL") == 0) {
			printf("WARN: No destination menu for option %s\n", tmpItem.name);
		}
		if (strcmp(tmpItem.name, "TITLE") == 0) {
			menuSize--;
			menuTitle = tmpItem;
			found = true;
			continue;
		} else if (strcmp(tmpItem.nextmenu, "LoadLevel") == 0) {
			tmpItem.levelName = strdup(strtok(NULL, ":"));
		} else if (strcmp(tmpItem.nextmenu, "TextInput") == 0) {
			tmpItem.textInput = true;
			tmpItem.text = '\0';
			tmpItem.textBuf[0] = '\0';
		}
		menu[menuSize - 1] = tmpItem;
	}
	if (!found) {
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

	tmpText = createColouredText(menuTitle.nextmenu, width / 2, 20, titleFont, Black);
	tmpText.rect.x -= tmpText.rect.w / 2;
	SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
	int offset = tmpText.rect.h + 20;
	SDL_DestroyTexture(tmpText.tex);

	SDL_Rect tmpRect;
	tmpRect.x = 0;
	tmpRect.y = offset + 5;
	tmpRect.w = width;
	tmpRect.h = 2;
	SDL_RenderDrawRect(renderer, &tmpRect);

	for (size_t x = 0; x < menuSize; x++) {
		if (x == selectedMenu) {
			tmpText = createColouredText(menu[x].name, 20, (Settings.menuSize + 8) * (x + 1) + offset, menuFont, Selected);
		} else {
			tmpText = createColouredText(menu[x].name, 20, (Settings.menuSize + 8) * (x + 1) + offset, menuFont, Black);
		}
		SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
		SDL_DestroyTexture(tmpText.tex);
		if (menu[x].textInput) {
			tmpRect.x = tmpText.rect.x + tmpText.rect.w + 10;
			tmpRect.y = tmpText.rect.y - 1;
			tmpRect.w = width - tmpText.rect.x - tmpText.rect.w - 20;
			tmpRect.h = tmpText.rect.h + 2;
			SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
			SDL_RenderDrawRect(renderer, &tmpRect);
			if (menu[x].text != NULL && strcmp(menu[x].text, "") != 0) {
				tmpText = createColouredText(menu[x].text, tmpRect.x + 5, tmpRect.y + 1, menuFont, Black);
				SDL_RenderCopy(renderer, tmpText.tex, NULL, &tmpText.rect);
				SDL_DestroyTexture(tmpText.tex);
			}
			if (x == selectedMenu) {
				inputting = true;
			}
		}
	}
	return;
}

int selectOption() {
	char *menuToGo = menu[selectedMenu].nextmenu;
	if (strcmp(menuToGo, "ResumeGame") == 0) {
		paused = false;
		return 0;
	} else if (strcmp(menuToGo, "ExitGame") == 0) {
		return 1;
	} else if (strcmp(menuToGo, "NULL") == 0) {
		printf("%s has no menu destination.\n", menu[selectedMenu].name);
		return 0;
	} else if (strcmp(menuToGo, "LoadLevel") == 0) {
		if (loadMap(menu[selectedMenu].levelName) != 0) {
			printf("Failed to load into level: %s\n", menu[selectedMenu].levelName);
			return 0;
		} else {
			paused = false;
			unpausable = true;
			return 0;
		}
	} else if (strcmp(menuToGo, "MenuBack") == 0) {
		menuToGo = strdup(menuFrom);
	} else if (strcmp(menuToGo, "TextInput") == 0) {
		inputting = true;
		return 0;
	} else if (strcmp(strtok(menuToGo, ","), "ServerConnect") == 0) {
		return 2;
	}
	createMenu(menuToGo);
	return 0;
}

void menuInputHandling() {
	switch (event.type) {
		case SDL_TEXTINPUT:
			if (inputting && (event.text.text != lastKey) && (strcmp(event.text.text, " ") != 0)) {
				strcat(menu[selectedMenu].textBuf, event.text.text);
				menu[selectedMenu].text = strdup(menu[selectedMenu].textBuf);
				lastKey = strdup(event.text.text);
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_BACKSPACE:
					if (strlen(menu[selectedMenu].text) != 0)
						menu[selectedMenu].text[strlen(menu[selectedMenu].text) - 1] = '\0';
					if (strlen(menu[selectedMenu].textBuf) != 0)
						menu[selectedMenu].textBuf[strlen(menu[selectedMenu].textBuf) - 1] = '\0';
			}
	}
}