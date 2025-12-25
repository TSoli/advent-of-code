#include <aoc/graph.h>
#include <aoc/io.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr size_t kDim{3};

struct Node {
  aoc::PositionND<kDim> pos;
  int id;
  int circuit;
};

struct Edge {
  int n1, n2;
};

vector<Edge> getEdges(vector<Node> &nodes) {
  vector<Edge> edges;
  for (int i = 0; i < nodes.size(); ++i) {
    for (int j = i + 1; j < nodes.size(); ++j) {
      edges.push_back({i, j});
    }
  }

  sort(edges.begin(), edges.end(), [&nodes](const auto &e1, const auto &e2) {
    return aoc::euclidean(nodes[e1.n1].pos, nodes[e1.n2].pos) <
           aoc::euclidean(nodes[e2.n1].pos, nodes[e2.n2].pos);
  });

  return edges;
}

vector<Node> getNodes(const vector<string> &lines) {
  vector<Node> nodes;
  for (int i = 0; i < lines.size(); ++i) {
    vector<int> coords{aoc::getListInt(lines[i], ",")};
    aoc::PositionND<kDim> pos;
    for (int j = 0; j < kDim; ++j) {
      pos[j] = coords[j];
    }

    nodes.emplace_back(pos, i, i);
  }
  return nodes;
}

bool connectNodes(vector<Node> &nodes, vector<vector<int>> &circuits,
                  const Edge &edge) {
  if (nodes[edge.n1].circuit == nodes[edge.n2].circuit) return false;

  int newCircuit{nodes[edge.n1].circuit};
  int oldCircuit{nodes[edge.n2].circuit};

  for (int i : circuits[oldCircuit]) {
    nodes[i].circuit = newCircuit;
    circuits[newCircuit].push_back(nodes[i].id);
  }

  circuits[oldCircuit] = {};
  return true;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};
  vector<Node> nodes{getNodes(lines)};
  vector<Edge> edges{getEdges(nodes)};

  vector<vector<int>> circuits;
  for (int i = 0; i < nodes.size(); ++i) {
    circuits.push_back({i});
  }

  int numConnections{string(argv[1]).find("test") != string::npos ? 10 : 1000};
  for (int i = 0; i < numConnections; ++i) {
    connectNodes(nodes, circuits, edges[i]);
  }

  vector<size_t> subCircuitSizes;
  for (const auto &c : circuits) {
    if (c.size() > 0) {
      subCircuitSizes.push_back(c.size());
    }
  }

  sort(subCircuitSizes.begin(), subCircuitSizes.end(), greater<>());

  cout << "Largest 3 Circuits: ";

  int prod{1};
  for (int i = 0; i < 3; ++i) {
    cout << subCircuitSizes[i] << " ";
    prod *= subCircuitSizes[i];
  }

  cout << "\n";
  cout << "Product: " << prod << endl;
  return 0;
}
