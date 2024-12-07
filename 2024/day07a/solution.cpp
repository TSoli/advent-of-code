#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using Results = std::vector<long>;
using Operands = std::vector<std::vector<long>>;

void getEquations(std::ifstream &inf, Results &results, Operands &operands) {
  std::string line;
  while (std::getline(inf, line)) {
    size_t sepPos{line.find(":")};
    long result{std::stol(line.substr(0, sepPos))};
    results.push_back(result);

    size_t start{sepPos + 2}; // move past the space after the colon
    size_t end;
    std::vector<long> ops;
    while ((end = line.find(" ", start)) != std::string::npos) {
      long operand{std::stol(line.substr(start, end - start))};
      ops.push_back(operand);
      start = end + 1;
    }

    long operand{std::stol(line.substr(start))};
    ops.push_back(operand);

    std::reverse(ops.begin(), ops.end());

    operands.push_back(ops);
  }
}

bool isPossible(long result, std::vector<long> &operands) {
  if (operands.size() == 1) {
    return operands[0] == result;
  }

  long op1{operands.back()};
  operands.pop_back();

  if (op1 > result) {
    operands.push_back(op1);
    return false;
  }

  long op2{operands.back()};
  operands.pop_back();

  operands.push_back(op1 + op2);
  if (isPossible(result, operands)) {
    return true;
  }

  operands.pop_back();
  operands.push_back(op1 * op2);
  if (isPossible(result, operands)) {
    return true;
  }
  operands.pop_back();
  operands.push_back(op2);
  operands.push_back(op1);
  return false;
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

  Results results;
  Operands operands;
  getEquations(inf, results, operands);
  long long total{0};
  for (size_t i = 0; i < results.size(); ++i) {
    total += isPossible(results[i], operands[i]) * results[i];
  }

  std::cout << "Total possible sum: " << total << std::endl;

  return 0;
}
