#include <iostream>
#include <sstream>
#include <vector>

#include <common/common.h>

#include <day03/intersection.h>
#include <day03/node.h>
#include <day03/point.h>
#include <day03/wire.h>

namespace day03 {

std::vector<Intersection> FindIntersections(const Wire &wire1,
                                            const Wire &wire2) {
  std::vector<Intersection> intersections;
  const Node *node1 = wire1.Origin();

  while (node1->next) {
    const Node *node2 = wire2.Origin();

    while (node2->next) {
      auto horizontal =
          Node::GetNodeByDirection(Direction::HORIZONTAL, node1, node2);
      auto vertical =
          Node::GetNodeByDirection(Direction::VERTICAL, node1, node2);

      if (horizontal && vertical && Node::Intersects(*horizontal, *vertical)) {
        intersections.emplace_back(*horizontal, *vertical);
      }

      node2 = node2->next.get();
    }

    node1 = node1->next.get();
  }

  return intersections;
}

int ClosestIntersection(std::vector<Intersection> intersections) {
  Point origin{0, 0};
  std::sort(std::begin(intersections), std::end(intersections),
            [&](Intersection a, Intersection b) {
              return Point::Distance(a.GetPoint(), origin) <
                     Point::Distance(b.GetPoint(), origin);
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

} // namespace day03

int main() {
  std::string line;
  std::ifstream input_file("../input/input.txt");

  std::getline(input_file, line);
  auto wire1 = day03::Wire(line);

  std::getline(input_file, line);
  auto wire2 = day03::Wire(line);

  auto intersections = day03::FindIntersections(wire1, wire2);

  // part 1
  {
    std::cout << "part 1: " << day03::ClosestIntersection(intersections)
              << std::endl;
  }

  // part 2
  {
    std::cout << "part 2: "
              << day03::IntersectionWithMinimalDelay(intersections)
              << std::endl;
  }

  return EXIT_SUCCESS;
}
