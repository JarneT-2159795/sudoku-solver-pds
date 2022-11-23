# Adapted from Sudoku Generator Algorithm - www.101computing.net/sudoku-generator-algorithm/
from random import randint, shuffle
from time import sleep

SUB_SIZE = 3
SIZE = SUB_SIZE * SUB_SIZE
isSolvable = True

# initialise empty grid
grid = []
row = []
for i in range(0, SIZE):
    row.append(0)
for i in range(0, SIZE):
    grid.append(row[:])

# A function to check if the grid is full
def checkGrid(grid):
    for row in range(0, SIZE):
        for col in range(0, SIZE):
            if grid[row][col] == 0:
                return False

    # We have a complete grid!
    return True

# A backtracking/recursive function to check all possible combinations of numbers until a solution is found
def solveGrid(grid):
    global counter
    # Find next empty cell
    for i in range(0, SIZE * SIZE):
        row = i//SIZE
        col = i % SIZE
        if grid[row][col] == 0:
            for value in range(1, SIZE + 1):
                # Check that this value has not already be used on this row
                if not (value in grid[row]):
                    # Check that this value has not already be used on this column
                    used = False
                    for j in range(0, SIZE):
                        if grid[j][col] == value:
                            used = True
                            break
                    if not used:
                        # Identify which of the squares we are working on
                        square = []
                        for j in range(1, SUB_SIZE + 1):
                            if row < j * SUB_SIZE:
                                for k in range(1, SUB_SIZE + 1):
                                    if col < k * SUB_SIZE:
                                        square = [grid[i][(k - 1) * SUB_SIZE:k * SUB_SIZE] for i in range((j - 1) * SUB_SIZE, j * SUB_SIZE)]
                                        break
                                break
                        # Check that this value has not already be used on this square
                        used = False
                        for j in range(0, SUB_SIZE):
                            for k in range(0, SUB_SIZE):
                                if square[j][k] == value:
                                    used = True
                                    break
                        if not used:
                            grid[row][col] = value
                            if checkGrid(grid):
                                counter += 1
                                break
                            else:
                                if solveGrid(grid):
                                    return True
            break
    grid[row][col] = 0


numberList = []
for i in range(1, SIZE + 1):
    numberList.append(i)
shuffle(numberList)

# A backtracking/recursive function to check all possible combinations of numbers until a solution is found
def fillGrid(grid):
    global counter
    # Find next empty cell
    for i in range(0, SIZE * SIZE):
        row = i//SIZE
        col = i % SIZE
        if grid[row][col] == 0:
            shuffle(numberList)
            for value in numberList:
                # Check that this value has not already be used on this row
                if not (value in grid[row]):
                    # Check that this value has not already be used on this column
                    used = False
                    for j in range(0, SIZE):
                        if grid[j][col] == value:
                            used = True
                            break
                    if not used:
                        # Identify which of the squares we are working on
                        square = []
                        for j in range(1, SUB_SIZE + 1):
                            if row < j * SUB_SIZE:
                                for k in range(1, SUB_SIZE + 1):
                                    if col < k * SUB_SIZE:
                                        square = [grid[i][(k - 1) * SUB_SIZE:k * SUB_SIZE] for i in range((j - 1) * SUB_SIZE, j * SUB_SIZE)]
                                        break
                                break
                        # Check that this value has not already be used on this 3x3 square
                        used = False
                        for j in range(0, SUB_SIZE):
                            for k in range(0, SUB_SIZE):
                                if square[j][k] == value:
                                    used = True
                                    break
                        if not used:
                            grid[row][col] = value
                            if checkGrid(grid):
                                return True
                            else:
                                if fillGrid(grid):
                                    return True
            break
    grid[row][col] = 0


# Generate a Fully Solved Grid
if isSolvable:
    fillGrid(grid)
else:
    # Fill with random numbers
    for i in range(0, SIZE):
        shuffle(numberList)
        grid[i] = numberList[:]
    # Remove random numbers
    for i in range(0, SIZE):
        for j in range(0, SIZE):
            if randint(0, 100) <= 70:
                grid[i][j] = 0

# Start Removing Numbers one by one
# A higher number of attempts will end up removing more numbers from the grid
# Potentially resulting in more difficiult grids to solve!
attempts = 5
if not isSolvable:
    attempts = 0
counter = 1
while attempts > 0:
    # Select a random cell that is not already empty
    row = randint(0, SIZE - 1)
    col = randint(0, SIZE - 1)
    while grid[row][col] == 0:
        row = randint(0, SIZE - 1)
        col = randint(0, SIZE - 1)
    # Remember its cell value in case we need to put it back
    backup = grid[row][col]
    grid[row][col] = 0

    # Take a full copy of the grid
    copyGrid = []
    for r in range(0, SIZE):
        copyGrid.append([])
        for c in range(0, SIZE):
            copyGrid[r].append(grid[r][c])

    # Count the number of solutions that this grid has (using a backtracking approach implemented in the solveGrid() function)
    counter = 0
    solveGrid(copyGrid)
    # If the number of solution is different from 1 then we need to cancel the change by putting the value we took away back in the grid
    if counter != 1:
        grid[row][col] = backup
        # We could stop here, but we can also have another attempt with a different cell just to try to remove more numbers
        attempts -= 1

def writeToFile(grid):
    f = open("sudoku.h", "w")
    f.write("const int SUB_SIZE = {}, SIZE = {};".format(SUB_SIZE, SIZE))
    f.write("\ntypedef struct {\n\tint grid[SIZE][SIZE];\n} Sudoku;\nSudoku s = {")
    f.write("{\n")
    for i in range(0, SIZE):
        f.write("{")
        for j in range(0, SIZE):
            f.write(str(grid[i][j]))
            if j != SIZE-1:
                f.write(",")
        f.write("}")
        if i != SIZE-1:
            f.write(",\n")
    f.write("}")
    f.write("};")

writeToFile(grid)
print("Sudoku Grid Ready")
