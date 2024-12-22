#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Position {
  int x, y;

  Position operator-(const Position &other) const {
    return {x - other.x, y - other.y};
  }

  bool operator==(const Position &other) const {
    return x == other.x && y == other.y;
  }
};

using Keypad = std::unordered_map<char, Position>;
using Direction = Position;

struct State {
  Direction dir;
  int depth;
  Position pos;

  bool operator==(const State &other) const {
    return dir == other.dir && depth == other.depth && pos == other.pos;
  }
};

namespace std {
template <> struct hash<State> {
  size_t operator()(const State &obj) const {
    size_t h1 = std::hash<int>{}(obj.dir.x);
    size_t h2 = std::hash<int>{}(obj.dir.y);
    size_t h3 = std::hash<int>{}(obj.depth);
    size_t h4 = std::hash<int>{}(obj.pos.x);
    size_t h5 = std::hash<int>{}(obj.pos.y);
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
  }
};
} // namespace std

using Cache = std::unordered_map<State, long>;

const Keypad kNumericKeypad{{'0', {1, 0}}, {'A', {2, 0}}, {'1', {0, 1}},
                            {'2', {1, 1}}, {'3', {2, 1}}, {'4', {0, 2}},
                            {'5', {1, 2}}, {'6', {2, 2}}, {'7', {0, 3}},
                            {'8', {1, 3}}, {'9', {2, 3}}};

const Keypad kDirectionalKeypad{
    {'<', {0, 0}}, {'v', {1, 0}}, {'>', {2, 0}}, {'^', {1, 1}}, {'A', {2, 1}}};

long getDirectionalLength(const Direction &dir, const long pushes,
                          std::vector<Position> &pos, const int depth,
                          Cache &cache, const bool maxDepth = false) {
  if (depth == 0) {
    return std::abs(dir.x) + std::abs(dir.y) + pushes;
  }

  State state{dir, depth, pos[depth + 1]};
  if (cache.contains(state) && !maxDepth) {
    return cache[state] + pushes;
  }

  char horizontal{dir.x < 0 ? '<' : '>'};
  char vertical{dir.y < 0 ? 'v' : '^'};

  Position nHPos{kDirectionalKeypad.at(horizontal)};
  Position nVPos{kDirectionalKeypad.at(vertical)};

  Direction dx{nHPos - pos[depth]};
  Direction dy{nVPos - pos[depth]};

  std::vector<Position> xPos{pos};
  std::vector<Position> yPos{pos};

  long l1{0};
  long l2{0};
  Direction dyPrev{dy};

  bool illegalX, illegalY;
  if (maxDepth) {
    illegalY = pos[depth + 1].x == 0 && pos[depth + 1].y + dir.y == 0;
    illegalX = pos[depth + 1].y == 0 && pos[depth + 1].x + dir.x == 0;
  } else {
    illegalY = pos[depth + 1].x == 0 && pos[depth + 1].y + dir.y == 1;
    illegalX = pos[depth + 1].y == 1 && pos[depth + 1].x + dir.x == 0;
  }

  Position pf;

  if (!illegalX) {
    if (std::abs(dir.x)) {
      l1 += getDirectionalLength(dx, std::abs(dir.x), xPos, depth - 1, cache);
      xPos[depth] = nHPos;
      dy = dy - dx;
      pf = nHPos;
    }

    if (std::abs(dir.y)) {
      l1 += getDirectionalLength(dy, std::abs(dir.y), xPos, depth - 1, cache);
      xPos[depth] = nVPos;
      pf = nVPos;
    }

    Direction da{kDirectionalKeypad.at('A') - pf};
    if (pushes) {
      l1 += getDirectionalLength(da, pushes, xPos, depth - 1, cache);
      xPos[depth] = kDirectionalKeypad.at('A');
    }
  }

  dy = dyPrev;

  if (!illegalY) {
    if (std::abs(dir.y)) {
      l2 += getDirectionalLength(dy, std::abs(dir.y), yPos, depth - 1, cache);
      yPos[depth] = nVPos;
      pf = nVPos;
      dx = dx - dy;
    }

    if (std::abs(dir.x)) {
      l2 += getDirectionalLength(dx, std::abs(dir.x), yPos, depth - 1, cache);
      yPos[depth] = nHPos;
      pf = nHPos;
    }

    Direction da = kDirectionalKeypad.at('A') - pf;
    if (pushes) {
      l2 += getDirectionalLength(da, pushes, yPos, depth - 1, cache);
      yPos[depth] = kDirectionalKeypad.at('A');
    }
  }

  if (illegalX) {
    pos = yPos;
    cache[state] = l2 - pushes;
    return l2;
  } else if (illegalY) {
    pos = xPos;
    cache[state] = l1 - pushes;
    return l1;
  } else if (l1 < l2) {
    pos = xPos;
    cache[state] = l1 - pushes;
    return l1;
  }

  pos = yPos;
  cache[state] = l2 - pushes;
  return l2;
}

long getCodeLength(const std::string &code, const int depth) {
  std::vector<Position> pos(depth + 2, kDirectionalKeypad.at('A'));
  pos[depth + 1] = kNumericKeypad.at('A');

  long len{0};
  Cache cache;
  for (const auto c : code) {
    Position nPos{kNumericKeypad.at(c)};
    Direction dir{nPos - pos[depth + 1]};

    len += getDirectionalLength(dir, 1, pos, depth, cache, true);
    pos[depth + 1] = nPos;
  }

  return len;
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

  std::vector<std::string> codes;
  std::string line;
  while (inf >> line) {
    codes.push_back(line);
  }

  long complexity{0};
  for (const auto &code : codes) {
    complexity +=
        getCodeLength(code, 25) * std::stoi(code.substr(0, code.find('A')));
  }

  std::cout << "Complexity: " << complexity << "\n";

  return 0;
}
