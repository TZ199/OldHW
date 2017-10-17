#include <iostream>
using namespace std;

int countPaths(int row, int col)
{
int counter = 0;
if (row==0&&col==0)
  return 1;
else
{
  if (row<0||col<0)
    return 0;
  counter += countPaths(row-1, col) + countPaths(row, col-1);
  return counter;
}
}

int main()
{
cout << "Number of paths from (1,1) to (0,0): " << countPaths(1,1) << endl;
cout << "Number of paths from (2,1) to (0,0): " << countPaths(2,1) << endl;
cout << "Number of paths from (2,2) to (0,0): " << countPaths(2,2) << endl;
}
