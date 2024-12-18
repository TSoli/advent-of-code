#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const int kGridSize{71};

struct Position {
  int i, j;

  bool operator==(const Position &other) const {
    return i == other.i && j == other.j;
  }
  bool operator<(const Position &other) const {
    return i < other.i || (i == other.i && j < other.j);
  }
};

const std::array<Position, 4> kDirections{{{0, 1}, {-1, 0}, {0, -1}, {1, 0}}};

bool inbounds(const int i, const int j) {
  return i >= 0 && i < kGridSize && j >= 0 && j < kGridSize;
}

int bfs(const std::unordered_set<int> &corrupted,
        const std::vector<Position> &frontiers,
        std::unordered_set<int> &visited, const Position &goal) {
  std::vector<Position> nextFrontier;
  for (const auto pos : frontiers) {
    visited.insert(pos.i * kGridSize + pos.j);

    if (pos == goal) return 1;
  }

  if (nextFrontier.empty()) return -1;

  return 1 + bfs(corrupted, nextFrontier, visited, goal);
}

int aStar(const Position &start, const Position &goal,
          const std::unordered_set<int> &corrupted) {
  std::unordered_set<int> visited;
  std::priority_queue<std::tuple<int, int, Position>,
                      std::vector<std::tuple<int, int, Position>>,
                      std::greater<>>
      pq;

  std::unordered_map<int, int> inPq;

  pq.push({0, 0, start});
  inPq[0] = 0;
  while (!pq.empty()) {
    const auto [h, cost, pos] = pq.top();
    visited.insert(pos.i * kGridSize + pos.j);

    pq.pop();
    if (pos == goal) return cost;

    for (const auto dir : kDirections) {
      int i{pos.i + dir.i};
      int j{pos.j + dir.j};

      if (inbounds(i, j) && !corrupted.contains(i * kGridSize + j) &&
          !visited.contains(i * kGridSize + j)) {
        int heuristic{std::abs(goal.i - i) + std::abs(goal.j - j)};

        if (!inPq.contains(i * kGridSize + j) ||
            inPq[i * kGridSize + j] > heuristic + cost + 1) {
          pq.push({heuristic + cost + 1, cost + 1, {i, j}});
          inPq[i * kGridSize + j] = heuristic + cost + 1;
        }
      }
    }
  }

  return -1;
}

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

  const int numBytes{1024};
  std::string line;
  std::unordered_set<int> corrupted;
  for (int k = 0; k < numBytes; ++k) {
    std::getline(inf, line);
    size_t sep{line.find(',')};
    int j{std::stoi(line.substr(0, sep))};
    int i{std::stoi(line.substr(sep + 1))};
    corrupted.insert(i * kGridSize + j);
  }

  std::cout << "Shortest path: " << aStar({0, 0}, {70, 70}, corrupted)
            << std::endl;

  return 0;
}
