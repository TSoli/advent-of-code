#include <aoc/graph.h>
#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct IDRange {
  long lower;
  long upper;
};

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  vector<IDRange> idRanges;
  int i{0};
  size_t splitPos;
  while (splitPos = lines[i].find('-'), splitPos != string::npos) {
    idRanges.emplace_back(stol(lines[i].substr(0, splitPos)),
                          stol(lines[i].substr(splitPos + 1)));
    ++i;
  }

  int total{0};
  for (++i; i < lines.size(); ++i) {
    long id{stol(lines[i])};
    for (const auto &idRange : idRanges) {
      if (id >= idRange.lower && id <= idRange.upper) {
        ++total;
        break;
      }
    }
  }

  cout << "Fresh Ingredients: " << total << endl;
  return 0;
}
