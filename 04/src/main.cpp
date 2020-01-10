#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <common/common.h>

#include "day04/password_generator.h"

int main() {
  int input_min{231832};
  int input_max{767346};

  // part 1
  {
    auto has_double_digits = [](int number) -> bool {
      do {
        auto last_two_digits = number % 100;
        if (last_two_digits % 11 == 0) {
          return true;
        }
      } while (number /= 10);

      return false;
    };

    day04::PasswordGenerator pw_gen{input_min, input_max, has_double_digits};
    auto passwords = pw_gen.Passwords();

    std::cout << "part 1: " << passwords.size() << std::endl;
  }

  // part 2
  {
    auto has_double_digits = [](int number) -> bool {
      enum class State { NOT_SAME, DOUBLE, MULTIPLE };
      State state = State::NOT_SAME;

      bool are_previous_two_digits_same = false;
      do {
        auto last_two_digits = number % 100;
        bool last_two_digits_are_same = (last_two_digits % 11 == 0);

        switch (state) {
        case State::NOT_SAME:
          if (last_two_digits_are_same)
            state = State::DOUBLE;
          break;
        case State::DOUBLE:
          if (last_two_digits_are_same)
            state = State::MULTIPLE;
          else
            return true;
          break;
        case State::MULTIPLE:
          if (last_two_digits_are_same)
            state = State::MULTIPLE;
          else
            state = State::NOT_SAME;
          break;
        default:
          throw std::runtime_error("unknown state");
        }
      } while (number /= 10);
      return false;
    };

    day04::PasswordGenerator pw_gen{input_min, input_max, has_double_digits};
    auto passwords = pw_gen.Passwords();

    std::cout << "part 2: " << passwords.size() << std::endl;
  }

  return EXIT_SUCCESS;
}
