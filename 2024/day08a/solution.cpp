#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Nodes = std::unordered_map<char, std::vector<std::pair<int, int>>>;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: solution.out <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  int i{0};
  int numRows{0};
  std::string row;
  Nodes nodes;

  while (std::getline(inf, row)) {
    numRows = row.length();
    for (int j = 0; j < row.length(); ++j) {
      if (std::isdigit(row[j]) || std::isalpha(row[j]))
        nodes[row[j]].push_back({i, j});
    }
    ++i;
  }

  int numCols{i};

  std::unordered_set<int> antinodes;
  for (const auto &[c, ns] : nodes) {
    for (int i = 0; i < ns.size(); ++i) {
      for (int j = i + 1; j < ns.size(); ++j) {
        int dy{ns[j].first - ns[i].first};
        int dx{ns[j].second - ns[i].second};
        int y1{ns[i].first - dy};
        int x1{ns[i].second - dx};
        int y2{ns[j].first + dy};
        int x2{ns[j].second + dx};

        if (x1 >= 0 && x1 < numCols && y1 >= 0 && y1 < numRows) {
          antinodes.insert(y1 * numCols + x1);
        }

        if (x2 >= 0 && x2 < numCols && y2 >= 0 && y2 < numRows) {
          antinodes.insert(y2 * numCols + x2);
        }
      }
    }
  }

  std::cout << "Number of antinodes: " << antinodes.size() << std::endl;

  return 0;
}
