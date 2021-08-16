typedef struct MenuItem {
	char *name;
	char *nextmenu;
	char *levelName;
	char *text;
	char textBuf[512];
	bool textInput;
	Text textObj;
} MenuItem;

typedef struct Menu {
	char *name;
	MenuItem *items;
	size_t numItems;
	int selected;
} Menu;

extern size_t selectedMenu;
extern size_t menuSize;
extern size_t inputNum;

extern struct MenuItem *menu;

extern SDL_Color Selected;
extern SDL_Color Black;

extern bool pressed;

int createMenu();
void renderMenu();
int selectOption();
void menuInputHandling();