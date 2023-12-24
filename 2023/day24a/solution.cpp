#include <array>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define NANO 1000000000

#define MIN_POS 200000000000000
#define MAX_POS 400000000000000
// #define MIN_POS 7
// #define MAX_POS 27

const double kEpsilon{1e-5};

using Pos = std::array<double, 3>;
using Vel = std::array<double, 3>;

struct Hail {
  Pos pos;
  Vel vel;
};

struct Equation2d {
  double yInt;
  Pos initPos;
  Vel direction;
  double speed;
};

bool isClose(double a, double b) { return std::abs(a - b) < kEpsilon; }

bool isIntersectingInFuture(const Equation2d &eq1, const Equation2d &eq2) {
  if (isClose(eq1.direction[0], eq2.direction[0]) &&
      isClose(eq1.direction[1], eq2.direction[1])) {
    return false;
  }

  double m1{eq1.direction[1] / eq1.direction[0]};
  double m2{eq2.direction[1] / eq2.direction[0]};

  double x{(eq2.yInt - eq1.yInt) / (m1 - m2)};
  double y{m1 * x + eq1.yInt};

  for (const auto &eq : {eq1, eq2}) {
    if ((x < eq.initPos[0] && eq.direction[0] > 0) ||
        (x > eq.initPos[0] && eq.direction[0] < 0)) {
      return false;
    }
  }

  return x >= MIN_POS && x <= MAX_POS && y >= MIN_POS && y <= MAX_POS;
}

Equation2d hailToEquation2d(const Hail &hail) {
  Vel direction{hail.vel};
  double speed{
      std::sqrt(hail.vel[0] * hail.vel[0] + hail.vel[1] * hail.vel[1])};

  direction[0] /= speed;
  direction[1] /= speed;
  direction[2] /= speed;

  double m{direction[1] / direction[0]};
  double yInt{hail.pos[1] - m * hail.pos[0]};

  return {yInt, hail.pos, direction, speed};
}

std::vector<Equation2d> getEquations2d(std::ifstream &inf) {
  std::vector<Equation2d> equations;
  std::string line;
  while (std::getline(inf, line)) {
    std::string posStr{line.substr(0, line.find('@') - 2)};
    std::string velStr(line.substr(line.find('@') + 2));
    Pos pos;
    Vel vel;

    for (int i = 0; i < 3; ++i) {
      pos[i] = std::stod(posStr.substr(0, posStr.find(',')));
      vel[i] = std::stod(velStr.substr(0, velStr.find(',')));

      posStr = posStr.substr(posStr.find(',') + 1);
      velStr = velStr.substr(velStr.find(',') + 1);
    }

    Hail hail{pos, vel};
    equations.push_back(hailToEquation2d(hail));
  }

  return equations;
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

  int sum{0};
  std::vector<Equation2d> equations{getEquations2d(inf)};
  for (int i = 0; i < equations.size(); ++i) {
    for (int j = i + 1; j < equations.size(); ++j) {
      sum += isIntersectingInFuture(equations[i], equations[j]);
    }
  }

  std::cout << "Num intersections: " << sum << "\n";

  return 0;
}
