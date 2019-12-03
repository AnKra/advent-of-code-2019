#include <iostream>
#include <vector>

#include "../../common.h"

struct Register {
  int value;
};

std::istream &operator>>(std::istream &is, Register &output) {
  std::string value_string;
  std::getline(is, value_string, ',');
  if (value_string.size() > 0) {
    output.value = std::stoi(value_string);
  }
  return is;
}

class Computer {
public:
  Computer(std::vector<Register> initial_registers)
      : memory_(initial_registers) {}
  virtual ~Computer() {}

  void Set(int address, int value) { memory_[address].value = value; }

  int Get(int address) { return memory_[address].value; }

  void Run() {
    int instruction_ptr = 0;
    auto opcode = Get(instruction_ptr);

    while (opcode != 99) {
      switch (opcode) {
      case 1:
        Set(Get(instruction_ptr + 3),
            Get(Get(instruction_ptr + 1)) + Get(Get(instruction_ptr + 2)));
        instruction_ptr += 4;
        break;
      case 2:
        Set(Get(instruction_ptr + 3),
            Get(Get(instruction_ptr + 1)) * Get(Get(instruction_ptr + 2)));
        instruction_ptr += 4;
        break;
      default:
        std::cout << "unknown opcode " << opcode << std::endl;
      }

      opcode = Get(instruction_ptr);
    }
  }

  void SetInput(int noun, int verb) {
    Set(1, noun);
    Set(2, verb);
  }

private:
  std::vector<Register> memory_;
};

int main() {
  auto memory = GetVectorFromFile<Register>("../input/input.txt");

  // part 1
  {
    Computer computer(memory);
    computer.SetInput(12, 2);
    computer.Run();
    std::cout << "part 1: " << computer.Get(0) << std::endl;
  }

  // part 2
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      Computer computer(memory);
      computer.SetInput(noun, verb);
      computer.Run();
      if (computer.Get(0) == 19690720) {
        std::cout << "part 2: " << (100 * noun + verb) << std::endl;
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
