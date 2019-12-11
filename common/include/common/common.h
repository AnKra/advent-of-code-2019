#ifndef ADVENT_OF_CODE_2018_COMMON_H_
#define ADVENT_OF_CODE_2018_COMMON_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

// Return containers by value (relying on move or copy elision for efficiency)
// Reasons:
//   * looks cleaner
//   * probably as fast as returning by pointer
// Source:
// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Cr-value-return
template <typename T>
std::vector<T> GetVectorFromFile(const std::string &file_name) {
  // Prefer { } to ( )
  // Reasons:
  //   * it always initializes the variable
  //   * it is never ambiguous with a function declaration
  //   * is safer because it does not allow narrowing (a.k.a. “lossy”)
  //     conversions
  // Source: https://herbsutter.com/2013/05/09/gotw-1-solution/
  std::ifstream input_file(file_name);
  std::vector<T> result{std::istream_iterator<T>(input_file),
                        std::istream_iterator<T>()};
  return result;
}

#endif // ADVENT_OF_CODE_2018_COMMON_H_
