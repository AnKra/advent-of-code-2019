#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../../common.h"

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
  Node *next_node = NULL;
  Direction direction_from_prev_node = Direction::UNKNOWN;
  int distance_from_prev_node = 0;
  int distance_from_origin = 0;

  bool DirectionIs(Direction direction) {
    return this->next_node->direction_from_prev_node == direction;
  }
};

class Intersection {
public:
  Intersection(Node *horizontal_segment, Node *vertical_segment) {

    point_.x = vertical_segment->point.x;
    point_.y = horizontal_segment->point.y;

    combined_steps_from_origin_ = horizontal_segment->distance_from_origin +
                                  Distance(point_, horizontal_segment->point) +
                                  vertical_segment->distance_from_origin +
                                  Distance(point_, vertical_segment->point);
  }

  virtual ~Intersection(){};

  Point GetPoint() { return point_; }

  int CombinedStepsFromOrigin() { return combined_steps_from_origin_; }

private:
  Point point_;
  int combined_steps_from_origin_;
};

Node *Wire(std::string line) {

  Node *origin = new Node{};

  // extract direction and distance
  {
    Node *current_node = origin;

    std::stringstream ss(line);
    std::string value_string;
    while (std::getline(ss, value_string, ',')) {
      if (value_string.size() > 0) {

        current_node->next_node = new Node{};
        current_node = current_node->next_node;

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
    Node *current_node = origin;
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

      current_node = current_node->next_node;
    }
  }

  return origin;
}

void PrintWireSegment(Node *origin) {
  std::cout << origin->point.x << " / " << origin->point.y << " <--> "
            << origin->next_node->point.x << " / " << origin->next_node->point.y
            << std::endl;
}

void PrintWire(Node *origin) {
  Node *current_node = origin;
  while (current_node->next_node) {
    std::cout << "wire1 " << current_node->point.x << " / "
              << current_node->point.y << std::endl;
    current_node = current_node->next_node;
  }
}

Node *GetNodeByDirection(Direction direction, Node *node1, Node *node2) {
  if (node1->DirectionIs(direction))
    return node1;
  if (node2->DirectionIs(direction))
    return node2;
  return NULL;
}

bool IsInbetween(int a, int b, int c) {
  return (a < b && b < c) || (a > b && b > c);
}

bool XCoordinatesIntersect(Node *segment, Point point) {
  return IsInbetween(segment->point.x, point.x, segment->next_node->point.x);
}

bool YCoordinatesIntersect(Node *segment, Point point) {
  return IsInbetween(segment->point.y, point.y, segment->next_node->point.y);
}

bool Intersects(Node *horizontal_segment, Node *vertical_segment) {
  return (XCoordinatesIntersect(horizontal_segment, vertical_segment->point) &&
          YCoordinatesIntersect(vertical_segment, horizontal_segment->point));
}

std::vector<Intersection> FindIntersections(Node *wire1_origin,
                                            Node *wire2_origin) {
  std::vector<Intersection> intersections;

  Node *current_node1 = wire1_origin;
  while (current_node1->next_node) {

    Node *current_node2 = wire2_origin;
    while (current_node2->next_node) {

      auto horizontal_segment = 
          GetNodeByDirection(Direction::HORIZONTAL, current_node1, current_node2);
      auto vertical_segment =
          GetNodeByDirection(Direction::VERTICAL, current_node1, current_node2);

      if (horizontal_segment && vertical_segment &&
          Intersects(horizontal_segment, vertical_segment)) {
        intersections.emplace_back(horizontal_segment, vertical_segment);
      }

      current_node2 = current_node2->next_node;
    }

    current_node1 = current_node1->next_node;
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

  auto intersections = FindIntersections(wire1, wire2);

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
