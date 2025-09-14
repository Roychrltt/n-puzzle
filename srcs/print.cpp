#include "../include/n-puzzle.hpp"

void	printUsage(void)
{
	std::cout << "Usage: ./n_puzzle [options]\n\n";
	std::cout << "Options:\n";
	std::cout << "  --filename <file>        Load puzzle from input file\n";
	std::cout << "  --random <n>             Generate a random solvable puzzle of size n x n\n";
	std::cout << "  --heuristic <type>       Choose heuristic function (default: manhattan)\n";
	std::cout << "                           Options: manhattan, linear_conflict, hamming\n";
	std::cout << "  --solver <type>          Choose search algorithm (default: a_star)\n";
	std::cout << "                           Options: a_star, greedy, uniform_cost\n";
	std::cout << "  --help, -h               Show this help message and exit\n";
	std::cout << "\nExamples:\n";
	std::cout << " ./n-puzzle --filename puzzle.txt --heuristic linear_conflict --solver greedy\n";
	std::cout << " ./n-puzzle --random 3 --heuristic manhattan --solver uniform_cost\n";
}

void	printSolution(unsigned long long int cnt, unsigned long long int mx, const std::string& move)
{
	std::cout << MAGENTA << BOLD << "Time complexity: " << UNDER << cnt << RESET << MAGENTA << " states selected in total" << std::endl;
	std::cout << MAGENTA << BOLD << "Size complexity: " << RESET << MAGENTA << "at most " << BOLD << UNDER << mx << RESET << MAGENTA << " states ever represented in memory at the same time" << std::endl;
	std::cout << MAGENTA << BOLD << "Number of moves to achieve the final state: " << UNDER << move.size() << RESET << std::endl;
	std::cout << MAGENTA << BOLD << "Solution: " << RESET << MAGENTA << move << RESET << std::endl;
}
