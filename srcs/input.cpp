#include "../include/n-puzzle.hpp"

void	readFile(const std::string &filename, int &n, std::vector<int> &grid) {
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error: input file open failure" << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if (!line.empty() && line[0] != '#') break;
	}

	n = std::stoi(line);

	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		std::stringstream ss(line);
		int val;
		while (ss >> val) grid.push_back(val);
	}
}

void	generateRandomPuzzle(int n, std::vector<int> &grid)
{
	std::string command = "python case-gen.py " + std::to_string(n);
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe)
	{
		std::cerr << "Error: popen failure" << std::endl;
		exit(1);
	}

	int count = 0;
	char buffer[128];
	std::string line;

	while (fgets(buffer, sizeof(buffer), pipe))
	{
		line = buffer;
		std::cout << line;
		if (!line.empty() && line[0] == '#') continue;
		std::stringstream ss(line);
		int val;
		while (ss >> val)
			if (count++ > 0) grid.push_back(val);
	}
	std::cout << std::endl;
	pclose(pipe);
}
