// Adapted from https://www.geeksforgeeks.org/sudoku-backtracking-7/

#include <iostream>
#include "timer.h"
#include <iomanip>
#include "sudokus.h"

using namespace std;

bool serialSolve(int grid[SIZE][SIZE], int row, int col);
void printSudoku(int arr[SIZE][SIZE]);
bool isSafe(int grid[SIZE][SIZE], int row, int col, int num);

int main()
{
	// SudokuSpawner spawner;
	// Sudoku s = spawner.getSudoku();
	Sudoku s = {{{3, 0, 6, 5, 0, 8, 4, 0, 0},
				 {5, 2, 0, 0, 0, 0, 0, 0, 0},
				 {0, 8, 7, 0, 0, 0, 0, 3, 1},
				 {0, 0, 3, 0, 1, 0, 0, 8, 0},
				 {9, 0, 0, 8, 6, 3, 0, 0, 5},
				 {0, 5, 0, 0, 9, 0, 6, 0, 0},
				 {1, 3, 0, 0, 0, 0, 2, 5, 0},
				 {0, 0, 0, 0, 0, 0, 0, 7, 4},
				 {0, 0, 5, 2, 0, 6, 3, 0, 0}}};
	printSudoku(s.grid);
	if (serialSolve(s.grid, 0, 0))
	{
		cout << "Solved!" << endl;
	}
	else
	{
		cout << "No solution exists" << endl;
	}
	return 0;
}

bool serialSolve(int grid[SIZE][SIZE], int row, int col)
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

void printSudoku(int arr[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			cout << arr[i][j] << " ";
		cout << endl;
	}
}

bool isSafe(int grid[SIZE][SIZE], int row, int col, int num)
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
