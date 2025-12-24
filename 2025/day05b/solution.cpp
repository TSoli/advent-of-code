#include <aoc/graph.h>
#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

struct IDRange {
  long lower;
  long upper;
};

void addRange(list<IDRange> &idRanges, IDRange idRange) {
  auto it{idRanges.begin()};
  while (it != idRanges.end()) {
    if (it->lower <= idRange.lower && it->upper >= idRange.upper) {
      return;
    }

    if (idRange.lower <= it->lower && idRange.upper >= it->upper) {
      it = idRanges.erase(it);
    } else if (it->lower < idRange.lower && it->upper >= idRange.lower) {
      idRange.lower = it->lower;
      it = idRanges.erase(it);
    } else if (it->upper > idRange.upper && it->lower <= idRange.upper) {
      idRange.upper = it->upper;
    } else {
      ++it;
    }
  }

  idRanges.push_back(idRange);
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  list<IDRange> idRanges;
  int i{0};
  size_t splitPos;
  while (splitPos = lines[i].find('-'), splitPos != string::npos) {
    addRange(idRanges, {stol(lines[i].substr(0, splitPos)),
                        stol(lines[i].substr(splitPos + 1))});
    ++i;
  }

  long total{0};
  for (const auto idRange : idRanges) {
    total += idRange.upper - idRange.lower + 1;
  }

  cout << "Fresh Ingredients: " << total << endl;
  return 0;
}
