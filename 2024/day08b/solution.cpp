#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Nodes = std::unordered_map<char, std::vector<std::pair<int, int>>>;

bool inbounds(int x, int bound) { return x >= 0 && x < bound; }

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
        int div{std::gcd(dx, dy)};
        dx /= div;
        dy /= div;

        int y{ns[i].first};
        int x{ns[i].second};
        int xp{x};
        int yp{y};

        while (inbounds(xp, numCols) && inbounds(yp, numRows)) {
          antinodes.insert(yp * numCols + xp);
          xp -= dx;
          yp -= dy;
        }

        xp = x + dx;
        yp = y + dy;
        while (inbounds(xp, numCols) && inbounds(yp, numRows)) {
          antinodes.insert(yp * numCols + xp);
          xp += dx;
          yp += dy;
        }
      }
    }
  }

  std::cout << "Number of antinodes: " << antinodes.size() << std::endl;

  return 0;
}
