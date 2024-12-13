#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <string>

const double addVal{10000000000000};

Eigen::Vector2d getCoords(const std::string &line, const char sep) {
  size_t start{line.find(sep) + 1};
  size_t end{line.find(',')};

  float x = std::stof(line.substr(start, end - start));

  start = line.find(sep, end + 1) + 1;
  float y = std::stof(line.substr(start));

  return {x, y};
}

bool isCloseInt(const Eigen::Vector2d &x, double tol = 1e-3) {
  for (int i = 0; i < 2; ++i) {
    if (std::abs(x[i] - std::round(x[i])) > tol) {
      return false;
    }
  }

  return true;
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

  long total{0};
  std::string line;
  while (std::getline(inf, line)) {
    Eigen::Vector2d a{getCoords(line, '+')};

    std::getline(inf, line);
    Eigen::Vector2d b{getCoords(line, '+')};

    std::getline(inf, line);
    Eigen::Vector2d prize{getCoords(line, '=')};
    prize[0] += addVal;
    prize[1] += addVal;
    std::getline(inf, line);

    Eigen::Matrix2d mat;
    mat.col(0) = a;
    mat.col(1) = b;

    if (mat.determinant() == 0) {
      std::cout << "Not invertible!\n";
      continue;
    }

    auto res{mat.inverse() * prize};
    if (isCloseInt(res)) {
      total += std::round(3 * res[0]) + std::round(res[1]);
    }
  }

  std::cout << total << " tokens used" << std::endl;

  return 0;
}
