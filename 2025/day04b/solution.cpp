#include <aoc/graph.h>
#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> graph{aoc::readLines(inf)};

  int total{0};
  while (true) {
    int removed{0};
    for (int i = 0; i < graph.size(); ++i) {
      for (int j = 0; j < graph[i].size(); ++j) {
        if (graph[i][j] == '@' &&
            aoc::getNumEightConnected(graph, {i, j}, '@') < 4) {
          graph[i][j] = '.';
          ++removed;
        }
      }
    }

    total += removed;
    if (removed == 0) break;
  }

  cout << "Rolls: " << total << endl;
  return 0;
}
