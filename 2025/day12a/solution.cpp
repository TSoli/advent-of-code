#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

int canFit(const string &line, const vector<int> &areas) {
  size_t splitPos(line.find(':'));
  string sizeStr{line.substr(0, splitPos)};
  string numStr{line.substr(splitPos + 2)};

  vector<int> size{aoc::getListInt(sizeStr, "x")};
  vector<int> nums{aoc::getListInt(numStr, " ")};
  int area{size[0] * size[1]};
  int maxArea{accumulate(nums.begin(), nums.end(), 0) *
              9}; // each fits in a 3x3 box
  int minArea{transform_reduce(areas.begin(), areas.end(), nums.begin(), 0)};
  if (maxArea <= area) {
    return true;
  }

  if (minArea > area) {
    return false;
  }

  // If neither of the above we don't know
  cout << "Not sure: " << line << "\n";
  return -1;
}

int getAreas(const vector<string> &lines, vector<int> &areas) {
  int i{0};
  while (lines[i][1] == ':') {
    ++i;
    int area{0};
    while (lines[i][0] == '.' || lines[i][0] == '#') {
      for (auto c : lines[i]) {
        area += c == '#';
      }
      ++i;
    }

    areas.push_back(area);
    ++i;
  }

  return i;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  vector<int> areas;
  int i = getAreas(lines, areas);
  int total{0};
  while (i < lines.size()) {
    int fits{canFit(lines[i], areas)};
    if (fits == -1) {
      cerr << "Could not determine: " << lines[i] << "\n";
      continue;
    }
    total += fits;
    ++i;
  }

  cout << "Can fit: " << total << endl;

  return 0;
}
