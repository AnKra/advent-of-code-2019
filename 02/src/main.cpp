#include <iostream>
#include <vector>

#include <common/common.h>

#include "day02/computer.h"

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
