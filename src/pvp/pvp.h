struct Line {
	double gradient;
	double yIntersect;
	double prevGrav;
};

struct Bullet {
	SDL_Rect rect;
	double prevGrav;
	double xVel;
	int lifespan;
};

void shoot(struct Player *player, int x2, int y2);
void applyBulletGrav();
void renderBullets();