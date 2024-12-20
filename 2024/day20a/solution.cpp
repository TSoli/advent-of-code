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

using Cache = std::unordered_map<State, int>;
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
          std::unordered_set<State> &visited, Cache &cache) {
  if (pq.empty())
    return -1;

  const auto [h, cost, state] = pq.top();
  pq.pop();

  if (state.pos == goal)
    return cost;
  if (cache.contains(state))
    return cost + cache[state];

  visited.insert(state);

  for (const auto &dir : kDirections) {
    const Direction nPos{state.pos + dir};

    if (inbounds(nPos, track) && !visited.contains({nPos, state.cheated}) &&
        track[nPos.i][nPos.j] != '#') {
      int heuristic{std::abs(goal.i - nPos.i) + std::abs(goal.j - nPos.j)};
      State nState{nPos, state.cheated};

      if (!inPq.contains(nState) || inPq[nState] > heuristic + cost + 1) {
        pq.push({heuristic + cost + 1, cost + 1, nState});
        inPq[nState] = heuristic + cost + 1;
      }
    }

    if (!state.cheated) {
      for (const auto &dir : kDirections) {
        const Direction nnPos{nPos + dir};

        if (inbounds(nnPos, track) && !visited.contains({nnPos, true}) &&
            track[nnPos.i][nnPos.j] != '#') {
          int heuristic{std::abs(goal.i - nnPos.i) +
                        std::abs(goal.j - nnPos.j)};
          State nState{nnPos, true};

          if (!inPq.contains(nState) || inPq[nState] > heuristic + cost + 2) {
            pq.push({heuristic + cost + 2, cost + 2, nState});
            inPq[nState] = heuristic + cost + 2;
          }
        }
      }
    }
  }

  cache[state] = aStar(goal, track, pq, inPq, visited, cache) - cost;
  return cache[state] + cost;
}

Times getTimes(const Direction &start, const Direction &goal,
               const Track &track, Cache &cache) {
  Times times;
  int cost{0};
  Direction pos{start};
  std::unordered_set<Direction> visited;

  while (pos != goal) {
    visited.insert(pos);
    Direction nextPos;
    for (const auto &dir : kDirections) {
      Direction nPos{pos + dir};

      if (!inbounds(nPos, track))
        continue;

      if (track[nPos.i][nPos.j] == '#') {
        for (const auto &dir : kDirections) {
          Direction nnPos{nPos + dir};
          if (inbounds(nnPos, track) && track[nnPos.i][nnPos.j] != '#') {
            Pq pq;
            InPq inPq;
            std::unordered_set<State> aVisited;
            pq.push({0, cost + 2, {nnPos, true}});
            inPq[{nnPos, true}] = 0;
            aVisited.insert({nnPos, true});
            int time{aStar(goal, track, pq, inPq, aVisited, cache)};
            ++times[time];
          }
        }
      } else if (!visited.contains(nPos)) {
        nextPos = nPos;
      }
    }

    pos = nextPos;
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
  std::unordered_set<State> visited;
  pq.push({0, 0, {start, true}});
  inPq[{start, true}] = 0;

  int baseline{aStar(goal, track, pq, inPq, visited, cache)};

  Times times{getTimes(start, goal, track, cache)};
  long numGoodTimes{0};
  for (const auto [time, num] : times) {
    if (time <= baseline - 100 && time != -1) {
      numGoodTimes += num;
    }
  }

  std::cout << numGoodTimes << " cheats saved at least 100ps\n";

  return 0;
}
