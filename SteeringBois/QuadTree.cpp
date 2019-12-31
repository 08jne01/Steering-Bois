#include "QuadTree.h"

QuadTree::QuadTree(Boundary boundary_, int capacity_): boundary(boundary_), capacity(capacity_)

{

}

QuadTree::~QuadTree()

{
	if (northwest != nullptr) delete northwest;
	if (northeast != nullptr) delete northeast;
	if (southwest != nullptr) delete southwest;
	if (southeast != nullptr) delete southeast;
}

void QuadTree::subDivide()

{
	double x = boundary.x;
	double y = boundary.y;
	double w = boundary.w;
	double h = boundary.h;

	Boundary nw = Boundary(w / 2.0, h / 2.0, x - w / 4.0, y - h / 4.0);
	Boundary ne = Boundary(w / 2.0, h / 2.0, x + w / 4.0, y - h / 4.0);
	Boundary sw = Boundary(w / 2.0, h / 2.0, x - w / 4.0, y + h / 4.0);
	Boundary se = Boundary(w / 2.0, h / 2.0, x + w / 4.0, y + h / 4.0);

	northwest = new QuadTree(nw, capacity);
	northeast = new QuadTree(ne, capacity);
	southwest = new QuadTree(sw, capacity);
	southeast = new QuadTree(se, capacity);

	for (int i = 0; i < points.size(); i++)

	{
		northwest->insert(points[i]);
		northeast->insert(points[i]);
		southwest->insert(points[i]);
		southeast->insert(points[i]);
	}

	points.clear();

	divided = 1;
}

void QuadTree::insert(Point point)

{
	if (!boundary.contains(point))

	{
		return;
	}


	if (points.size() < capacity && !divided)

	{
		points.push_back(point);
	}

	else

	{
		if (!divided)

		{
			subDivide();
		}
	}

	if (divided)

	{
		northwest->insert(point);
		northeast->insert(point);
		southwest->insert(point);
		southeast->insert(point);
	}
}

void QuadTree::query(std::vector<Point>& output, Boundary range)

{
	if (!boundary.intersects(range))

	{
		return;
	}

	else if (!divided)

	{
		for (int i = 0; i < points.size(); i++)

		{
			if (range.contains(points[i]))

			{
				output.push_back(points[i]);
			}
		}
	}

	else if (divided)

	{
		northwest->query(output, range);
		northeast->query(output, range);
		southwest->query(output, range);
		southeast->query(output, range);
	}
}

void QuadTree::draw(sf::RenderWindow& window)

{
	//drawRectangle(window, 300, 200, 100, 100);

	if (divided)

	{
		northwest->draw(window);
		northeast->draw(window);
		southwest->draw(window);
		southeast->draw(window);
	}


	sf::CircleShape circle(2);

	for (int i = 0; i < points.size(); i++)

	{
		circle.setPosition({ (float)points[i].x, (float)points[i].y });
		if (points[i].poison) circle.setFillColor(sf::Color(255, 0, 0, 255));
		else circle.setFillColor(sf::Color(0, 255, 0, 255));
		window.draw(circle);
		//drawablePoints.append(sf::Vertex(sf::Vector2f(points[i].x, points[i].y)));
	}

	window.draw(drawablePoints);

	drawRectangle(window, boundary.x, boundary.y, boundary.w, boundary.h);
}

void QuadTree::drawRectangle(sf::RenderWindow& window, double x, double y, double w, double h)

{
	sf::RectangleShape rect(sf::Vector2f( w, h ));
	rect.setPosition(sf::Vector2f(x - (w / 2.0), y - (h / 2.0)));
	rect.setFillColor({ 0,0,0,0 });
	rect.setOutlineThickness(0.5);
	rect.setOutlineColor({ 255,255,255,10});
	window.draw(rect);
}