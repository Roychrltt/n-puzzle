#include "../include/n-puzzle.hpp"

int	manhattan(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	int dist = 0;
	for (int i = 0; i < n * n; i++)
		dist += std::abs(i / n - pos[state[i]].first) + std::abs(i % n - pos[state[i]].second);
	return dist;
}

int	linearConflict(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
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

int	hamming(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n, const std::vector<int>& goal)
{
	(void)pos;
	int dist = 0;

	for (int i = 0; i < n * n; i++)
		if (state[i] != 0 && state[i] != goal[i]) dist++;
	return dist;
}

int	chebyshev(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	int dist = 0;
	for (int i = 0; i < n * n; i++)
		dist += std::max(std::abs(i / n - pos[state[i]].first), std::abs(i % n - pos[state[i]].second));
	return dist;
}

int	euclidean(const std::vector<int>& state, const std::vector<std::pair<int, int>>& pos, int n)
{
	double dist = 0;
	for (int i = 0; i < n * n; i++)
	{
		int x = std::abs(i / n - pos[state[i]].first);
		int y = std::abs(i % n - pos[state[i]].second);
		dist += std::sqrt(x * x + y * y);
	}
	return static_cast<int>(dist);
}
