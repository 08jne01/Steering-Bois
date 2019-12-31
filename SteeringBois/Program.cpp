#include "Program.h"

Program::Program(int width, int height, std::vector<int> hiddenLayers_) : w(width), h(height), window(sf::VideoMode(width, height), "Steering Bois"), hiddenLayers(hiddenLayers_)

{
	for (int i = 0; i < 10; i++)

	{
		//points.push_back(new Point(random(0, w), random(0, h), i));
		//if (randomi(0, 3) == 0) points[i]->poison = 0;
		//else points[i]->poison = 1;
	}

	for (int i = 0; i < population; i++)

	{
		int x = randomi(0, width);
		int y = randomi(0, height);
		creatures.push_back(new Creature(&qtree, &points, Boundary(width, height, x , y), &totalCollected, hiddenLayers));
		creatures[i]->pos.x = x;
		creatures[i]->pos.y = y;
		creatures[i]->setHealth(50);
	}
}

Program::~Program()

{
	for (int i = 0; i < points.size(); i++)

	{
		delete points[i];
	}

	for (int i = 0; i < creatures.size(); i++)

	{
		delete creatures[i];
	}
}

int Program::mainLoop()

{
	if (!window.isOpen()) return EXIT_FAILURE;
	else

	{
		sf::Event events;
		sf::Clock clock;

		double frameTime = 1 / 240.0;

		

		while (window.isOpen())

		{
			while (window.pollEvent(events))

			{
				eventHandler(events);
			}

			if (clock.getElapsedTime().asSeconds() > frameTime)

			{
				update();
				clock.restart();
				
			}


			window.clear(sf::Color(255));
			//Draw
			draw();
			//
			window.display();
		}

		return EXIT_SUCCESS;
	}
}

void Program::spawnRandom()

{
	int x = randomi(0, w);
	int y = randomi(0, h);
	creatures.push_back(new Creature(&qtree, &points, Boundary(w, h, x, y), &totalCollected, hiddenLayers));
	int last = creatures.size() - 1;
	creatures[last]->pos.x = x;
	creatures[last]->pos.y = y;
	creatures[last]->setHealth(50);
}

void Program::spawn()

{
	std::vector<Creature*> newCreatures;

	for (int i = 0; i < creatures.size(); i++)

	{
		/*
		if ((creatures[i]->aliveTicks > 10000))

		{
			creatures[i]->aliveTicks = 0;
			Creature* newCreature = new Creature;
			*newCreature = *(creatures[i]);
			newCreature->mutate(0.01);
			int health = creatures[i]->getHealth();
			newCreature->setHealth(health / 2);
			creatures[i]->setHealth(health / 2);
			newCreatures.push_back(newCreature);

			//std::cout << "New one is born" << std::endl;
		}
		*/
		if (creatures[i]->getHealth() > 255)

		{
			creatures[i]->aliveTicks = 0;
			creatures[i]->setHealth(255);
			Creature* newCreature = new Creature;
			*newCreature = *(creatures[i]);
			newCreature->mutate(mutationRate);
			newCreature->setHealth(50);
			newCreature->pos.x += randomi(-10, 10);
			newCreature->pos.y += randomi(-10, 10);
			//int health = creatures[i]->getHealth();
			if (newCreature != nullptr) newCreatures.push_back(newCreature);

			//std::cout << "New one is born" << std::endl;
		}
	}

	for (int i = 0; i < newCreatures.size(); i++)

	{
		creatures.push_back(newCreatures[i]);
	}
	newCreatures.clear();
}

void Program::cull()

{
	std::vector<Creature*> buffer;

	for (int i = 0; i < creatures.size(); i++)

	{
		if (creatures[i]->getHealth() < 0)

		{
			delete creatures[i];
		}

		else if (nullptr)

		{
			std::cout << "nullptr" << std::endl;
		}

		else buffer.push_back(creatures[i]);
	}

	creatures.clear();
	creatures = buffer;
	buffer.clear();
}

void Program::sortCreatures()

{
	for (int i = 0; i < creatures.size(); i++)

	{
		int index = -1;
		int max = -1;

		for (int j = i; j < creatures.size(); j++)

		{
			if (creatures[i]->getHealth() > max)

			{
				index = j;
				max = creatures[i]->getHealth();
			}
		}

		Creature* temp = creatures[i];
		creatures[i] = creatures[index];
		creatures[index] = temp;
	}

	for (int i = 0; i < creatures.size(); i++)

	{
		if (i > population / 2)

		{
			delete creatures[i];
		}

		else

		{
			savedCreatures.push_back(creatures[i]);
		}
	}

	creatures.clear();
}

void Program::spawnCreature()

{
	//std::cout << creatures.size() << std::endl;
	if (genTime > 20000 || creatures.size() < ((population / 2) + 1))

	{
		sortCreatures();
		std::cout << "New Generation " << std::endl << "Net collected: " << totalCollected << std::endl;
		mutationRate = 25.0 / (float)totalCollected;
		totalCollected = 0;
		genTime = 0;
		for (int i = 0; i < savedCreatures.size(); i++)

		{
			if (savedCreatures[i] != nullptr)

			{
				savedCreatures[i]->reset(mutationRate);
			}
		}

		//std::vector<Creature*> buffer;

		for (int i = 0; i < savedCreatures.size() / 2; i++)

		{
			if (savedCreatures[2 * i] != nullptr && savedCreatures[2 * i + 1] != nullptr)

			{
				savedCreatures[2 * i]->reproduce(savedCreatures[2 * i + 1], 0.5);
				creatures.push_back(savedCreatures[2 * i]);
				Creature* baby1; 
				Creature* baby2; 
				
				baby1 = new Creature;
				(*baby1) = (*savedCreatures[2 * i]);
				baby1->reset(mutationRate);
				baby1->mutate(mutationRate);
				if (baby1 != nullptr) creatures.push_back(baby1);

				baby2 = new Creature;
				(*baby2) = (*savedCreatures[2 * i + 1]);
				baby2->reset(mutationRate);
				baby2->mutate(mutationRate);
				if (baby2 != nullptr) creatures.push_back(baby2);

				creatures.push_back(savedCreatures[2 * i + 1]);
			}
		}
		std::cout << "New population: " << creatures.size() << std::endl;
		if (savedCreatures.size() % 2 != 0) delete savedCreatures[savedCreatures.size() - 1];
		savedCreatures.clear();
	}
}

void Program::addFood()

{
	if (foodTicks > 20 && creatures.size() != 0)

	{
		int number = 0;
		for (int i = 0; i < points.size(); i++)

		{
			if (points[i]->poison) number++;
		}
		//std::cout << number << std::endl;

		if (randomi(0, 1) == 0 && number < 20)

		{
			Point* point = new Point(randomi(0, w), randomi(0, h), points.size() - 1);
			point->poison = 1;
			if (point != nullptr) points.push_back(point);
		}
		else if (points.size() - number < 20)

		{
			Point* point = new Point(randomi(0, w), randomi(0, h), points.size() - 1);
			point->poison = 0;
			if (point != nullptr) points.push_back(point);
		}
		
		foodTicks = 0;
	}
	
}

void Program::update()

{
	cull();
	//std::cout << creatures.size() << std::endl;
	genTime += updateSpeed;
	spawnCreature();
	foodTicks += 1.0*updateSpeed;
	//spawn();
	addFood();
	
	delete qtree;
	qtree = new QuadTree(Boundary(w, h, w/2.0, h/2.0), 4);
	//std::cout << points.size() << std::endl;
	for (int i = 0; i < points.size(); i++)

	{
		qtree->insert((*points[i]));
	}

	

	for (int i = 0; i < creatures.size(); i++)

	{
		creatures[i]->update(updateSpeed);
	}

	getBest();
}

void Program::getBest()

{
	if (creatures.size() > 0)

	{
		int index = 0;
		int max = -1;

		for (int i = 0; i < creatures.size(); i++)

		{
			if (creatures[i]->getHealth() > max)

			{
				max = creatures[i]->getHealth();
				index = i;
			}
		}

		if (creatures[best] == nullptr) best = index;
		else if (creatures[index] != nullptr)
		{
			if (creatures[index]->getHealth() > creatures[best]->getHealth())

			{

				best = index;
			}

		}
	}
}

void Program::drawBest()

{
	if (creatures.size() > 0)

	{
		sf::CircleShape highlight(10);
		highlight.setOutlineColor(sf::Color(100, 255, 100, 100));
		highlight.setOutlineThickness(2);
		highlight.setFillColor(sf::Color(0));
		if (creatures[best] != nullptr)

		{
			creatures[best]->getNet().draw(window, best);
			highlight.setPosition(sf::Vector2f(creatures[best]->pos.x - 10, creatures[best]->pos.y - 10));
		}

		window.draw(highlight);
	}
}

void Program::draw()

{
	drawBest();

	for (int i = 0; i < creatures.size(); i++)

	{
		if (creatures[i] != nullptr) creatures[i]->draw(window);
	}

	//sf::VertexArray drawPoints;
	//sf::Color color = sf::Color(0, 255, 0, 255);

	//for (int i = 0; i < detected.size(); i++)

	//{
		//drawPoints.append(sf::Vertex(sf::Vector2f(detected[i].x, detected[i].y), color));
	//}

	if (qtree != nullptr) qtree->draw(window);

	//sf::CircleShape circle(100);
	//circle.setFillColor(sf::Color(0, 0, 0, 0));
	//circle.setOutlineColor(sf::Color(0, 255, 0, 255));
	//circle.setOutlineThickness(1);
	//sf::Vector2i pos = sf::Mouse::getPosition(window);
	//circle.setPosition(sf::Vector2f(pos.x - 100, pos.y - 100));
	//window.draw(circle);

	//window.draw(drawPoints);

	
}

void Program::eventHandler(sf::Event events)

{
	if (events.type == sf::Event::EventType::Closed)

	{
		window.close();
	}

	if (events.type == sf::Event::KeyPressed)

	{
		switch (events.key.code)

		{
		case sf::Keyboard::Up:
			updateSpeed += 1.0;
			std::cout << "Timestep: " << updateSpeed << std::endl;
			break;
		case sf::Keyboard::Down:
			updateSpeed -= 1.0;
			std::cout << "Timestep: " << updateSpeed << std::endl;
			break;
		case sf::Keyboard::Space:
			spawnRandom();
			break;
		}
	}
}

int Program::random(int min, int max)

{
	return rand() % (max - min + 1) + min;
}