#include <iostream>
#include <memory>

#include <day02/memory.h>

#include "day05/computer.h"

int main() {
  // part 1
  {
    auto memory = std::make_unique<day02::Memory>("../input/input.txt");
    day05::SuperComputer computer(std::move(memory), 1);
    std::cout << "part 1: " << std::endl;
    computer.Run();
  }

  // part 2
  {
    auto memory = std::make_unique<day02::Memory>("../input/input.txt");
    day05::SuperComputer computer(std::move(memory), 5);
    std::cout << "part 1: " << std::endl;
    computer.Run();
  }

  return EXIT_SUCCESS;
}
