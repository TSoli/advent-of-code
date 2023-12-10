#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::string line;
  std::vector<std::vector<int>> sequences;

  while (std::getline(inf, line)) {
    std::istringstream lineStream{line};
    std::vector<int> sequence;
    int num;
    while (lineStream >> num) {
      sequence.push_back(num);
    }

    sequences.push_back(sequence);
  }

  int sum{0};
  for (const auto &sequence : sequences) {
    std::vector<int> diff(sequence.begin(), sequence.end());
    int nextNum{sequence.back()};
    while (true) {
      std::adjacent_difference(diff.begin(), diff.end(), diff.begin());
      diff.erase(diff.begin());
      nextNum += diff.back();

      if (std::adjacent_find(diff.begin(), diff.end(), std::not_equal_to<>()) ==
          diff.end()) {
        break;
      }
    }

    sum += nextNum;
  }

  std::cout << "Sum: " << sum << "\n";
  return 0;
}
