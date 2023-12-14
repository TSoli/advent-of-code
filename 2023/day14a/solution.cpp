#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> getMap(std::ifstream &inf) {
  std::vector<std::string> map;
  std::string line;
  while (inf >> line) {
    map.push_back(line);
  }

  return map;
}

int getLoad(const std::vector<std::string> &map) {
  std::vector<int> maxPos(map[0].size(), map.size());
  int load{0};

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      char square{map[i][j]};
      switch (square) {
        case 'O':
          load += maxPos[j]--;
          break;
        case '#':
          maxPos[j] = map.size() - i - 1;
          break;
      }
    }
  }

  return load;
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
  std::cout << "Total load: " << getLoad(map) << "\n";

  return 0;
}
