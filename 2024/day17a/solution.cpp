#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Registers = std::array<long, 3>;

long getCombo(const int operand, const Registers &regs) {
  if (operand < 4) return operand;
  return regs[operand - 4];
}

int adv(int ptr, int operand, Registers &regs) {
  operand = getCombo(operand, regs);
  regs[0] = regs[0] / std::pow(2, operand);
  return ptr + 2;
}

int bxl(int ptr, int operand, Registers &regs) {
  regs[1] = regs[1] ^ operand;
  return ptr + 2;
}

int bst(int ptr, int operand, Registers &regs) {
  operand = getCombo(operand, regs);
  regs[1] = operand % 8;
  return ptr + 2;
}

int jnz(int ptr, int operand, Registers &regs) {
  if (!regs[0]) return ptr + 2;
  return operand;
}

int bxc(int ptr, int operand, Registers &regs) {
  regs[1] = regs[1] ^ regs[2];
  return ptr + 2;
}

int out(int ptr, int operand, Registers &regs) {
  std::string o{std::to_string(getCombo(operand, regs) % 8)};
  std::cout << o << ",";
  return ptr + 2;
}

int bdv(int ptr, int operand, Registers &regs) {
  operand = getCombo(operand, regs);
  regs[1] = regs[0] / std::pow(2, operand);
  return ptr + 2;
}

int cdv(int ptr, int operand, Registers &regs) {
  operand = getCombo(operand, regs);
  regs[2] = regs[0] / std::pow(2, operand);
  return ptr + 2;
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

  int ptr{0};
  std::array<int (*)(int, int, Registers &), 8> operations{
      {adv, bxl, bst, jnz, bxc, out, bdv, cdv}};
  std::cout << "Out: ";

  while (ptr < instructions.size() - 1) {
    int opcode{instructions[ptr]};
    int operand{instructions[ptr + 1]};
    ptr = operations[opcode](ptr, operand, registers);
  }

  std::cout << std::endl;

  return 0;
}
