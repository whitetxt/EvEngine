struct Player {
	SDL_Texture *tex;
	SDL_Texture *crouchTex;
	SDL_Rect rect;
	SDL_Rect worldRect;
	bool onGround;
	bool isCrouching;
	float normSpeed;
	float crouchSpeed;
	int dir;
};

extern int64_t worldScrollX;

struct Player createPlayer(char *fp, char *crouchfp, int x, int y, float normSpeed, float crouchSpeed);

void movePlayer(struct Player *Player, int dir);

void grav(struct Player *Player);

void playerJump(struct Player *Player);

bool checkCollision(SDL_Rect Rect);

void playerCollision(struct Player *Player);

void startCrouch(struct Player *Player);
void endCrouch(struct Player *Player);