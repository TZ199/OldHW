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
#include "time.h"


Time::Time()
{
	hour = 0;
	min = 0;
	sec = 0;
}

Time::Time(uintptr_t aHour, uintptr_t aMin, uintptr_t aSec)
{
	hour = aHour;
	min = aMin;
	sec = aSec;
}

uintptr_t Time::getHour() const
{
	return hour;
}

uintptr_t Time::getMin() const
{
	return min;
}

uintptr_t Time::getSec() const
{
	return sec;
}

void Time::setHour()
{
	hour = hour + 1;
}

void Time::setMin()
{
	min = min + 30;
}

void Time::setSec(uintptr_t s)
{
	sec = s;
}

void Time::PrintAmPm()
{
	if (hour>12)
	{
		uintptr_t printHour = hour-12;
		std::cout << printHour << ":" ;
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << min << ":";
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << sec;
		std::cout << " pm" << std::endl;
	}
	else if (hour == 0)
	{
		std::cout << "12" << ":" ;
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << min << ":";
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << sec;
		std::cout << " am" << std::endl;
	}

	else if (hour == 12)
	{
		std::cout << "12" << ":" ;
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << min << ":";
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << sec;
		std::cout << " pm" << std::endl;
	}
	else
	{
		std::cout << hour << ":" ;
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << min << ":";
		std::cout << std::setfill('0') << std::setw(2);
		std::cout << sec;
		std::cout << " am" << std::endl;
	}
}

void change_times(Time& t1,Time t2)
{
	t1.setHour();
	t1.setMin();
	t2.setHour();
	t2.setMin();

}