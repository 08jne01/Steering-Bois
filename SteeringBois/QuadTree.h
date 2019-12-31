#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Boundary.h"
#include "Point.h"
class QuadTree

{
public:
	QuadTree(Boundary boundary_, int capacity_);
	~QuadTree();
	void draw(sf::RenderWindow& window);
	void drawRectangle(sf::RenderWindow& window, double x, double y, double w, double h);
	void subDivide();
	void insert(Point point);
	void query(std::vector<Point>& output, Boundary range);
private:
	int divided = 0;
	int capacity;
	std::vector<Point> points;
	sf::VertexArray drawablePoints;
	Boundary boundary;
	QuadTree *northwest = nullptr;
	QuadTree *northeast = nullptr;
	QuadTree *southwest = nullptr;
	QuadTree *southeast = nullptr;
};