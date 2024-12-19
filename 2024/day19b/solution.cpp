#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Towels = std::unordered_set<std::string>;
using Designs = std::vector<std::string>;
using DesignCache = std::unordered_map<std::string, long>;

long numPossibleDesigns(std::string &design, const Towels &towels,
                        DesignCache &cache, const int maxTowelSize) {
  if (design.empty()) return 1;

  if (cache.contains(design)) return cache[design];

  long n{0};
  int patternSize{std::min(maxTowelSize, static_cast<int>(design.size()))};
  while (patternSize > 0) {
    std::string pattern{design.substr(0, patternSize)};

    if (towels.contains(pattern)) {
      design = design.substr(pattern.size());
      n += numPossibleDesigns(design, towels, cache, maxTowelSize);
      design = pattern + design;
    }
    --patternSize;
  }

  cache[design] = n;
  return n;
}

Towels compressTowels(std::vector<std::string> &allTowels,
                      DesignCache &designCache, int &maxTowelSize) {
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
    towels.insert(allTowels[i]);
    designCache[allTowels[i]] = numPossibleDesigns(
        allTowels[i], towels, designCache, allTowels[i].size());
    maxTowelSize =
        std::max(static_cast<int>(allTowels[i].size()), maxTowelSize);
  }

  return towels;
}

void getTowelsAndDesigns(std::ifstream &inf, Towels &towels,
                         Towels &substitutions, Designs &designs,
                         DesignCache &designCache, int &maxTowelSize) {
  std::string towel;
  std::vector<std::string> allTowels;
  while (inf >> towel) {
    size_t end{towel.find(",")};
    towel = towel.substr(0, end);
    allTowels.push_back(towel);

    if (end == std::string::npos) break;
  }

  towels = compressTowels(allTowels, designCache, maxTowelSize);

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
  DesignCache designCache;
  Towels towels;
  Towels substitutions;
  int maxTowelSize;
  getTowelsAndDesigns(inf, towels, substitutions, designs, designCache,
                      maxTowelSize);

  long numPossible{0};
  for (auto &design : designs) {
    numPossible +=
        numPossibleDesigns(design, towels, designCache, maxTowelSize);
  }

  std::cout << "Possible designs: " << numPossible << std::endl;

  return 0;
}
