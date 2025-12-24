#pragma once
#include <array>
#include <string>
#include <vector>

using namespace std;

namespace aoc {
struct Direction {
  int i;
  int j;

  constexpr Direction operator+(const Direction &d) const {
    return {i + d.i, j + d.j};
  }

  constexpr Direction operator-(const Direction &d) const {
    return {i - d.i, j - d.j};
  }

  constexpr Direction &operator+=(const Direction &d) {
    i += d.i;
    j += d.j;
    return *this;
  }

  constexpr Direction &operator-=(const Direction &d) {
    i -= d.i;
    j -= d.j;
    return *this;
  }
};

struct Position {
  int i;
  int j;

  constexpr Position operator+(const Direction &d) const {
    return {i + d.i, j + d.j};
  }

  constexpr Position operator-(const Direction &d) const {
    return {i - d.i, j - d.j};
  }

  constexpr Position &operator+=(const Direction &d) {
    i += d.i;
    j += d.j;
    return *this;
  }

  constexpr Position &operator-=(const Direction &d) {
    i -= d.i;
    j -= d.j;
    return *this;
  }

  constexpr bool operator==(const Position &p) { return i == p.i && j == p.j; }
};

constexpr Direction kUp{-1, 0};
constexpr Direction kRight{0, 1};
constexpr Direction kDown{1, 0};
constexpr Direction kLeft{0, -1};
constexpr Direction kUpRight{kUp + kRight};
constexpr Direction kDownRight{kDown + kRight};
constexpr Direction kDownLeft{kDown + kLeft};
constexpr Direction kUpLeft{kUp + kLeft};

constexpr array<Direction, 4> kDirections4{{kUp, kRight, kDown, kLeft}};
constexpr array<Direction, 8> kDirections8{
    {kUp, kUpRight, kRight, kDownRight, kDown, kDownLeft, kLeft, kUpLeft}};

bool inBounds(const vector<string> &graph, const Position &pos);
int getNumEightConnected(const vector<string> &graph, const Position &pos,
                         char symbol);
} // namespace aoc
