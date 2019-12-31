#pragma once
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <algorithm>
#include "Maths.h"
#include "QuadTree.h"
#include "NeuralNetwork.h"

class Creature

{
public:

	typedef Eigen::Vector2d Vec2;
	Creature();
	Creature(QuadTree** qtree_, std::vector<Point*>* points_,  Boundary window, int* totalHealthptr, std::vector<int> hiddenLayers_);
	void draw(sf::RenderWindow& window);
	void update(float timestep);
	void eat(Point* point);
	void mutate(double rate);
	int getHealth();
	void setHealth(int health_);
	void reset(double rate);
	void reproduce(Creature* creature, double rate);
	NeuralNetwork getNet();
	vec2 pos, vel, acc;
	std::vector<Point*>* points;
	int aliveTicks = 0;
	float startHealth = 30;
private:
	sf::CircleShape shape;
	
	float maxSpeed = 0.5;
	float maxForce = 0.01;
	float size = 5;
	float health = startHealth;
	QuadTree** qtree;
	int w, h;
	int* totalHealth;
	NeuralNetwork net;
};