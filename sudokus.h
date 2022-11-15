#include <stdlib.h>
#include <time.h>

using namespace std;

const int SUB_SIZE = 3, SIZE = SUB_SIZE * SUB_SIZE;

typedef struct
{
    int grid[SIZE][SIZE];
} Sudoku;

class SudokuSpawner
{
public:
    SudokuSpawner()
    {
        srand(time(NULL));
    }

    Sudoku getSudoku()
    {
        Sudoku s;
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                int num = rand() % (SIZE + 10);
                num = num > SIZE ? 0 : num;
                s.grid[i][j] = num;
            }
        }
        return s;
    }

private:
};