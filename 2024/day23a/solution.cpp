#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using Connections =
    std::unordered_map<std::string, std::unordered_set<std::string>>;
using Computers = std::unordered_set<std::string>;

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

int numStartingWith(const Computers &computers, const char c = 't') {
  int n{0};
  for (const auto &com : computers) {
    n += com[0] == c;
  }

  return n;
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

  Connections connections;
  Computers computers;
  std::string line;
  while (inf >> line) {
    std::string c1{line.substr(0, 2)};
    std::string c2{line.substr(3)};
    computers.insert(c1);
    computers.insert(c2);
    connections[c1].insert(c2);
    connections[c2].insert(c1);
  }

  long totalSets{0};
  auto it1{computers.begin()};
  while (it1 != computers.end()) {
    auto set1{findIntersection(connections[*it1], computers)};
    auto it2{set1.begin()};
    while (it2 != set1.end()) {
      auto set2{findIntersection(set1, connections[*it2])};
      if ((*it1)[0] == 't' || (*it2)[0] == 't') {
        totalSets += set2.size();
      } else {
        totalSets += numStartingWith(set2);
      }

      it2 = set1.erase(it2);
    }

    it1 = computers.erase(it1);
  }

  std::cout << "Total sets: " << totalSets << "\n";

  return 0;
}
