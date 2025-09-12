#include <bits/stdc++.h>
#include <cstdlib>
#define __Made return
#define in 0
#define France__ ;

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define GRAY "\033[90m"
# define BOLD "\033[1m"
# define UNDER "\033[4m"
# define BLINK "\033[5m"
# define ERASE = "\033[2K\r"
# define RESET "\033[0m"

int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
int heuristic = 0, solver = 0;
std::vector<int> goal;
bool verbose = false;

static std::vector<int> makeGoal(int n)
{
	std::vector<int> goal(n * n);
	int top = 0, left = 0, bottom = n - 1, right = n - 1;
	int val = 1;
	while (top <= bottom && left <= right)
	{
		for (int i = left; i <= right; i++) goal[top * n + i] = val++;
		top++;
		for (int i = top; i <= bottom; i++) goal[i * n + right] = val++;
		right--;
		if (top <= bottom)
		{
			for (int i = right; i >= left; i--) goal[bottom * n + i] = val++;
			bottom--;
		}
		if (left <= right)
		{
			for (int i = bottom; i >= top; i--) goal[i * n + left] = val++;
			left++;
		}
	}
	if (n & 1) goal[n / 2 * n + n / 2] = 0;
	else goal[n / 2 * n + n / 2 - 1] = 0;
	return goal;
}

static int manhattan(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	int dist = 0;
	for (int i = 0; i < n * n; i++)
		dist += std::abs(i / n - pos[state[i]].first) + std::abs(i % n - pos[state[i]].second);
	return dist;
}

static int linearConflict(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	int dist = manhattan(state, pos, n);

	for (int r = 0; r < n; r++)
	{
		for (int i = 0; i < n; i++)
		{
			int idx1 = r * n + i;
			int val1 = state[idx1];
			if (val1 == 0 || pos[val1].first != r) continue;
			for (int j = i + 1; j < n; j++)
			{
				int idx2 = r * n + j;
				int val2 = state[idx2];
				if (val2 == 0 || pos[val2].first != r) continue;
				if (pos[val1].second > pos[val2].second) dist += 2;
			}
		}
	}

	for (int c = 0; c < n; c++)
	{
		for (int i = 0; i < n; i++)
		{
			int idx1 = i * n + c;
			int val1 = state[idx1];
			if (val1 == 0 || pos[val1].second != c) continue;
			for (int j = i + 1; j < n; j++)
			{
				int idx2 = j * n + c;
				int val2 = state[idx2];
				if (val2 == 0 || pos[val2].second != c) continue;
				if (pos[val1].first > pos[val2].first) dist += 2;
			}
		}
	}
	return dist;
}

static int hamming(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	(void)pos;
	int dist = 0;

	for (int i = 0; i < n * n; i++)
		if (state[i] != 0 && state[i] != goal[i]) dist++;
	return dist;
}

static int heuristicFunction(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	if (heuristic == 0) return manhattan(state, pos, n);
	else if (heuristic == 1) return linearConflict(state, pos, n);
	else if (heuristic == 2) return hamming(state, pos, n);
	return manhattan(state, pos, n);
}

static void printSolution(unsigned long long int cnt, unsigned long long int mx, const std::string& move)
{
	std::cout << MAGENTA << BOLD << "Time complexity: " << UNDER << cnt << RESET << MAGENTA << " states selected in total" << std::endl;
	std::cout << MAGENTA << BOLD << "Size complexity: " << RESET << MAGENTA << "at most " << BOLD << UNDER << mx << RESET << MAGENTA << " states ever represented in memory at the same time" << std::endl;
	std::cout << MAGENTA << BOLD << "Number of moves to achieve the final state: " << UNDER << move.size() << RESET << std::endl;
	std::cout << MAGENTA << BOLD << "Solution: " << RESET << MAGENTA << move << RESET << std::endl;
}

static std::string encode(const std::vector<int>& state)
{
	std::string s = "";
	for (int i : state) s += std::to_string(i) + "$";
	return s;
}

bool solve(std::vector<int>& grid, int n)
{
	goal = makeGoal(n);
	std::vector<std::pair<int, int>> pos(n * n);
	for (int i = 0; i < n * n; i++)
		pos[goal[i]] = {i / n, i % n};

	using Node = std::tuple<int, int, std::vector<int>, int, int, std::string>;
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
	std::unordered_set<std::string> vis;

	int h = heuristicFunction(grid, pos, n);
	int zero = (int)(std::find(grid.begin(), grid.end(), 0) - grid.begin());
	pq.push({h, 0, grid, zero, h, ""});

	unsigned long long int cnt = 0, mx = 0;
	while (!pq.empty())
	{
		cnt++;
		mx = std::max(mx, (unsigned long long int)pq.size());
		auto [f, g, state, z, h, move] = pq.top();
		pq.pop();

		if (state == goal)
		{
			printSolution(cnt, mx, move);
			return true;
		}

		std::string s = encode(state);
		if (vis.find(s) != vis.end()) continue;
		vis.insert(s);

		int x = z / n, y = z % n;
		for (int i = 0; i < 4; i++)
		{
			int dx = dirs[i][0] + x, dy = dirs[i][1] + y;
			if (dx < 0 || dx >= n || dy < 0 || dy >= n) continue;
			int dz = dx * n + dy;
			int tile = state[dz];
			if (tile == 0) continue;
			std::vector<int> next = state;
			std::swap(next[z], next[dz]);
			int curh = heuristicFunction(next, pos, n);
			int f;
			if (solver == 0) f = g + curh + 1;
			else if (solver == 1) f = g + 1;
			else f = curh + 1;
			if (i == 0)
				pq.emplace(f, g + 1, next, dz, curh, move + "U");
			else if (i == 1)
				pq.emplace(f, g + 1, next, dz, curh, move + "D");
			else if (i == 2)
				pq.emplace(f, g + 1, next, dz, curh, move + "L");
			else
				pq.emplace(f, g + 1, next, dz, curh, move + "R");
		}
	}
	return false;
}

static void printUsage(void)
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

void readFile(const std::string &filename, int &n, std::vector<int> &grid) {
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

void generateRandomPuzzle(int n, std::vector<int> &grid)
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

int main(int ac, char** av)
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);
	if (ac < 2)
	{
		printUsage();
		return 0;
	}
	std::vector<std::string> args(av + 1, av + ac);
	std::string inputfile = "";
	int n = -1;
	std::vector<int> grid(0);

	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i] == "--filename" && i + 1 < args.size()) inputfile = args[++i];
		else if (args[i] == "--random" && i + 1 < args.size()) n = std::stoi(args[++i]);
		else if (args[i] == "--heuristic" && i + 1 < args.size())
		{
			std::string h = args[++i];
			if (h == "manhattan") heuristic = 0;
			else if (h == "linear_conflict") heuristic = 1;
			else if (h == "hamming") heuristic = 2;
			else
			{
				std::cerr << "Unknown heuristic: " << h << std::endl;
				return 1;
			}
		}
		else if (args[i] == "--solver" && i + 1 < args.size())
		{
			std::string s = args[++i];
			if (s == "a_star") solver = 0;
			else if (s == "uniform_cost") solver = 1;
			else if (s == "greedy") solver = 2;
			else
			{
				std::cerr << "Unknown solver: " << s << std::endl;
				return 1;
			}
		}
		else if (args[i] == "--help" || args[i] == "-h")
		{
			std::cout << "Usage: ./program [--filename <file>] [--random <n>] [--heuristic <type>] [--solver <type>]\n";
			return 0;
		}
	}

	if (inputfile != "") readFile(inputfile, n, grid);
	else if (n < 3)
	{
		std::cerr << "Error: Grid size should be at least 3x3" << std::endl;
		return 0;
	}
	else generateRandomPuzzle(n, grid);
	// ------------------------- Start timing -----------------------------
	auto start = std::chrono::high_resolution_clock::now();
	auto check = [&]() -> bool
	{
		int cnt = 0;
		for (int i = 0; i < n * n; i++)
			for (int j = i + 1; j < n * n; j++)
				if (grid[i] && grid[j] && grid[i] > grid[j]) cnt++;

		if (n & 1) return (cnt & 1);
		else
		{
			int z = (int)(std::find(grid.begin(), grid.end(), 0) - grid.begin());
			int r = n - (z / n);
			return ((cnt + r) % 2 == 1);
		}
	};

	if (n == 3)
	{
		if (check()) solve(grid, n);
		else std::cout << RED << "The puzzle is unsolvable" << RESET << std::endl;
	}
	else
	{
		if (!solve(grid, n))
			std::cout << RED << "The puzzle is unsolvable" << RESET << std::endl;
	}

	// ------------------------- End timing -----------------------------
	auto end = std::chrono::high_resolution_clock::now();
	double duration = (double)(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	std::cout << "Execution Time: " << duration / 1e6 << " seconds\n";

	__Made in France__
}
