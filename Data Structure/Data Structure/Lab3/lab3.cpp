#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip> 
#include <algorithm>
#include <cmath>


void compute_squares(unsigned int *a, unsigned int *b, unsigned int c )
{
	for(int i = 0; i < c; i++)
	{
		*(b+i) = *(a+i) * *(a+i);
	}
}

int main()
{
	unsigned int a[5] = {1,2,3,4,5};
	unsigned int b[5];
	unsigned int c = 5;
	compute_squares(a,b,c);
	for(int i = 0; i<c; i++)
	{
		std::cout << "b[" << i << "]" << ": " <<b[i] << std::endl;
	}
	return 0;
}