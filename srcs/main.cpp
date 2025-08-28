#include <bits/stdc++.h>

int dirs[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

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
	int cnt = 0;
	for (int i = 0; i < n * n; i++)
		cnt += std::abs(i / n - pos[state[i]].first) + std::abs(i % n - pos[state[i]].second);
	return cnt;
}

/*static int linearConflict(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	int dist = manhattan(state, pos, n);

	 for (int r = 0; r < n; r++) {
        for (int i = 0; i < n; i++) {
            int idx1 = r * n + i;
            int val1 = state[idx1];
            if (val1 == 0 || pos[val1].first != r) continue;
            for (int j = i + 1; j < n; j++) {
                int idx2 = r * n + j;
                int val2 = state[idx2];
                if (val2 == 0 || pos[val2].first != r) continue;
                if (pos[val1].second > pos[val2].second) dist += 2;
            }
        }
    }

    for (int c = 0; c < n; c++) {
        for (int i = 0; i < n; i++) {
            int idx1 = i * n + c;
            int val1 = state[idx1];
            if (val1 == 0 || pos[val1].second != c) continue;
            for (int j = i + 1; j < n; j++) {
                int idx2 = j * n + c;
                int val2 = state[idx2];
                if (val2 == 0 || pos[val2].second != c) continue;
                if (pos[val1].first > pos[val2].first) dist += 2;
            }
        }
    }
	return dist;
}*/

int updateManhattan(int oldH, int tile, int oldPos, int newPos, const std::vector<std::pair<int,int>>& pos, int n) {
    int ox = oldPos / n, oy = oldPos % n;
    int nx = newPos / n, ny = newPos % n;
    int goalx = pos[tile].first, goaly = pos[tile].second;

    oldH -= abs(ox - goalx) + abs(oy - goaly);
    oldH += abs(nx - goalx) + abs(ny - goaly);
    return oldH;
}

static int heuristic(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n, int type = 0)
{
	if (type == 0) return manhattan(state, pos, n);
	else if (type == 1) return manhattan(state, pos, n);
	else if (type == 2) return manhattan(state, pos, n);
	return manhattan(state, pos, n);
}

bool solve(std::vector<int>& grid, int n, int type = 0)
{
	std::vector<int> goal = makeGoal(n);
	std::vector<std::pair<int, int>> pos(n * n);
	for (int i = 0; i < n * n; i++)
		pos[goal[i]] = {i / n, i % n};

	using Node = std::tuple<int, int, std::vector<int>, int, int, std::string>;
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
	std::unordered_set<unsigned long long int> vis;

	int h = heuristic(grid, pos, n, type);
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
			std::cout << "Time complexity: " << cnt << " states selected in total" << std::endl;
			std::cout << "Size complexity: at most " << mx << " states ever represented in memory at the same time" << std::endl;
			std::cout << "Number of moves to achieve the final state: " << move.size() << std::endl;
			std::cout << "Solution: " << move << std::endl;
			return true;
		}

		unsigned long long int num = 0;
		for (int i : state) num = ((num << 4) | i);
		if (vis.find(num) != vis.end()) continue;
		vis.insert(num);

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
			//int ch = heuristic(next, pos, n, type);
			int ch = updateManhattan(h, tile, dz, z, pos, n);
			if (i == 0) move += "U";
			else if (i == 1) move += "D";
			else if (i == 2) move += "L";
			else if (i == 3) move += "R";
			pq.emplace(g + ch + 1, g + 1, next, dz, ch, move);
		}
	}
	return false;
}

int main()
{
	int n;
	std::cin >> n;
	std::vector<int> grid(n * n);
	for (int i = 0; i < n * n; i++)
		std::cin >> grid[i];

	auto start = std::chrono::high_resolution_clock::now();

	// ------------------------- Test cases -------------------------
	//int n = 3;
	//std::vector<int> grid = {2,3,7,6,1,4,5,8,0}; // yes
	//std::vector<int> grid = {7,4,3,8,0,1,5,6,2}; // yes
	//std::vector<int> grid = {4,7,1,3,0,8,5,6,2}; // no
	//std::vector<int> grid = {5,8,6,1,0,2,3,4,7}; // no
	//int n = 4;
	//std::vector<int> grid = {9,14,1,5,15,12,0,4,2,6,3,8,10,7,13,11}; // no
	//std::vector<int> grid = {5,9,2,6,1,14,0,13,3,12,7,8,11,4,10,15}; // no
	//std::vector<int> grid = {7,4,6,15,14,10,9,5,12,13,2,11,0,1,3,8}; // yes
	//std::vector<int> grid = {0,2,3,4,1,13,14,5,12,11,15,6,10,9,8,7}; // yes
	//std::vector<int> grid = {10,14,5,1,4,9,3,11,2,7,13,6,15,12,0,8}; // yes
	//std::vector<int> grid = {1,4,14,12,5,8,11,15,9,10,13,0,2,7,3,6}; // yes

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
	if (check())
		solve(grid, n, 1);
	else std::cout << "The case is unsolvable" << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	double duration = (double)(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	std::cout << "Execution Time: " << duration / 1e6 << " seconds\n";

	return 0;
}
