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
} // namespace aoc
