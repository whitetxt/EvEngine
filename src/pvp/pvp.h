struct Line {
	double yIntercept;
	double grad;
};

struct Bullet {
	SDL_Rect rect;
	SDL_Rect worldRect;
	struct Line line;
	double speed;
	int lifespan;
};

extern struct Bullet *bullets;
extern size_t numBullets;

void shoot(struct Player *player, int x2, int y2);
void applyBulletGrav();
void renderBullets();