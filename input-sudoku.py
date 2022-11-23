SUB_SIZE = 4
SIZE = SUB_SIZE * SUB_SIZE

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

grid = []

for i in range(0, SIZE):
    row = []
    for j in range(0, SIZE):
        num = -1
        while num > SIZE or num < 0:
            strNum = input("Enter number for [{}][{}]: ".format(i + 1, j + 1))
            if strNum == "":
                print("Invalid number, try again.")
                continue
            num = int(strNum)
            if num > SIZE or num < 0:
                print("Invalid number, try again.")
        row.append(num)
    grid.append(row)

writeToFile(grid)
