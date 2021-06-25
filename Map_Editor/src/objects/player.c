#include "../main.h"

double PrevGrav = 0;
int BaseGrav = 0;
int worldScrollX = 0;
int worldScrollY = 0;

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
	// Function to move the player around. Also handles scrolling.
	/* 	0 = N
	1 = E
	2 = S
	3 = W */
	// Change player speed if crouching.
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
	// If the player is at the center and moving to the right:
	if (Player->rect.x + (Player->rect.w / 2) > width / 2 && dir == 1) {
		// Change the world scroll.
		worldScrollX = worldScrollX + (Player->isCrouching ? Player->crouchSpeed * dt : Player->normSpeed * dt);
		// Lock the player at the center.
		Player->rect.x = (width / 2) - (Player->rect.w / 2);
		// Change the positions of all the tiles.
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScrollX;
		}
	}
	// If the player is at the center, not at the left edge and moving to the left:
	if (worldScrollX != 0 && Player->rect.x + (Player->rect.w / 2) < width / 2 && dir == 3) {
		// Change the world scroll.
		worldScrollX = worldScrollX - (Player->isCrouching ? Player->crouchSpeed * dt : Player->normSpeed * dt);
		// Lock the world scroll at the left edge.
		if (worldScrollX < 0)
			worldScrollX = 0;
		// Lock the player at the center.
		Player->rect.x = (width / 2) - (Player->rect.w / 2);
		// Update positions of all the tiles.
		for (size_t x = 0; x < mapSize; x++) {
			map[x].rect.x = map[x].worldRect.x - worldScrollX;
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
	// Checks if player would get clipped inside a block if we uncrouched.
	// First check will push the player downwards if they collide with the block above when uncrouched.
	if (checkCollision(Player->rect)) {
		// Second check will push the player upwards if they collide with the block below.
		if (checkCollision(Player->rect)) {
			// If we collide above then below, we cannot uncrouch.
			Player->rect.h = 35;
			Player->rect.y += 35;
			return;
		}
	}
	Player->isCrouching = false;
}

void grav(struct Player *Player) {
	// Function to apply accelerating gravity.
	PrevGrav += 0.01 * dt;
	Player->rect.y += PrevGrav;
	// If the player is below the window.
	// TODO: Make this respawn the player.
	if (Player->rect.y + Player->rect.h > height) {
		Player->rect.y = height - Player->rect.h;
		PrevGrav = 0;
		Player->onGround = true;
	}
}

void playerJump(struct Player *Player) {
	// Simple function that uses gravity to make the player jump.
	if (Player->onGround) {
		PrevGrav = -4;
		Player->onGround = false;
	}
}

bool checkCollision(SDL_Rect Rect) {
	// Collision function for SDL_Rects.
	// Identical to playerCollision except it takes an SDL_Rect not a struct Player.
	for (size_t x = 0; x < mapSize; x++) {

		// If the tile is too far away, dont waste time checking.
		if (abs(Rect.x - map[x].rect.x) > 100)
			continue;
		if (abs(Rect.y - map[x].rect.y) > 100)
			continue;

		// First, check if there is a collision.
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

			// Handle the collision side appropriately.
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
	// Function to handle player collision with tiles.
	// Takes a struct Player to allow for multiple player checking.

	// If the player is too low down, clamp at the bottom.
	// TODO: Make this respawn the player
	if (Player->rect.y + Player->rect.h > height)
		Player->rect.y = height - Player->rect.h;

	// If too far left, clamp at the left side.
	if (Player->rect.x < 0)
		Player->rect.x = 0;

	// If too far right, clamp at the right side.
	if (Player->rect.x + Player->rect.w > width)
		Player->rect.x = width - Player->rect.w;

	// If too high, clamp at the top.
	if (Player->rect.y < 0) {
		Player->rect.y = 0;
		PrevGrav = 0;
	}

	for (size_t x = 0; x < mapSize; x++) {

		// If the tile is too far away, don't waste time checking for collision.
		if (abs(Player->rect.x - map[x].rect.x) > 100)
			continue;
		if (abs(Player->rect.y - map[x].rect.y) > 100)
			continue;

		// First, check if it collides.
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

			// Handle collision side accordingly.
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