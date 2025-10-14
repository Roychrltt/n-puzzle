#include "../include/n-puzzle.hpp"

std::vector<int>	makeGoal(int n)
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

std::string	encode(const std::vector<int>& state)
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
