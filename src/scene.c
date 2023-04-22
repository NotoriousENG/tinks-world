#include "scene.h"
#include "defs.h"

Scene scene_new(Resources *resources)
{
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.resources = resources;
	return scene;
}

void scene_begin(Scene *scene)
{
	// Create Player
	scene->player.position =
		vec2_new(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f);
	scene->player.texture = scene->resources->tink;
	scene->player.rotation = 0;
	scene->player.scale = 2;
	scene->player.visible = 1;
	scene->playerSpeed =
		PLAYER_SPEED + ADDED_SPEED_PER_LEVEL * scene->level;
	scene->playerScore = 0;

	// Create Enemy
	scene->enemy.position =
		vec2_new(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.2f);
	scene->enemy.texture = scene->resources->anya;
	scene->enemy.rotation = 0;
	scene->enemy.scale = 1;
	scene->enemy.visible = 1;
	scene->enemySpeed =
		PLAYER_SPEED * 0.5f + ADDED_SPEED_PER_LEVEL * scene->level;
	scene->enemyScore = 0;

	// Create Collectables
	for (int i = 0; i < NUM_COLLECTABLES; i++) {
		// give the collectables a random position
		vec2 pos = vec2_new(rand() % (SCREEN_WIDTH - 100),
				    rand() % (SCREEN_HEIGHT - 100));
		for (int j = 0; j < i; j++) {
			// if the collectables are too close to each other, give them a new
			// position
			while (vec2_distance(pos,
					     scene->collectables[j].position) <
			       100) {
				pos = vec2_new(rand() % (SCREEN_WIDTH - 100),
					       rand() % (SCREEN_HEIGHT - 100));
				j = 0;
			}
		}
		scene->collectables[i].position = pos;
		scene->collectables[i].visible = 1;
		scene->collectables[i].scale = 1;
		scene->collectables[i].texture = scene->resources->food;
	}
}

void scene_draw(SDL_Renderer *renderer, Scene *scene)
{
	for (int i = 0; i < NUM_COLLECTABLES; i++) {
		entity_draw(renderer, scene->collectables[i]);
	}
	entity_draw(renderer, scene->enemy);
	entity_draw(renderer, scene->player);

	// draw the player score
	char scoreText[64];
	sprintf(scoreText, "Tink: %d", scene->playerScore);
	drawText(renderer, scoreText, 10, 10, scene->resources->font);

	// draw the enemy score
	sprintf(scoreText, "Anya: %d", scene->enemyScore);
	drawText(renderer, scoreText, 10, 72, scene->resources->font);

	// draw the level
	sprintf(scoreText, "Level: %d", scene->level + 1);
	drawText(renderer, scoreText, SCREEN_WIDTH - 260, 10,
		 scene->resources->font);
}

void scene_logic(Scene *scene, float delta)
{
	playerLogic(scene, delta);
	enemyLogic(scene, delta);
	collectableLogic(scene);
}

void playerLogic(Scene *scene, float delta)
{
	vec2 inputDir = (vec2){ 0, 0 };
	inputDir.x = (scene->resources->keyboard[SDL_SCANCODE_RIGHT] ||
		      scene->resources->keyboard[SDL_SCANCODE_D]) -
		     (scene->resources->keyboard[SDL_SCANCODE_LEFT] ||
		      scene->resources->keyboard[SDL_SCANCODE_A]);
	inputDir.y = (scene->resources->keyboard[SDL_SCANCODE_DOWN] ||
		      scene->resources->keyboard[SDL_SCANCODE_S]) -
		     (scene->resources->keyboard[SDL_SCANCODE_UP] ||
		      scene->resources->keyboard[SDL_SCANCODE_W]);

	// if we get joystick input, use that instead
	if (scene->resources->joystick != NULL) {
		SDL_Joystick *joystick = scene->resources->joystick;

		inputDir.x +=
			(float)SDL_JoystickGetAxis(joystick, 0) / 32767.0f;
		inputDir.y +=
			(float)SDL_JoystickGetAxis(joystick, 1) / 32767.0f;

		Uint8 hatState = SDL_JoystickGetHat(joystick, 0);
		if (hatState & SDL_HAT_UP) {
			inputDir.y -= 1;
		}
		if (hatState & SDL_HAT_DOWN) {
			inputDir.y += 1;
		}
		if (hatState & SDL_HAT_LEFT) {
			inputDir.x -= 1;
		}
		if (hatState & SDL_HAT_RIGHT) {
			inputDir.x += 1;
		}
	}

	// clamp the input x and y to -1 and 1
	inputDir.x = clampf(inputDir.x, -1, 1);
	inputDir.y = clampf(inputDir.y, -1, 1);

	if (vec2_length(inputDir) > 0) {
		inputDir = vec2_normalize(inputDir);
	}

	Entity *player = &scene->player;

	player->position =
		vec2_add(player->position,
			 vec2_scale(inputDir, scene->playerSpeed * delta));

	// rotate the player to face in the direction they are moving
	if (vec2_length(inputDir) > 0) {
		float rot = vec2_angle_degrees(inputDir, (vec2){ 0, -1 });
		player->rotation = smooth_rotation(player->rotation, rot, 0.1f);
	}

	// if the player is off the screen by 100 pixels, wrap them around to the
	// other side
	if (player->position.x < -100) {
		player->position.x = SCREEN_WIDTH + 100;
	}
	if (player->position.x > SCREEN_WIDTH + 100) {
		player->position.x = -100;
	}
	if (player->position.y < -100) {
		player->position.y = SCREEN_HEIGHT + 100;
	}
	if (player->position.y > SCREEN_HEIGHT + 100) {
		player->position.y = -100;
	}
}

void enemyLogic(Scene *scene, float delta)
{
	// enemy moves towards the closest collectable
	Entity *enemy = &scene->enemy;
	Entity *closestCollectable = NULL;
	for (int i = 0; i < NUM_COLLECTABLES; i++) {
		if (!scene->collectables[i].visible) {
			continue;
		}
		if (closestCollectable == NULL ||
		    vec2_distance(enemy->position,
				  scene->collectables[i].position) <
			    vec2_distance(enemy->position,
					  closestCollectable->position)) {
			closestCollectable = &scene->collectables[i];
		}
	}

	if (closestCollectable != NULL) {
		vec2 dir = vec2_subtract(closestCollectable->position,
					 enemy->position);
		if (vec2_length(dir) > 0) {
			dir = vec2_normalize(dir);
		}

		// rotate the enemy to face in the direction they are moving
		if (vec2_length(dir) > 0) {
			float rot = vec2_angle_degrees(dir, (vec2){ 0, -1 });
			enemy->rotation =
				smooth_rotation(enemy->rotation, rot, 0.1f);
		}

		enemy->position =
			vec2_add(enemy->position,
				 vec2_scale(dir, scene->enemySpeed * delta));
	}
}

void collectableLogic(Scene *scene)
{
	// if the player is close to a collectable, remove it
	Entity *player = &scene->player;
	Entity *enemy = &scene->enemy;
	int numCollected = 0;

	for (int i = 0; i < NUM_COLLECTABLES; i++) {
		if (!scene->collectables[i].visible) {
			numCollected++;
			continue;
		}
		if (vec2_distance(player->position,
				  scene->collectables[i].position) < 100) {
			scene->collectables[i].visible = 0;
			scene->playerScore++;
		}
		if (vec2_distance(enemy->position,
				  scene->collectables[i].position) < 50) {
			scene->collectables[i].visible = 0;
			scene->enemyScore++;
		}
	}

	if (numCollected == NUM_COLLECTABLES) {
		if (scene->playerScore > scene->enemyScore) {
			printf("Tink wins!\n");
			scene->level++;
		} else if (scene->playerScore < scene->enemyScore) {
			printf("Anya wins!\n");
		} else {
			printf("It's a tie!\n");
		}
		scene_begin(scene);
	}
}

float smooth_rotation(float from_rot, float to_rot, float delta)
{
	// fix lerping the rotation for 0 and 360 degrees
	if (from_rot > 270 && to_rot < 90) {
		from_rot -= 360;
	}
	if (from_rot < 90 && to_rot > 270) {
		from_rot += 360;
	}

	// lerping the rotation makes it look smoother
	return lerpf(from_rot, to_rot, delta);
}