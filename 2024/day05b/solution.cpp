#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Rules = std::unordered_map<int, std::unordered_set<int>>;

Rules getRules(std::ifstream &inf, const std::string &delim) {
  std::string line;
  Rules rules;
  while (std::getline(inf, line)) {
    if (line == "") break;

    size_t splitPos{line.find(delim, 0)};
    int r1{std::stoi(line.substr(0, splitPos))};
    int r2{std::stoi(line.substr(splitPos + delim.length()))};
    rules[r2].insert(r1);
  }

  return rules;
}

std::unordered_set<int> getIntersection(std::unordered_set<int> s1,
                                        std::unordered_set<int> s2) {
  std::unordered_set<int> result;
  for (const auto &s : s1) {
    if (s2.contains(s)) result.insert(s);
  }

  return result;
}

std::vector<std::list<int>> getUpdates(std::ifstream &inf,
                                       const std::string &delim) {
  std::string line;
  std::vector<std::list<int>> updates;

  while (std::getline(inf, line)) {
    std::list<int> update;
    size_t start{0};
    size_t end{0};

    while ((end = line.find(delim, start)) != std::string::npos) {
      int pageNum{std::stoi(line.substr(start, end))};
      update.push_back(pageNum);
      start = end + delim.length();
    }
    int pageNum{std::stoi(line.substr(start, end))};
    update.push_back(pageNum);
    updates.push_back(update);
  }

  return updates;
}

int fixUpdates(const Rules &rules, std::vector<std::list<int>> &updates) {
  int total{0};
  for (auto &update : updates) {
    std::unordered_set<int> pages;
    for (const auto &page : update) {
      pages.insert(page);
    }

    bool isValid{true};
    for (const auto &[r2, r1s] : rules) {
      if (!pages.contains(r2)) continue;  // rule is not applicable

      auto bs{getIntersection(pages, r1s)};
      auto it{update.begin()};
      bool removed{false};
      while (!bs.empty()) {
        if (*it == r2) {
          it = update.erase(it);
          isValid = false;
          removed = true;
          continue;
        }

        if (bs.contains(*it)) bs.erase(*it);

        ++it;
      }

      if (removed) update.insert(it, r2);
    }

    if (!isValid) {
      auto it = update.begin();
      std::advance(it, update.size() / 2);
      total += *it;
    }
  }

  return total;
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

  Rules rules{getRules(inf, "|")};
  std::vector<std::list<int>> updates{getUpdates(inf, ",")};

  std::cout << "Pages sum: " << fixUpdates(rules, updates) << std::endl;

  return 0;
}
