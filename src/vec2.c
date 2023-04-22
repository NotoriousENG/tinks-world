#include "vec2.h"

#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

vec2 vec2_new(float x, float y)
{
	return (vec2){ x, y };
}
  
float vec2_length(vec2 v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

vec2 vec2_normalize(vec2 v)
{
	float len = vec2_length(v);
	return (vec2){ v.x / len, v.y / len };
}

vec2 vec2_add(vec2 a, vec2 b)
{
	return (vec2){ a.x + b.x, a.y + b.y };
}

vec2 vec2_subtract(vec2 a, vec2 b)
{
	return (vec2){ a.x - b.x, a.y - b.y };
}

vec2 vec2_scale(vec2 v, float s)
{
	return (vec2){ v.x * s, v.y * s };
}

float vec2_distance(vec2 a, vec2 b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrtf(dx * dx + dy * dy);
}

float vec2_angle_degrees(vec2 a, vec2 b)
{
	float angle = atan2f(b.x, b.y) - atan2f(a.x, a.y);
	return angle * 180 / M_PI;
}