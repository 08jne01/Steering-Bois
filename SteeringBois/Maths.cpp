#include "Maths.h"

vec2 normalize(vec2 z)

{
	float mag = sqrt(z*z);
	vec2 normalised = z / mag;
	return normalised;
}

float mag(vec2 z)

{
	return sqrt(z*z);
}

int randomi(int min, int max)

{
	return rand() % (max - min + 1) + min;
}

vec2 rot(vec2 vec, float angle)

{
	return vec2(vec.x*cos(angle) - vec.y*sin(angle), vec.x*sin(angle) + vec.y*cos(angle));
}