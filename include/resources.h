#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "defs.h"

typedef struct Resources Resources;

struct Resources {
	Mix_Music *music;
	TTF_Font *font;
	SDL_Texture *tink;
	SDL_Texture *anya;
	SDL_Texture *food;
	int keyboard[MAX_KEYBOARD_KEYS];
	SDL_Joystick *joystick;
	int running;
};

Resources load_resources(SDL_Renderer *renderer);

void free_resources(Resources resources);

Mix_Music *load_music(char *path);

TTF_Font *loadFont(char *filename, int size);

SDL_Texture *loadTexture(SDL_Renderer *renderer, char *filename);

// blit with rotation
void draw(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
	  float rotation, float scale);

// draw text with SDL_ttf with specified font
void drawText(SDL_Renderer *renderer, char *text, int x, int y, TTF_Font *font);

void play_music_on_loop(Mix_Music *music);

void doKeyUp(int *keyboard, SDL_KeyboardEvent *event);

void doKeyDown(int *keyboard, SDL_KeyboardEvent *event);

void handleJoystick(SDL_JoyAxisEvent *event);

void doInput(int *keyboard);