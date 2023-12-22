#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

struct Support {
  int id;
  std::unordered_set<int> supports;
  std::unordered_set<int> supportedBy;
};

struct Brick {
  std::array<int, 6> coords;
  int id;
};

struct Block {
  int id;
  int height;
};

using Tower = std::vector<std::vector<Block>>;

std::vector<Brick> getBricks(std::ifstream &inf) {
  std::vector<Brick> bricks;

  std::string line;
  int id{1};
  while (inf >> line) {
    Brick brick;
    brick.id = id;
    for (int i = 0; i < brick.coords.size(); ++i) {
      char sep{i == 2 ? '~' : ','};
      brick.coords[i] = std::stoi(line.substr(0, line.find(sep)));
      line = line.substr(line.find(sep) + 1);
    }

    bricks.push_back(brick);
    ++id;
  }

  return bricks;
}

void buildTower(Tower &tower, std::vector<Support> &supports,
                const std::vector<Brick> &bricks) {
  for (const auto &brick : bricks) {
    bool xMatch, yMatch, zMatch;
    xMatch = brick.coords[0] == brick.coords[3];
    yMatch = brick.coords[1] == brick.coords[4];
    zMatch = brick.coords[2] == brick.coords[5];

    std::vector<std::array<int, 3>> coords;

    if (xMatch && yMatch && zMatch) {
      coords.push_back({brick.coords[0], brick.coords[1], brick.coords[2]});
    } else if (!xMatch) {
      for (int x = brick.coords[0]; x <= brick.coords[3]; ++x) {
        coords.push_back({x, brick.coords[1], brick.coords[2]});
      }
    } else if (!yMatch) {
      for (int y = brick.coords[1]; y <= brick.coords[4]; ++y) {
        coords.push_back({brick.coords[0], y, brick.coords[2]});
      }
    } else {
      for (int z = brick.coords[2]; z <= brick.coords[5]; ++z) {
        coords.push_back({brick.coords[0], brick.coords[1], z});
      }
    }

    int height{0};
    for (const auto &coord : coords) {
      height = std::max(height, tower[coord[0]][coord[1]].height + 1);
    }

    for (const auto &coord : coords) {
      Block *prevBlock{&tower[coord[0]][coord[1]]};

      if (prevBlock->id != brick.id && prevBlock->id != 0 &&
          prevBlock->height == height - 1) {
        supports[brick.id].supportedBy.insert(prevBlock->id);
        supports[prevBlock->id].supports.insert(brick.id);
      }

      prevBlock->height = height;
      prevBlock->id = brick.id;

      if (!zMatch) {
        height++;
      }
    }
  }
}

int getDisintegrations(int id, std::vector<Support> &supports) {
  static std::vector<int> cache(supports.size());

  for (const auto supportedBy : supports[id].supportedBy) {
    supports[supportedBy].supports.extract(id);
  }

  std::vector<int> willDisintegrate;
  for (const auto supporting : supports[id].supports) {
    supports[supporting].supportedBy.extract(id);
    if (supports[supporting].supportedBy.size() == 0) {
      willDisintegrate.push_back(supporting);
    }
  }

  if (willDisintegrate.size() == 0) {
    return 0;
  }

  supports[id].supports = {};

  int totalDisintegrations{static_cast<int>(willDisintegrate.size())};
  for (const auto s : willDisintegrate) {
    totalDisintegrations += getDisintegrations(s, supports);
  }

  return totalDisintegrations;
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

  std::vector<Brick> bricks{getBricks(inf)};

  // sort by height, z
  std::sort(bricks.begin(), bricks.end(), [](const Brick &a, const Brick &b) {
    return a.coords[2] < b.coords[2];
  });

  int xMax, yMax;
  xMax = yMax = 0;
  for (const auto &brick : bricks) {
    int currX{std::max(brick.coords[0], brick.coords[3])};
    int currY{std::max(brick.coords[1], brick.coords[4])};
    xMax = std::max(xMax, currX);
    yMax = std::max(yMax, currY);
  }

  Tower tower(xMax + 1, std::vector<Block>(yMax + 1));
  std::vector<Support> supports(bricks.size() + 1);
  buildTower(tower, supports, bricks);

  int sum{0};
  auto supportsCopy{supports};
  for (int i = 1; i < supports.size(); ++i) {
    sum += getDisintegrations(i, supports);
    supports = supportsCopy;
  }

  std::cout << "Num bricks: " << sum << "\n";

  return 0;
}
