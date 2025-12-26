#include <aoc/graph.h>
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

  vector<vector<long>> vertices;
  for (const auto &line : lines) {
    vertices.push_back(aoc::getListLong(line, ","));
  }

  long maxArea{0};
  for (int i = 0; i < vertices.size(); ++i) {
    for (int j = i + 1; j < vertices.size(); ++j) {
      long area{(abs(vertices[j][0] - vertices[i][0]) + 1) *
                (abs(vertices[j][1] - vertices[i][1]) + 1)};
      maxArea = max(maxArea, area);
    }
  }

  cout << "Largest rectangle area: " << maxArea << endl;
  return 0;
}
