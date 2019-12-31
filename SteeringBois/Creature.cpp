#include "Creature.h"

Creature::Creature()

{

}

Creature::Creature(QuadTree** qtree_, std::vector<Point*>* points_, Boundary window, int* totalHealthptr, std::vector<int> hiddenLayers_) : shape(10, 3), qtree(qtree_), w(window.w), h(window.h), points(points_),
totalHealth(totalHealthptr), net(hiddenLayers_)

{
	shape.setRadius(size);
	shape.setOrigin(shape.getGlobalBounds().width / 2.0, shape.getGlobalBounds().height / 2.0);
	pos.x = 400;
	pos.y = 300;

	vel.x = (float)(randomi(0, 100))/100.0;
	vel.y = (float)(randomi(0, 100))/100.0;
}

void Creature::mutate(double rate)

{
	net.mutate(rate);
}

int Creature::getHealth()

{
	return health;
}

void Creature::setHealth(int health_)

{
	health = health_;
}

void Creature::update(float timestep)

{
	aliveTicks += timestep;

	if (mag(acc) > maxForce) acc = normalize(acc)*maxForce;
	vel += acc * timestep;
	if (mag(vel) > maxSpeed) vel = normalize(vel)*maxSpeed;
	//vel = normalize(vel)*maxSpeed;
	pos += vel * timestep;

	//health -= timestep*0.01;

	if (pos.x - size > w) pos.x = 0.0;
	if (pos.x + size < 0) pos.x = (float)w;
	if (pos.y - size > h) pos.y = 0.0;
	if (pos.y + size < 0) pos.y = (float)h;

	//if (pos.x - 10 > w) health = -2000000;
	//if (pos.x + 10 < 0) health = -2000000;
	//if (pos.y - 10 > h) health = -2000000;
	//if (pos.y + 10 < 0) health = -2000000;

	std::vector<Point> detected;
	if (*qtree != nullptr) (*qtree)->query(detected, Boundary(1.0, 1.0, pos.x, pos.y, 50.0));


	//detected = { Point(0, 2, 0), Point(0, 1, 0) , Point(0, 5, 0) ,Point(0, 4, 0) };

	for (int i = 0; i < detected.size(); i++)

	{
		double minDistSqr = 100000000.0;
		int close = -1;
		for (int j = i; j < detected.size(); j++)

		{
			float distSqr = pow(detected[j].x - pos.x, 2) + pow(detected[j].y - pos.y, 2);
			if (distSqr < minDistSqr)

			{
				minDistSqr = distSqr;
				close = j;
			}
		}
		Point temp = detected[i];
		detected[i] = detected[close];
		detected[close] = temp;
	}

	Eigen::VectorXd input;
	input.resize(4);

	std::vector<vec2> foods;
	std::vector<vec2> poisons;

	for (int i = 0; i < detected.size(); i++)

	{
		if (detected[i].poison == 1)

		{
			
			poisons.push_back(vec2(detected[i].x, detected[i].y));
		}

		else if (detected[i].poison == 0)

		{
			foods.push_back(vec2(detected[i].x, detected[i].y));
		}
	}

	if (foods.size() > 0)

	{
		input[0] = (foods[0].x - pos.x);
		input[1] = (foods[0].y - pos.y);
	}

	else

	{
		input[0] = 0.0;
		input[1] = 0.0;
	}

	if (poisons.size() > 0)

	{
		input[2] = (poisons[0].x - pos.x);
		input[3] = (poisons[0].y - pos.y);
	}

	else

	{
		input[2] = 0.0;
		input[3] = 0.0;
	}
	/*
	for (int i = 0; i < 1; i++)

	{
		if (i < foods.size())

		{
			input[i * 2] = foods[i].x - pos.x;
			input[i * 2 + 1] = foods[i].y - pos.y;
		}

		else

		{
			input[i * 3] = vel.x;
			input[i * 3 + 1] = vel.y;
		}

		if (i < poisons.size())

		{
			input[6 + (i * 2)] = poisons[i].x - pos.x;
			input[6 + (i * 2 + 1)] = poisons[i].y - pos.y;
		}

		else

		{
			input[6 + (i * 2)] = vel.x;
			input[6 + (i * 2 + 1)] = vel.y;
		}

		if (i < detected.size())

		{
			input[i * 3] = detected[i].x;
			input[i * 3 + 1] = detected[i].y;
			int type = (double)detected[i].poison;
			if (type) input[i * 3 + 2] = 1;
			else input[i * 3 + 2] = -1;
			
		}

		else

		{
			input[i * 3] = vel.x;
			input[i * 3 + 1] = vel.y;
			input[i * 3 + 2] = -1.0;
		}
	//}
	*/
	
	//if (pos.x < w / 2.0) input[15] = pos.x;
	//else input[15] = w - pos.x;

	//if (pos.y < h / 2.0) input[16] = pos.y;
	//else input[16] = h - pos.y;

	//input[15] = pos.x;
	//input[16] = pos.y;
	//input[17] = 1.0;

	//if (detected.size() > 0)  input = { detected[close].x - pos.x, detected[close].y - pos.y, (double)detected[close].poison};
	//else input = { vel.x, vel.y, 0.0 };
	net.calculate(input);

	acc.x = net.getOutput()[0]*maxForce;
	acc.y = net.getOutput()[1]*maxForce;

	detected.clear();
	if (*qtree != nullptr) (*qtree)->query(detected, Boundary(1.0, 1.0, pos.x, pos.y, size));
	for (int i = 0; i < detected.size(); i++)

	{
		if (detected[i].poison) health -= 40, (*totalHealth)--, eat(detected[i].here);
		else health += 40, (*totalHealth)++, eat(detected[i].here);
	}

	if (health > 255) health = 256;
}

void Creature::reset(double rate)

{
	health = startHealth;
	pos.x = randomi(0, w);
	pos.y = randomi(0, h);
	vel.x = 0.0;
	vel.y = 0.0;
	acc.x = 0.0;
	acc.y = 0.0;
	aliveTicks = 0;
	//net.mutate(rate);
}

NeuralNetwork Creature::getNet()

{
	return net;
}

void Creature::reproduce(Creature* creature, double rate)

{
	if (creature == nullptr) return;

	net.crossover(0.5, &(creature->getNet()));
	net.mutate(rate);
}

void Creature::draw(sf::RenderWindow& window)

{
	shape.setPosition({ pos.x, pos.y});

	float angle = 180 * atan(vel.x / -vel.y) / PI - 180;
	if (-vel.y > 0) angle += 180;

	shape.setRotation(angle);
	shape.setFillColor(sf::Color(clamp(255.0-health,0.0,255.0), clamp(health, 0.0f, 255.0f), 0, 200));
	window.draw(shape);
}

void Creature::eat(Point* point)

{
	std::vector<Point*> buffer;
	//int total = 100;

	//if (points == nullptr) std::cout << "Test" << std::endl;

	if (points == nullptr) std::cout << "Test" << std::endl;

	for (int i = 0; i < points->size(); i++)

	{
		if (point != (*points)[i]) buffer.push_back((*points)[i]);
		else delete (*points)[i];
	}

	//for (int i = 0; i < buffer.size(); i++)

	//{
		//buffer[i]->index = i;
	//}

	(*points) = buffer;
}