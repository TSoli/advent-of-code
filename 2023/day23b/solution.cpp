#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Pos = std::array<int, 2>;

struct Edge {
  int cost;
  int id;
  Pos pos;
};

struct Junction {
  int id;
  Pos pos;
  std::vector<Edge> edges;
};

template <>
struct std::hash<Pos> {
  size_t operator()(const Pos &s) const noexcept {
    size_t h1 = std::hash<int>{}(s[0]);
    size_t h2 = std::hash<int>{}(s[1]);
    return h1 ^ (h2 << 1);
  }
};

const std::array<Pos, 4> directions{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

bool isJunction(const Pos &pos, const std::vector<std::string> &map) {
  if (map[pos[0]][pos[1]] == '#') {
    return false;
  }

  int numPaths{0};
  for (const auto &direction : directions) {
    Pos newPos{pos[0] + direction[0], pos[1] + direction[1]};
    if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
        newPos[1] > map.front().size() - 1) {
      continue;
    }

    if (map[newPos[0]][newPos[1]] != '#') {
      ++numPaths;
    }
  }

  return numPaths > 2;
}

Edge getEdge(Pos pos, const Pos &startPos, const Pos &endPos,
             const std::vector<std::string> &map,
             const std::unordered_map<Pos, Junction> &junctions,
             std::unordered_set<Pos> &visited) {
  int cost{1};
  int id{-1};
  while (!isJunction(pos, map)) {
    if (pos == startPos) {
      return {cost, 0, pos};
    }
    if (pos == endPos) {
      return {cost, 63, pos};
    }

    visited.insert(pos);

    bool isDeadEnd{true};
    for (const auto &direction : directions) {
      Pos newPos{pos[0] + direction[0], pos[1] + direction[1]};
      if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
          newPos[1] > map.front().size() - 1 ||
          map[newPos[0]][newPos[1]] == '#' || visited.contains(newPos)) {
        continue;
      }

      pos = newPos;
      ++cost;

      isDeadEnd = false;
      break;
    }

    if (isDeadEnd) {
      return {0, -1, pos};
    }
  }

  id = junctions.at(pos).id;
  return {cost, id, pos};
}

std::vector<Edge> getEdges(const Pos &pos, const Pos &startPos,
                           const Pos &endPos,
                           const std::vector<std::string> &map,
                           const std::unordered_map<Pos, Junction> &junctions) {
  std::vector<Edge> edges;

  for (const auto &direction : directions) {
    Pos newPos{pos[0] + direction[0], pos[1] + direction[1]};
    if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
        newPos[1] > map.front().size() - 1 ||
        map[newPos[0]][newPos[1]] == '#') {
      continue;
    }

    std::unordered_set<Pos> visited;
    visited.insert(pos);
    Edge edge{getEdge(newPos, startPos, endPos, map, junctions, visited)};
    if (edge.cost != 0) {
      edges.push_back(edge);
    }
  }

  return edges;
}

std::unordered_map<Pos, Junction> getJunctions(
    const std::vector<std::string> &map, const Pos &startPos,
    const Pos &endPos) {
  int id{1};
  std::unordered_map<Pos, Junction> junctions;

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      if (isJunction({i, j}, map)) {
        junctions[{i, j}] = {id, {i, j}, {}};
        ++id;
      } else if (Pos{i, j} == startPos) {
        junctions[{i, j}] = {0, {i, j}, {}};
      } else if (Pos{i, j} == endPos) {
        junctions[{i, j}] = {63, {i, j}, {}};
      }
    }
  }

  for (auto &[pos, junction] : junctions) {
    junction.edges = getEdges(junction.pos, startPos, endPos, map, junctions);
  }

  return junctions;
}

void getLongestPath(const Pos &startPos, int currSteps,
                    std::unordered_map<Pos, Junction> &junctions,
                    unsigned long visited, std::vector<int> &maxDistances) {
  int id{junctions.at(startPos).id};
  visited |= (1L << id);
  maxDistances[id] = std::max(currSteps, maxDistances[id]);

  for (const auto &edge : junctions.at(startPos).edges) {
    if ((visited & (1L << edge.id)) == 0) {
      getLongestPath(edge.pos, currSteps + edge.cost, junctions, visited,
                     maxDistances);
    }
  }
}

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

  std::vector<std::string> map;
  std::string line;
  while (inf >> line) {
    map.push_back(line);
  }

  int i{0};
  while (i < map[0].size()) {
    if (map[0][i] == '.') {
      break;
    }
    ++i;
  }

  Pos startPos{0, i};

  i = 0;
  while (i < map.back().size()) {
    if (map.back()[i] == '.') {
      break;
    }
    ++i;
  }

  Pos endPos{static_cast<int>(map.size()) - 1, i};

  std::unordered_map<Pos, Junction> junctions{
      getJunctions(map, startPos, endPos)};

  int currSteps{0};
  std::vector<int> maxDistances(64);
  getLongestPath(startPos, currSteps, junctions, 0, maxDistances);

  std::cout << "Longest path distance: " << maxDistances[63] << "\n";

  return 0;
}
