struct Text {
	SDL_Rect rect;
	SDL_Texture *tex;
};

extern TTF_Font *menuFont;
extern TTF_Font *playerFont;
extern TTF_Font *notifFont;
extern TTF_Font *titleFont;
extern TTF_Font *mainMenuFont;

void initFonts();

struct Text createText(char *text, int x, int y, TTF_Font *Font);

struct Text createColouredText(char *text, int x, int y, TTF_Font *Font, SDL_Color Colour);

void freeText();