#include "../include/n-puzzle.hpp"

int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
int verbose = 0;
std::vector<int> goal;
bool benchmark = false;

static int heuristicFunction(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n, const std::vector<int>& goal, int heuristic)
{
	if (heuristic == 0) return linearConflict(state, pos, n);
	else if (heuristic == 1) return manhattan(state, pos, n);
	else if (heuristic == 2) return hamming(state, pos, n, goal);
	return linearConflict(state, pos, n);
}

static std::vector<unsigned long long int>	solve(const std::vector<int>& grid, int n, int heuristic, int solver)
{
	std::string s1, s2;
	if (heuristic == 1) s1 = "manhattan";
	else if (heuristic == 2) s1 = "hamming";
	else s1 = "manhattan with linear conflict";
	if (solver == 1) s2 = "uniform cost";
	else if (solver == 2) s2 = "greedy";
	else s2 = "classic a_star";
	std::cout << BOLD << GREEN << "\nSolving the puzzle using heuristic function " << UNDER << s1 << RESET << GREEN << BOLD << " and the " << UNDER << s2 << RESET << GREEN << BOLD << " search\n" << RESET << std::endl;
	auto start = std::chrono::high_resolution_clock::now();

	bool flag = false;
	std::string res = "";
	std::vector<std::pair<int, int>> pos(n * n);
	for (int i = 0; i < n * n; i++)
		pos[goal[i]] = {i / n, i % n};

	using Node = std::tuple<int, int, std::vector<int>, int, int, std::string>;
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
	std::unordered_set<std::string> vis;

	int h = heuristicFunction(grid, pos, n, goal, heuristic);
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
			return {};
		}
		auto [f, g, state, z, h, move] = pq.top();
		pq.pop();

		if (state == goal)
		{
			if (verbose) printMoves(grid, move);
			printSolution(cnt, mx, move);
			flag = true;
			res = std::move(move);
			break;
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
			int curh = heuristicFunction(next, pos, n, goal, heuristic);
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
	auto end = std::chrono::high_resolution_clock::now();
	double duration = (double)(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	std::cout << "Execution Time: " << duration / 1e6 << " seconds\n";
	if (flag) return {cnt, mx, res.size()};
	return {};
}

static void runBenchmark(const std::vector<int>& grid, int n)
{
	int cnt = HEURISTIC_NUM * SOLVER_NUM;
	std::vector<unsigned long long int> time(cnt, -1), space(cnt, -1), sol(cnt, -1);
	std::vector<double> speed(cnt, -1);
	for (int i = 0; i < HEURISTIC_NUM; i++)
	{
		for (int j = 0; j < SOLVER_NUM; j++)
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::vector<unsigned long long int> tmp = solve(grid, n, i, j);
			auto end = std::chrono::high_resolution_clock::now();
			
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			int x = i * HEURISTIC_NUM + j;
			if (!tmp.empty())
			{
				time[x] = tmp[0];
				space[x] = tmp[1];
				sol[x] = tmp[2];
			}
			speed[x] = static_cast<double>(duration) / 1e6;
		}
	}
	std::cout << YELLOW << "\nComprehensive Benchmark Results:" << std::endl;
	std::cout << "Running time:" << std::endl;
	std::cout << "Linear conflict < manhattan < hamming" << std::endl;
	std::cout << "Greedy < a_star < uniform cost" << std::endl;
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
	int n = -1, solver = 0, heuristic = 0;
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
		else if (args[i] == "--verbose") verbose = 1;
		else if (args[i] == "--benchmark") benchmark = true;
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

	goal = makeGoal(n);
	if (!check(grid, goal, n))
	{
		std::cout << RED << "The puzzle is unsolvable" << RESET << std::endl;
		__Made in France__

	}
	if (benchmark) runBenchmark(grid, n);
	else
	{
		auto start = std::chrono::high_resolution_clock::now();
		solve(grid, n, heuristic, solver);
		auto end = std::chrono::high_resolution_clock::now();
		
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << "Execution Time: " << static_cast<double>(duration) / 1e6 << " seconds\n";
	}

	__Made in France__
}
