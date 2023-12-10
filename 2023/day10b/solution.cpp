#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Edge = std::pair<int, int>;
using Graph = std::vector<std::string>;

bool updateStartPipe(Graph &graph, const std::pair<int, int> &startPos);
std::vector<std::pair<int, int>> getPath(const Graph &graph,
                                         const std::pair<int, int> &startPos);
int getAreaEnclosed(const Graph &graph, std::vector<std::pair<int, int>> path);

const Edge kUp{-1, 0};
const Edge kRight{0, 1};
const Edge kDown{1, 0};
const Edge kLeft{0, -1};

const std::vector<Edge> kEdges{kUp, kRight, kDown, kLeft};

const std::unordered_map<char, std::pair<Edge, Edge>> kNodeMap{
    {'|', {kUp, kDown}}, {'-', {kRight, kLeft}}, {'L', {kUp, kRight}},
    {'J', {kUp, kLeft}}, {'7', {kDown, kLeft}},  {'F', {kRight, kDown}}};

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

  std::string row;
  Graph graph;
  std::pair<int, int> startPos{-1, -1};

  int i{0};
  while (std::getline(inf, row)) {
    size_t col{row.find('S')};
    if (col != std::string::npos) {
      startPos = {i, col};
    }

    graph.push_back(row);
    ++i;
  }

  if (startPos.first == -1 && startPos.second == -1) {
    std::cerr << "Could not find the starting position.\n";
    return 3;
  }

  if (!updateStartPipe(graph, startPos)) {
    std::cerr << "Could not find a valid starting position pipe.\n";
    return 3;
  }

  std::vector<std::pair<int, int>> path{getPath(graph, startPos)};
  std::cout << "Area enclosed is: " << getAreaEnclosed(graph, path) << "\n";

  return 0;
}

bool updateStartPipe(Graph &graph, const std::pair<int, int> &startPos) {
  std::vector<Edge> edges;
  for (const Edge &edge : kEdges) {
    if (startPos.first + edge.first < 0 || startPos.second + edge.second < 0) {
      continue;
    }
    char pipe{
        graph[startPos.first + edge.first][startPos.second + edge.second]};
    if (pipe == '.') {
      continue;
    }

    Edge requiredEdge{-edge.first, -edge.second};
    if (kNodeMap.at(pipe).first == requiredEdge ||
        kNodeMap.at(pipe).second == requiredEdge) {
      edges.push_back(edge);
    }
  }

  if (edges.size() != 2) {
    return false;
  }

  std::pair<Edge, Edge> node{edges[0], edges[1]};
  for (const auto &[k, v] : kNodeMap) {
    if (v == node) {
      graph[startPos.first][startPos.second] = k;
      return true;
    }
  }

  return false;
}

std::vector<std::pair<int, int>> getPath(const Graph &graph,
                                         const std::pair<int, int> &startPos) {
  std::vector<std::pair<int, int>> path;
  path.push_back(startPos);
  std::pair<int, int> currPos;
  do {
    currPos = path.back();
    std::pair<int, int> prevPos{path.size() == 1 ? std::pair<int, int>(-1, -1)
                                                 : *(path.end() - 2)};
    char pipe{graph[currPos.first][currPos.second]};

    std::pair<Edge, Edge> edges{kNodeMap.at(pipe)};
    if (prevPos != std::pair<int, int>(currPos.first + edges.first.first,
                                       currPos.second + edges.first.second)) {
      path.push_back({currPos.first + edges.first.first,
                      currPos.second + edges.first.second});
    } else {
      path.push_back({currPos.first + edges.second.first,
                      currPos.second + edges.second.second});
    }

  } while (path.back() != startPos);

  return path;
}

int getAreaEnclosed(const Graph &graph, std::vector<std::pair<int, int>> path) {
  int area{0};
  for (int i = 0; i < graph.size(); ++i) {
    int parity{0};
    char enterPipe{0};
    for (int j = 0; j < graph[i].size(); ++j) {
      auto pathPos{std::ranges::find(path, std::pair<int, int>(i, j))};
      bool inPath{pathPos != path.end()};

      if (!inPath) {
        if (parity % 2) {
          // You are only inside the path if you have crossed it an odd number
          // of times
          ++area;
        }
        continue;
      } else {
        // in the path
        path.erase(pathPos);
        char pipe{graph[i][j]};

        switch (pipe) {
          case '|':
            ++parity;
            break;
          // You must be entering the path on the below
          case 'F':
          case 'L':
            enterPipe = pipe;
            break;
          // You must be exiting the path on the below
          case '7':
            parity += enterPipe == 'L';
            break;
          case 'J':
            parity += enterPipe == 'F';
        }
      }
    }
  }
  return area;
}
