#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
using namespace std;

class Time
{
public:
	Time();
	Time(uintptr_t hour, uintptr_t min, uintptr_t sec);

	uintptr_t getHour() const;
	uintptr_t getMin() const;
	uintptr_t getSec() const;

	void setHour();
	void setMin();
	void setSec(uintptr_t aSec);

	void PrintAmPm();

private:
	uintptr_t hour;
	uintptr_t min;
	uintptr_t sec;
};

bool IsEarlierThan(const Time& t1, const Time& t2);
void change_times(Time& t1,Time t2);
void print_stack();
void label_stack();

