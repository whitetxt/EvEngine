#include "../main.h"

double PrevGrav = 0;
int BaseGrav = 0;
int worldScroll = 0;

struct Player createPlayer(char *fp, char *crouchfp, int x, int y, float normSpeed, float crouchSpeed) {
	struct Player tmpPlayer;
	tmpPlayer.tex = loadImage(renderer, fp);
	tmpPlayer.crouchTex = loadImage(renderer, crouchfp);
	tmpPlayer.rect = createRect(tmpPlayer.tex, x, y);
	tmpPlayer.normSpeed = normSpeed;
	tmpPlayer.crouchSpeed = crouchSpeed;
	return tmpPlayer;
}

void movePlayer(struct Player *Player, int dir) {
	/* 	0 = N
	1 = E
	2 = S
	3 = W */
	if (Player->isCrouching) {
		switch (dir) { 
			case 0:
				Player->rect.y -= Player->crouchSpeed * dt;
				break;
			case 1:
				Player->rect.x += Player->crouchSpeed * dt;
				break;
			case 2:
				Player->rect.y += Player->crouchSpeed * dt;
				break;
			case 3:
				Player->rect.x -= Player->crouchSpeed * dt;
				break;
		}
	} else {
		switch (dir) { 
			case 0:
				Player->rect.y -= Player->normSpeed * dt;
				break;
			case 1:
				Player->rect.x += Player->normSpeed * dt;
				break;
			case 2:
				Player->rect.y += Player->normSpeed * dt;
				break;
			case 3:
				Player->rect.x -= Player->normSpeed * dt;
				break;
		}
	}
	if (Player->rect.x + (Player->rect.w / 2) > width / 2 && dir == 1) {
		worldScroll = worldScroll + (Player->isCrouching ? Player->crouchSpeed * dt : Player->normSpeed * dt);
		Player->rect.x = (width / 2) - (Player->rect.w / 2);
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScroll;
		}
	}
	if (worldScroll != 0 && Player->rect.x + (Player->rect.w / 2) < width / 2 && dir == 3) {
		worldScroll = worldScroll - (Player->isCrouching ? Player->crouchSpeed * dt : Player->normSpeed * dt);
		if (worldScroll < 0)
			worldScroll = 0;
		Player->rect.x = (width / 2) - (Player->rect.w / 2);
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScroll;
		}
	}
}

void startCrouch(struct Player *Player) {
	Player->rect.h = 35;
	Player->isCrouching = true;
	Player->rect.y += 35;
}

void endCrouch(struct Player *Player) {
	Player->rect.h = 70;
	Player->rect.y -= 35;
	if (checkCollision(Player->rect)) {
		if (checkCollision(Player->rect)) {
			Player->rect.h = 35;
			Player->rect.y += 35;
			return;
		}
	}
	Player->isCrouching = false;
}

void grav(struct Player *Player) {
	PrevGrav += 0.01 * dt;
	Player->rect.y += PrevGrav;
	if (Player->rect.y + Player->rect.h > height) {
		Player->rect.y = height - Player->rect.h;
		PrevGrav = 0;
		Player->onGround = true;
	}
}

void playerJump(struct Player *Player) {
	if (Player->onGround) {
		PrevGrav = -4;
		Player->onGround = false;
	}
}

bool checkCollision(SDL_Rect Rect) {
	for (size_t x = 0; x < mapSize; x++) {

		if (abs(Rect.x - map[x].rect.x) > 100)
			continue;
		if (abs(Rect.y - map[x].rect.y) > 100)
			continue;

		if(	Rect.x + Rect.w > map[x].rect.x &&
			Rect.x < map[x].rect.x + map[x].rect.w &&
			Rect.y + Rect.h > map[x].rect.y &&
			Rect.y < map[x].rect.y + map[x].rect.h) {
			// Figure out collision side
			int amtRight = abs(Rect.x + Rect.w - map[x].rect.x);
			int amtLeft = abs(map[x].rect.x + map[x].rect.w - Rect.x);
			int amtTop = abs(map[x].rect.y - Rect.y + Rect.h);
			int amtBottom = abs(Rect.y + Rect.h - map[x].rect.y);

			int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
			int lowest = values[0];
			// Get lowest value, side it collided on
			for (int x = 0; x < 4; x++) {
				if (values[x] < lowest) {
					lowest = values[x];
				}
			}

			if (lowest == amtRight)
				Rect.x = map[x].rect.x - Rect.w - 0.05;
			else if (lowest == amtLeft)
				Rect.x = map[x].rect.x + map[x].rect.w + 0.05;
			else if (lowest == amtTop)
				Rect.y = map[x].rect.y + map[x].rect.h + 0.05;
			else if (lowest == amtBottom)
				Rect.y = map[x].rect.y - Rect.h - 0.05;
			return true;
		}
	}
	return false;
}

void playerCollision(struct Player *Player) {
	if (Player->rect.y + Player->rect.h > height)
		Player->rect.y = height - Player->rect.h;

	if (Player->rect.x < 0)
		Player->rect.x = 0;

	if (Player->rect.x + Player->rect.w > width)
		Player->rect.x = width - Player->rect.w;
	
	if (Player->rect.y < 0) {
		Player->rect.y = 0;
		PrevGrav = 0;
	}

	for (size_t x = 0; x < mapSize; x++) {

		if (abs(Player->rect.x - map[x].rect.x) > 100)
			continue;
		if (abs(Player->rect.y - map[x].rect.y) > 100)
			continue;

		if(	Player->rect.x + Player->rect.w > map[x].rect.x &&
			Player->rect.x < map[x].rect.x + map[x].rect.w &&
			Player->rect.y + Player->rect.h > map[x].rect.y &&
			Player->rect.y < map[x].rect.y + map[x].rect.h) {
			// Figure out collision side
			int amtRight = abs(Player->rect.x + Player->rect.w - map[x].rect.x);
			int amtLeft = abs(map[x].rect.x + map[x].rect.w - Player->rect.x);
			int amtTop = abs(map[x].rect.y - Player->rect.y + Player->rect.h);
			int amtBottom = abs(Player->rect.y + Player->rect.h - map[x].rect.y);

			int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
			int lowest = values[0];
			// Get lowest value, side it collided on
			for (int x = 0; x < 4; x++) {
				if (values[x] < lowest) {
					lowest = values[x];
				}
			}

			if (lowest == amtRight)
				Player->rect.x = map[x].rect.x - Player->rect.w - 0.05;
			else if (lowest == amtLeft)
				Player->rect.x = map[x].rect.x + map[x].rect.w + 0.05;
			else if (lowest == amtTop) {
				Player->rect.y = map[x].rect.y + map[x].rect.h + 0.05;
				PrevGrav = 0;
			}
			else if (lowest == amtBottom) {
				Player->rect.y = map[x].rect.y - Player->rect.h - 0.05;
				Player->onGround = true;
				PrevGrav = 0;
			}
		}
	}
}