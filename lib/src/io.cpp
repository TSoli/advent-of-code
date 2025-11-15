#include <aoc/io.h>

#include <fstream>
#include <iostream>

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
}  // namespace aoc
