#pragma once
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Maths.h"

class NeuralNetwork

{
public:
	typedef Eigen::MatrixXd Matrix;
	typedef Eigen::VectorXd Vector;

	NeuralNetwork();
	NeuralNetwork(std::vector<int> hiddenLayers_);
	void setup();
	NeuralNetwork(Matrix weights1_, Matrix weights2_, Vector bias1_, Vector bias2_);
	void sigmoid(Vector& output);
	void relu(Vector& output);
	void tanhNN(Vector& output);
	void calculate(const Vector inputs);
	void mutate(double rate);
	void crossover(double percentage, NeuralNetwork* other);
	void textSetup();
	void draw(sf::RenderWindow& window, int id);
	float sigmoidSingle(float input);
	Vector getOutput();
private:
	std::vector<int> sizes = {4, 1, 2};
	std::vector<Vector> vectors;
	std::vector<Matrix> weights;
	std::vector<Vector> biases;
	Vector hidden, output;
	Vector bias1, bias2;
	Matrix weights1, weights2;

	sf::Font font;
	sf::Text text;
};