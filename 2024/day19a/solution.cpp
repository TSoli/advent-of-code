#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using Towels = std::unordered_set<std::string>;
using Designs = std::vector<std::string>;

bool isPossibleDesign(std::string &design, const Towels &towels,
                      const int maxTowelSize) {
  if (design.empty())
    return true;

  int patternSize{std::min(maxTowelSize, static_cast<int>(design.size()))};
  while (patternSize > 0) {
    std::string pattern{design.substr(0, patternSize)};

    if (towels.contains(pattern)) {
      design = design.substr(pattern.size());
      if (isPossibleDesign(design, towels, maxTowelSize)) {
        return true;
      }
      design = pattern + design;
    }
    --patternSize;
  }

  return false;
}

Towels compressTowels(std::vector<std::string> &allTowels, int &maxTowelSize) {
  std::sort(allTowels.begin(), allTowels.end(),
            [](const std::string &a, const std::string &b) {
              if (a.length() == b.length()) {
                return a < b;
              }
              return a.length() < b.length();
            });

  Towels towels;
  maxTowelSize = 0;

  for (int i = 0; i < allTowels.size(); ++i) {
    if (!isPossibleDesign(allTowels[i], towels, allTowels[i].size())) {
      towels.insert(allTowels[i]);
      maxTowelSize =
          std::max(static_cast<int>(allTowels[i].size()), maxTowelSize);
    }
  }

  return towels;
}

void getTowelsAndDesigns(std::ifstream &inf, Towels &towels, Designs &designs,
                         int &maxTowelSize) {
  std::string towel;
  std::vector<std::string> allTowels;
  while (inf >> towel) {
    size_t end{towel.find(",")};
    towel = towel.substr(0, end);
    allTowels.push_back(towel);

    if (end == std::string::npos)
      break;
  }

  towels = compressTowels(allTowels, maxTowelSize);

  while (inf >> towel) {
    designs.push_back(towel);
  }
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

  Designs designs;
  Towels towels;
  int maxTowelSize;
  getTowelsAndDesigns(inf, towels, designs, maxTowelSize);

  int numPossible{0};
  for (auto &design : designs) {
    numPossible += isPossibleDesign(design, towels, maxTowelSize);
  }

  std::cout << "Possible designs: " << numPossible << std::endl;

  return 0;
}
