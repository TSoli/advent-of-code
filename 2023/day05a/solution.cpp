#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

  std::string line;
  std::getline(inf, line);
  std::stringstream seedsLine{line.substr(line.find(":") + 1)};

  std::vector<uint32_t> seeds;
  uint32_t seed;
  while (seedsLine >> seed) {
    seeds.push_back(seed);
  }
  std::vector<bool> transformed(seeds.size());

  while (std::getline(inf, line)) {
    if (line == "" || line.back() == ':') {
      std::fill(transformed.begin(), transformed.end(), false);
      continue;
    }

    std::stringstream lineStream{line};
    uint32_t to, from, range;
    lineStream >> to >> from >> range;

    for (int i = 0; i < seeds.size(); ++i) {
      if (!transformed[i] && seeds[i] >= from && seeds[i] < from + range) {
        seeds[i] = to + (seeds[i] - from);
        transformed[i] = true;
      }
    }
  }

  std::cout << "Min location: " << *std::ranges::min_element(seeds) << "\n";
  return 0;
}
