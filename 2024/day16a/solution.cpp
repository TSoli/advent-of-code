#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Grid = std::vector<std::string>;

struct Direction {
  int i, j;

  bool operator<(const Direction &other) const {
    if (i != other.i) return i < other.i;
    return j < other.j;
  }
};

const std::array<Direction, 4> kDirections{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

template <typename T>
int sgn(T x) {
  return (x > 0) - (x < 0);
}

class Maze {
 public:
  Maze(const Grid grid, const Direction &start, const Direction &goal)
      : grid(grid),
        iSize(grid.size()),
        jSize(grid[0].size()),
        start(start),
        goal(goal) {
    updateNeighbours(start.i, start.j);
    updateNeighbours(goal.i, goal.j);

    for (int i = 0; i < grid.size(); ++i) {
      for (int j = 0; j < grid[i].size(); ++j) {
        if (grid[i][j] == '#' || (i == start.i && j == start.j) ||
            (i == goal.i && j == goal.j)) {
          continue;
        }

        if (grid[i][j] == '.' && isNode(i, j)) {
          updateNeighbours(i, j);
        }
      }
    }
  }

  long dijkstra() {
    // Yuck!
    std::priority_queue<
        std::pair<long, std::pair<Direction, Direction>>,
        std::vector<std::pair<long, std::pair<Direction, Direction>>>,
        std::greater<>>
        pq;

    long distance{0};
    pq.push({0, {start, kDirections[0]}});
    std::unordered_set<int> visited;
    while (!pq.empty()) {
      std::pair<Direction, Direction> posAndDir{pq.top().second};
      distance = pq.top().first;
      pq.pop();

      Direction pos{posAndDir.first};
      Direction dir{posAndDir.second};
      visited.insert(pos.i * jSize + pos.j);

      if (pos.i == goal.i && pos.j == goal.j) {
        return distance;
      }

      std::vector<Direction> neighbours{nodes[pos.i * jSize + pos.j]};

      for (const auto &n : neighbours) {
        if (visited.contains(n.i * jSize + n.j)) continue;

        bool willTurn{(sgn(dir.i) != sgn(n.i - pos.i)) ||
                      (sgn(dir.j) != sgn(n.j - pos.j))};
        long nDistance{std::abs(n.i - pos.i + n.j - pos.j) + 1000 * willTurn +
                       distance};
        Direction nDir{sgn(n.i - pos.i), sgn(n.j - pos.j)};
        pq.push({nDistance, {n, nDir}});
      }
    }

    return -1;
  }

 private:
  const Grid grid;
  const int iSize, jSize;
  std::unordered_map<int, std::vector<Direction>> nodes;

  const Direction start;
  const Direction goal;

  bool isNode(int i, int j) {
    if ((i == start.i && j == start.j) || (i == goal.i && j == goal.j))
      return true;

    int horizontal{0};
    int vertical{0};
    for (const auto &dir : kDirections) {
      int ni{i + dir.i};
      int nj{j + dir.j};
      if (!inbounds(ni, nj)) continue;

      if (grid[ni][nj] == '.') {
        if (dir.i)
          ++vertical;
        else
          ++horizontal;
      }
    }
    return vertical > 0 && horizontal > 0;
  }

  bool inbounds(int i, int j) {
    return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
  }

  void updateNeighbours(const int i, const int j) {
    for (const auto &dir : kDirections) {
      int ni{i + dir.i};
      int nj{j + dir.j};
      while (inbounds(ni, nj) && grid[ni][nj] != '#') {
        if (isNode(ni, nj)) {
          nodes[i * jSize + j].push_back({ni, nj});
          break;
        }

        ni += dir.i;
        nj += dir.j;
      }
    }
  }
};

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

  std::string line;
  Grid grid;
  Direction start, goal;
  int i{0};
  while (std::getline(inf, line)) {
    size_t startPos{line.find('S')};
    size_t goalPos{line.find('E')};
    if (startPos != std::string::npos) {
      start = {i, static_cast<int>(startPos)};
      line[startPos] = '.';
    }
    if (goalPos != std::string::npos) {
      goal = {i, static_cast<int>(goalPos)};
      line[goalPos] = '.';
    }
    grid.push_back(line);
    ++i;
  }

  Maze maze(grid, start, goal);

  std::cout << "Total distance: " << maze.dijkstra() << "\n";

  return 0;
}
