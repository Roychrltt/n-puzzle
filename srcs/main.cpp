#include "../include/n-puzzle.hpp"

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

static int heuristicFunction(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n, const std::vector<int>& goal)
{
	if (heuristic == 0) return linearConflict(state, pos, n);
	else if (heuristic == 1) return manhattan(state, pos, n);
	else if (heuristic == 2) return hamming(state, pos, n, goal);
	return linearConflict(state, pos, n);
}

static std::string encode(const std::vector<int>& state)
{
	std::string s = "";
	for (int i : state) s += std::to_string(i) + "$";
	return s;
}

int	parityPermutation(const std::vector<int>& grid)
{
	std::vector<bool> vis(grid.size(), false);
	int ok = 0;
	for (int i = 0; i < static_cast<int>(grid.size()); i++)
	{
		if (vis[i]) continue;
		vis[i] = true;
		int j = grid[i];
		while (j != i)
		{
			vis[j] = true;
			j = grid[j];
			ok ^= 1;
		}
	}
	return ok;
}

bool	check(const std::vector<int>& grid, const std::vector<int>& goal, int n)
{
	int idx = static_cast<int>(std::find(grid.begin(), grid.end(), 0) - grid.begin());
	int py = idx / n, px = idx % n;
	int parityZero = (static_cast<int>(grid.size()) ^ py ^ px ^ 1) & 1;
	return parityZero == (parityPermutation(grid) ^ parityPermutation(goal));
}

bool solve(std::vector<int>& grid, int n)
{
	goal = makeGoal(n);
	if (!check(grid, goal, n)) return false;
	std::vector<std::pair<int, int>> pos(n * n);
	for (int i = 0; i < n * n; i++)
		pos[goal[i]] = {i / n, i % n};

	using Node = std::tuple<int, int, std::vector<int>, int, int, std::string>;
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
	std::unordered_set<std::string> vis;

	int h = heuristicFunction(grid, pos, n, goal);
	int zero = (int)(std::find(grid.begin(), grid.end(), 0) - grid.begin());
	pq.push({h, 0, grid, zero, h, ""});

	unsigned long long int cnt = 0, mx = 0;
	while (!pq.empty())
	{
		cnt++;
		mx = std::max(mx, (unsigned long long int)pq.size());
		if (mx == std::numeric_limits<unsigned long long int>::max() || pq.size() > MAX_SPACE || vis.size() > MAX_SPACE)
		{
			std::cerr << RED << "Error: programm occupying too much space, aborting..." << RESET << std::endl;
			return false;
		}
		auto [f, g, state, z, h, move] = pq.top();
		pq.pop();

		if (state == goal)
		{
			printSolution(cnt, mx, move);
			return true;
		}


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
			std::string s = encode(next);
			if (vis.find(s) != vis.end()) continue;
			vis.insert(s);
			int curh = heuristicFunction(next, pos, n, goal);
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
			if (h == "linear_conflict") heuristic = 0;
			else if (h == "manhattan") heuristic = 1;
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
			if (s == "linear_conflict") solver = 0;
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
		std::cerr << "Error: puzzle size should be at least 3x3" << std::endl;
		return 0;
	}
	else if (n > 17)
	{
		std::cerr << "Error: puzzle size too large to solve feasibly" << std::endl;
		return 0;
	}
	else generateRandomPuzzle(n, grid);
	if (grid.size() != n * n)
	{
		std::cerr << RED << "Invalid input file: puzzle is not complete." << std::endl;
		return 0;
	}
	// ------------------------- Start timing -----------------------------
	auto start = std::chrono::high_resolution_clock::now();

	if (!solve(grid, n))
		std::cout << RED << "The puzzle is unsolvable" << RESET << std::endl;

	// ------------------------- End timing -----------------------------
	auto end = std::chrono::high_resolution_clock::now();
	double duration = (double)(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	std::cout << "Execution Time: " << duration / 1e6 << " seconds\n";

	__Made in France__
}
