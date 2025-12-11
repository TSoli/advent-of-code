#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, vector<string>> makeGraph(
    const vector<string> &connections, const char delim = ':') {
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

long getNumPaths(const string &start, const string &target,
                 const unordered_map<string, vector<string>> &graph,
                 const set<string> &illegalNodes,
                 unordered_map<string, long> &cache) {
  if (!graph.contains(start)) return 0;
  string id{start + target};
  for (const auto &i : illegalNodes) {
    id += i;
  }

  if (cache.contains(id)) return cache[id];

  long total{0};
  const auto &outputs{graph.at(start)};
  for (const auto &o : outputs) {
    if (illegalNodes.contains(o)) continue;

    if (o == target) {
      total += 1;
    } else {
      total += getNumPaths(o, target, graph, illegalNodes, cache);
    }
  }

  cache[id] = total;
  return total;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> connections{aoc::readLines(inf)};
  auto graph{makeGraph(connections)};
  unordered_map<string, long> cache;
  long paths1 = getNumPaths("svr", "dac", graph, {"out", "fft"}, cache) *
                getNumPaths("dac", "fft", graph, {"out", "svr"}, cache) *
                getNumPaths("fft", "out", graph, {"dac", "svr"}, cache);
  long paths2 = getNumPaths("svr", "fft", graph, {"out", "dac"}, cache) *
                getNumPaths("fft", "dac", graph, {"svr", "out"}, cache) *
                getNumPaths("dac", "out", graph, {"svr", "fft"}, cache);

  cout << "Total paths: " << paths1 + paths2 << endl;

  return 0;
}
