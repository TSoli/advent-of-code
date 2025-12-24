#include <aoc/graph.h>
#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

int getNumSplits(const vector<string> &graph, aoc::Position pos,
                 unordered_set<aoc::Position, aoc::PositionHash> &visited) {
  while (aoc::inBounds(graph, pos)) {
    if (visited.contains(pos)) return 0;

    visited.insert(pos);

    if (graph[pos.i][pos.j] == '^') {
      return 1 + getNumSplits(graph, {pos.i, pos.j - 1}, visited) +
             getNumSplits(graph, {pos.i, pos.j + 1}, visited);
    }
    ++pos.i;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> graph{aoc::readLines(inf)};
  aoc::Position start{aoc::findSymbol(graph, 'S')};
  unordered_set<aoc::Position, aoc::PositionHash> visited;

  cout << "Number of Beams: " << getNumSplits(graph, start, visited) << endl;
  return 0;
}
