#pragma once

typedef struct color_rgb color_rgb;

struct color_rgb {
	float r;
	float g;
	float b;
};

typedef struct color_hsv color_hsv;

struct color_hsv {
	float h;
	float s;
	float v;
};

color_rgb hue_shift(color_rgb color, float delta);

color_rgb hsv_to_rgb(color_hsv hsv);

color_hsv rgb_to_hsv(color_rgb rgb);