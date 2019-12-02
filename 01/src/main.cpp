#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "../../common.h"

int main() {
  auto input_values = GetVectorFromFile<int>("../input/input.txt");

  // part 1
  {
    auto result =
        std::accumulate(input_values.begin(), input_values.end(), 0,
                        [&](int current_sum, int mass) {
                          return current_sum + std::floor(mass / 3.) - 2;
                        });

    std::cout << "part 1: " << std::fixed << result << std::endl;
  }

  // part 2
  {
    std::function<int(int, int)> calculate_fuel;
    calculate_fuel = [&](int current_sum, int mass) {
      auto fuel = static_cast<int>(std::floor(mass / 3.) - 2);
      return fuel > 0 ? fuel + calculate_fuel(current_sum, fuel) : current_sum;
    };

    auto result = std::accumulate(input_values.begin(), input_values.end(), 0,
                                  calculate_fuel);

    std::cout << "part 2: " << std::fixed << result << std::endl;
  }

  return EXIT_SUCCESS;
}
