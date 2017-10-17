#ifndef Rectangle_h_
#define Rectangle_h_

#include <vector>
#include "Point2D.h"


class Rectangle {

public:
  //  Constructor based on the corners of the rectangle
  Rectangle(const Point2D& lower_left, const Point2D& upper_right);

  //  Access the corners of the rectangle and the points stored in the rectangle.
  const Point2D& upper_right_corner() const { return m_upper_right; }
  const Point2D& lower_left_corner() const { return m_lower_left; }
  const std::vector<Point2D>& points_contained() const { return m_points_contained; }

  //  Return true if the point is within the bounds of the rectangle
  bool is_point_within(const Point2D& p) const;

  //  If a point is within the bounds of the rectangle and not already
  //  stored with the rectangle then add it.  Return true if the point
  //  was added and false otherwise.
  bool add_point(const Point2D& p)
  {
    if(is_point_within(p))
    {
      for(int i = 0; i < m_points_contained.size(); i++)
      {
        if(p.x() == m_points_contained[i].x() &&p.y() == m_points_contained[i].y())
        {
          return false;
        }
      }
      m_points_contained.push_back(p);
      return true;
    }
    return false;
  }

private:
  std::vector<Point2D> m_points_contained;
  Point2D m_upper_right, m_lower_left;
};


// return a vector with the points that are in both rectangles
std::vector<Point2D> points_in_both(const Rectangle& r1, const Rectangle& r2);

// to aid with debugging, output all the private member data for this rectangle
void print_rectangle(const Rectangle& r1);

#endif

