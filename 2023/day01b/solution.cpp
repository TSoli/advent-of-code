#include <limits.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>

template <typename Iter>
int getDigit(Iter begin, Iter end, bool fromEnd = false);

const std::unordered_map<std::string, int> DIGITS = {
    {"zero", 0}, {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4},
    {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

int main() {
  std::string filename = "input.txt";
  std::ifstream inf(filename);
  if (!inf) {
    std::cerr << "Couldn't open " + filename + "\n";
    return 1;
  }

  int sum = 0;
  while (inf) {
    std::string line;
    inf >> line;
    sum += 10 * getDigit(line.begin(), line.end());
    sum += getDigit(line.rbegin(), line.rend(), true);
  }

  std::cout << sum << std::endl;
  return 0;
}

template <typename Iter>
int getDigit(Iter begin, Iter end, bool fromEnd) {
  int pos = INT_MAX;
  int digit = 0;
  std::string line(begin, end);

  for (auto it = begin; it != end; ++it) {
    if (std::isdigit(*it)) {
      pos = it - begin;
      digit = *it - '0';
      break;
    }
  }

  for (auto &[str, num] : DIGITS) {
    int currPos = -1;
    if (fromEnd) {
      std::string revStr(str.rbegin(), str.rend());
      currPos = line.find(revStr);
    } else {
      currPos = line.find(str);
    }

    if (currPos != std::string::npos && currPos < pos) {
      pos = currPos;
      digit = num;
    }
  }

  return digit;
}
