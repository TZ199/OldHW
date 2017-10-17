#include "Rectangle.h"
int main()
{
	Rectangle r1(Point2D (1,2),Point2D(5,6));
	r1.add_point(Point2D (2,3));
	print_rectangle(r1);
	Point2D pt(4,4);
	pt.set(3,3);
	Rectangle r2(Point2D (0,0),Point2D(3,3));

	print_rectangle(r2);
	return 0;
}