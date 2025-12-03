#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aoc {
void getInput(int argc, char *argv[], std::ifstream &inf) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    exit(1);
  }

  inf.open(argv[1]);

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    exit(2);
  }
}

std::vector<std::string> readLines(std::ifstream &inf, char delim) {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(inf, line)) {
    lines.push_back(line);
  }
  return lines;
}
} // namespace aoc
