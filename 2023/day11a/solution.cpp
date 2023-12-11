#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

typedef struct Point {
  int x;
  int y;
} Point;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::string line;
  std::vector<int> colMissingGalaxy;
  std::vector<int> rowMissingGalaxy;
  std::vector<Point> galaxyPositions;
  int row{0};
  while (inf >> line) {
    if (colMissingGalaxy.size() == 0) {
      colMissingGalaxy.resize(line.size());
      std::fill(colMissingGalaxy.begin(), colMissingGalaxy.end(), true);
    }

    auto it{std::ranges::find(line, '#')};
    if (it == line.end()) {
      rowMissingGalaxy.push_back(true);
    } else {
      rowMissingGalaxy.push_back(false);
    }

    while (it != line.end()) {
      int col{static_cast<int>(it - line.begin())};
      colMissingGalaxy[col] = false;
      galaxyPositions.push_back({row, col});
      it = std::find(it + 1, line.end(), '#');
    }

    ++row;
  }

  std::vector<int> cumRows(rowMissingGalaxy.size());
  std::vector<int> cumCols(colMissingGalaxy.size());
  std::partial_sum(rowMissingGalaxy.begin(), rowMissingGalaxy.end(),
                   cumRows.begin());
  std::partial_sum(colMissingGalaxy.begin(), colMissingGalaxy.end(),
                   cumCols.begin());

  long sum{0};
  for (int i = 0; i < galaxyPositions.size(); ++i) {
    for (int j = i + 1; j < galaxyPositions.size(); ++j) {
      int row1{galaxyPositions[i].x};
      int row2{galaxyPositions[j].x};
      int col1{galaxyPositions[i].y};
      int col2{galaxyPositions[j].y};

      sum += std::abs(row2 - row1) + std::abs(col2 - col1);
      sum += std::abs(cumRows[row2] - cumRows[row1]) +
             std::abs(cumCols[col2] - cumCols[col1]);
    }
  }

  std::cout << "Sum of SP: " << sum << "\n";
  return 0;
}
