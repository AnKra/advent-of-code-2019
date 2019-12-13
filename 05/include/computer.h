#ifndef DAY_05_INCLUDE_SQALRE_H_
#define DAY_05_INCLUDE_SQALRE_H_

#include <day02/computer.h>

class SuperComputer : public Computer {
public:
  SuperComputer(std::vector<Register> initial_registers, int input)
      : Computer(initial_registers), input_(input) {}
  virtual ~SuperComputer() {}

protected:
  int Get(int parameter, bool immediate_mode) {
    if (immediate_mode) {
      return Computer::Get(parameter);
    } else {
      return Computer::Get(Computer::Get(parameter));
    }
  }

  bool ExecuteInstruction(int opcode, int &instruction_ptr) override {
    std::vector<bool> parameter_modes;

    Decode(opcode, parameter_modes);

    switch (opcode) {
    case 1:
      Set(Get(instruction_ptr + 3, true),
          Get(instruction_ptr + 1, parameter_modes[1]) +
              Get(instruction_ptr + 2, parameter_modes[2]));
      instruction_ptr += 4;
      break;
    case 2:
      Set(Get(instruction_ptr + 3, true),
          Get(instruction_ptr + 1, parameter_modes[1]) *
              Get(instruction_ptr + 2, parameter_modes[2]));
      instruction_ptr += 4;
      break;
    case 3:
      Set(Get(instruction_ptr + 1, true), input_);
      instruction_ptr += 2;
      break;
    case 4:
      std::cout << (Get(instruction_ptr + 1, parameter_modes[1])) << std::endl;
      instruction_ptr += 2;
      break;
    case 5:
      if (Get(instruction_ptr + 1, parameter_modes[1]) != 0) {
        instruction_ptr = Get(instruction_ptr + 2, parameter_modes[2]);
      } else {
        instruction_ptr += 3;
      }
      break;
    case 6:
      if (Get(instruction_ptr + 1, parameter_modes[1]) == 0) {
        instruction_ptr = Get(instruction_ptr + 2, parameter_modes[2]);
      } else {
        instruction_ptr += 3;
      }
      break;
    case 7:
      if (Get(instruction_ptr + 1, parameter_modes[1]) <
          Get(instruction_ptr + 2, parameter_modes[2])) {
        Set(Get(instruction_ptr + 3, true), 1);
      } else {
        Set(Get(instruction_ptr + 3, true), 0);
      }
      instruction_ptr += 4;
      break;
    case 8:
      if (Get(instruction_ptr + 1, parameter_modes[1]) ==
          Get(instruction_ptr + 2, parameter_modes[2])) {
        Set(Get(instruction_ptr + 3, true), 1);
      } else {
        Set(Get(instruction_ptr + 3, true), 0);
      }
      instruction_ptr += 4;
      break;
    default:
      return false;
    }

    return true;
  }

  void Decode(int &opcode, std::vector<bool> &parameter_modes) {
    auto opcode_copy = opcode / 100;

    opcode = opcode % 100;

    parameter_modes.push_back(
        false); // insert dummy value at position 0 to make indexing equivalent
                // to instruction pointer

    for (size_t i = 0; i < 3; ++i) {
      parameter_modes.push_back((opcode_copy % 10) == 1);
      opcode_copy /= 10;
    }
  }

  int input_;
};

#endif