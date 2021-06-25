#include "../main.h"

struct Bullet *bullets = NULL;
size_t numBullets = 0;

double getYPositionOnLine(struct Line line, double xPos) {
	double yPos;
	yPos = line.grad * xPos + line.yIntercept;
	printf("Returning %f\n", yPos);
	return yPos;
}

void shoot(struct Player *player, int x2, int y2) {
	int x1 = player->rect.x + player->rect.w / 2;
	int y1 = player->rect.y + player->rect.h / 2;
	struct Line tmpLine;
	double ydiff = y2 - y1;
	double xdiff = x2 - x1;
	tmpLine.grad = ydiff / xdiff;
	tmpLine.yIntercept = y1 / (tmpLine.grad * x1);
	struct Bullet tmpBul;
	tmpBul.speed = sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
	tmpBul.worldRect.x = x1;
	tmpBul.worldRect.y = y1;
	tmpBul.worldRect.w = 2;
	tmpBul.worldRect.h = 2;
	tmpBul.line = tmpLine;
	tmpBul.rect = tmpBul.worldRect;
	tmpBul.rect.x = tmpBul.rect.x - worldScrollX;
	tmpBul.lifespan = 250;
	bullets = realloc(bullets, ++numBullets * sizeof(*bullets));
	bullets[numBullets - 1] = tmpBul;
	printf("Bullet created at %d, %d", x1, y1);
}

void applyBulletGrav() {
	for (size_t x = 0; x < numBullets; x++) {
		bullets[x].worldRect.x += bullets[x].speed * dt;
		bullets[x].worldRect.y = (int)getYPositionOnLine(bullets[x].line, bullets[x].worldRect.x);
		printf("Bullet %d rendered:\nX: %d\nY: %d\nSpeed: %f\n", x, bullets[x].worldRect.x, bullets[x].worldRect.y, bullets[x].speed);
		bullets[x].lifespan--;
		bullets[x].rect.x = bullets[x].worldRect.x - worldScrollX;
		bullets[x].rect.y = bullets[x].worldRect.y;
	}
	for (size_t x = 0; x < numBullets; x++) {
		if (bullets[x].lifespan < 0) {
			for (size_t y = x; y < numBullets; y++) {
				bullets[y] = bullets[y + 1];
			}
			bullets = realloc(bullets, --numBullets * sizeof(*bullets));
		}
	}
}

void renderBullets() {
	if (paused)
		return;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (size_t x = 0; x < numBullets; x++) {
		SDL_RenderDrawRect(renderer, &bullets[x].rect);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}