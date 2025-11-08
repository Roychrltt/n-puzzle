# N-Puzzle Solver

A modern C++ implementation of the N-Puzzle solver using A* search with multiple heuristics with a clean, modular design.

## Features
- Solves n-puzzle of any dimension (3*3, 4*4, or bigger)

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

``Usage: ./n_puzzle [options]

Options:
  --filename <file>        Load puzzle from input file
  --random <n>             Generate a random solvable puzzle of size n x n
  --heuristic <type>       Choose heuristic function (default: manhattan)
                           Options: manhattan, linear_conflict, hamming
  --solver <type>          Choose search algorithm (default: a_star)
                           Options: a_star, greedy, uniform_cost
  --verbose                Print each step of the solution
  --help, -h               Show this help message and exit

Examples:
 ./n-puzzle --filename puzzle.txt --heuristic linear_conflict --solver greedy
 ./n-puzzle --random 3 --heuristic manhattan --solver uniform_cost``
