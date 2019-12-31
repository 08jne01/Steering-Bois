#include "Program.h"

int main(int argc, char** argv)

{
	int input = 1;

	std::vector<int> hidden;
	if (input)

	{
		int layers = 0;
		std::cout << "number of hidden layers: ";
		std::cin >> layers;

		hidden.resize(layers);
		for (int i = 0; i < layers; i++)

		{
			std::cout << "Layer " << i << ": ";
			std::cin >> hidden[i];
		}
	}
	
	else

	{
		hidden = { 10 };
	}

	Program program(800,600, hidden);

	return program.mainLoop();
}