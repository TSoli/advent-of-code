#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// This code is digusting :(

using Grid = std::vector<std::string>;

struct Direction {
  int i, j;

  bool operator<(const Direction &other) const {
    if (i != other.i) return i < other.i;
    return j < other.j;
  }
};

const std::array<Direction, 4> kDirections{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

int getDirIdx(int i, int j) {
  if (i == 0 && j == 1) return 0;
  if (i == 1 && j == 0) return 1;
  if (i == 0 && j == -1) return 2;
  return 3;
}

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
        std::pair<long, std::tuple<Direction, Direction, Direction>>,
        std::vector<
            std::pair<long, std::tuple<Direction, Direction, Direction>>>,
        std::greater<>>
        pq;

    pq.push({0, {start, kDirections[0], start}});
    // should use an unordered_set to avoid duplicates
    std::unordered_map<int, std::vector<std::pair<Direction, Direction>>>
        visited;
    while (!pq.empty()) {
      auto [distance, posPrevDirPrev] = pq.top();
      auto [pos, prevDir, prev] = posPrevDirPrev;
      // int prevDirIdx{getDirIdx(prevDir.i, prevDir.j)};
      int dirIdx{getDirIdx(sgn(pos.i - prev.i), sgn(pos.j - prev.j))};
      pq.pop();

      if (visited.contains(pos.i * jSize * kDirections.size() +
                           pos.j * kDirections.size() + dirIdx))
        continue;

      visited[pos.i * jSize * kDirections.size() + pos.j * kDirections.size() +
              dirIdx]
          .push_back({prev, prevDir});

      auto [cDist, cPosDirPrev] = pq.top();
      auto [cPos, cPrevDir, cPrev] = cPosDirPrev;
      int cDirIdx{getDirIdx(sgn(cPos.i - cPrev.i), sgn(cPos.j - cPrev.j))};
      while (cPos.i == pos.i && cPos.j == pos.j && cDist == distance &&
             cDirIdx == dirIdx && !pq.empty()) {
        visited[pos.i * jSize * kDirections.size() +
                pos.j * kDirections.size() + cDirIdx]
            .push_back({cPrev, cPrevDir});
        pq.pop();
        std::tie(cDist, cPosDirPrev) = pq.top();
        std::tie(cPos, cPrevDir, cPrev) = cPosDirPrev;
        cDirIdx = getDirIdx(sgn(cPos.i - cPrev.i), sgn(cPos.j - cPrev.j));
      }

      if (pos.i == goal.i && pos.j == goal.j) {
        std::unordered_set<int> spots;
        getBestPositions(visited, goal, dirIdx, spots);
        return spots.size();
      }

      std::vector<Direction> neighbours{nodes[pos.i * jSize + pos.j]};

      for (const auto &n : neighbours) {
        int nDirIdx{getDirIdx(sgn(n.i - pos.i), sgn(n.j - pos.j))};
        bool willTurn{nDirIdx != dirIdx};

        if (visited.contains(n.i * jSize * kDirections.size() +
                             n.j * kDirections.size() + nDirIdx))
          continue;

        long nDistance{std::abs(n.i - pos.i + n.j - pos.j) + 1000 * willTurn +
                       distance};
        pq.push({nDistance, {n, kDirections[dirIdx], pos}});
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

  void getBestPositions(
      const std::unordered_map<
          int, std::vector<std::pair<Direction, Direction>>> &paths,
      const Direction &endNode, int dirIdx, std::unordered_set<int> &visited) {
    if (endNode.i == start.i && endNode.j == start.j) {
      visited.insert(start.i * jSize + start.j);
      return;
    }

    for (const auto &[prev, prevDir] :
         paths.at(endNode.i * jSize * kDirections.size() +
                  endNode.j * kDirections.size() + dirIdx)) {
      Direction dir{sgn(prev.i - endNode.i), sgn(prev.j - endNode.j)};
      int i{endNode.i};
      int j{endNode.j};
      while (prev.i != i || prev.j != j) {
        visited.insert(i * jSize + j);
        i += dir.i;
        j += dir.j;
      }

      int prevDirIdx{getDirIdx(prevDir.i, prevDir.j)};
      getBestPositions(paths, prev, prevDirIdx, visited);
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

  std::cout << "Total good spots: " << maze.dijkstra() << "\n";

  return 0;
}
