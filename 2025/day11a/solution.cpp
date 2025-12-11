#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, vector<string>>
makeGraph(const vector<string> &connections, const char delim = ':') {
  unordered_map<string, vector<string>> graph;
  for (const auto &c : connections) {
    size_t delimPos{c.find(delim)};
    string from{c.substr(0, delimPos)};

    vector<string> to;
    istringstream cs{c.substr(delimPos + 2)};
    string t;
    while (cs >> t) {
      to.push_back(t);
    }

    graph[from] = to;
  }

  return graph;
}

int getNumPaths(const string &start, const string &target,
                const unordered_map<string, vector<string>> &graph) {
  if (!graph.contains(start))
    return 0;

  int total{0};
  const auto &outputs{graph.at(start)};
  for (const auto &o : outputs) {
    if (o == target) {
      total += 1;
    } else {
      total += getNumPaths(o, target, graph);
    }
  }

  return total;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> connections{aoc::readLines(inf)};
  auto graph{makeGraph(connections)};
  cout << "Total paths: " << getNumPaths("you", "out", graph) << endl;
  return 0;
}
