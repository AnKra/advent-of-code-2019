#ifndef DAY_03_INCLUDE_DAY03_WIRE_H_
#define DAY_03_INCLUDE_DAY03_WIRE_H_

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <day03/point.h>

namespace day03 {

class Wire {
public:
  explicit Wire(std::string line) {
    // create origin
    int distance_from_origin = 0;
    Point point = {0, 0};
    origin_ =
        std::make_unique<Node>(Direction::UNKNOWN, point, distance_from_origin);

    // function to calculate next point
    std::function<Point(Direction, int, Point)> get_point =
        [](Direction dir, int distance, Point previous_point) {
          if (dir == Direction::UNKNOWN)
            throw std::runtime_error("direction not set");

          return dir == Direction::HORIZONTAL
                     ? Point{previous_point.x + distance, previous_point.y}
                     : Point{previous_point.x, previous_point.y + distance};
        };

    // parse line and create wire
    Node *current{origin_.get()};
    std::stringstream ss(line);
    std::string value_string;
    while (std::getline(ss, value_string, ',')) {
      if (value_string.size() > 0) {
        Direction direction;
        int distance; // distance from previous node

        switch (value_string[0]) {
        case 'R':
          direction = Direction::HORIZONTAL;
          distance = std::stoi(value_string.substr(1));
          break;
        case 'L':
          direction = Direction::HORIZONTAL;
          distance = -std::stoi(value_string.substr(1));
          break;
        case 'U':
          direction = Direction::VERTICAL;
          distance = std::stoi(value_string.substr(1));
          break;
        case 'D':
          direction = Direction::VERTICAL;
          distance = -std::stoi(value_string.substr(1));
          break;
        default:
          throw std::runtime_error("unknown direction " + value_string[0]);
        }

        point = get_point(direction, distance, point);
        distance_from_origin += std::abs(distance);
        current->next =
            std::make_unique<Node>(direction, point, distance_from_origin);

        current = current->next.get();
      }
    }
  }

  virtual ~Wire() {}

  const Node *Origin() const { return origin_.get(); }

private:
  std::unique_ptr<Node> origin_;
};

} // namespace day03

#endif // DAY_03_INCLUDE_DAY03_WIRE_H_
