#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Direction {
  int i, j;

  bool operator==(const Direction &other) const {
    return i == other.i && j == other.j;
  }
  bool operator<(const Direction &other) const {
    return i < other.i || (i == other.i && j < other.j);
  }

  Direction operator+(const Direction &other) const {
    return {i + other.i, j + other.j};
  }
};

struct State {
  Direction pos;
  bool cheated;

  bool operator==(const State &other) const {
    return pos == other.pos && cheated == other.cheated;
  }

  bool operator<(const State &other) const {
    return pos < other.pos || (pos == other.pos && cheated < other.cheated);
  }
};

namespace std {
template <> struct hash<Direction> {
  size_t operator()(const Direction &obj) const {
    size_t h1 = std::hash<int>{}(obj.i);
    size_t h2 = std::hash<int>{}(obj.j);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std

namespace std {
template <> struct hash<State> {
  size_t operator()(const State &obj) const {
    size_t h1 = std::hash<Direction>{}(obj.pos);
    size_t h2 = std::hash<bool>{}(obj.cheated);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std

const std::array<Direction, 4> kDirections{{{0, 1}, {-1, 0}, {0, -1}, {1, 0}}};
const std::array<Direction, 4> kRadiiDirs{{{1, 1}, {-1, 1}, {-1, -1}, {1, -1}}};

using Cache = std::unordered_map<Direction, int>;
using Track = std::vector<std::string>;
using Pq = std::priority_queue<std::tuple<int, int, State>,
                               std::vector<std::tuple<int, int, State>>,
                               std::greater<>>;
using InPq = Cache;
using Times = std::unordered_map<int, int>;

Track getTrack(std::ifstream &inf, Direction &start, Direction &goal) {
  std::string row;
  Track track;
  int i{0};

  while (inf >> row) {
    size_t pos{row.find('S')};
    if (pos != std::string::npos) {
      start = {i, static_cast<int>(pos)};
      row[pos] = '.';
    }

    pos = row.find('E');
    if (pos != std::string::npos) {
      goal = {i, static_cast<int>(pos)};
      row[pos] = '.';
    }

    track.push_back(row);
    ++i;
  }

  return track;
}

bool inbounds(const Direction &pos, const Track &track) {
  return pos.i >= 0 && pos.i < track.size() && pos.j >= 0 &&
         pos.j < track[pos.i].size();
}

int aStar(const Direction &goal, const Track &track, Pq &pq, InPq &inPq,
          std::unordered_set<Direction> &visited, Cache &cache) {
  if (pq.empty())
    return -1;

  const auto [h, cost, state] = pq.top();
  pq.pop();

  if (state.pos == goal)
    return cost;
  if (cache.contains(state.pos))
    return cost + cache[state.pos];

  visited.insert(state.pos);

  for (const auto &dir : kDirections) {
    const Direction nPos{state.pos + dir};

    if (inbounds(nPos, track) && !visited.contains(nPos) &&
        track[nPos.i][nPos.j] != '#') {
      int heuristic{std::abs(goal.i - nPos.i) + std::abs(goal.j - nPos.j)};
      State nState{nPos, state.cheated};

      if (!inPq.contains(nState.pos) ||
          inPq[nState.pos] > heuristic + cost + 1) {
        pq.push({heuristic + cost + 1, cost + 1, nState});
        inPq[nState.pos] = heuristic + cost + 1;
      }
    }
  }

  int total{aStar(goal, track, pq, inPq, visited, cache)};
  if (!state.cheated)
    cache[state.pos] = total - cost;
  return total;
}

void testCheat(Times &times, const Direction &pos, const int cost,
               const Direction &goal, const Track &track, Cache &cache) {
  if (!inbounds(pos, track) || track[pos.i][pos.j] == '#')
    return;

  Pq pq;
  InPq inPq;
  std::unordered_set<Direction> aVisited;
  pq.push({0, cost, {pos, true}});
  inPq[pos] = 0;
  aVisited.insert(pos);
  int time{aStar(goal, track, pq, inPq, aVisited, cache)};
  ++times[time];
}

Times getTimes(const Direction &start, const Direction &goal,
               const Track &track, Cache &cache, int maxCost = 1e9,
               int radius = 20) {
  Times times;
  int cost{0};
  Direction pos{start};
  std::unordered_set<Direction> visited;

  while (pos != goal &&
         cost + std::abs(pos.i - goal.i) + std::abs(pos.j - goal.j) <=
             maxCost) {
    visited.insert(pos);

    for (int r = 2; r <= radius; ++r) {
      // everything except edges of +
      for (int a = 1; a < r; ++a) {
        int b{r - a};
        for (const auto &dir : kRadiiDirs) {
          Direction nPos{pos.i + a * dir.i, pos.j + b * dir.j};
          testCheat(times, nPos, cost + r, goal, track, cache);
        }
      }

      // edges of +
      for (const auto &dir : kDirections) {
        Direction nPos{pos.i + r * dir.i, pos.j + r * dir.j};
        testCheat(times, nPos, cost + r, goal, track, cache);
      }
    }

    for (const auto &dir : kDirections) {
      if (inbounds(pos + dir, track) && !visited.contains(pos + dir) &&
          track[pos.i + dir.i][pos.j + dir.j] != '#') {
        pos = pos + dir;
        break;
      }
    }
    ++cost;
  }

  return times;
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

  Direction start, goal;
  Track track{getTrack(inf, start, goal)};
  Pq pq;
  InPq inPq;
  Cache cache;
  std::unordered_set<Direction> visited;
  pq.push({0, 0, {start, false}});
  inPq[start] = 0;

  int baseline{aStar(goal, track, pq, inPq, visited, cache)};

  Times times{getTimes(start, goal, track, cache, baseline - 100)};
  long numGoodTimes{0};
  for (const auto [time, num] : times) {
    if (time <= baseline - 100 && time != -1) {
      numGoodTimes += num;
    }
  }

  std::cout << numGoodTimes << " cheats saved at least 100ps\n";

  return 0;
}
