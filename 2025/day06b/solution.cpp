#include <aoc/graph.h>
#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Problem {
  vector<long> operands;
  char operation;
};

long solveProblem(const Problem &problem) {
  long total{problem.operation == '+' ? 0 : 1};
  for (auto operand : problem.operands) {
    if (problem.operation == '+') {
      total += operand;
    } else {
      total *= operand;
    }
  }

  return total;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};

  vector<char> operations;
  istringstream iss{lines.back()};
  char op;
  while (iss >> op) {
    operations.push_back(op);
  }

  long total{0};
  Problem problem;
  vector<long> operands;
  for (int j = lines[0].size() - 1; j >= 0; --j) {
    long operand{0};
    for (int i = 0; i < lines.size() - 1; ++i) {
      if (isdigit(lines[i][j])) {
        operand *= 10;
        operand += lines[i][j] - '0';
      }
    }

    operands.push_back(operand);
    if (lines.back()[j] == '+' || lines.back()[j] == '*') {
      total += solveProblem({operands, operations.back()});
      operands = {};
      operations.pop_back();
      --j;
    }
  }

  cout << "Sum of Solutions: " << total << endl;
  return 0;
}
