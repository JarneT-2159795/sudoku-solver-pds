// Adapted from https://www.geeksforgeeks.org/sudoku-backtracking-7/

#include <iostream>
#include "timer.h"
#include <iomanip>
#include "sudoku.h"
#include "omp.h"
#include <vector>

using namespace std;
static bool isSolved = false;

bool serialSolve(vector<vector<int>> grid, int row, int col);
bool parallelSolve(vector<vector<int>> grid, int row, int col);
void parallelSolveHelper(vector<vector<int>> grid, int row, int col);
bool isSafe(vector<vector<int>> grid, int row, int col, int num);
void printGrid(vector<vector<int>> grid);

int main()
{
	AutoAverageTimer t1("Serial  "), t2("Parallel");
	vector<vector<int>> grid(SIZE, vector<int>(SIZE));
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			grid[i][j] = s.grid[i][j];
	for (int i = 0; i < 1; i++)
	{
		bool resultSerial = false, resultParallel = false;
		t1.start();
		resultSerial = serialSolve(grid, 0, 0);
		t1.stop();
		t2.start();
		resultParallel = parallelSolve(grid, 0, 0);
		t2.stop();
		if (resultSerial != resultParallel)
			cout << "Serial and Parallel results do not match!" << endl;
		else if (resultSerial)
			cout << "Solved!" << endl;
		else
			cout << "No solution!" << endl;
	}
	t1.report();
	t2.report();
	return 0;
}

bool parallelSolve(vector<vector<int>> grid, int row, int col)
{
	int threadCount = SIZE;
	if (threadCount > omp_get_max_threads())
		threadCount = omp_get_max_threads();
	#pragma omp parallel for num_threads(threadCount) shared(isSolved)
	for (int num = 1; num <= SIZE; num++)
	{
		auto localGrid = grid;
		if (isSafe(localGrid, row, col, num) || localGrid[row][col] != 0)
		{
			localGrid[row][col] = num;
			parallelSolveHelper(localGrid, row, col + 1);
		}
	}
	return isSolved;
}

void parallelSolveHelper(vector<vector<int>> grid, int row, int col)
{
	if (isSolved)
	{
		return;
	}
	if (row == SIZE - 1 && col == SIZE)
	{
		isSolved = true;
		return;
	}

	if (col == SIZE)
	{
		row++;
		col = 0;
	}

	if (grid[row][col] > 0)
	{
		parallelSolveHelper(grid, row, col + 1);
		return;
	}

	for (int num = 1; num <= SIZE; num++)
	{
		if (isSafe(grid, row, col, num))
		{
			grid[row][col] = num;

			parallelSolveHelper(grid, row, col + 1);
			if (isSolved)
				return;
		}

		grid[row][col] = 0;
	}
}

bool serialSolve(vector<vector<int>> grid, int row, int col)
{
	if (row == SIZE - 1 && col == SIZE)
		return true;

	if (col == SIZE)
	{
		row++;
		col = 0;
	}

	if (grid[row][col] > 0)
		return serialSolve(grid, row, col + 1);

	for (int num = 1; num <= SIZE; num++)
	{
		if (isSafe(grid, row, col, num))
		{
			grid[row][col] = num;

			if (serialSolve(grid, row, col + 1))
				return true;
		}

		grid[row][col] = 0;
	}
	return false;
}

bool isSafe(vector<vector<int>> grid, int row, int col, int num)
{
	for (int x = 0; x < SIZE; x++)
		if (grid[row][x] == num)
			return false;

	for (int x = 0; x < SIZE; x++)
		if (grid[x][col] == num)
			return false;

	int startRow = row - row % SUB_SIZE,
		startCol = col - col % SUB_SIZE;

	for (int i = 0; i < SUB_SIZE; i++)
		for (int j = 0; j < SUB_SIZE; j++)
			if (grid[i + startRow][j +
								   startCol] == num)
				return false;

	return true;
}

void printGrid(vector<vector<int>> grid)
{
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
			cout << grid[row][col] << " ";
		cout << endl;
	}
}
