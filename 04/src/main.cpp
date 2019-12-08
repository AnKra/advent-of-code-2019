#include <cmath>
#include <iostream>
#include <vector>

#include "../../common.h"

std::vector<int> passwords{};

std::vector<int> digits{};

// int input_min{231832};
// int input_max{767346};

int input_min{123};
int input_max{777};

void Password(int previous_digit, int power, int intermediate_sum) {
  if (power >= 0) {
    auto i_init = previous_digit;

    if (passwords.size() == 0) {
      i_init = digits[power];
    }

    for (int i = i_init; i <= 9; ++i) {
      Password(i, power - 1, intermediate_sum + i * pow(10, power));
    }
  } else {
    if (intermediate_sum > input_max) {
      return;
    }

    passwords.push_back(intermediate_sum);
  }
}

std::vector<int> Passwords(const int min, const int max) {

  int number = min;
  while (number) {
    auto digit = number % 10;
    number /= 10;
    digits.push_back(digit);
  }

  Password(digits.back(), digits.size() - 1, 0);

  return passwords;
}

int main() {
  Passwords(input_min, input_max);

  for (const auto &password : passwords) {
    std::cout << password << std::endl;
  }

  // part 1
  { std::cout << "part 1: " << passwords.size() << std::endl; }

  // part 2
  // { std::cout << "part 2: " << ??? << std::endl; }

  return EXIT_SUCCESS;
}
