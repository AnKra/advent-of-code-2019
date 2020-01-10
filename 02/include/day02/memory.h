#ifndef DAY_02_INCLUDE_DAY02_MEMORY_H_
#define DAY_02_INCLUDE_DAY02_MEMORY_H_

#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace day02 {

class Memory {
public:
  enum class Mode { IMMEDIATE = 0, POSITION = 1 };

  explicit Memory(const std::string file_name) {
    std::ifstream input_file(file_name);
    std::string value_string;
    while (std::getline(input_file, value_string, ',')) {
      memory_.push_back(std::stoi(value_string));
    }
  }

  void Set(const int address, const int value,
           const Mode mode = Mode::POSITION) {
    if (mode == Mode::POSITION) {
      memory_[Get(address)] = value;
    } else {
      memory_[address] = value;
    }
  }

  int Get(const int param) const { return memory_[param]; }

  int Get(const std::pair<int, Mode> param_mode_pair) const {
    if (std::get<1>(param_mode_pair) == Mode::POSITION) {
      return Get(memory_[std::get<0>(param_mode_pair)]);
    } else {
      return memory_[std::get<0>(param_mode_pair)];
    }
  }

private:
  std::vector<int> memory_;
};

} // namespace day02

#endif // DAY_02_INCLUDE_DAY02_MEMORY_H_
