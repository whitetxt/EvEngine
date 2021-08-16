#pragma once

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "game.h"
#include "objects/player.h"
#include "objects/gameObj.h"
#include "events.h"
#include "objects/maps.h"
#include "menu/text.h"
#include "settings.h"
#include "client.h"
#include "menu/menu.h"
#include "notif.h"
#include "menu/mainMenu.h"
#include "pvp/pvp.h"

extern SDL_Texture *playerTexture;
extern SDL_Texture *playerTextureCrouch;
extern struct Player MainPlayer;
extern struct OtherPlayer *players;
extern size_t numPlayers;
extern bool paused;
extern double dt;
extern bool playing;

extern TTF_Font *Font;
extern SDL_Color White;

extern SDL_Texture **loadedTextures;
extern size_t textureSize;

extern SDL_Texture **loadedInteractablesActive;
extern SDL_Texture **loadedInteractablesInactive;
extern size_t interactableSize;

extern size_t textArrSize;
extern struct Text *textArr;

extern bool onServer;

void render();
void calculateDT();