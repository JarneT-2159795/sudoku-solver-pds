// Adapted from https://www.geeksforgeeks.org/sudoku-backtracking-7/

#include <iostream>
#include "timer.h"
#include <iomanip>
#include "sudoku.h"
#include "omp.h"
#include <vector>
#include <semaphore.h>

using namespace std;
sem_t isSolved;

bool serialSolve(vector<vector<int>> grid, int row, int col);
bool parallelSolve(vector<vector<int>> grid, int row, int col);
void parallelSolveHelper(vector<vector<int>> grid, int row, int col, bool *solved);
bool isSafe(vector<vector<int>> grid, int row, int col, int num);

int main()
{
	sem_init(&isSolved, 0, 1);
	AutoAverageTimer t1("Serial  "), t2("Parallel");
	vector<vector<int>> grid(SIZE, vector<int>(SIZE));
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			grid[i][j] = s.grid[i][j];
	for (int i = 0; i < 1; i++)
	{
		bool resultSerial = false, resultParallel = false;
		t1.start();
		//resultSerial = serialSolve(grid, 0, 0);
		t1.stop();
		t2.start();
		resultParallel = parallelSolve(grid, 0, 0);
		t2.stop();
		if (resultSerial)
		{
			cout << "Serial solution found" << endl;
		}
		if (resultParallel)
		{
			cout << "Parallel solved!" << endl;
		}
	}
	t1.report();
	t2.report();
	sem_destroy(&isSolved);
	return 0;
}

bool parallelSolve(vector<vector<int>> grid, int row, int col)
{
	bool solved[SIZE] = {false};
	int threadCount = SIZE;
	if (threadCount > omp_get_max_threads())
		threadCount = omp_get_max_threads();
	#pragma omp parallel for num_threads(threadCount)
	for (int num = 1; num <= SIZE; num++)
	{
		bool local_result = false;
		if (isSafe(grid, row, col, num))
		{
			grid[row][col] = num;
			parallelSolveHelper(grid, row, col + 1, &local_result);
		}
		if (local_result)
		{
			solved[num - 1] = true;
		}
	}
	for (int i = 0; i < SIZE; i++)
	{
		if (solved[i])
		{
			return true;
		}
	}
	return false;
}

void parallelSolveHelper(vector<vector<int>> grid, int row, int col, bool *solved)
{
	int sem_value = 0;
	sem_getvalue(&isSolved, &sem_value);
	if (sem_value == 0)
	{
		return;
	}
	if (row == SIZE - 1 && col == SIZE)
	{
		*solved = true;
		sem_wait(&isSolved);
		return;
	}

	if (col == SIZE)
	{
		row++;
		col = 0;
	}

	if (grid[row][col] > 0)
	{
		parallelSolveHelper(grid, row, col + 1, solved);
		return;
	}

	for (int num = 1; num <= SIZE; num++)
	{
		if (isSafe(grid, row, col, num))
		{
			grid[row][col] = num;

			parallelSolveHelper(grid, row, col + 1, solved);
			if (*solved)
				return;
		}

		grid[row][col] = 0;
	}

	*solved = false;
	return;
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
