#include <iostream>
#include <vector>

#include "../../common.h"

std::vector<int> Passwords(const int min, const int max) {
  std::vector<int> passwords{};

  std::vector<int> digits{};

  int number = min;
  while (number) {
    auto digit = number % 10;
    number /= 10;
    digits.push_back(digit);
  }



  int j_init = digits[1];
  int i_init = digits[0];
  for (int j = j_init; j <= 9; ++j) {
    for (int i = i_init; i <= 9; ++i) {
      passwords.emplace_back(i + j * 10 + 1 * 100);
    }
    i_init = j + 1;
  }
  j_init = 0;

  for (const auto &password : passwords) {
    std::cout << password << std::endl;
  }

  return passwords;
}

int main() {
  // int input_min{231832};
  // int input_max{767346};

  int input_min{123};
  int input_max{1000};

  auto passwords = Passwords(input_min, input_max);

  // part 1
  { std::cout << "part 1: " << passwords.size() << std::endl; }

  // part 2
  // { std::cout << "part 2: " << ??? << std::endl; }

  return EXIT_SUCCESS;
}
