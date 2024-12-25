#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using Locks = std::array<std::array<std::unordered_set<int>, 6>, 5>;
using Keys = std::vector<std::array<int, 5>>;

template <typename T>
std::unordered_set<T> findIntersection(const std::unordered_set<T> &set1,
                                       const std::unordered_set<T> &set2) {
  std::unordered_set<T> intersection;

  const std::unordered_set<T> &smaller =
      (set1.size() < set2.size()) ? set1 : set2;
  const std::unordered_set<T> &larger =
      (set1.size() < set2.size()) ? set2 : set1;

  for (const T &element : smaller) {
    if (larger.find(element) != larger.end()) {
      intersection.insert(element);
    }
  }

  return intersection;
}

void getKeysAndLocks(std::ifstream &inf, Keys &keys, Locks &locks) {
  int id{0};
  int row{0};
  std::array<int, 5> combo{};
  std::string line;
  bool isKey;

  while (inf >> line) {
    if (line == "") continue;

    if (row == 0) {
      // Handle first row
      if (line == "#####") {
        isKey = false;
      } else {
        isKey = true;
      }
    } else if (row == 6) {
      // process key/lock
      if (isKey) {
        keys.push_back(combo);
      } else {
        for (int i = 0; i < combo.size(); ++i) {
          locks[i][combo[i]].insert(id);
        }
        ++id;
      }

      row = -1;
      combo = {};
    } else {
      // process row
      for (int i = 0; i < line.size(); ++i) {
        combo[i] += line[i] == '#';
      }
    }

    ++row;
  }
}

long getCombos(const Keys &keys, const Locks &locks) {
  long combos{0};
  for (const auto &k : keys) {
    std::unordered_set<int> keyLocks;
    for (int c = 0; c < k.size(); ++c) {
      int n{k[c]};
      std::unordered_set<int> comboLocks;
      while (5 - n >= 0) {
        comboLocks.insert(locks[c][5 - n].begin(), locks[c][5 - n].end());
        ++n;
      }

      if (c == 0) {
        keyLocks = comboLocks;
      } else {
        keyLocks = findIntersection(keyLocks, comboLocks);
      }
    }

    combos += keyLocks.size();
  }

  return combos;
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

  Keys keys;
  Locks locks{};
  getKeysAndLocks(inf, keys, locks);
  std::cout << "Number of possible combos: " << getCombos(keys, locks) << "\n";

  return 0;
}
