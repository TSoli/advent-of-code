#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

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

  std::map<uint32_t, uint32_t> seeds;
  uint32_t start;
  while (seedsLine >> start) {
    uint32_t range;
    seedsLine >> range;
    seeds[start] = range;
  }
  std::map<uint32_t, uint32_t> transformed;

  while (std::getline(inf, line)) {
    if (line == "") {
      continue;
    } else if (line.back() == ':') {
      for (const auto &[start, range] : seeds) {
        transformed[start] = range;
      }
      seeds = transformed;
      transformed.clear();
      continue;
    }

    std::stringstream lineStream{line};
    uint32_t to, from, range;
    lineStream >> to >> from >> range;

    auto it = seeds.upper_bound(from);
    if (it != seeds.begin()) {
      --it;
    }

    // still not great...
    while ((*it).first < from + range && it != seeds.end()) {
      if ((*it).first + (*it).second < from) {
        // end of current seed range is before the transformation range
        ++it;
        continue;
      }

      if ((*it).first < from) {
        // seed start is before the transformation start
        transformed[to] =
            std::min((*it).first + (*it).second, from + range) - from;
        (*it).second = from - (*it).first;
        ++it;
      } else if (from + range >= (*it).first + (*it).second) {
        // seeds end is before the transformation range end
        // => seed range is fully contained in transform
        transformed[to + ((*it).first - from)] = (*it).second;
        it = seeds.erase(it);
      } else {
        // end of transform is before the seeds end
        seeds[from + range] = (*it).first + (*it).second - from - range;
        // seeds start is after transformation start
        transformed[to + ((*it).first - from)] = from + range - (*it).first;
        it = seeds.erase(it);
      }
    }
  }

  for (const auto &[start, range] : seeds) {
    transformed[start] = range;
  }
  seeds = transformed;
  transformed.clear();

  std::cout << "Min location: " << (*(seeds.begin())).first << "\n";
  return 0;
}
