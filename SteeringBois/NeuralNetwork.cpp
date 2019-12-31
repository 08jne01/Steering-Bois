#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()

{
	//weights1 = Eigen::MatrixXd::Random(20,18);
	//weights2 = Eigen::MatrixXd::Random(2, 20);

	//hidden.resize(20);
	//output.resize(2);

	//bias1 = Eigen::VectorXd::Random(20);
	//bias2 = Eigen::VectorXd::Random(2);

	setup();
}

NeuralNetwork::NeuralNetwork(std::vector<int> hiddenLayers_)

{
	int input = sizes[0];
	int output = sizes[sizes.size() - 1];
	sizes.clear();
	sizes.push_back(input);
	for (int i = 0; i < hiddenLayers_.size(); i++)

	{
		sizes.push_back(hiddenLayers_[i]);
	}
	sizes.push_back(output);

	setup();
}

NeuralNetwork::NeuralNetwork(Matrix weights1_, Matrix weights2_, Vector bias1_, Vector bias2_):
	weights1(weights1_), weights2(weights2_), bias1(bias1_), bias2(bias2_)

{
	setup();
}

void NeuralNetwork::setup()

{
	for (int i = 0; i < sizes.size() - 1; i++)

	{
		weights.push_back(Eigen::MatrixXd::Random(sizes[i + 1], sizes[i]));
		biases.push_back(Eigen::VectorXd::Zero(sizes[i + 1]));
	}

	for (int i = 0; i < sizes.size(); i++)

	{
		Vector vector(sizes[i]);
		vectors.push_back(vector);
	}

	textSetup();
}

NeuralNetwork::Vector NeuralNetwork::getOutput()

{
	return vectors[vectors.size()-1];
}

void NeuralNetwork::sigmoid(Vector& output)

{
	Eigen::ArrayXd outputNew = output.array();
	outputNew = outputNew.tanh();

	outputNew = (outputNew + 1.0)/2.0;

	output = outputNew.matrix();
}

void NeuralNetwork::relu(Vector& output)

{
	output.cwiseMax(0.0);
}

void NeuralNetwork::tanhNN(Vector& output)

{
	Eigen::ArrayXd outputNew = output.array();
	outputNew = outputNew.tanh();

	output = outputNew.matrix();
}

float NeuralNetwork::sigmoidSingle(float input)

{
	return (tanh(input) + 1.0) / 2.0;
}

void NeuralNetwork::calculate(const Vector input)

{
	//hidden = (weights1 * input) + bias1;
	//sigmoid(hidden);

	//output = (weights2 * hidden) + bias2;
	//sigmoid(output);

	vectors[0] = input;
	//sigmoid(vectors[0]);

	for (int i = 0; i < sizes.size() - 1; i++)

	{
		vectors[i+1] = (weights[i]*vectors[i]) + biases[i];
		if (i+1 != vectors.size() - 1) relu(vectors[i + 1]);
		else tanhNN(vectors[i + 1]);
		//std::cout << vectors[i + 1].size() << std::endl;;
	}
}

void NeuralNetwork::mutate(double rate)

{
	//weights1 += rate*Eigen::MatrixXd::Random(20, 18);
	//weights2 += rate*Eigen::MatrixXd::Random(2, 20);

	//bias1 += rate*Eigen::VectorXd::Random(20);
	//bias2 += rate*Eigen::VectorXd::Random(2);
	//std::cout << "Mutate" << std::endl;
	for (int i = 0; i < weights.size(); i++)

	{
		weights[i] += rate*Eigen::MatrixXd::Random(weights[i].rows(), weights[i].cols());
		biases[i] += rate * Eigen::VectorXd::Random(biases[i].size());
	}
	//std::cout << "Mutate Done" << std::endl;
}

void NeuralNetwork::crossover(double percentage, NeuralNetwork* other)

{
	for (int i = 0; i < weights.size(); i++)

	{
		for (int j = 0; j < int((double)(weights[i].cols()) * percentage); j++)

		{
			int splice = randomi(0, weights[i].cols() - 1);
			int splicy = randomi(0, weights[i].cols() - 1);

			int start = randomi(0, weights[i].col(splice).size() - 1);
			int end = randomi(start, weights[i].col(splice).size());
			for (int k = start; k < end; k++)

			{
				double saved = weights[i].col(splice)[k];
				weights[i].col(splice)[k] = other->weights[i].col(splicy)[k];
				other->weights[i].col(splicy)[k] = saved;
			}
			//Vector saved = weights[i].row(splice);
			//weights[i].row(splice) = other->weights[i].row(splicy);
			//other->weights[i].row(splicy) = saved;
		}
	}
}

void NeuralNetwork::textSetup()

{
	font.loadFromFile("arial.ttf");
	text.setCharacterSize(20);
	text.setFillColor(sf::Color(255, 255, 255, 255));
	text.setPosition({ 10, 10 });
	text.setFont(font);
	text.setString("0.1");
}

void NeuralNetwork::draw(sf::RenderWindow& window, int id)

{
	
	text.setFont(font);
	std::stringstream os;
	os << id;
	text.setString(os.str());

	sf::CircleShape circle(10);
	circle.setOutlineColor(sf::Color(255, 255, 255));
	circle.setOutlineThickness(1);
	double x = 10.0;
	double y = 10.0 + 150;
	text.setPosition(x, y - 130);

	//std::cout << id << std::endl;
	window.draw(text);

	sf::VertexArray line;
	line.setPrimitiveType(sf::Lines);
	line.append(sf::Vertex(sf::Vector2f(0, 0)));
	line.append(sf::Vertex(sf::Vector2f(0, 0)));

	for (int i = 0; i < vectors.size(); i++)

	{
		for (int j = 0; j < vectors[i].size(); j++)

		{
			double offset = vectors[i].size() * 25 / 2.0;
			double color = sigmoidSingle(vectors[i][j])*255.0;
			//if (i == 0) std::cout << color << std::endl;
			circle.setFillColor(sf::Color(color, color, color, 200));
			circle.setPosition(sf::Vector2f(x + i * 50, y + j * 25 - offset));
			window.draw(circle);

			if (i + 1 < vectors.size())

			{
				line[0].position.x = x + i * 50 + 10;
				line[0].position.y = y + j * 25 + 10 - offset;

				double max = weights[i].maxCoeff();

				for (int k = 0; k < vectors[i + 1].size(); k++)

				{
					sf::Color lineColor;

					double value = 255.0 * sigmoidSingle(weights[i].row(k).col(j).value() / max);
					lineColor = sf::Color(value, value, value, 255);

					line[0].color = lineColor;
					line[1].color = lineColor;

					line[1].position.x = x + (i + 1) * 50 + 10;
					line[1].position.y = y + k * 25 + 10 - vectors[i + 1].size() * 25 / 2.0;
					window.draw(line);
				}
			}

		}
	}
}