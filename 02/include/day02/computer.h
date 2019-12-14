#ifndef DAY_02_INCLUDE_SQALRE_H_
#define DAY_02_INCLUDE_SQALRE_H_

#include <vector>


static std::vector<int> CreateMemory(std::string file_name) {
  std::vector<int> memory = {};

  std::ifstream input_file(file_name);
  std::string value_string;
  while (std::getline(input_file, value_string, ',')) {
    memory.push_back(std::stoi(value_string));
  }

  return memory;
}


struct Instruction {
  enum class GetMode { IMMEDIATE = 0, POSITION = 1 };

  int opcode;
  std::vector<int> params;
  std::vector<GetMode> param_modes;
};


class Computer {
public:
  enum class Opcode { ADD = 1, MULTIPLY = 2 };

  Computer(std::vector<int> initial_registers)
      : memory_(initial_registers), instruction_ptr_(0) {}
  virtual ~Computer() {}

  void Run() {
    auto instruction = Decode(instruction_ptr_);

    while (instruction.opcode != 99) {
      if (!ExecuteInstruction(instruction)) {
        std::cout << "unknown opcode " << instruction.opcode << std::endl;
        return;
      }
      instruction = Decode(instruction_ptr_);
    }
  }

  void SetInput(int noun, int verb) {
    Set(1, noun);
    Set(2, verb);
  }

  int Get(const int param, const Instruction::GetMode mode =
                               Instruction::GetMode::IMMEDIATE) const {
    if (mode == Instruction::GetMode::IMMEDIATE) {
      return memory_[param];
    } else {
      return Computer::Get(memory_[param], Instruction::GetMode::IMMEDIATE);
    }
  }

protected:
  void Set(int address, int value) { memory_[address] = value; }

  virtual Instruction Decode(const int instruction_ptr) const {
    Instruction instruction = {Get(instruction_ptr_), {}};

    for (size_t i = 1; i <= 3; ++i) {
      instruction.params.push_back(instruction_ptr_ + i);
      instruction.param_modes.push_back(Instruction::GetMode::POSITION);
    }

    return instruction; 
  }

  virtual bool ExecuteInstruction(const Instruction &instruction) {
    switch (static_cast<Opcode>(instruction.opcode)) {
    case Opcode::ADD:
      Set(Get(instruction.params[2]),
          Get(instruction.params[0], instruction.param_modes[0]) +
              Get(instruction.params[1], instruction.param_modes[1]));
      instruction_ptr_ += 4;
      break;
    case Opcode::MULTIPLY:
      Set(Get(instruction.params[2]),
          Get(instruction.params[0], instruction.param_modes[0]) *
              Get(instruction.params[1], instruction.param_modes[1]));
      instruction_ptr_ += 4;
      break;
    default:
      return false;
    }

    return true;
  }

  std::vector<int> memory_;
  int instruction_ptr_;
};

#endif
