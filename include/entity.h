#pragma once

#include <SDL.h>
#include "vec2.h"

#include "resources.h"

typedef struct Entity Entity;
struct Entity {
	vec2 position;
	float rotation;
	float scale;
	SDL_Texture *texture;
	int visible;
};

void entity_draw(SDL_Renderer *renderer, Entity entity);