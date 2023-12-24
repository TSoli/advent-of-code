#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

struct Hail {
  Eigen::Vector3d pos;
  Eigen::Vector3d vel;
};

std::vector<Hail> getHails(std::ifstream &inf) {
  std::vector<Hail> hails;
  std::string line;
  while (std::getline(inf, line)) {
    std::string posStr{line.substr(0, line.find('@') - 1)};
    std::string velStr(line.substr(line.find('@') + 2));
    Eigen::Vector3d pos;
    Eigen::Vector3d vel;

    for (int i = 0; i < 3; ++i) {
      pos[i] = std::stod(posStr.substr(0, posStr.find(',')));
      vel[i] = std::stod(velStr.substr(0, velStr.find(',')));

      posStr = posStr.substr(posStr.find(',') + 1);
      velStr = velStr.substr(velStr.find(',') + 1);
    }

    Hail hail{pos, vel};
    hails.push_back(hail);
  }

  return hails;
}

Eigen::Matrix3d getCoeffMat(const Eigen::Vector3d &v) {
  Eigen::Matrix3d coeffMat;
  coeffMat << 0, -v[2], v[1], v[2], 0, -v[0], -v[1], v[0], 0;
  return coeffMat;
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

  std::vector<Hail> hails{getHails(inf)};

  // Numerical stability issues meant I had to take a majority vote
  std::unordered_map<long, int> freqs;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, hails.size() - 1);

  for (int i = 0; i < 100; ++i) {
    int p0{distr(gen)};
    int p1{distr(gen)};
    int p2{distr(gen)};
    int p3{distr(gen)};

    if (p0 == p1 || p0 == p2 || p0 == p3 || p1 == p2 || p1 == p3 || p2 == p3) {
      continue;
    }

    // The current position of a particle can be described by P(t) = P_0 + V * t
    // where P(t) is its position at time t, P_0 is the initial position, V is
    // its velocity.
    //
    // Therefore, for each particle i and there is a solution particle Px(t)
    // such that Px(t_i) = Pi(t_i)
    // => Px_0 + Vx * t_i = Pi_0 + Vi * t_i
    // => Px_0 - Pi_0 = (Vi - Vx) * t_i
    // Therefore since t_i is scalar the vectors on either side are parallel and
    // so their cross product is zero
    //
    // => (Px_0 - Pi_0) x (Vx - Vi) = 0 for all i
    // => (Px_0 x Vx) + (Pi_0 x Vi) - (Px_0 x Vi) - (Pi_0 x Vx) = 0
    //
    // Now we can eliminate the (Px_0 x Vx) term by choosing another point Pj
    // and subtracting the two equations from each other. After some rearranging
    //
    // (Pi_0 x Vi) - (Pj_0 x Vj) = (Pi_0 - Pj_0) x Vx + (Vj - Vi) Px_0
    //
    // There are six unknowns in this equation (the start pos and velocity are
    // 3d) so we need to make another equation using at least one more point
    // and then form a matrix equation. We get a matrix of the form
    //
    // C = AX
    //
    // where X = [Px_0, Vx]^T. Therefore the solution is simply
    //
    // X = A^{-1} C
    Eigen::MatrixXd equations(6, 6);
    Eigen::VectorXd rhs(6);

    rhs.segment<3>(0) =
        hails[p0].pos.cross(hails[p0].vel) - hails[p1].pos.cross(hails[p1].vel);
    rhs.segment<3>(3) =
        hails[p2].pos.cross(hails[p2].vel) - hails[p3].pos.cross(hails[p3].vel);

    equations.block<3, 3>(0, 0) = getCoeffMat(hails[p1].vel - hails[p0].vel);
    equations.block<3, 3>(3, 0) = getCoeffMat(hails[p3].vel - hails[p2].vel);
    equations.block<3, 3>(0, 3) = getCoeffMat(hails[p0].pos - hails[p1].pos);
    equations.block<3, 3>(3, 3) = getCoeffMat(hails[p2].pos - hails[p3].pos);

    Eigen::VectorXd result = equations.inverse() * rhs;

    long sum{0};
    for (int i = 0; i < 3; ++i) {
      sum += std::roundl(result[i]);
    }
    freqs[sum]++;
  }

  int maxFreq{0};
  long sum{0};
  for (const auto &[k, v] : freqs) {
    if (v > maxFreq) {
      maxFreq = v;
      sum = k;
    }
  }

  std::cout << "Sum of positions: " << sum << " " << maxFreq << "\n";

  return 0;
}
