#include <vector>
#include <set>
#include <iostream>
#include <math.h>
#include <cassert>
#include "sudoku.h"

using std::set;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

// ======================================================
// A program to solve very simple Sudoku puzzles
// ======================================================

int main() {

  int size;
  while (cin >> size) {
    // Read in the board size and create an empty board
    Sudoku board(size);

    // Read in and set the known positions in the board
    char c;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        cin >> c;
        if (c != '.') {
          board.Set(i,j,c-'0');
        }
      }
    }

    // The iterative solving routine
    while (1) {

      // Is the puzzle is already solved?
      if (board.IsSolved()) {
        cout << "\nPuzzle is solved:" << endl;
        board.Print();
        break;
      }

      // Is the puzzle impossible?
      if (board.Impossible()) {
        cout << "\nCannot solve this puzzle:" << endl;
        board.Print();
        break;
      }

      // Try to reduce the number of choices in one or more board cells
      // by propagating known values within each row, column, and quadrant
      int changed = 0;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          if (board.KnownValue(i,j))
            changed += board.Propagate(i,j);
        }
      }

      // If nothing changed this iteration, give up
      if (changed == 0) {
        cout << "\nPuzzle is too difficult for this solver!" << endl;
        board.Print();
        break;
      }
    }
  }
}

// ======================================================

// Construct an empty board
Sudoku::Sudoku(int s) {
  // set the size
  size = s;
  quadrant_size = (int)sqrt(size);
  assert (quadrant_size*quadrant_size == s);
  std::vector<std::set<int> > temp;
  std::set<int> temp3;
  for(int k = 1; k <= size; k++)
  {
    temp3.insert(k);
  }
  for(int i = 0; i < size;i++)
  {
    board.push_back(temp);
    for(int j =0; j<size;j++)
    {
      board[i].push_back(temp3);

    }
  }

  // You need to finish this function!


}

// To construct the puzzle, set the value of a particular cell
void Sudoku::Set(int i, int j, int value) {
  // make sure the value is one of the legal values
  assert (board[i][j].find(value) != board[i][j].end());
  board[i][j].clear();
  board[i][j].insert(value);
}

// Does the cell have exactly one legal value?
bool Sudoku::KnownValue(int i, int j) const {
  return (board[i][j].size() == 1);
}

// If there is exactly one number in each cell of the grid, the puzzle is solved!
bool Sudoku::IsSolved() const {
  for(int i = 0; i<size; i++){
    for(int j = 0; j < size; j++){
      if(board[i][j].size() != 1){
        return false;
      }
    }
  }
  return true;
}

  // You need to write this function



// If there are no legal choices in one or more cells of the grid, the puzzle is impossible
bool Sudoku::Impossible() const {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (board[i][j].size() == 0)
        return true;
    }
  }
  return false;
}

// print out the solution
void Sudoku::Print() const {
  int solved = IsSolved();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int count = size+1;
      for (set<int>::iterator itr = board[i][j].begin(); itr != board[i][j].end(); itr++) {
        count--;
        cout << *itr;
      }
      if (!solved) {
        // if it's not solved, print out spaces to separate the values
        for (int k = 0; k < count; k++) {
          cout << " ";
        }
      }
    }
    cout << endl;
  }
}


// Propagate information from this cell along columns & rows &
// within quadrant. Return the number of cells that changed.
int Sudoku::Propagate(int i, int j) {
  int count = 0;
  set<int>::iterator itr = board[i][j].begin();
  for (int x = 0; x < size; x++) {
    if (board[i][x].count(*(itr)) != 0 && x != j) {
      board[i][x].erase(*(itr));
      count ++;
    }
    if (board[x][j].count(*(itr)) != 0 && x != i) {
      board[x][j].erase(*(itr));
      count ++;
    }
  }
  int w = (i/quadrant_size)*quadrant_size;
  int c = (j/quadrant_size)*quadrant_size;

  for(int s = w; s < w+quadrant_size; s++)
  {
    for(int b = c; b< c+quadrant_size; b++)
    {
      if(s!=i || b!= j)
      {
        int ss = board[s][b].size();
        board[s][b].erase(*itr);
        if(ss> board[s][b].size())
        {
          count++;
        }
      }
    }
  }
  return count;
}

  // You need to write this function
