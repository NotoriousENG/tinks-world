#include "resources.h"

Resources load_resources(SDL_Renderer *renderer)
{
	Resources resources;
	memset(&resources, 0, sizeof(Resources));
	resources.music = load_music("assets/cat_song.ogg");
	resources.font = loadFont("assets/Cattie-Regular.ttf", 64);
	resources.tink = loadTexture(renderer, "assets/tink.png");
	resources.anya = loadTexture(renderer, "assets/anya.png");
	resources.food = loadTexture(renderer, "assets/wf.png");
	resources.joystick = SDL_JoystickOpen(0);
	return resources;
}

void free_resources(Resources resources)
{
	Mix_FreeMusic(resources.music);
	TTF_CloseFont(resources.font);
	SDL_DestroyTexture(resources.tink);
	SDL_DestroyTexture(resources.anya);
	SDL_DestroyTexture(resources.food);

	if (resources.joystick != NULL) {
		SDL_JoystickClose(resources.joystick);
	}
}

Mix_Music *load_music(char *filename)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
		       "Loading %s", filename);

	Mix_Music *music = Mix_LoadMUS(filename);

	if (music == NULL) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			       SDL_LOG_PRIORITY_ERROR, "Failed to load %s",
			       filename);
		exit(1);
	}
	return music;
}

void play_music_on_loop(Mix_Music *music)
{
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(music, -1);
	}
}

TTF_Font *loadFont(char *filename, int size)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
		       "Loading %s", filename);
	TTF_Font *font = TTF_OpenFont(filename, size);
	if (font == NULL) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			       SDL_LOG_PRIORITY_ERROR,
			       "Failed to load font: %s", TTF_GetError());
		exit(1);
	}
	return font;
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, char *filename)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
		       "Loading %s", filename);
	SDL_Texture *texture = IMG_LoadTexture(renderer, filename);

	if (renderer == NULL) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			       SDL_LOG_PRIORITY_ERROR,
			       "Failed to load %s RENDERER IS NULL", filename);
		exit(1);
	}

	if (texture == NULL) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
			       SDL_LOG_PRIORITY_ERROR, "Failed to load %s",
			       filename);
		exit(1);
	}
	return texture;
}

void draw(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
	  float rotation, float scale)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	dest.w *= scale;
	dest.h *= scale;
	SDL_RenderCopyEx(renderer, texture, NULL, &dest, rotation, NULL,
			 SDL_FLIP_NONE);
}

void drawText(SDL_Renderer *renderer, char *text, int x, int y, TTF_Font *font)
{
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
}

void doKeyUp(int *keyboard, SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
		keyboard[event->keysym.scancode] = 0;
	}
}

void doKeyDown(int *keyboard, SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
		keyboard[event->keysym.scancode] = 1;
	}
}

void doInput(int *keyboard)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_KEYDOWN:
			doKeyDown(keyboard, &event.key);
			break;
		case SDL_KEYUP:
			doKeyUp(keyboard, &event.key);
			break;
		default:
			break;
		}
	}
}