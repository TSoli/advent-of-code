#include <aoc/graph.h>
#include <aoc/io.h>

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T> constexpr int sign(T x) {
  return (T(0) < x) - (x < T(0));
}

bool isInsidePoint(const vector<long> &p, const array<long, 2> &xRange,
                   const array<long, 2> &yRange) {
  return p[0] > xRange[0] && p[0] < xRange[1] && p[1] > yRange[0] &&
         p[1] < yRange[1];
}

bool isInsideLine(const vector<long> &p1, const vector<long> &p2,
                  const array<long, 2> &xRange, const array<long, 2> &yRange) {
  if (isInsidePoint(p1, xRange, yRange) || isInsidePoint(p2, xRange, yRange)) {
    return true;
  }

  bool outsideX{(p1[0] >= xRange[1] && p2[0] >= xRange[1]) ||
                (p1[0] <= xRange[0] && p2[0] <= xRange[0])};
  bool outsideY{(p1[1] >= yRange[1] && p2[1] >= yRange[1]) ||
                (p1[1] <= yRange[0] && p2[1] <= yRange[0])};
  if (outsideX || outsideY) {
    return false;
  }

  if ((p1[0] <= xRange[0] && p2[0] >= xRange[1]) ||
      (p1[0] >= xRange[1] && p2[0] <= xRange[0])) {
    return true;
  }

  if ((p1[1] <= yRange[0] && p2[1] >= yRange[1]) ||
      (p1[1] >= yRange[1] && p2[1] <= yRange[0])) {
    return true;
  }

  return false;
}

bool isValid(const vector<vector<long>> &vertices, int idx1, int idx2) {
  array<long, 2> xRange{min(vertices[idx1][0], vertices[idx2][0]),
                        max(vertices[idx1][0], vertices[idx2][0])};
  array<long, 2> yRange{min(vertices[idx1][1], vertices[idx2][1]),
                        max(vertices[idx1][1], vertices[idx2][1])};

  for (int i = 0; i < vertices.size(); ++i) {
    int j = (i + 1) % vertices.size();
    if (isInsideLine(vertices[i], vertices[j], xRange, yRange)) {
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  vector<vector<long>> vertices;
  for (const auto &line : lines) {
    vertices.push_back(aoc::getListLong(line, ","));
  }

  long maxArea{0};
  int mi, mj;
  for (int i = 0; i < vertices.size(); ++i) {
    for (int j = i + 1; j < vertices.size(); ++j) {
      long area{(abs(vertices[j][0] - vertices[i][0]) + 1) *
                (abs(vertices[j][1] - vertices[i][1]) + 1)};
      if (area > maxArea && isValid(vertices, i, j)) {
        maxArea = area;
        mi = i;
        mj = j;
      }
    }
  }

  cout << vertices[mi][0] << " " << vertices[mi][1] << "\n";
  cout << vertices[mj][0] << " " << vertices[mj][1] << "\n";

  cout << "Largest rectangle area: " << maxArea << endl;
  return 0;
}
