#ifndef DAY_05_INCLUDE_DAY05_COMPUTER_H_
#define DAY_05_INCLUDE_DAY05_COMPUTER_H_

#include <memory>
#include <utility>

#include <day02/computer.h>
#include <day02/memory.h>

namespace day05 {

class SuperComputer : public day02::Computer {
public:
  SuperComputer(std::unique_ptr<day02::Memory> memory, int input)
      : day02::Computer(std::move(memory)), input_(input) {}
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

  bool ExecuteInstruction(const day02::Instruction &instruction) override {
    if (day02::Computer::ExecuteInstruction(instruction)) {
      return true;
    }

    switch (static_cast<SuperOpcode>(instruction.opcode)) {
    case SuperOpcode::SAVE:
      memory_->Set(std::get<0>(instruction.params[0]), input_);
      instruction_ptr_ += 2;
      break;
    case SuperOpcode::OUTPUT:
      std::cout << (memory_->Get(instruction.params[0])) << std::endl;
      instruction_ptr_ += 2;
      break;
    case SuperOpcode::JUMP_IF_TRUE:
      if (memory_->Get(instruction.params[0]) != 0) {
        instruction_ptr_ = memory_->Get(instruction.params[1]);
      } else {
        instruction_ptr_ += 3;
      }
      break;
    case SuperOpcode::JUMP_IF_FALSE:
      if (memory_->Get(instruction.params[0]) == 0) {
        instruction_ptr_ = memory_->Get(instruction.params[1]);
      } else {
        instruction_ptr_ += 3;
      }
      break;
    case SuperOpcode::LESS_THAN:
      if (memory_->Get(instruction.params[0]) <
          memory_->Get(instruction.params[1])) {
        memory_->Set(std::get<0>(instruction.params[2]), 1);
      } else {
        memory_->Set(std::get<0>(instruction.params[2]), 0);
      }
      instruction_ptr_ += 4;
      break;
    case SuperOpcode::EQUALS:
      if (memory_->Get(instruction.params[0]) ==
          memory_->Get(instruction.params[1])) {
        memory_->Set(std::get<0>(instruction.params[2]), 1);
      } else {
        memory_->Set(std::get<0>(instruction.params[2]), 0);
      }
      instruction_ptr_ += 4;
      break;
    default:
      return false;
    }

    return true;
  }

  day02::Instruction Decode(const int instruction_ptr) const override {
    day02::Instruction instruction = {memory_->Get(instruction_ptr_), {}};

    auto opcode_copy = instruction.opcode / 100;
    instruction.opcode = instruction.opcode % 100;

    for (size_t i = 1; i <= 3; ++i) {
      instruction.params.push_back(std::make_pair(
          instruction_ptr_ + i, (opcode_copy % 10) == 1
                                    ? day02::Memory::Mode::IMMEDIATE
                                    : day02::Memory::Mode::POSITION));
      opcode_copy /= 10;
    }

    return instruction;
  }

  int input_;
};

} // namespace day05

#endif // DAY_05_INCLUDE_DAY05_COMPUTER_H_
