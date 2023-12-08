#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

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

  std::string directions;
  std::getline(inf, directions);

  std::unordered_map<std::string, std::pair<std::string, std::string>> map;
  std::string line;
  std::getline(inf, line);

  while (std::getline(inf, line)) {
    std::string from{line.begin(), line.begin() + line.find("=") - 1};
    std::string left{line.begin() + line.find("(") + 1,
                     line.begin() + line.find(",")};
    std::string right{line.begin() + line.find(",") + 2,
                      line.begin() + line.find(")")};
    map[from] = {left, right};
  }

  std::string pos{"AAA"};
  int steps{0};
  while (pos != "ZZZ") {
    char direction{directions[steps % directions.size()]};
    if (direction == 'L') {
      pos = map[pos].first;
    } else if (direction == 'R') {
      pos = map[pos].second;
    }
    ++steps;
  }

  std::cout << "Steps: " << steps << "\n";

  return 0;
}
