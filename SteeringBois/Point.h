#pragma once
struct Point

{
	Point(double x_, double y_, int index_): x(x_), y(y_), index(index_)

	{

	}

	double x, y;
	int index, poison;
	Point* here = this;
};