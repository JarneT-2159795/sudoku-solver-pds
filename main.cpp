// Adapted from https://www.geeksforgeeks.org/sudoku-backtracking-7/

#include <iostream>
#include "timer.h"
#include <iomanip>
#include "sudoku.h"
#include "omp.h"
#include <vector>

using namespace std;
static int serialSolved = 0, parallelSolved = 0;

void serialSolve(vector<vector<int>> grid, int row, int col);
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
		t1.start();
		serialSolve(grid, 0, 0);
		t1.stop();
		t2.start();
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				parallelSolveHelper(grid, 0, 0);
			}
		}
		t2.stop();
		if (serialSolved != parallelSolved)
			cout << "Serial and Parallel results do not match!" << endl;
		else if (serialSolved > 0)
			cout << "Found " << serialSolved << " solutions!" << endl;
		else
			cout << "No solution!" << endl;
	}
	t1.report();
	t2.report();
	return 0;
}

void parallelSolveHelper(vector<vector<int>> grid, int row, int col)
{
	if (row == SIZE - 1 && col == SIZE)
	{
		#pragma omp atomic
		parallelSolved++;
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
	#pragma omp task
	{
	for (int num = 1; num <= SIZE; num++)
		{
			if (isSafe(grid, row, col, num))
			{
				grid[row][col] = num;

				parallelSolveHelper(grid, row, col + 1);
			}

			grid[row][col] = 0;
		}
	}
}

void serialSolve(vector<vector<int>> grid, int row, int col)
{
	if (row == SIZE - 1 && col == SIZE)
	{
		serialSolved++;
		return;
	}

	if (col == SIZE)
	{
		row++;
		col = 0;
	}

	if (grid[row][col] > 0)
	{
		serialSolve(grid, row, col + 1);
		return;
	}

	for (int num = 1; num <= SIZE; num++)
	{
		if (isSafe(grid, row, col, num))
		{
			grid[row][col] = num;
			serialSolve(grid, row, col + 1);
		}

		grid[row][col] = 0;
	}
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
