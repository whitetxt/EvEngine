#pragma once

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
<<<<<<< HEAD:src/main.h
#include <stdint.h>
#include <math.h>
=======
>>>>>>> 1bf14c7a0d33f7162fddec3c2934d02128b27071:Map_Editor/src/main.h
#include "game.h"
#include "objects/gameObj.h"
#include "objects/player.h"
#include "events.h"
#include "objects/maps.h"
<<<<<<< HEAD:src/main.h
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
=======
>>>>>>> 1bf14c7a0d33f7162fddec3c2934d02128b27071:Map_Editor/src/main.h

extern TTF_Font *Font;
extern SDL_Color White;

extern SDL_Texture **loadedTextures;
extern size_t textureSize;
extern size_t selectedTexture;

extern SDL_Texture **loadedInteractablesActive;
extern SDL_Texture **loadedInteractablesInactive;
extern size_t interactableSize;
extern size_t selectedInteractable;

extern struct Tile currentTile;

extern bool onServer;

void render();
void calculateDT();