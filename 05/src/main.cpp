#include <iostream>
#include <vector>

#include "computer.h"

int main() {
  auto memory = CreateMemory("../input/input.txt");

  // part 1
  {
    SuperComputer computer(memory, 1);
    std::cout << "part 1: " << std::endl;
    computer.Run();
  }

  // part 2
  {
    SuperComputer computer(memory, 5);
    std::cout << "part 1: " << std::endl;
    computer.Run();
  }

  return EXIT_SUCCESS;
}
