#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct Move {
  int x;
  int y;
} Move;

std::vector<std::string> getMap(std::ifstream &inf) {
  std::vector<std::string> map;
  std::string line;
  while (inf >> line) {
    map.push_back(line);
  }

  return map;
}

int getLoad(const std::vector<std::string> &map) {
  int load{0};
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == 'O') {
        load += map.size() - i;
      }
    }
  }
  return load;
}

void moveRocks(std::vector<std::string> &map, const Move &move) {
  // wish I knew how to generalise this with iterators etc
  if (move.x == 1) {
    std::vector<int> maxPos(map.front().size(), map.size() - 1);
    for (int i = map.size() - 1; i >= 0; --i) {
      for (int j = 0; j < map[i].size(); j++) {
        char square{map[i][j]};
        switch (square) {
          case '#':
            maxPos[j] = i - 1;
            break;
          case 'O':
            map[i][j] = '.';
            map[maxPos[j]][j] = 'O';
            maxPos[j]--;
            break;
        }
      }
    }
  } else if (move.x == -1) {
    std::vector<int> maxPos(map.front().size(), 0);
    for (int i = 0; i < map.size(); ++i) {
      for (int j = 0; j < map[i].size(); j++) {
        char square{map[i][j]};
        switch (square) {
          case '#':
            maxPos[j] = i + 1;
            break;
          case 'O':
            map[i][j] = '.';
            map[maxPos[j]][j] = 'O';
            maxPos[j]++;
            break;
        }
      }
    }
  } else if (move.y == 1) {
    std::vector<int> maxPos(map.size(), map.front().size() - 1);
    for (int j = map.front().size() - 1; j >= 0; --j) {
      for (int i = 0; i < map.size(); ++i) {
        char square{map[i][j]};
        switch (square) {
          case '#':
            maxPos[i] = j - 1;
            break;
          case 'O':
            map[i][j] = '.';
            map[i][maxPos[i]] = 'O';
            maxPos[i]--;
            break;
        }
      }
    }
  } else {
    // move.y == -1
    std::vector<int> maxPos(map.size(), 0);
    for (int j = 0; j < map.front().size(); ++j) {
      for (int i = 0; i < map.size(); ++i) {
        char square{map[i][j]};
        switch (square) {
          case '#':
            maxPos[i] = j + 1;
            break;
          case 'O':
            map[i][j] = '.';
            map[i][maxPos[i]] = 'O';
            maxPos[i]++;
            break;
        }
      }
    }
  }
}

void findCycles(const std::vector<std::vector<std::string>> &history,
                int &cycleLength, int &offset) {
  static std::unordered_map<std::string, int> stateHistory;
  std::string state;
  for (auto h : history.back()) {
    state += h;
  }

  if (stateHistory.contains(state)) {
    cycleLength = history.size() - 1 - stateHistory[state];
    offset = stateHistory[state];
  } else {
    stateHistory[state] = history.size() - 1;
  }
}

void spin(std::vector<std::string> &map,
          std::vector<std::vector<std::string>> &history, int &cycleLength,
          int &offset, int cycles) {
  const std::array<Move, 4> moves{{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};

  for (int i = 0; i < cycles; ++i) {
    std::cout << "Done " << i << "\n";
    history.push_back(map);
    findCycles(history, cycleLength, offset);
    if (cycleLength) {
      return;
    }

    for (auto &m : moves) {
      moveRocks(map, m);
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

  std::vector<std::string> map{getMap(inf)};
  std::vector<std::vector<std::string>> history;
  int cycleLength{0};
  int offset{0};
  int cycles{1000000000};
  spin(map, history, cycleLength, offset, cycles);
  map = history[(cycles - offset) % cycleLength + offset];

  std::cout << "Load: " << getLoad(map) << "\n";

  return 0;
}
