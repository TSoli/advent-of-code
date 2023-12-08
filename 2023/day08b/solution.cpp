#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

// It's nt actually guaranteed that using the LCM will work for a general
// input. It just happens to work for this problem. For a more general solution
// see
// https://github.com/edoannunziata/jardin/blob/master/misc/Aoc23Day8BonusRound.ipynb
// for a Python solution or http://clb.confined.space/aoc2023/#day8code for a
// C solution
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
  std::vector<std::string> pos;

  while (std::getline(inf, line)) {
    std::string from{line.begin(), line.begin() + line.find("=") - 1};
    std::string left{line.begin() + line.find("(") + 1,
                     line.begin() + line.find(",")};
    std::string right{line.begin() + line.find(",") + 2,
                      line.begin() + line.find(")")};

    if (from.back() == 'A') {
      pos.push_back(from);
    }

    map[from] = {left, right};
  }

  std::vector<int> steps(pos.size());
  auto isEnd = [](std::string_view i) { return i.back() == 'Z'; };
  for (int i = 0; i < pos.size(); ++i) {
    int step{0};
    while (!isEnd(pos[i])) {
      char direction{directions[step % directions.size()]};
      if (direction == 'L') {
        pos[i] = map[pos[i]].first;
      } else if (direction == 'R') {
        pos[i] = map[pos[i]].second;
      }

      ++step;
    }

    steps[i] = step;
  }

  long lcm{1};

  for (int step : steps) {
    lcm = std::lcm(lcm, step);
  }

  std::cout << "Steps: " << lcm << "\n";

  return 0;
}
