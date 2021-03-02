#include "../main.h"

struct Bullet *bullets = NULL;
size_t numBullets = 0;

void shoot(struct Player *player, int x2, int y2) {
	int x1 = player->rect.x + player->rect.w / 2;
	int y1 = player->rect.y + player->rect.h / 2;
	struct Bullet tmpBul;
	tmpBul.prevGrav = (y2 - y1) / 50;
	tmpBul.xVel = (x2 - x1) / 5; 
	tmpBul.rect.x = x1;
	tmpBul.rect.y = y1;
	tmpBul.rect.w = 3;
	tmpBul.rect.h = 3;
	tmpBul.lifespan = 500;
	bullets = realloc(bullets, ++numBullets * sizeof(*bullets));
	bullets[numBullets - 1] = tmpBul;
}

void applyBulletGrav() {
	for (size_t x = 0; x < numBullets; x++) {
		bullets[x].prevGrav += 7 * dt;
		bullets[x].xVel = bullets[x].xVel / 1.05;
		printf("Xvel: %d\n", bullets[x].xVel);
		bullets[x].rect.x += bullets[x].xVel * dt;
		bullets[x].rect.y += bullets[x].prevGrav;
		bullets[x].lifespan--;
		if (bullets[x].lifespan < 0) {
			for (size_t y = x; y < numBullets; y++) {
				bullets[y] = bullets[y + 1];
			}
			bullets = realloc(bullets, --numBullets * sizeof(*bullets));
		}
	}
}

void renderBullets() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (size_t x = 0; x < numBullets; x++) {
		SDL_RenderDrawRect(renderer, &bullets[x].rect);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}