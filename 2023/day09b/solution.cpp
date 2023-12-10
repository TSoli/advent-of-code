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
    int prevNum{sequence.front()};
    int i{0};

    while (true) {
      std::adjacent_difference(diff.begin(), diff.end(), diff.begin());
      if (diff.size() < 2) {
        std::cerr << "Could not find common difference\n";
        return 3;
      }

      diff.erase(diff.begin());

      if (i++ % 2 == 0) {
        prevNum -= diff.front();
      } else {
        prevNum += diff.front();
      }

      if (std::adjacent_find(diff.begin(), diff.end(), std::not_equal_to<>()) ==
          diff.end()) {
        break;
      }
    }

    sum += prevNum;
  }

  std::cout << "Sum: " << sum << "\n";
  return 0;
}
