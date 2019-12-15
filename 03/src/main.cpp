#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <common/common.h>

struct Point {
  int x;
  int y;
};

int Distance(Point a, Point b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

enum class Direction { UNKNOWN, VERTICAL, HORIZONTAL };

struct Node {
  Point point = {0, 0};
  std::unique_ptr<Node> next_node = {};
  Direction direction_from_prev_node = Direction::UNKNOWN;
  int distance_from_prev_node = 0;
  int distance_from_origin = 0;

  bool DirectionIs(const Direction direction) const {
    return this->next_node->direction_from_prev_node == direction;
  }
};

class Intersection {
public:
  Intersection(const Node &horizontal_segment, const Node &vertical_segment) {

    point_.x = vertical_segment.point.x;
    point_.y = horizontal_segment.point.y;

    combined_steps_from_origin_ = horizontal_segment.distance_from_origin +
                                  Distance(point_, horizontal_segment.point) +
                                  vertical_segment.distance_from_origin +
                                  Distance(point_, vertical_segment.point);
  }

  virtual ~Intersection(){};

  Point GetPoint() const { return point_; }

  int CombinedStepsFromOrigin() const { return combined_steps_from_origin_; }

private:
  Point point_;
  int combined_steps_from_origin_;
};

std::unique_ptr<Node> Wire(std::string line) {

  std::unique_ptr<Node> origin = std::make_unique<Node>();

  // extract direction and distance
  {
    Node *current_node{origin.get()};

    std::stringstream ss(line);
    std::string value_string;
    while (std::getline(ss, value_string, ',')) {
      if (value_string.size() > 0) {

        current_node->next_node = std::make_unique<Node>();
        current_node = current_node->next_node.get();

        auto direction = value_string[0];
        switch (direction) {
        case 'R':
          current_node->direction_from_prev_node = Direction::HORIZONTAL;
          current_node->distance_from_prev_node =
              std::stoi(value_string.substr(1));
          break;
        case 'L':
          current_node->direction_from_prev_node = Direction::HORIZONTAL;
          current_node->distance_from_prev_node =
              -std::stoi(value_string.substr(1));
          break;
        case 'U':
          current_node->direction_from_prev_node = Direction::VERTICAL;
          current_node->distance_from_prev_node =
              std::stoi(value_string.substr(1));
          break;
        case 'D':
          current_node->direction_from_prev_node = Direction::VERTICAL;
          current_node->distance_from_prev_node =
              -std::stoi(value_string.substr(1));
          break;
        default:
          throw std::runtime_error("unknown direction " + direction);
        }
      }
    }
  }

  {
    Node *current_node{origin.get()};
    while (current_node->next_node) {

      // calculate coordinates
      switch (current_node->next_node->direction_from_prev_node) {
      case Direction::HORIZONTAL:
        current_node->next_node->point.x =
            current_node->point.x +
            current_node->next_node->distance_from_prev_node;
        current_node->next_node->point.y = current_node->point.y;
        break;
      case Direction::VERTICAL:
        current_node->next_node->point.x = current_node->point.x;
        current_node->next_node->point.y =
            current_node->point.y +
            current_node->next_node->distance_from_prev_node;
        break;
      default:
        throw std::runtime_error("direction not set");
      }

      // calculate distance from origin
      current_node->next_node->distance_from_origin =
          current_node->distance_from_origin +
          std::abs(current_node->next_node->distance_from_prev_node);

      current_node = current_node->next_node.get();
    }
  }

  return std::move(origin);
}

void PrintWireSegment(Node origin) {
  std::cout << origin.point.x << " / " << origin.point.y << " <--> "
            << origin.next_node->point.x << " / " << origin.next_node->point.y
            << std::endl;
}

void PrintWire(Node origin) {
  Node *current_node = &origin;
  while (current_node->next_node) {
    std::cout << "wire1 " << current_node->point.x << " / "
              << current_node->point.y << std::endl;
    current_node = current_node->next_node.get();
  }
}

const Node *const GetNodeByDirection(const Direction direction,
                                     const Node *const node1,
                                     const Node *const node2) {
  if (node1->DirectionIs(direction))
    return node1;
  if (node2->DirectionIs(direction))
    return node2;
  return NULL;
}

bool IsInbetween(int a, int b, int c) {
  return (a < b && b < c) || (a > b && b > c);
}

bool XCoordinatesIntersect(const Node &segment, const Point &point) {
  return IsInbetween(segment.point.x, point.x, segment.next_node->point.x);
}

bool YCoordinatesIntersect(const Node &segment, const Point &point) {
  return IsInbetween(segment.point.y, point.y, segment.next_node->point.y);
}

bool Intersects(const Node &horizontal_segment, const Node &vertical_segment) {
  return (XCoordinatesIntersect(horizontal_segment, vertical_segment.point) &&
          YCoordinatesIntersect(vertical_segment, horizontal_segment.point));
}

std::vector<Intersection> FindIntersections(const Node &wire1_origin,
                                            const Node &wire2_origin) {
  std::vector<Intersection> intersections;

  const Node *current_node1 = &wire1_origin;
  while (current_node1->next_node) {

    const Node *current_node2 = &wire2_origin;
    while (current_node2->next_node) {

      auto horizontal_segment = GetNodeByDirection(
          Direction::HORIZONTAL, current_node1, current_node2);
      auto vertical_segment =
          GetNodeByDirection(Direction::VERTICAL, current_node1, current_node2);

      if (horizontal_segment && vertical_segment &&
          Intersects(*horizontal_segment, *vertical_segment)) {
        intersections.emplace_back(*horizontal_segment, *vertical_segment);
      }

      current_node2 = current_node2->next_node.get();
    }

    current_node1 = current_node1->next_node.get();
  }

  return intersections;
}

int ClosestIntersection(std::vector<Intersection> intersections) {
  Point origin{0, 0};
  std::sort(std::begin(intersections), std::end(intersections),
            [&](Intersection a, Intersection b) {
              return Distance(a.GetPoint(), origin) <
                     Distance(b.GetPoint(), origin);
            });

  return std::abs(intersections.front().GetPoint().x) +
         std::abs(intersections.front().GetPoint().y);
}

int IntersectionWithMinimalDelay(std::vector<Intersection> intersections) {
  std::sort(std::begin(intersections), std::end(intersections),
            [&](Intersection a, Intersection b) {
              return a.CombinedStepsFromOrigin() < b.CombinedStepsFromOrigin();
            });

  return intersections.front().CombinedStepsFromOrigin();
}

int main() {
  std::string line;
  std::ifstream input_file("../input/input.txt");

  std::getline(input_file, line);
  auto wire1 = Wire(line);

  std::getline(input_file, line);
  auto wire2 = Wire(line);

  if (!wire1 || !wire2) {
    throw std::runtime_error("at least one wire is invalid");
  }
  auto intersections = FindIntersections(*wire1, *wire2);

  if (intersections.size() == 0) {
    throw std::runtime_error("no intersections");
  }

  // part 1
  {
    std::cout << "part 1: " << ClosestIntersection(intersections) << std::endl;
  }

  // part 2
  {
    std::cout << "part 2: " << IntersectionWithMinimalDelay(intersections)
              << std::endl;
  }

  return EXIT_SUCCESS;
}
