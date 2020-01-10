#ifndef DAY_03_INCLUDE_DAY03_POINT_H_
#define DAY_03_INCLUDE_DAY03_POINT_H_

#include <cmath>

namespace day03 {

struct Point {
  int x, y;

  static int Distance(Point a, Point b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  }
};

} // namespace day03

#endif // DAY_03_INCLUDE_DAY03_POINT_H_
