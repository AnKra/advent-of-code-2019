#include <iostream>
#include <memory>

#include <common/common.h>

#include "day02/computer.h"
#include "day02/memory.h"

int main() {
  // part 1
  {
    day02::Computer computer(
        std::make_unique<day02::Memory>("../input/input.txt"));
    computer.SetInput(12, 2);
    computer.Run();
    std::cout << "part 1: " << computer.Get(0) << std::endl;
  }

  // part 2
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      day02::Computer computer(
          std::make_unique<day02::Memory>("../input/input.txt"));
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
