#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Registers = std::array<long, 3>;
using Operations =
    std::array<int (*)(int, int, Registers &, std::vector<int> &), 8>;

struct State {
  Registers regs;
  int ptr;
};

void getOutput(State &state, const std::vector<int> &instructions,
               const Operations &operations, std::vector<int> &output) {
  if (state.ptr > instructions.size() - 2) {
    return;
  }

  int opcode{instructions[state.ptr]};
  int operand{instructions[state.ptr + 1]};
  state.ptr = operations[opcode](state.ptr, operand, state.regs, output);
  getOutput(state, instructions, operations, output);
}

long getCombo(const int operand, const Registers &regs) {
  if (operand < 4)
    return operand;
  return regs[operand - 4];
}

int adv(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  operand = getCombo(operand, regs);
  regs[0] = regs[0] >> operand;
  return ptr + 2;
}

int bxl(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  regs[1] = regs[1] ^ operand;
  return ptr + 2;
}

int bst(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  operand = getCombo(operand, regs);
  regs[1] = operand & 7;
  return ptr + 2;
}

int jnz(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  if (!regs[0])
    return ptr + 2;
  return operand;
}

int bxc(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  regs[1] = regs[1] ^ regs[2];
  return ptr + 2;
}

int out(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  output.push_back(getCombo(operand, regs) & 7);
  return ptr + 2;
}

int bdv(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  operand = getCombo(operand, regs);
  regs[1] = regs[0] >> operand;
  return ptr + 2;
}

int cdv(int ptr, int operand, Registers &regs, std::vector<int> &output) {
  operand = getCombo(operand, regs);
  regs[2] = regs[0] >> operand;
  return ptr + 2;
}

int numMatch(const std::vector<int> &pred, const std::vector<int> &gold) {
  int i{static_cast<int>(gold.size() - 1)};
  int j{static_cast<int>(pred.size() - 1)};
  int num{0};
  while (i >= 0 && j >= 0) {
    if (gold[i] != pred[j]) {
      return num;
    }
    ++num;
    --j;
    --i;
  }
  return num;
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

  std::vector<int> instructions;
  Registers registers;

  std::string line;
  int i{0};
  while (std::getline(inf, line), line != "") {
    size_t pos{line.find(":") + 2};
    registers[i] = std::stol(line.substr(pos));
    ++i;
  }

  std::getline(inf, line);
  line = line.substr(line.find(":") + 2);

  for (const char c : line) {
    if (c == ',') {
      continue;
    }
    instructions.push_back(c - '0');
  }

  Operations operations{{adv, bxl, bst, jnz, bxc, out, bdv, cdv}};

  std::vector<int> output;
  long a{0};
  int a7{0};
  int n{0};

  while (true) {
    output = {};
    Registers regs{registers};
    regs[0] = a;
    State state{regs, 0};
    getOutput(state, instructions, operations, output);

    int nMatch{numMatch(output, instructions)};

    if (nMatch > n) {
      // new match
      if (nMatch == instructions.size())
        break;
      n = nMatch;
      // check the next three bits
      a <<= 3;
    } else if (a7 == 7) {
      // no matches were found so backtrack
      a >>= 3;
      ++a;
      --n;
    } else {
      // try the combo
      ++a;
    }
    a7 = a & 7;
  }

  std::cout << "Register A was: " << a << std::endl;

  return 0;
}
