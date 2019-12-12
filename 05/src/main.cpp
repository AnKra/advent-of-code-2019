#include <iostream>
#include <vector>

#include <common/common.h>

#include "computer.h"

int main() {
  auto memory = GetVectorFromFile<Register>("../input/input.txt");

  // part 1
  {
    SuperComputer computer(memory, 1);
    std::cout << "part 1: " << std::endl;
    computer.Run();
  }

  // part 2

  return EXIT_SUCCESS;
}
