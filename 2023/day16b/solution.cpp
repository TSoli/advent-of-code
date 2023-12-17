#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

enum class Direction { UP, LEFT, DOWN, RIGHT };

typedef struct Energy {
  int up;
  int down;
  int left;
  int right;
} Energy;

using EnergyMap = std::vector<std::vector<Energy>>;

// fuck it brute force
bool updateEnergyMap(EnergyMap &energyMap, Direction direction, int x, int y) {
  switch (direction) {
    case Direction::UP:
      if (energyMap[y][x].up) {
        return true;
      }
      energyMap[y][x].up++;
      break;
    case Direction::DOWN:
      if (energyMap[y][x].down) {
        return true;
      }
      energyMap[y][x].down++;
      break;
    case Direction::LEFT:
      if (energyMap[y][x].left) {
        return true;
      }
      energyMap[y][x].left++;
      break;
    case Direction::RIGHT:
      if (energyMap[y][x].right) {
        return true;
      }
      energyMap[y][x].right++;
  }

  return false;
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

void updatePath(int x, int y, Direction direction,
                const std::vector<std::string> &map, EnergyMap &energyMap) {
  if (x >= map[y].size() || y >= map.size() || x < 0 || y < 0) {
    return;
  }

  if (updateEnergyMap(energyMap, direction, x, y)) {
    return;
  }

  int secondDirection{-1};

  switch (map[y][x]) {
    case '/':
      switch (direction) {
        case Direction::RIGHT:
        case Direction::LEFT:
          direction =
              static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
          break;
        default:
          direction =
              static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
      }
      break;
    case '\\':
      switch (direction) {
        case Direction::LEFT:
        case Direction::RIGHT:
          direction =
              static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
          break;
        default:
          direction =
              static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
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

  if (secondDirection != -1) {
    Direction newDir{static_cast<int>(secondDirection)};
    int newX{x};
    int newY{y};
    updatePosition(newX, newY, newDir);
    updatePath(newX, newY, newDir, map, energyMap);
  }

  updatePosition(x, y, direction);
  updatePath(x, y, direction, map, energyMap);
}

int getEnergy(const EnergyMap &energyMap) {
  int totalEnergy{0};
  for (const auto &row : energyMap) {
    for (const auto energy : row) {
      if (energy.up || energy.down || energy.left || energy.right) {
        totalEnergy++;
      }
    }
  }

  return totalEnergy;
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

  int maxEnergy{0};
  // starting position
  int x{0};
  int y{0};

  Direction direction{Direction::RIGHT};
  for (y = 0; y < map.size(); ++y) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    updatePath(x, y, direction, map, energyMap);
    maxEnergy = std::max(maxEnergy, getEnergy(energyMap));
  }

  direction = Direction::LEFT;
  x = map[0].size() - 1;
  for (y = 0; y < map.size(); ++y) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    updatePath(x, y, direction, map, energyMap);
    maxEnergy = std::max(maxEnergy, getEnergy(energyMap));
  }

  direction = Direction::DOWN;
  y = 0;
  for (x = 0; x < map[0].size(); ++x) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    updatePath(x, y, direction, map, energyMap);
    maxEnergy = std::max(maxEnergy, getEnergy(energyMap));
  }

  direction = Direction::UP;
  y = map.size() - 1;
  for (x = 0; x < map[0].size(); ++x) {
    EnergyMap energyMap(map.size(), std::vector<Energy>(map.front().size()));
    updatePath(x, y, direction, map, energyMap);
    maxEnergy = std::max(maxEnergy, getEnergy(energyMap));
  }

  std::cout << "Total Energy: " << maxEnergy << "\n";

  return 0;
}
