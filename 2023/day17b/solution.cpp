#include <array>
#include <fstream>
#include <ios>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

using Pos = std::array<int, 2>;

enum class Direction { NONE = -1, UP, LEFT, DOWN, RIGHT };

struct State {
  Pos pos;
  Direction direction;
  int steps;
  int cost;
};

using Path = std::vector<Pos>;

const auto stateCmp = [](const State &a, const State &b) {
  if (a.cost == b.cost) {
    return a.steps > b.steps;
  }
  return a.cost > b.cost;
};

std::string getStateStr(State state) {
  std::string posStr{std::to_string(state.pos[0]) + ',' +
                     std::to_string(state.pos[1] + ',')};
  std::string dirStr{std::to_string(static_cast<int>(state.direction)) + ','};
  std::string stepStr{std::to_string(state.steps)};
  return posStr + dirStr + stepStr;
}

void updateNeighbours(const State &currState,
                      const std::vector<std::string> &map,
                      std::priority_queue<State, std::vector<State>,
                                          decltype(stateCmp)> &neighbours,
                      std::unordered_set<std::string> &visited) {
  switch (currState.direction) {
  case Direction::UP:
    // up
    if (currState.steps < 10 && currState.pos[0] > 0) {
      Pos nextPos{currState.pos[0] - 1, currState.pos[1]};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, currState.direction, currState.steps + 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // left
    if (currState.pos[1] > 0 && currState.steps > 3) {
      Pos nextPos{currState.pos[0], currState.pos[1] - 1};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::LEFT, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // right
    if (currState.pos[1] < map[currState.pos[0]].size() - 1 &&
        currState.steps > 3) {
      Pos nextPos{currState.pos[0], currState.pos[1] + 1};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::RIGHT, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }
    break;

  case Direction::LEFT:
    // left
    if (currState.steps < 10 && currState.pos[1] > 0) {
      Pos nextPos{currState.pos[0], currState.pos[1] - 1};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, currState.direction, currState.steps + 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // up
    if (currState.pos[0] > 0 && currState.steps > 3) {
      Pos nextPos{currState.pos[0] - 1, currState.pos[1]};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::UP, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // down
    if (currState.pos[0] < map.size() - 1 && currState.steps > 3) {
      Pos nextPos{currState.pos[0] + 1, currState.pos[1]};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::DOWN, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }
    break;

  case Direction::DOWN:
    // down
    if (currState.steps < 10 && currState.pos[0] < map.size() - 1) {
      Pos nextPos{currState.pos[0] + 1, currState.pos[1]};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, currState.direction, currState.steps + 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // left
    if (currState.pos[1] > 0 && currState.steps > 3) {
      Pos nextPos{currState.pos[0], currState.pos[1] - 1};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::LEFT, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // right
    if (currState.pos[1] < map[currState.pos[0]].size() - 1 &&
        currState.steps > 3) {
      Pos nextPos{currState.pos[0], currState.pos[1] + 1};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::RIGHT, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }
    break;

  case Direction::RIGHT:
    // right
    if (currState.steps < 10 &&
        currState.pos[1] < map[currState.pos[0]].size() - 1) {
      Pos nextPos{currState.pos[0], currState.pos[1] + 1};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, currState.direction, currState.steps + 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // up
    if (currState.pos[0] > 0 && currState.steps > 3) {
      Pos nextPos{currState.pos[0] - 1, currState.pos[1]};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::UP, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }

    // down
    if (currState.pos[0] < map.size() - 1 && currState.steps > 3) {
      Pos nextPos{currState.pos[0] + 1, currState.pos[1]};
      int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
      State nextState{nextPos, Direction::DOWN, 1, cost};
      if (!visited.contains(getStateStr(nextState))) {
        neighbours.push(nextState);
      }
    }
    break;
  default:
    // none assume at 0, 0
    Pos nextPos{currState.pos[0] + 1, currState.pos[1]};
    int cost{currState.cost + map[nextPos[0]][nextPos[1]] - '0'};
    State nextState{nextPos, Direction::DOWN, 1, cost};
    neighbours.push(nextState);

    nextPos = {currState.pos[0], currState.pos[1] + 1};
    cost = currState.cost + map[nextPos[0]][nextPos[1]] - '0';
    State nextState2 = {nextPos, Direction::RIGHT, 1, cost};
    neighbours.push(nextState2);
  }
}

int shortestPath(const State &startState, const Pos &target,
                 const std::vector<std::string> &map,
                 std::priority_queue<State, std::vector<State>,
                                     decltype(stateCmp)> &neighbours,
                 std::unordered_set<std::string> &visited) {
  State currState{startState};
  while (currState.pos != target) {
    visited.insert(getStateStr(currState));
    updateNeighbours(currState, map, neighbours, visited);
    currState = neighbours.top();
    neighbours.pop();
    while (visited.contains(getStateStr(currState))) {
      currState = neighbours.top();
      neighbours.pop();
    }
  }

  std::cout << "Found target!\n";
  return currState.cost;
}

int getPathCost(const Path &path, const std::vector<std::string> &map) {
  int cost{0};
  for (auto &pos : path) {
    cost += map[pos[0]][pos[1]] - '0';
  }

  return cost;
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

  Pos startPos{0, 0};
  Pos target{static_cast<int>(map.size() - 1),
             static_cast<int>(map.back().size() - 1)};
  State currState{startPos, Direction::NONE, 0, 0};
  std::priority_queue<State, std::vector<State>, decltype(stateCmp)> neighbours;
  std::unordered_set<std::string> visited;
  std::cout << "SP: "
            << shortestPath(currState, target, map, neighbours, visited)
            << "\n";

  return 0;
}
