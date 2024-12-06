#include <array>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

// An obstacle on a row or column
struct Obstacle {
  size_t pos;                   // index position
  std::array<bool, 2> visited;  // visited from negative or positive side
};

enum Direction { Up, Right, Down, Left };

struct DirectionDelta {
  int dx, dy;
};

const std::array<DirectionDelta, 4> kDeltas{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

using Grid = std::vector<std::string>;

void getObstacles(std::ifstream &inf,
                  std::vector<std::vector<Obstacle>> &obstaclesX,
                  std::vector<std::vector<Obstacle>> &obstaclesY, Grid &grid,
                  size_t &startX, size_t &startY) {
  std::string row;
  size_t i{0};
  while (std::getline(inf, row)) {
    size_t startPos{row.find("^")};
    if (startPos != std::string::npos) {
      startY = i;
      startX = startPos;
    }
    grid.push_back(row);

    if (!obstaclesX.size()) obstaclesX.resize(row.length());

    size_t obstaclePos{row.find('#')};
    std::vector<Obstacle> obstacleY;
    while (obstaclePos != std::string::npos) {
      obstacleY.push_back({obstaclePos, std::array<bool, 2>()});
      obstaclesX[obstaclePos].push_back({i, std::array<bool, 2>()});
      obstaclePos = row.find('#', obstaclePos + 1);
    }

    obstaclesY.push_back(obstacleY);

    ++i;
  }
}

bool isLoop(std::vector<std::vector<Obstacle>> obstaclesX,
            std::vector<std::vector<Obstacle>> obstaclesY, size_t currX,
            size_t currY, Direction currDirection) {
  bool inbounds{true};
  while (inbounds) {
    inbounds = false;
    switch (currDirection) {
      case Direction::Up:
        for (auto &obstacle : std::ranges::reverse_view(obstaclesX[currX])) {
          if (obstacle.pos < currY) {
            currY = obstacle.pos + 1;
            inbounds = true;
            if (obstacle.visited[1]) return true;
            obstacle.visited[1] = true;
            break;
          }
        }
        break;
      case Direction::Right:
        for (auto &obstacle : obstaclesY[currY]) {
          if (obstacle.pos > currX) {
            currX = obstacle.pos - 1;
            inbounds = true;
            if (obstacle.visited[0]) return true;
            obstacle.visited[0] = true;
            break;
          }
        }
        break;
      case Direction::Down:
        for (auto &obstacle : obstaclesX[currX]) {
          if (obstacle.pos > currY) {
            currY = obstacle.pos - 1;
            inbounds = true;
            if (obstacle.visited[0]) return true;
            obstacle.visited[0] = true;
            break;
          }
        }
        break;
      case Direction::Left:
        for (auto &obstacle : std::ranges::reverse_view(obstaclesY[currY])) {
          if (obstacle.pos < currX) {
            currX = obstacle.pos + 1;
            inbounds = true;
            if (obstacle.visited[1]) return true;
            obstacle.visited[1] = true;
            break;
          }
        }
        break;
    }

    // There are 4 directions in enum
    currDirection =
        static_cast<Direction>((static_cast<int>(currDirection) + 1) % 4);
  }

  return false;
}

int followPath(std::vector<std::vector<Obstacle>> &obstaclesX,
               std::vector<std::vector<Obstacle>> &obstaclesY, Grid &grid,
               int currX, int currY) {
  int loops{0};
  int numTimes{0};
  Direction currDirection{Direction::Up};
  Grid placed{grid};
  grid[currY][currX] = 'X';

  while (currX < obstaclesX.size() && currX >= 0 && currY < obstaclesY.size() &&
         currY >= 0) {
    int nextX{currX + kDeltas[currDirection].dx};
    int nextY{currY + kDeltas[currDirection].dy};
    if (nextX == obstaclesX.size() || nextX == -1 ||
        nextY == obstaclesY.size() || nextY == -1)
      break;
    char square{grid[nextY][nextX]};

    if (square == '#') {
      ++numTimes;
      currDirection =
          static_cast<Direction>((static_cast<int>(currDirection) + 1) % 4);
      nextX = currX;
      nextY = currY;
    } else if (square != 'X' && placed[nextY][nextX] != 'X') {
      std::vector<std::vector<Obstacle>> testObstaclesX{obstaclesX};
      std::vector<std::vector<Obstacle>> testObstaclesY{obstaclesY};
      Obstacle testObstacleX{static_cast<size_t>(nextY), {0, 0}};
      Obstacle testObstacleY{static_cast<size_t>(nextX), {0, 0}};

      // I should have used a set oops
      auto it{testObstaclesX[nextX].begin()};
      while (it != testObstaclesX[nextX].end() && it->pos < testObstacleX.pos) {
        ++it;
      }
      testObstaclesX[nextX].insert(it, testObstacleX);

      it = testObstaclesY[nextY].begin();
      while (it != testObstaclesY[nextY].end() && it->pos < testObstacleY.pos) {
        ++it;
      }
      testObstaclesY[nextY].insert(it, testObstacleY);

      Direction testDirection{
          static_cast<Direction>((static_cast<int>(currDirection) + 1) % 4)};
      if (isLoop(testObstaclesX, testObstaclesY, currX, currY, testDirection)) {
        ++loops;
        placed[nextY][nextX] = 'X';
      }
    }

    grid[currY][currX] = 'X';
    currX = nextX;
    currY = nextY;
  }
  std::cout << numTimes << "\n";
  return loops;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: solution.out <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::vector<std::vector<Obstacle>> obstaclesX, obstaclesY;
  size_t currX, currY;
  Grid grid;

  getObstacles(inf, obstaclesX, obstaclesY, grid, currX, currY);
  std::cout << followPath(obstaclesX, obstaclesY, grid, currX, currY)
            << std::endl;

  return 0;
}
