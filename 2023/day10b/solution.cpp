#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Edge = std::pair<int, int>;
using Graph = std::vector<std::string>;

struct pair_hash {
    std::size_t operator () (const std::pair<int, int> &p) const {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);

        return h1 ^ h2;
    }
};

bool updateStartPipe(Graph &graph, const std::pair<int, int> &startPos);
std::vector<std::pair<int, int>> getPath(const Graph &graph,
                                         const std::pair<int, int> &startPos);
int getAreaEnclosed(const std::vector<std::string> &graph, std::unordered_set<std::pair<int, int>, pair_hash> path);

const Edge kUp{-1, 0};
const Edge kRight{0, 1};
	@@ -62,8 +72,9 @@ int main(int argc, char *argv[]) {
    return 3;
  }

    std::vector<std::pair<int, int>> pathVector = getPath(graph, startPos);
    std::unordered_set<std::pair<int, int>, pair_hash> path(pathVector.begin(), pathVector.end());
    std::cout << "Area enclosed is: " << getAreaEnclosed(graph, path) << "\n";

  return 0;
}
	@@ -128,45 +139,33 @@ std::vector<std::pair<int, int>> getPath(const Graph &graph,
  return path;
}

int getAreaEnclosed(const std::vector<std::string> &graph, std::unordered_set<std::pair<int, int>, pair_hash> path) {
    int area = 0;
    for (int i = 0; i < graph.size(); ++i) {
        int parityBit = 0;
        char enterPipe = 0;
        for (int j = 0; j < graph[i].size(); ++j) {
            Edge currentPos(i, j);
            bool inPath = path.count(currentPos);

            if (!inPath){
                if (parityBit){
                    ++area;
                }
                continue;
            }

            path.erase(currentPos);

            char pipe = graph[i][j];

            if (pipe == '|' || pipe == '7' && enterPipe == 'L' || pipe == 'J' && enterPipe == 'F'){
                parityBit ^= 1;
            }
            else if (pipe == 'F' || pipe == 'L'){
                enterPipe = pipe;
            }
        }
    }
    return area;
}
