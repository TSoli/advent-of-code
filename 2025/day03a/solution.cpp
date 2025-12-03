#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  int totalJoltage{0};
  for (auto &line : lines) {
    string maxJoltage;
    int pos{0};
    char maxC{'0'};
    for (int i = 0; i < line.length() - 1; ++i) {
      if (line[i] > maxC) {
        pos = i;
        maxC = line[i];
      }
    }

    maxJoltage += maxC;

    maxC = '0';
    for (auto c : line | views::drop(pos + 1)) {
      maxC = max(maxC, c);
    }
    maxJoltage += maxC;

    totalJoltage += stoi(maxJoltage);
  }

  cout << "Total Joltage is: " << totalJoltage << endl;
  return 0;
}
