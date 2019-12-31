#pragma once

struct vec2

{
	vec2(float x_, float y_) : x(x_), y(y_)

	{

	}

	vec2(float v) : x(v), y(v)

	{

	}

	vec2()

	{

	}

	vec2 operator+ (const vec2& v) const

	{
		vec2 result;
		result.x = this->x + v.x;
		result.y = this->y + v.y;
		return result;
	}

	vec2 operator- (const vec2& v) const

	{
		vec2 result;
		result.x = this->x - v.x;
		result.y = this->y - v.y;
		return result;
	}

	vec2 operator+= (const vec2& v)

	{
		this->x += v.x;
		this->y += v.y;
		return 0;
	}

	vec2 operator+= (const float& v)

	{
		this->x += v;
		this->y += v;
		return 0;
	}

	float operator* (const vec2& v) const

	{
		float result;
		result = (this->x * v.x) + (this->y * v.y);
		return result;
	}

	vec2 operator* (const float& v) const

	{
		vec2 result;
		result.x = this->x*v;
		result.y = this->y*v;
		return result;
	}

	vec2 operator/ (const float& v) const

	{
		vec2 result;
		result.x = this->x/v;
		result.y = this->y/v;
		return result;
	}

	vec2 operator*= (const float& v)

	{
		this->x *= v;
		this->y *= v;
		return 0;
	}

	vec2 operator- () const

	{
		return vec2(-(this->x), -(this->y));
	}

	float x;
	float y;
};