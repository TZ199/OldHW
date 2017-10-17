#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <assert.h>
#include <algorithm>
#include "mtrand.h"

int main()
{
static MTRand_int32 mtrand;
  int temp = mtrand();
  int max = 30;
  std::cout <<  mtrand() % max;
}
