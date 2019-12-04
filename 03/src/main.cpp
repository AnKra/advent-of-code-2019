#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../../common.h"

struct Point {
  int x;
  int y;
};

enum class Direction { UNKNOWN, VERTICAL, HORIZONTAL };

struct WireSegment {
  Point start = {0, 0};
  Point end = {0, 0};
  int length;
  Direction direction = Direction::UNKNOWN;
};

std::istream &operator>>(std::istream &is, WireSegment &output) {
  std::string value_string;
  std::getline(is, value_string, ',');
  if (value_string.size() > 0) {
    auto direction = value_string[0];
    switch (direction) {
    case 'R':
      output.direction = Direction::HORIZONTAL;
      output.length = std::stoi(value_string.substr(1));
      break;
    case 'L':
      output.direction = Direction::HORIZONTAL;
      output.length = -std::stoi(value_string.substr(1));
      break;
    case 'U':
      output.direction = Direction::VERTICAL;
      output.length = std::stoi(value_string.substr(1));
      break;
    case 'D':
      output.direction = Direction::VERTICAL;
      output.length = -std::stoi(value_string.substr(1));
      break;
    default:
      throw std::runtime_error("unknown direction " + direction);
    }
  }
  return is;
};

std::vector<WireSegment> GenerateWire(std::string line) {
  std::stringstream ss(line);
  std::vector<WireSegment> wire{std::istream_iterator<WireSegment>(ss),
                                std::istream_iterator<WireSegment>()};
  for (int i = 0; i < wire.size(); ++i) {
    if (i > 0) {
      wire[i].start = wire[i - 1].end;
    }

    switch (wire[i].direction) {
    case Direction::HORIZONTAL:
      wire[i].end.x = wire[i].start.x + wire[i].length;
      wire[i].end.y = wire[i].start.y;
      break;
    case Direction::VERTICAL:
      wire[i].end.x = wire[i].start.x;
      wire[i].end.y = wire[i].start.y + wire[i].length;
      break;
    default:
      throw std::runtime_error("direction not set");
    }
  }

  return wire;
}

bool Intersects(WireSegment horizontal_segment, WireSegment vertical_segment) {
  return (vertical_segment.start.y < horizontal_segment.start.y &&
          horizontal_segment.start.y < vertical_segment.end.y &&
          horizontal_segment.start.x < vertical_segment.start.x &&
          vertical_segment.start.x < horizontal_segment.end.x) ||
         (vertical_segment.start.y > horizontal_segment.start.y &&
          horizontal_segment.start.y > vertical_segment.end.y &&
          horizontal_segment.start.x > vertical_segment.start.x &&
          vertical_segment.start.x > horizontal_segment.end.x);
}

std::vector<Point> FindIntersections(std::vector<WireSegment> wire1,
                                     std::vector<WireSegment> wire2) {
  std::vector<Point> intersections;

  for (const auto &segment1 : wire1) {
    std::cout << "wire1 " << segment1.start.x << " / " << segment1.start.y
              << " <--> " << segment1.end.x << " / " << segment1.end.y
              << std::endl;
  }
  for (const auto &segment2 : wire2) {
    std::cout << "wire2 " << segment2.start.x << " / " << segment2.start.y
              << " <--> " << segment2.end.x << " / " << segment2.end.y
              << std::endl;
  }

  for (const auto &segment1 : wire1) {
    for (const auto &segment2 : wire2) {
      if (segment1.direction == Direction::HORIZONTAL &&
          segment2.direction == Direction::VERTICAL) {
        if (Intersects(segment1, segment2)) {
          intersections.emplace_back(Point{segment2.start.x, segment1.start.y});
          std::cout << "INTERSECT" << std::endl;
          std::cout << "wire1 " << segment1.start.x << " / " << segment1.start.y
                    << " <--> " << segment1.end.x << " / " << segment1.end.y
                    << std::endl;
          std::cout << "wire2 " << segment2.start.x << " / " << segment2.start.y
                    << " <--> " << segment2.end.x << " / " << segment2.end.y
                    << std::endl;
        }

      } else if (segment1.direction == Direction::VERTICAL &&
                 segment2.direction == Direction::HORIZONTAL) {
        if (Intersects(segment2, segment1)) {
          intersections.emplace_back(Point{segment1.start.x, segment2.start.y});
          std::cout << "INTERSECT" << std::endl;
          std::cout << "wire1 " << segment1.start.x << " / " << segment1.start.y
                    << " <--> " << segment1.end.x << " / " << segment1.end.y
                    << std::endl;
          std::cout << "wire2 " << segment2.start.x << " / " << segment2.start.y
                    << " <--> " << segment2.end.x << " / " << segment2.end.y
                    << std::endl;
        }
      }
    }
    std::cout << std::endl;
  }

  for (auto intersection : intersections) {
    std::cout << "intersection: " << intersection.x << " / " << intersection.y
              << std::endl;
  }

  return intersections;
}

int ClosestIntersection(std::vector<Point> intersections) {
  std::sort(std::begin(intersections), std::end(intersections),
            [](Point a, Point b) {
              return (std::abs(a.x) + std::abs(a.y)) <
                     (std::abs(b.x) + std::abs(b.y));
            });

  return std::abs(intersections[0].x) + std::abs(intersections[0].y);
}

int main() {
  std::string line;
  std::ifstream input_file("../input/input.txt");

  std::getline(input_file, line);
  auto wire1 = GenerateWire(line);

  std::getline(input_file, line);
  auto wire2 = GenerateWire(line);

  auto intersections = FindIntersections(wire1, wire2);

  // part 1
  {
    std::cout << "part 1: " << ClosestIntersection(intersections) << std::endl;
  }

  // part 2
  {
    // std::cout << "part 2: " << IntersectionWithMinimalDelay(intersections) << std::endl;
  }

  return EXIT_SUCCESS;
}
