#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

Computers largestNetworkSize(Computers &computers,
                             const Connections &connections,
                             Computers &connectedSet) {
  if (computers.empty())
    return connectedSet;

  Computers largestSet{connectedSet};
  auto it{computers.begin()};

  while (it != computers.end()) {
    connectedSet.insert(*it);
    auto set{findIntersection(computers, connections.at(*it))};
    auto resSet{largestNetworkSize(set, connections, connectedSet)};

    if (resSet.size() > largestSet.size()) {
      largestSet = resSet;
    }

    connectedSet.erase(*it);
    it = computers.erase(it);
  }

  return largestSet;
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

  Computers connectedSet;
  Computers largestSet{
      largestNetworkSize(computers, connections, connectedSet)};

  std::vector<std::string> network{largestSet.begin(), largestSet.end()};
  std::sort(network.begin(), network.end());

  std::cout << "Network: ";
  for (const auto &c : network) {
    std::cout << c << ",";
  }

  std::cout << "\n";

  return 0;
}
