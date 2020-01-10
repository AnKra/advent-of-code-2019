#ifndef DAY_03_INCLUDE_DAY03_NODE_H_
#define DAY_03_INCLUDE_DAY03_NODE_H_

#include <memory>

#include <day03/point.h>

namespace day03 {

enum class Direction { UNKNOWN, VERTICAL, HORIZONTAL };

class Node {
public:
  explicit Node(Direction dir, Point p, int dist_orig)
      : direction_(dir), point(p), distance_from_origin(dist_orig) {}

  virtual ~Node(){};

  static const Node *const GetNodeByDirection(const Direction direction,
                                              const Node *const node1,
                                              const Node *const node2) {
    return node1->next->direction_ == direction
               ? node1
               : node2->next->direction_ == direction ? node2 : nullptr;
  }

  static bool Intersects(const Node &horizontal, const Node &vertical) {
    std::function<bool(int, int, int)> is_inbetween = [](int a, int b, int c) {
      return (a < b && b < c) || (a > b && b > c);
    };

    return is_inbetween(horizontal.point.x, vertical.point.x,
                        horizontal.next->point.x) &&
           is_inbetween(vertical.point.y, horizontal.point.y,
                        vertical.next->point.y);
  }

  Point point;
  int distance_from_origin;
  std::unique_ptr<Node> next = {};

private:
  Direction direction_; // direction from previous node
};

} // namespace day03

#endif // DAY_03_INCLUDE_DAY03_NODE_H_
