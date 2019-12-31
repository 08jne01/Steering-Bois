#pragma once
#include <math.h>
#include <random>
#include "vec2.h"

#define PI 3.14159

vec2 normalize(vec2 z);

float mag(vec2 z);

int randomi(int min, int max);

vec2 rot(vec2 vec, float angle);

template<class T>

T clamp(const T& v, const T&lo, const T& hi)

{
	T output;
	output = std::min(hi, v);
	output = std::max(lo, output);
	return output;
}