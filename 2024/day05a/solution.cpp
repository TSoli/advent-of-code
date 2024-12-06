#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Rule = std::pair<int, int>;

std::vector<Rule> getRules(std::ifstream &inf, const std::string &delim) {
  std::string line;
  std::vector<std::pair<int, int>> rules;
  while (std::getline(inf, line)) {
    if (line == "") break;

    size_t splitPos{line.find(delim, 0)};
    rules.push_back({std::stoi(line.substr(0, splitPos)),
                     std::stoi(line.substr(splitPos + delim.length()))});
  }

  return rules;
}

bool isValidUpdate(const std::vector<Rule> &rules,
                   const std::unordered_map<int, int> &index) {
  for (const auto &rule : rules) {
    if (!(index.contains(rule.first) && index.contains(rule.second))) continue;

    if (index.at(rule.first) > index.at(rule.second)) return false;
  }

  return true;
}

int getPages(std::ifstream &inf, const std::string &delim,
             const std::vector<Rule> &rules) {
  int total{0};
  std::string line;

  while (std::getline(inf, line)) {
    int numPages{0};
    std::unordered_map<int, int> index;
    std::vector<int> pages;
    size_t start{0};
    size_t end{0};

    while ((end = line.find(delim, start)) != std::string::npos) {
      int pageNum{std::stoi(line.substr(start, end))};
      index[pageNum] = ++numPages;
      pages.push_back(pageNum);
      start = end + delim.length();
    }

    if (isValidUpdate(rules, index)) total += pages[numPages / 2];
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

  std::vector<Rule> rules{getRules(inf, "|")};

  std::cout << "Pages sum: " << getPages(inf, ",", rules) << std::endl;

  return 0;
}
