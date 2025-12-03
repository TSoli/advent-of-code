#include <aoc/io.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

long getInvalidIds(string start, string end) {
  long total{0};

  size_t len{start.length()};
  size_t endLen{end.length()};

  if (len % 2) {
    start = to_string(static_cast<long>(pow(10, len)));
    len = start.length();
  }

  while (len < endLen) {
    total +=
        getInvalidIds(start, to_string(static_cast<long>(pow(10, len) - 1)));
    start = to_string(static_cast<long>(pow(10, len + 1)));
    len = start.length();
  }

  if (len > endLen)
    return total;

  long leftLower{stol(start.substr(0, len / 2))};
  long leftUpper{stol(end.substr(0, len / 2))};
  long rightLower{stol(start.substr(len / 2))};
  long rightUpper{numeric_limits<long>::max()};

  while (leftLower <= leftUpper) {
    if (leftLower == leftUpper) {
      rightUpper = stol(end.substr(len / 2));
    }

    if ((leftLower >= rightLower) && (leftLower <= rightUpper)) {
      string id{to_string(leftLower) + to_string(leftLower)};
      total += stol(id);
    }
    rightLower = 0;
    ++leftLower;
  }
  return total;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);

  long total{0};
  string idRange;
  while (getline(inf, idRange, ',')) {
    size_t splitPos{idRange.find('-')};
    string start{idRange.substr(0, splitPos)};
    string end{idRange.substr(splitPos + 1)};

    if (end.back() == '\n')
      end.pop_back();

    long numInvalid{getInvalidIds(start, end)};
    total += numInvalid;
  }

  cout << "Total invalid IDS: " << total << endl;
  return 0;
}
