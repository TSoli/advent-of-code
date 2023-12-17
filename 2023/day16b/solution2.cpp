#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

enum class Direction { UP, LEFT, DOWN, RIGHT };

typedef struct Energy {
  long up;
  long down;
  long left;
  long right;
} Energy;

using EnergyMap = std::vector<std::vector<Energy>>;

EnergyMap cache(110, std::vector<Energy>(110));

// what a mess

bool updateEnergyMap(EnergyMap &energyMap, Direction direction, int x, int y,
                     long step) {
  switch (direction) {
  case Direction::UP:
    if (energyMap[y][x].up) {
      return false;
    }
    energyMap[y][x].up = step;
    break;
  case Direction::DOWN:
    if (energyMap[y][x].down) {
      return false;
    }
    energyMap[y][x].down = step;
    break;
  case Direction::LEFT:
    if (energyMap[y][x].left) {
      return false;
    }
    energyMap[y][x].left = step;
    break;
  case Direction::RIGHT:
    if (energyMap[y][x].right) {
      return false;
    }
    energyMap[y][x].right = step;
  }

  return true;
}

void updatePosition(int &x, int &y, const Direction direction) {
  switch (direction) {
  case Direction::UP:
    y--;
    break;
  case Direction::DOWN:
    y++;
    break;
  case Direction::LEFT:
    x--;
    break;
  case Direction::RIGHT:
    x++;
  }
}

bool isEnergised(const int x, const int y, const EnergyMap &energyMap) {
  Energy energy = energyMap[y][x];
  return energy.up || energy.down || energy.left || energy.right;
}

long getEnergy(const int x, const int y, long step, Direction direction,
               const std::vector<std::string> &map, EnergyMap &energyMap) {
  if (x >= map[y].size() || y >= map.size() || x < 0 || y < 0) {
    return 0;
  }

  bool wasEnergised{isEnergised(x, y, energyMap)};

  if (!updateEnergyMap(energyMap, direction, x, y, step)) {
    // found a loop
    switch (direction) {
    case Direction::UP:
      return step - energyMap[y][x].up - 2;
    case Direction::DOWN:
      return step - energyMap[y][x].down - 2;
    case Direction::LEFT:
      return step - energyMap[y][x].left - 2;
    case Direction::RIGHT:
      return step - energyMap[y][x].right - 2;
    }
  }

  // check cache
  if (direction == Direction::UP && cache[y][x].up) {
    return !wasEnergised + cache[y][x].up;
  }
  if (direction == Direction::DOWN && cache[y][x].down) {
    return !wasEnergised + cache[y][x].down;
  }
  if (direction == Direction::LEFT && cache[y][x].left) {
    return !wasEnergised + cache[y][x].left;
  }
  if (direction == Direction::RIGHT && cache[y][x].right) {
    return !wasEnergised + cache[y][x].right;
  }

  int secondDirection{-1};
  const Direction originalDirection{direction};

  switch (map[y][x]) {
  case '/':
    switch (direction) {
    case Direction::RIGHT:
    case Direction::LEFT:
      direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
      break;
    default:
      direction = static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
    }
    break;
  case '\\':
    switch (direction) {
    case Direction::LEFT:
    case Direction::RIGHT:
      direction = static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
      break;
    default:
      direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
      break;
    }
    break;
  case '|':
    if (direction == Direction::LEFT || direction == Direction::RIGHT) {
      direction = Direction::UP;
      secondDirection = static_cast<int>(Direction::DOWN);
    }
    break;
  case '-':
    if (direction == Direction::UP || direction == Direction::DOWN) {
      direction = Direction::LEFT;
      secondDirection = static_cast<int>(Direction::RIGHT);
    }
    break;
  }

  int secondDirEnergy{0};
  if (secondDirection != -1) {
    Direction newDir{static_cast<int>(secondDirection)};
    int newX{x};
    int newY{y};
    updatePosition(newX, newY, newDir);
    ++step;
    switch (originalDirection) {
    case Direction::UP:
      secondDirEnergy = getEnergy(newX, newY, step, newDir, map, energyMap);
      break;
    case Direction::DOWN:
      secondDirEnergy = getEnergy(newX, newY, step, newDir, map, energyMap);
      break;
    case Direction::LEFT:
      secondDirEnergy = getEnergy(newX, newY, step, newDir, map, energyMap);
      break;
    case Direction::RIGHT:
      secondDirEnergy = getEnergy(newX, newY, step, newDir, map, energyMap);
      break;
    }
  }

  int newX{x};
  int newY{y};
  Direction newDir{direction};
  updatePosition(newX, newY, newDir);
  switch (originalDirection) {
  case Direction::UP:
    cache[y][x].up =
        secondDirEnergy + getEnergy(newX, newY, step, newDir, map, energyMap);
    return !wasEnergised + cache[y][x].up;
  case Direction::DOWN:
    cache[y][x].down =
        secondDirEnergy + getEnergy(newX, newY, step, newDir, map, energyMap);
    return !wasEnergised + cache[y][x].down;
  case Direction::LEFT:
    cache[y][x].left =
        secondDirEnergy + getEnergy(newX, newY, step, newDir, map, energyMap);
    return !wasEnergised + cache[y][x].left;
  case Direction::RIGHT:
    cache[y][x].right =
        secondDirEnergy + getEnergy(newX, newY, step, newDir, map, energyMap);
    return !wasEnergised + cache[y][x].right;
  }

  std::cerr << "This shouldn't happen!\n";
  return 0;
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

  long maxEnergy{0};
  int x{0};
  int y{0};
  long step{1};
  Direction direction{Direction::RIGHT};

  for (y = 0; y < map.size(); ++y) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    long energy{getEnergy(x, y, step, direction, map, energyMap)};
    maxEnergy = std::max(maxEnergy, energy);
  }

  x = map[0].size() - 1;
  direction = Direction::LEFT;
  for (y = 0; y < map.size(); ++y) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    long energy{getEnergy(x, y, step, direction, map, energyMap)};
    maxEnergy = std::max(maxEnergy, energy);
  }

  y = 0;
  direction = Direction::DOWN;
  for (x = 0; x < map.size(); ++x) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    long energy{getEnergy(x, y, step, direction, map, energyMap)};
    maxEnergy = std::max(maxEnergy, energy);
  }

  y = map.size() - 1;
  direction = Direction::UP;
  for (x = 0; x < map.size(); ++x) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    long energy{getEnergy(x, y, step, direction, map, energyMap)};
    maxEnergy = std::max(maxEnergy, energy);
  }

  // std::cout << "cache up:\n";
  // for (auto &row : cache) {
  //   for (auto c : row) {
  //     std::cout << c.up << "|";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "cache down:\n";
  // for (auto &row : cache) {
  //   for (auto c : row) {
  //     std::cout << c.down << "|";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "cache left:\n";
  // for (auto &row : cache) {
  //   for (auto c : row) {
  //     std::cout << c.left << "|";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "cache right:\n";
  // for (auto &row : cache) {
  //   for (auto c : row) {
  //     std::cout << c.right << "|";
  //   }
  //   std::cout << "\n";
  // }

  std::cout << "Total Energy: " << maxEnergy << "\n";

  return 0;
}
