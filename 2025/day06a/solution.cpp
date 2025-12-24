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

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};
  vector<Problem> problems;
  for (const auto &line : lines) {
    istringstream iss{line};
    string operand;
    int i{0};
    while (iss >> operand) {
      if (i >= problems.size()) {
        problems.push_back({{}, '.'});
      }
      if (operand == "+" || operand == "*") {
        problems[i].operation = operand[0];
      } else {
        problems[i].operands.push_back(stol(operand));
      }
      ++i;
    }

    if (problems[0].operation != '.') break;
  }

  long total{0};
  for (const auto &problem : problems) {
    long subtotal{problem.operation == '+' ? 0 : 1};
    for (auto operand : problem.operands) {
      if (problem.operation == '+') {
        subtotal += operand;
      } else {
        subtotal *= operand;
      }
    }
    total += subtotal;
  }

  cout << "Sum of Solutions: " << total << endl;
  return 0;
}
