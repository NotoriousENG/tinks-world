#pragma once

#include "defs.h"
#include "entity.h"
#include "resources.h"

typedef struct {
	Entity collectables[NUM_COLLECTABLES];
	Entity player;
	Entity enemy;
	float playerSpeed;
	float enemySpeed;
	int playerScore;
	int enemyScore;
	int level;
	Resources *resources;

} Scene;

Scene scene_new(Resources *resources);

void scene_begin(Scene *scene);

void scene_draw(SDL_Renderer *renderer, Scene *scene);

void scene_logic(Scene *scene, float delta);

void playerLogic(Scene *scene, float delta);

void enemyLogic(Scene *scene, float delta);

void collectableLogic(Scene *scene);