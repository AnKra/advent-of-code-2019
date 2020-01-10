#ifndef DAY_02_INCLUDE_DAY02_COMPUTER_H_
#define DAY_02_INCLUDE_DAY02_COMPUTER_H_

#include <memory>
#include <utility>
#include <vector>

#include "day02/memory.h"

namespace day02 {

struct Instruction {
  int opcode;
  std::vector<std::pair<int, Memory::Mode>> params;
};

class Computer {
public:
  enum class Opcode { ADD = 1, MULTIPLY = 2 };

  explicit Computer(std::unique_ptr<Memory> memory)
      : memory_(std::move(memory)), instruction_ptr_(0) {}
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
    memory_->Set(1, noun, Memory::Mode::IMMEDIATE);
    memory_->Set(2, verb, Memory::Mode::IMMEDIATE);
  }

  int Get(const int param) const { return memory_->Get(param); }

protected:
  virtual Instruction Decode(const int instruction_ptr) const {
    Instruction instruction = {memory_->Get(instruction_ptr_), {}};

    for (size_t i = 1; i <= 3; ++i) {
      instruction.params.push_back(
          std::make_pair(instruction_ptr_ + i, Memory::Mode::POSITION));
    }

    return instruction;
  }

  virtual bool ExecuteInstruction(const Instruction &instruction) {
    switch (static_cast<Opcode>(instruction.opcode)) {
    case Opcode::ADD:
      memory_->Set(std::get<0>(instruction.params[2]),
                   memory_->Get(instruction.params[0]) +
                       memory_->Get(instruction.params[1]));
      instruction_ptr_ += 4;
      break;
    case Opcode::MULTIPLY:
      memory_->Set(std::get<0>(instruction.params[2]),
                   memory_->Get(instruction.params[0]) *
                       memory_->Get(instruction.params[1]));
      instruction_ptr_ += 4;
      break;
    default:
      return false;
    }

    return true;
  }

  int instruction_ptr_;
  std::unique_ptr<Memory> memory_;
};

} // namespace day02

#endif // DAY_02_INCLUDE_DAY02_COMPUTER_H_
