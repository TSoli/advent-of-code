#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Pos = std::array<int, 2>;

template <> struct std::hash<Pos> {
  size_t operator()(const Pos &s) const noexcept {
    size_t h1 = std::hash<int>{}(s[0]);
    size_t h2 = std::hash<int>{}(s[1]);
    return h1 ^ (h2 << 1);
  }
};

Pos getMap(std::ifstream &inf, std::vector<std::string> &map) {
  std::string line;
  Pos pos;

  int row{0};
  while (inf >> line) {
    map.push_back(line);
    size_t startCol{line.find('S')};
    if (startCol != std::string::npos) {
      pos[0] = row;
      pos[1] = startCol;
      map[row][startCol] = '.';
    }
    ++row;
  }

  return pos;
}

void bfs(std::unordered_set<Pos> &startPositions,
         std::unordered_set<Pos> &visited, std::vector<std::string> &map) {
  std::unordered_set<Pos> nextPositions;

  int iSize{static_cast<int>(map.size())};
  int jSize{static_cast<int>(map.front().size())};

  for (const auto &pos : startPositions) {
    std::array<Pos, 4> deltas{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
    for (const auto &delta : deltas) {
      Pos checkPos{(((pos[0] + delta[0]) % iSize) + iSize) % iSize,
                   (((pos[1] + delta[1]) % jSize) + jSize) % jSize};
      if (map[checkPos[0]][checkPos[1]] != '#' &&
          !visited.contains({pos[0] + delta[0], pos[1] + delta[1]})) {
        nextPositions.insert({pos[0] + delta[0], pos[1] + delta[1]});
        visited.insert({pos[0] + delta[0], pos[1] + delta[1]});
      }
    }
  }

  startPositions = nextPositions;
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
  Pos startPos{getMap(inf, map)};
  std::unordered_set<Pos> startPositions;
  startPositions.insert(startPos);
  std::unordered_set<Pos> visited;
  visited.insert(startPos);
  std::unordered_map<Pos, int> offsets;
  offsets[startPos] = 0;

  int steps{26501365};
  // it's quadratic because the middle row/column is empty so you can get to
  // the same start position by moving exactly the length of the map
  // From there you can go the same distance as found with bfs to get to
  // identical position but in a repeated board
  //
  // Since you can go up or right (or down or left) this creates a square number
  // of repeated boards that you can reach
  //
  // Therefore after n steps we could reach all the same positions as walking
  // n mod(board_len) steps.
  //
  // If we note the number of reachable steps each time this happens for the
  // first three times then we can solve the quadratic
  std::vector<int> params;
  std::vector<int> lens;
  lens.push_back(1);

  for (int i = 0; i < steps; ++i) {
    bfs(startPositions, visited, map);
    lens.push_back(startPositions.size());
    // we have done n mod(board_len) steps
    if (i % map.size() == steps % map.size()) {
      int len{0};
      // we want the squares just explored not the ones we are about to explore
      for (int j = 0; j < lens.size() - 1; ++j) {
        // We can find how many others we could get to by noting that you can
        // always get back to the same square in an even number of moves
        //
        // Therefore, in the bfs after n iterations you could also have reached
        // the positions at step i where i % 2 == n % 2. This is faster than
        // repeating already explored squares in the bfs
        if (j % 2 == i % 2) {
          len += lens[j];
        }
      }
      params.push_back(len);
    }

    if (params.size() == 3) {
      break;
    }
  }

  long long p1{params[0]};
  long long p2{params[1] - params[0]};
  long long p3{params[2] - params[1]};
  long long ip{steps / static_cast<long long>(map.size())};

  // solve the quadratic
  long long totalSteps{p1 + p2 * ip + (ip * (ip - 1) / 2) * (p3 - p2)};
  std::cout << "Number of positions: " << totalSteps << "\n";

  return 0;
}
