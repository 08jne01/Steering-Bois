#pragma once
#include <SFML/Graphics.hpp>
#include "QuadTree.h"
#include "Creature.h"

class Program

{
public:
	Program(int width, int height, std::vector<int> hiddenLayers_);
	~Program();
	int mainLoop();
	int random(int min, int max);
	void eventHandler(sf::Event events);
	void update();
	void draw();
	void cull();
	void addFood();
	void spawn();
	void spawnRandom();
	void spawnCreature();
	void sortCreatures();
	void drawBest();
	void getBest();
private:
	int w, h;
	double updateSpeed = 1.0;
	sf::RenderWindow window;
	QuadTree* qtree;
	std::vector<Point*> points;
	std::vector<Creature*> creatures;
	std::vector<Creature*> savedCreatures;
	std::vector<int> hiddenLayers;
	int foodTicks = 0;
	int population = 200;
	int mutationRate = 0.1;
	int genTime = 0;
	int best = 0;
	int totalCollected = 0;
};