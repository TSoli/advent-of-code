#include <aoc/graph.h>
#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

long getNumTimelines(
    const vector<string> &graph, aoc::Position pos,
    unordered_map<aoc::Position, long, aoc::PositionHash> &timelines) {
  if (!inBounds(graph, pos)) return 0;

  if (timelines.contains(pos)) return timelines[pos];

  long n;
  if (graph[pos.i][pos.j] == '^') {
    n = getNumTimelines(graph, {pos.i, pos.j - 1}, timelines) +
        getNumTimelines(graph, {pos.i, pos.j + 1}, timelines);
    timelines[pos] = n;
    return n;
  }

  if (!aoc::inBounds(graph, {pos.i + 1, pos.j})) {
    timelines[pos] = 1;
    return 1;
  }

  n = getNumTimelines(graph, {pos.i + 1, pos.j}, timelines);
  timelines[pos] = n;

  return n;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> graph{aoc::readLines(inf)};
  aoc::Position start{aoc::findSymbol(graph, 'S')};
  unordered_map<aoc::Position, long, aoc::PositionHash> timelines;

  cout << "Number of Beams: " << getNumTimelines(graph, start, timelines)
       << endl;
  return 0;
}
