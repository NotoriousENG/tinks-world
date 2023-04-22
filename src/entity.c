#include "entity.h"

void entity_draw(SDL_Renderer *renderer, Entity entity)
{
	if (entity.visible) {
		draw(renderer, entity.texture, entity.position.x,
		     entity.position.y, entity.rotation, entity.scale);
	}
}