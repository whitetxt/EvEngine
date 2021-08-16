#include "main.h"

struct Notif currentNotif;
SDL_Texture *notifBack;
SDL_Rect notifRect;
int FadeTime = 0;
bool exists = false;
int animTime = 20;

struct Notif *backlogNotifs;
size_t backlogSize = 0;

void initNotifs() {
	notifBack = loadImage(renderer, "assets/notifBack.png");
	notifRect.x = 0;
	SDL_QueryTexture(notifBack, NULL, NULL, &notifRect.w, &notifRect.h);
	notifRect.y = -notifRect.h;
	SDL_SetTextureBlendMode(notifBack, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(notifBack, 191);
}

void createNotif(char *text, float life) {
	if (exists) {
		backlogNotifs = realloc(backlogNotifs, ++backlogSize * sizeof(*backlogNotifs));
		backlogNotifs[backlogSize - 1].text = createText(text, 10, 0, notifFont);
		backlogNotifs[backlogSize - 1].fadeIn = true;
		backlogNotifs[backlogSize - 1].fadeOut = false;
		backlogNotifs[backlogSize - 1].timeLeft = life;
		return;
	}
	currentNotif.text = createText(text, 10, 0, notifFont);
	currentNotif.fadeIn = true;
	currentNotif.fadeOut = false;
	currentNotif.timeLeft = life;
	exists = true;
}

void renderNotifs() {
	if (!exists)
		return;
	bool delete = false;
	if (currentNotif.fadeIn) {
		FadeTime += 1;
		notifRect.y += notifRect.h / animTime;
		if (FadeTime == animTime) {
			currentNotif.fadeIn = false;
		}
	} else if (currentNotif.fadeOut) {
		FadeTime -= 1;
		notifRect.y -= notifRect.h / animTime;
		if (FadeTime == 0) {
			currentNotif.fadeOut = false;
			delete = true;
		}
	} else {
		currentNotif.timeLeft -= dt;
		if (currentNotif.timeLeft < 0) {
			currentNotif.fadeOut = true;
		}
	}
	currentNotif.text.rect.y = notifRect.y + 10;
	SDL_RenderCopy(renderer, notifBack, NULL, &notifRect);
	SDL_RenderCopy(renderer, currentNotif.text.tex, NULL, &currentNotif.text.rect);
	if (delete) {
		freeText(currentNotif.text);
		exists = false;
		if (backlogSize != 0) {
			currentNotif.fadeIn = backlogNotifs[0].fadeIn;
			currentNotif.fadeOut = backlogNotifs[0].fadeOut;
			currentNotif.timeLeft = backlogNotifs[0].timeLeft;
			currentNotif.text = backlogNotifs[0].text;
			exists = true;
			backlogSize--;
			for (size_t x = 0; x < backlogSize; x++) {
				backlogNotifs[x] = backlogNotifs[x + 1];
			}
			backlogNotifs = realloc(backlogNotifs, backlogSize * sizeof(*backlogNotifs));
		}
		return;
	}
}