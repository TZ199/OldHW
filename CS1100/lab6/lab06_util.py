
def read_sudoku(filename):
    board = []
    for line in open(filename):
        line = line.strip("\n")
        board.append(line.split(" "))
    return board


if __name__ == "__main__":
    fname = raw_input("Print enter a file name ==> ")
    print read_sudoku(fname)
