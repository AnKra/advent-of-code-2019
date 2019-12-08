#include <cmath>
#include <iostream>
#include <vector>

#include "../../common.h"

std::vector<int> passwords{};

std::vector<int> input_min_digits{};

int input_min{231832};
int input_max{767346};

bool HasConsicutiveDigits(int number) {
  while (number) {
    auto last_two_digits = number % 100;
    if (last_two_digits % 11 == 0) {
      return true;
    }
    number /= 10;
  }

  return false;
}

void Password(int previous_digit, int power, int intermediate_sum) {
  if (power >= 0) {
    auto i_init = previous_digit;

    if (passwords.size() == 0) {
      i_init = input_min_digits[power];
    }

    for (int i = i_init; i <= 9; ++i) {
      Password(i, power - 1, intermediate_sum + i * pow(10, power));
    }
  } else {
    if (intermediate_sum > input_max) {
      return;
    }

    if (HasConsicutiveDigits(intermediate_sum)) {
      passwords.push_back(intermediate_sum);
    }
  }
}

std::vector<int> Digits(int number) {
  std::vector<int> digits;

  while (number) {
    auto digit = number % 10;
    number /= 10;
    digits.push_back(digit);
  }

  return digits;
}

std::vector<int> FirstValidNumber(std::vector<int> digits) {
  int max_digit = digits.back();
  bool overwrite = false;

  for (int i = digits.size() - 1; i >= 0; --i) {
    if (digits[i] < max_digit || overwrite) {
      digits[i] = max_digit;
      overwrite = true;
    } else if (digits[i] > max_digit) {
      max_digit = digits[i];
    }
  }
  return digits;
}

int main() {
  input_min_digits = FirstValidNumber(Digits(input_min));
  Password(input_min_digits.back(), input_min_digits.size() - 1, 0);

  for (const auto &password : passwords) {
    std::cout << password << std::endl;
  }

  // part 1
  { std::cout << "part 1: " << passwords.size() << std::endl; }

  // part 2
  // { std::cout << "part 2: " << ??? << std::endl; }

  return EXIT_SUCCESS;
}
