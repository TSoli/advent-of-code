#include <aoc/io.h>

#include <Eigen/Dense>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
namespace rv = ranges::views;

constexpr double kTol{1e-4};

struct Machine {
  vector<int> target;
  vector<vector<int>> buttons;
};

vector<int> getButton(string_view button) {
  vector<int> result;
  for (auto s : views::split(button, ',')) {
    int i{stoi(string(s.begin(), s.end()))};
    result.push_back(i);
  }
  return result;
}

vector<int> getTarget(const string_view target) {
  vector<int> result;
  for (auto s : views::split(target, ',')) {
    int i{stoi(string(s.begin(), s.end()))};
    result.push_back(i);
  }
  return result;
}

void toRef(MatrixXd &m, VectorXd &b) {
  int i{0};
  int j{0};

  while (i < m.rows() && j < m.cols()) {
    Eigen::Index idx;
    double pivotValue{
        m.col(j).segment(i, m.rows() - i).cwiseAbs().maxCoeff(&idx)};
    if (abs(pivotValue) < kTol) {
      // no pivot
      ++j;
      continue;
    }
    m.row(i).swap(m.row(idx + i));
    swap(b(i), b(idx + i));

    if (abs(m(i, j) - 1) > kTol) {
      double factor{m(i, j)};
      m.row(i) = m.row(i) / factor;
      b(i) = b(i) / factor;
    }

    for (int k = i + 1; k < m.rows(); ++k) {
      double factor{m(k, j) / m(i, j)};
      m.row(k) = m.row(k) - (factor * m.row(i));
      b(k) = b(k) - (factor * b(i));
      m(k, j) = 0;
    }

    ++j;
    ++i;
  }
}

void toRref(MatrixXd &m, VectorXd &b) {
  toRef(m, b);
  for (int i = m.rows() - 1; i >= 0; --i) {
    int j{0};
    while (j < m.cols()) {
      if (m(i, j) > kTol) {
        for (int k = i - 1; k >= 0; --k) {
          double factor{m(k, j) / m(i, j)};
          m.row(k) = m.row(k) - (factor * m.row(i));
          b(k) = b(k) - (factor * b(i));
          m(k, j) = 0;
        }
        break;
      }
      ++j;
    }
  }
}

vector<int> getFreeIdx(const MatrixXd &m) {
  vector<int> freeIdx;
  int i{0};
  for (int j = 0; j < m.cols(); ++j) {
    if (i > m.rows() - 1) {
      freeIdx.push_back(j);
      continue;
    }

    if (abs(m(i, j)) < kTol) {
      freeIdx.push_back(j);
    } else {
      ++i;
    }
  }

  return freeIdx;
}

void backSub(VectorXd &solution, const MatrixXd &m, const VectorXd &b) {
  for (int i = m.rows() - 1; i >= 0; --i) {
    int j{0};
    while (j < m.cols()) {
      if (abs(m(i, j)) < kTol) {
        ++j;
      } else {
        solution(j) =
            b(i) - m.row(i)
                       .segment(j + 1, m.cols() - j - 1)
                       .dot(solution.segment(j + 1, solution.size() - j - 1));
        break;
      }
    }
  }
}

vector<int> getBounds(const MatrixXd &m, const VectorXd &b) {
  vector<int> bounds(m.cols());
  for (int j = 0; j < m.cols(); ++j) {
    bounds[j] = std::ranges::min(
        rv::iota(0, m.rows()) |
        rv::filter([&m, &j](const auto i) { return abs(m(i, j)) > kTol; }) |
        rv::transform([&b](const auto i) { return b(i); }));
  }
  return bounds;
}

int getMinPresses(const MatrixXd &m, const VectorXd &b) {
  int minPresses{0};
  for (int i = 0; i < m.rows(); ++i) {
    if ((m.row(i).array().cwiseAbs() > kTol).count() == 1) {
      minPresses += b(i);
    }
  }
  return minPresses;
}

void updateBounds(const MatrixXd &m, const VectorXd &b,
                  const vector<int> &freeIdx, vector<int> &lower,
                  vector<int> &upper) {
  for (int i = 0; i < m.rows(); ++i) {
    if ((m.row(i).array().cwiseAbs() > kTol).count() == 2) {
      for (int j : freeIdx) {
        if (abs(m(i, j)) > kTol) {
          if (m(i, j) > 0) {
            upper[j] = min(static_cast<int>(ceil(b(i) / m(i, j))), upper[j]);
          } else {
            lower[j] = max(static_cast<int>(floor(b(i) / m(i, j))), lower[j]);
          }
        }
      }
    }
  }
}

int getNumPresses(const Machine &machine) {
  int numButtons{static_cast<int>(machine.buttons.size())};
  int numTargets{static_cast<int>(machine.target.size())};

  MatrixXd m{MatrixXd::Zero(numTargets, numButtons)};
  for (int i = 0; i < machine.buttons.size(); ++i) {
    for (const auto j : machine.buttons[i]) {
      m(j, i) = 1;
    }
  }

  VectorXd b(machine.target.size());
  for (int i = 0; i < machine.target.size(); ++i) {
    b[i] = static_cast<double>(machine.target[i]);
  }
  vector<int> upper{getBounds(m, b)};
  vector<int> lower(upper.size());

  toRref(m, b);
  int minPresses{getMinPresses(m, b)};
  vector<int> freeIdx{getFreeIdx(m)};
  updateBounds(m, b, freeIdx, lower, upper);
  VectorXd solution(m.cols());
  if (freeIdx.empty()) {
    backSub(solution, m, b);
    return round(solution.sum());
  }

  auto currLower = lower;

  int minNum{numeric_limits<int>::max()};
  while (true) {
    solution = VectorXd::Zero(m.cols());
    for (auto j : freeIdx) {
      solution[j] = currLower[j];
    }
    backSub(solution, m, b);

    if (all_of(solution.begin(), solution.end(), [](const auto s) {
          return (abs(round(s) - s) < kTol) && (s > -kTol);
        })) {
      minNum = min(static_cast<int>(round(solution.sum())), minNum);
    }

    bool done = true;
    for (auto j : freeIdx) {
      if (currLower[j] < upper[j]) {
        done = false;
        break;
      }
    }

    if (done) break;

    for (auto j : freeIdx) {
      upper[j] = min(upper[j], minNum - minPresses);
      lower[j] = min(lower[j], upper[j]);
      currLower[j] = min(currLower[j], upper[j]);
    }

    int idx{0};
    bool updated{false};
    while (idx < freeIdx.size()) {
      int j{freeIdx[idx]};
      if ((currLower[j] < upper[j]) &&
          accumulate(currLower.begin(), currLower.end(), minPresses) < minNum) {
        ++currLower[j];
        updated = true;
        break;
      }
      currLower[j] = lower[j];
      ++idx;
    }

    if (!updated) break;
  }

  return minNum;
}

vector<Machine> getMachines(const vector<string> &lines) {
  vector<Machine> machines;
  for (const auto &line : lines) {
    istringstream iss{line};
    string data;
    iss >> data;
    vector<vector<int>> buttons;
    while (iss >> data) {
      if (data[0] == '{') break;

      string_view buttonStr{string_view(data) | views::take(data.length() - 1) |
                            views::drop(1)};
      buttons.push_back(getButton(buttonStr));
    }
    vector<int> target = getTarget(
        string_view(data) | views::take(data.length() - 1) | views::drop(1));

    machines.push_back({target, buttons});
  }
  return machines;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};
  vector<Machine> machines = getMachines(lines);

  int total{0};
  for (const auto &machine : machines) {
    vector<int> state(machine.target.size(), 0);
    int presses = getNumPresses(machine);
    cout << presses << "\n";
    total += presses;
  }

  cout << "Total presses: " << total << endl;

  return 0;
}
