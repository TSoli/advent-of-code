#include <aoc/io.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

long getInvalidIds(const string &start, const string &end) {
  long total{0};
  string s{start};

  while (s.length() < end.length()) {
    string e{to_string(static_cast<long>(pow(10, s.length())) - 1)};
    total += getInvalidIds(s, e);
    s = to_string(static_cast<long>(pow(10, s.length())));
  }

  int len{static_cast<int>(s.length())};
  set<string> invalidIds;
  for (int i = 2; len / i > 0; ++i) {
    if (len % i)
      continue;

    vector<long> lower;
    vector<long> upper;

    for (size_t j = 0; j + (len / i) <= len; j += len / i) {
      lower.push_back(stol(s.substr(j, len / i)));
      upper.push_back(static_cast<long>(pow(10, len / i) - 1));
    }

    upper[0] = stol(end.substr(0, len / i));

    while (lower[0] <= upper[0]) {
      int k = 1;
      while (k < lower.size() && lower[k - 1] == upper[k - 1]) {
        upper[k] = stol(end.substr(k * (len / i), len / i));
        ++k;
      }

      bool isInRange{true};
      for (int k = 0; k < lower.size(); ++k) {
        if (lower[0] > lower[k] && lower[0] < upper[k])
          break;
        if (lower[0] < lower[k] || lower[0] > upper[k]) {
          isInRange = false;
          break;
        }
      }

      string id;
      for (int n = 0; n < i; ++n) {
        id += to_string(lower[0]);
      }

      if (!invalidIds.contains(id) && isInRange) {
        total += stol(id);
        invalidIds.insert(id);
      }

      for (int k = 1; k < lower.size(); ++k) {
        lower[k] = 0;
      }

      ++lower[0];
    }
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
