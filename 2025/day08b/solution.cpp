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
                  const Edge &edge, int targetSize) {
  if (nodes[edge.n1].circuit == nodes[edge.n2].circuit)
    return false;

  int newCircuit{nodes[edge.n1].circuit};
  int oldCircuit{nodes[edge.n2].circuit};

  for (int i : circuits[oldCircuit]) {
    nodes[i].circuit = newCircuit;
    circuits[newCircuit].push_back(nodes[i].id);
  }

  circuits[oldCircuit] = {};
  return circuits[newCircuit].size() >= targetSize;
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

  int i{0};
  while (!connectNodes(nodes, circuits, edges[i], nodes.size())) {
    ++i;
  }

  cout << "Last Connection:\n";

  Node n1{nodes[edges[i].n1]};
  Node n2{nodes[edges[i].n2]};

  for (auto p : n1.pos) {
    cout << p << " ";
  }
  cout << "\n";
  for (auto p : n2.pos) {
    cout << p << " ";
  }
  cout << "\n";

  cout << "Product: " << n1.pos[0] * n2.pos[0] << endl;
  return 0;
}
