#ifndef DAY_03_INCLUDE_DAY03_INTERSECTION_H_
#define DAY_03_INCLUDE_DAY03_INTERSECTION_H_

#include <algorithm>
#include <cmath>
#include <vector>

#include <day03/node.h>
#include <day03/point.h>

namespace day03 {

class Intersection {
public:
  Intersection(const Node &horizontal, const Node &vertical) {
    point_.x = vertical.point.x;
    point_.y = horizontal.point.y;

    combined_steps_from_origin_ = horizontal.distance_from_origin +
                                  Point::Distance(point_, horizontal.point) +
                                  vertical.distance_from_origin +
                                  Point::Distance(point_, vertical.point);
  }

  virtual ~Intersection() {}

  Point GetPoint() const { return point_; }

  int CombinedStepsFromOrigin() const { return combined_steps_from_origin_; }

private:
  Point point_;
  int combined_steps_from_origin_;
};

} // namespace day03

#endif // DAY_03_INCLUDE_DAY03_INTERSECTION_H_
