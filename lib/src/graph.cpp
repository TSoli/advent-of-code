#include <aoc/graph.h>

#include <algorithm>
#include <vector>

using namespace std;

namespace aoc {
bool inBounds(const vector<string> &graph, const Position &pos) {
  return (pos.i >= 0) && (pos.j >= 0) && (pos.i < graph.size()) &&
         (pos.j < graph[pos.i].size());
}
int getNumEightConnected(const vector<string> &graph, const Position &pos,
                         char symbol) {
  return ranges::count_if(kDirections8, [&](const Direction &d) {
    Position newPos{pos + d};
    return inBounds(graph, newPos) && graph[newPos.i][newPos.j] == symbol;
  });
}

Position findSymbol(const vector<string> &graph, char symbol) {
  for (int i = 0; i < graph.size(); ++i) {
    for (int j = 0; j < graph[i].size(); ++j) {
      if (graph[i][j] == symbol) {
        return {i, j};
      }
    }
  }

  return {-1, -1};
}
}  // namespace aoc
