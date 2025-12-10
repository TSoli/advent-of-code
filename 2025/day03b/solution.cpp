#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  long totalJoltage{0};
  for (auto &line : lines) {
    string maxJoltage;
    int pos{-1};

    for (int j = 11; j >= 0; --j) {
      char maxC{'0'};
      for (int i = pos + 1; i < line.length() - j; ++i) {
        if (line[i] > maxC) {
          pos = i;
          maxC = line[i];
        }
      }

      maxJoltage += maxC;
    }

    totalJoltage += stol(maxJoltage);
  }

  cout << "Total Joltage is: " << totalJoltage << endl;
  return 0;
}
