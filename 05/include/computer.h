#ifndef DAY_05_INCLUDE_SQALRE_H_
#define DAY_05_INCLUDE_SQALRE_H_

#include <day02/computer.h>


class SuperComputer : public Computer {
public:
  SuperComputer(std::vector<int> initial_registers, int input)
      : Computer(initial_registers), input_(input) {}
  virtual ~SuperComputer() {}

protected:
  enum class SuperOpcode {
    SAVE = 3,
    OUTPUT = 4,
    JUMP_IF_TRUE = 5,
    JUMP_IF_FALSE = 6,
    LESS_THAN = 7,
    EQUALS = 8
  };

  virtual bool ExecuteInstruction(const Instruction &instruction) override {
    if (Computer::ExecuteInstruction(instruction)) {
      return true;
    }

    switch (static_cast<SuperOpcode>(instruction.opcode)) {
    case SuperOpcode::SAVE:
      Set(Get(instruction.params[0]), input_);
      instruction_ptr_ += 2;
      break;
    case SuperOpcode::OUTPUT:
      std::cout << (Get(instruction.params[0], instruction.param_modes[0]))
                << std::endl;
      instruction_ptr_ += 2;
      break;
    case SuperOpcode::JUMP_IF_TRUE:
      if (Get(instruction.params[0], instruction.param_modes[0]) != 0) {
        instruction_ptr_ =
            Get(instruction.params[1], instruction.param_modes[1]);
      } else {
        instruction_ptr_ += 3;
      }
      break;
    case SuperOpcode::JUMP_IF_FALSE:
      if (Get(instruction.params[0], instruction.param_modes[0]) == 0) {
        instruction_ptr_ =
            Get(instruction.params[1], instruction.param_modes[1]);
      } else {
        instruction_ptr_ += 3;
      }
      break;
    case SuperOpcode::LESS_THAN:
      if (Get(instruction.params[0], instruction.param_modes[0]) <
          Get(instruction.params[1], instruction.param_modes[1])) {
        Set(Get(instruction.params[2]), 1);
      } else {
        Set(Get(instruction.params[2]), 0);
      }
      instruction_ptr_ += 4;
      break;
    case SuperOpcode::EQUALS:
      if (Get(instruction.params[0], instruction.param_modes[0]) ==
          Get(instruction.params[1], instruction.param_modes[1])) {
        Set(Get(instruction.params[2]), 1);
      } else {
        Set(Get(instruction.params[2]), 0);
      }
      instruction_ptr_ += 4;
      break;
    default:
      return false;
    }

    return true;
  }

  Instruction Decode(const int instruction_ptr) const override {
    Instruction instruction = {Get(instruction_ptr_), {}};

    auto opcode_copy = instruction.opcode / 100;
    instruction.opcode = instruction.opcode % 100;

    for (size_t i = 1; i <= 3; ++i) {
      instruction.params.push_back(instruction_ptr_ + i);
      instruction.param_modes.push_back((opcode_copy % 10) == 1
                                            ? Instruction::GetMode::IMMEDIATE
                                            : Instruction::GetMode::POSITION);
      opcode_copy /= 10;
    }

    return instruction;
  }

  int input_;
};

#endif