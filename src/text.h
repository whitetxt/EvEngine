struct Text {
	SDL_Rect rect;
	SDL_Texture *tex;
};

struct Text createText(char *text, int x, int y);

void removeText(struct Text *text);