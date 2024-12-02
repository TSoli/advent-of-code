#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: solution.out <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::string line;
  int total = 0;
  while (std::getline(inf, line)) {
    int prev, curr, prevDiff, diff;
    std::istringstream linestream(line);
    linestream >> prev >> curr;
    prevDiff = curr - prev;
    if (prevDiff == 0 || std::abs(prevDiff) > 3) continue;

    prev = curr;
    while (linestream >> curr) {
      diff = curr - prev;
      if (((diff < 0) != (prevDiff < 0)) || std::abs(diff) > 3 || diff == 0) {
        total -= 1;
        break;
      }

      prevDiff = diff;
      prev = curr;
    }

    total += 1;
  }

  std::cout << "Number safe reports: " << total << std::endl;

  return 0;
}
