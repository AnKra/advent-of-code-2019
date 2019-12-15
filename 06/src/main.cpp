#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Object {
public:
  Object(const std::string &n) : name(n){};
  virtual ~Object(){};

  std::string name;
  std::shared_ptr<Object> predecessor;
  std::vector<std::shared_ptr<Object>> successors;
};

std::map<std::string, std::shared_ptr<Object>>
CreateMap(const std::string &file_name) {
  std::map<std::string, std::shared_ptr<Object>> objects;

  std::ifstream input_file(file_name);
  std::string value_string;
  while (std::getline(input_file, value_string)) {
    // extract object names using regex
    std::regex orbit_pattern("(.+)\\)(.+)");
    std::smatch matches;
    if (!std::regex_match(value_string, matches, orbit_pattern)) {
      throw std::runtime_error("parse error");
    }

    auto predecessor_name = *std::next(matches.begin());
    auto successor_name = *std::next(std::next(matches.begin()));

    // fill objects map
    if (objects.find(predecessor_name) == std::end(objects)) {
      objects[predecessor_name] = std::make_shared<Object>(predecessor_name);
    }
    if (objects.find(successor_name) == std::end(objects)) {
      objects[successor_name] = std::make_shared<Object>(successor_name);
    }
    objects[successor_name]->predecessor = objects[predecessor_name];
    objects[predecessor_name]->successors.push_back(objects[successor_name]);
  }

  return objects;
};

int NumberOfIndirectOrbits(const std::shared_ptr<Object> object,
                           int distance_to_com) {
  int children_distance_to_com = 0;
  for (const auto successor : object->successors) {
    children_distance_to_com +=
        NumberOfIndirectOrbits(successor, distance_to_com + 1);
  }

  return distance_to_com + children_distance_to_com;
}

std::pair<bool, int>
NumberOfTransfersToSanta(const std::shared_ptr<Object> object,
                         std::string caller_name, int number_of_hops) {
  if (caller_name == object->predecessor->name &&
      object->successors.size() == 0) {
    return std::make_pair(false, number_of_hops);
  }

  bool child_success = false;
  int child_number_of_hops_min = std::numeric_limits<int>::max();
  for (const auto successor : object->successors) {
    if (successor->name == "SAN") {
      return std::make_pair(true, number_of_hops);
    } else if (successor->name != caller_name) {
      auto result =
          NumberOfTransfersToSanta(successor, object->name, number_of_hops + 1);

      if (result.first && result.second < child_number_of_hops_min) {
        child_success = true;
        child_number_of_hops_min = result.second;
      }
    }
  }

  if (object->predecessor->name != caller_name &&
      object->predecessor->name != "COM") {
    auto result = NumberOfTransfersToSanta(object->predecessor, object->name,
                                           number_of_hops + 1);
    if (result.first && result.second < child_number_of_hops_min) {
      child_success = true;
      child_number_of_hops_min = result.second;
    }
  }

  return std::make_pair(child_success, child_number_of_hops_min);
}

int main() {
  auto map = CreateMap("../input/input.txt");

  // part 1
  {
    std::cout << "part 1: " << NumberOfIndirectOrbits(map["COM"], 0)
              << std::endl;
  }

  // part 2
  {
    std::cout << "part 2: "
              << NumberOfTransfersToSanta(map["YOU"]->predecessor, "", 0).second
              << std::endl;
  }

  return EXIT_SUCCESS;
}
