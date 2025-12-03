#include <aoc/io.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <vector>

using namespace std;

bool isInvalid(const string &id) {
  size_t len{id.length()};
  size_t i{2};
  while (len / i) {
    if (len % i) {
      ++i;
      continue;
    }

    vector<string> substrings;
    for (size_t j = 0; j + (len / i) <= len; j += len / i) {
      substrings.push_back(id.substr(j, len / i));
    }

    if (ranges::all_of(substrings,
                       [&](const auto &s) { return s == substrings.front(); }))
      return true;

    ++i;
  }
  return false;
}

long getInvalidIds(long start, long end) {
  long total{0};
  while (start <= end) {
    if (isInvalid(to_string(start))) {
      total += start;
    }
    ++start;
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

    if (end.back() == '\n') end.pop_back();

    long numInvalid{getInvalidIds(stol(start), stol(end))};
    total += numInvalid;
  }

  cout << "Total invalid IDS: " << total << endl;
  return 0;
}
