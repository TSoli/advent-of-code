#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string instructionTemplate{"mul(,)"};
const std::string doTemplate{"do()"};
const std::string dontTemplate{"don't()"};

int getMul(std::ifstream &inf) {
  char c;
  int i{1};
  std::vector<int> digits1;
  std::vector<int> digits2;
  while (inf.get(c)) {
    char nextMatch{instructionTemplate[i]};
    if (c == ',' && nextMatch == c) {
      ++i;
    } else if (c == ')' && nextMatch == c) {
      int num1{0};
      int power{0};
      while (digits1.size() > 0) {
        num1 += digits1.back() * std::pow(10, power++);
        digits1.pop_back();
      }

      power = 0;
      int num2{0};
      while (digits2.size() > 0) {
        num2 += digits2.back() * std::pow(10, power++);
        digits2.pop_back();
      }

      return num1 * num2;
    } else if (nextMatch == c) {
      ++i;
    } else if (nextMatch == ',' && std::isdigit(c)) {
      digits1.push_back(c - '0');
    } else if (nextMatch == ')' && std::isdigit(c)) {
      digits2.push_back(c - '0');
    } else {
      // invalid
      return 0;
    }
  }
  return 0;
}

void updateEnabled(std::ifstream &inf, bool &enabled) {
  char c;
  int i{1};
  bool useDont{true};
  while (inf.get(c)) {
    char dontChar = dontTemplate[i];
    if (c == dontChar && useDont) {
      if (c == ')') {
        enabled = false;
        return;
      }
      ++i;
    } else if (i < doTemplate.size() && c == doTemplate[i]) {
      if (c == '(') useDont = false;
      if (c == ')' && !useDont) {
        enabled = true;
        return;
      }
      ++i;
    } else if (c == dontTemplate[0]) {
      useDont = true;
      i = 1;
    } else {
      return;
    }
  }
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

  long result{0};
  char c;
  bool enabled{true};
  while (inf.get(c)) {
    if (c == 'm' && enabled) result += getMul(inf);
    if (c == 'd') updateEnabled(inf, enabled);
  }

  std::cout << "Result: " << result << std::endl;

  return 0;
}
