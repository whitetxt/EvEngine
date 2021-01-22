#pragma once

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "objects/gameObj.h"
#include "objects/player.h"
#include "events.h"
#include "objects/maps.h"
#include "text.h"

extern TTF_Font *Font;
extern SDL_Color Black;

extern SDL_Texture **loadedTextures;
extern size_t textureSize;

extern size_t textArrSize;
extern struct Text *textArr;

extern int dt;