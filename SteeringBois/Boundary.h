#pragma once
#include "Point.h"
class Boundary

{
public:
	Boundary(double width, double height, double xPos, double yPos, double radius = -1): w(width), h(height), x(xPos), y(yPos), r(radius)

	{

	}

	int contains(Point point)

	{
		if (r < 0.0)

		{
			return (point.x >= x - w / 2.0 &&
				point.x <= x + w / 2.0 &&
				point.y >= y - h / 2.0 &&
				point.y <= y + h / 2.0);
		}

		else

		{
			return (r*r > (pow(point.x - x, 2) + pow(point.y - y, 2)));
		}
		
	}

	int intersects(Boundary range)

	{
		if (range.r < 0.0)

		{
			return !(range.x - range.w > x + w || range.x + range.w < x - w || range.y - range.h > y + h || range.y + range.h < y - h);
		}

		else

		{
			//std::cout << "Test" << std::endl;
			double distX = abs(x - range.x);
			double distY = abs(y - range.y);

			//double distBLSqr = pow(x - range.x - w/2.0, 2) + pow(y - range.y + h/2.0, 2);
			//double distBRSqr = pow(x - range.x + w/2.0, 2) + pow(y - range.y + h/2.0, 2);
			//double distTLSqr = pow(x - range.x - w/2.0, 2) + pow(y - range.y - h/2.0, 2);
			//double distTRSqr = pow(x - range.x + w/2.0, 2) + pow(y - range.y - h/2.0, 2);
			//double distC = pow(x - range.x, 2) + pow(y - range.y, 2);

			//return (range.r*range.r > std::min(std::min(distBLSqr, std::min(distBRSqr, std::min(distTLSqr, distTRSqr))), distC));

			//std::cout << distX << " " << distY << std::endl;

			if (distX > (w / 2.0 + range.r)) return 0;
			//else return 1;

			if (distY > (h / 2.0 + range.r)) return 0;
			//else return 1;

			if (distX <= (w / 2.0)) return 1;
			if (distY <= (h / 2.0)) return 1;

			double cornerSq = pow(distX - w / 2.0, 2) + pow(distY - h / 2.0, 2);
			//std::cout << "Test" << std::endl;
			return (cornerSq <= range.r * range.r);
		}
	}

	double w, h, x, y, r;
private:

};