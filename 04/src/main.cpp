#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "../../common.h"

class PasswordGenerator {
public:
  PasswordGenerator(const int input_min, const int input_max,
                    std::function<bool(int)> condition)
      : input_min_(input_min), input_max_(input_max), condition_(condition),
        input_min_digits_(FirstValidNumber(Digits(input_min))) {
    ;
  }

  virtual ~PasswordGenerator() {}

  std::vector<int> Passwords() {
    Password(input_min_digits_.back(), input_min_digits_.size() - 1, 0);
    return passwords_;
  }

private:
  std::vector<int> Digits(int number) const {
    std::vector<int> digits;

    do {
      auto digit = number % 10;
      digits.push_back(digit);
    } while (number /= 10);

    return digits;
  }

  std::vector<int> FirstValidNumber(std::vector<int> digits) const {
    int max_digit = digits.back();
    bool overwrite = false;

    for (auto digit = std::rbegin(digits); digit != std::rend(digits); digit++) {
      if (*digit < max_digit || overwrite) {
        *digit = max_digit;
        overwrite = true;
      } else if (*digit > max_digit) {
        max_digit = *digit;
      }
    }

    return digits;
  }

  void Password(const int previous_digit, const int power,
                const int intermediate_sum) {
    if (power >= 0) {
      auto i_init = previous_digit;

      if (passwords_.size() == 0) {
        i_init = input_min_digits_[power];
      }

      for (size_t i = i_init; i <= 9; ++i) {
        Password(i, power - 1, intermediate_sum + i * pow(10, power));
      }
    } else {
      if (intermediate_sum > input_max_) {
        return;
      }

      if (condition_(intermediate_sum)) {
        passwords_.push_back(intermediate_sum);
      }
    }
  }

  int input_min_;
  int input_max_;

  std::function<bool(int)> condition_;
  std::vector<int> input_min_digits_;
  std::vector<int> passwords_;
};

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

    PasswordGenerator pw_gen{input_min, input_max, has_double_digits};
    auto passwords = pw_gen.Passwords();

    std::cout << "part 1: " << passwords.size() << std::endl;
  }

  // part 2
  {
    // std::cout << "part 2: " << passwords.size() << std::endl;
  }

  return EXIT_SUCCESS;
}
