#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Graph = std::vector<std::vector<int>>;
std::vector<std::string> names;

bool bfs(int s, int t, Graph &residual) {
  std::unordered_set<int> visited;
  visited.insert(s);
  std::queue<std::vector<int>> q;
  q.push({s});
  std::vector<int> path;
  bool pathWasFound{false};

  while (!q.empty()) {
    path = q.front();
    q.pop();
    int node{path.back()};

    if (node == t) {
      pathWasFound = true;
      break;
    }

    for (int nextNode : residual[node]) {
      if (visited.contains(nextNode)) {
        continue;
      }
      visited.insert(nextNode);

      std::vector<int> newPath{path};
      newPath.push_back(nextNode);
      q.push(newPath);
    }
  }

  if (!pathWasFound) {
    return false;
  }

  // update residual graph
  for (int i = 0; i < path.size() - 1; ++i) {
    int node{path[i]};
    int nextNode{path[i + 1]};

    if (std::find(residual[nextNode].begin(), residual[nextNode].end(), node) !=
        residual[nextNode].end()) {
      residual[node].erase(
          std::find(residual[node].begin(), residual[node].end(), nextNode));
    } else {
      residual[nextNode].push_back(node);
    }
  }

  return true;
}

// Ford-Fulkerson algo
int minCut(int s, int t, Graph &graph, int maxCuts = 3) {
  int numCuts{0};
  while (bfs(s, t, graph)) {
    ++numCuts;
    if (numCuts > maxCuts) {
      return numCuts;
    }
  }

  return numCuts;
}

Graph getGraph(std::ifstream &inf, std::vector<std::string> &names) {
  Graph graph;
  std::unordered_map<std::string, int> visited;

  std::string line;
  int id{0};
  while (std::getline(inf, line)) {
    std::string source{line.substr(0, line.find(':'))};

    if (!visited.contains(source)) {
      visited[source] = id;
      graph.push_back({});
      names.push_back(source);
      ++id;
    }

    while (line.find(' ') != std::string::npos) {
      line = line.substr(line.find(' ') + 1);
      std::string sink{line.substr(0, line.find(' '))};

      if (!visited.contains(sink)) {
        visited[sink] = id;
        graph.push_back({});
        names.push_back(sink);
        ++id;
      }

      graph[visited.at(source)].push_back({visited.at(sink)});
      graph[visited.at(sink)].push_back({visited.at(source)});
    }
  }

  return graph;
}

std::unordered_set<int> getConnected(int s, const Graph &residual) {
  std::unordered_set<int> visited;
  visited.insert(s);
  std::queue<int> toCheck;
  toCheck.push(s);

  while (!toCheck.empty()) {
    int node{toCheck.front()};
    toCheck.pop();

    for (int nextNode : residual[node]) {
      if (visited.contains(nextNode)) {
        continue;
      }
      visited.insert(nextNode);

      toCheck.push(nextNode);
    }
  }

  return visited;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  const Graph graph{getGraph(inf, names)};

  int s{0};
  size_t sNum{0};
  size_t tNum{0};

  for (int t = 1; t < graph.size(); ++t) {
    Graph residual{graph};
    if (minCut(s, t, residual) == 3) {
      sNum = getConnected(s, residual).size();
      tNum = graph.size() - sNum;
      break;
    }
  }

  std::cout << "s: " << sNum << " t: " << tNum << "\n";
  std::cout << "s * t = " << sNum * tNum << "\n";

  return 0;
}
