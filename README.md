# N-Puzzle Solver

A modern C++ implementation of the N-Puzzle solver using A* search with multiple heuristics with a clean, modular design.

## Features
- Solves n-puzzle of any dimension (3 * 3, 4 * 4, or bigger)

- Implements A* search algorithm and its variants (uniform cost and greedy search)

- Supports different heuristics:
  - Manhattan distance
  - Misplaced tiles
  - Linear conflict (if implemented)
  - chebyshev distance
  - Euclidean distance

- Displays solutions step by step

- Reads puzzle from files or generates one in specified dimension

- Handles unsolvable puzzles gracefully

## Requirements

- c++11 or newer
- <bits/stdc++.h>

## Usage
![](images/usage.png)
