#ifndef DAY_02_INCLUDE_SQALRE_H_
#define DAY_02_INCLUDE_SQALRE_H_
 
#include <vector>

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

protected:
  std::vector<Register> memory_;
};

#endif