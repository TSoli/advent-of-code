#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

struct Gate {
  std::string op;
  std::pair<std::string, std::string> inputs;
};

using Wires = std::unordered_map<std::string, bool>;
using Gates = std::unordered_map<std::string, Gate>;

bool performOperation(bool in1, bool in2, const std::string &op) {
  if (op == "AND") return in1 && in2;
  if (op == "OR") return in1 || in2;
  // XOR
  return (in1 && !in2) || (!in1 && in2);
}

bool getWire(const std::string &wire, Wires &wires, const Gates &gates) {
  if (wires.contains(wire)) return wires[wire];

  Gate gate{gates.at(wire)};
  bool in1{getWire(gate.inputs.first, wires, gates)};
  bool in2{getWire(gate.inputs.second, wires, gates)};
  wires[wire] = performOperation(in1, in2, gate.op);
  return wires[wire];
}

long getOutput(Wires &wires, const Gates &gates) {
  int i{0};
  long result{0};

  std::ostringstream oss;
  oss << "z";
  oss.width(2);
  oss.fill('0');
  oss << i;
  while (gates.contains(oss.str())) {
    result |= static_cast<long>(getWire(oss.str(), wires, gates)) << i;
    oss.str("");
    oss.clear();
    oss << "z";
    oss.width(2);
    oss.fill('0');
    oss << ++i;
  }

  return result;
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

  std::string line;
  Wires wires;
  while (std::getline(inf, line), line != "") {
    size_t sep{line.find(':')};
    std::string name{line.substr(0, sep)};
    wires[name] = std::stoi(line.substr(sep + 2));
  }

  std::string in1, in2, op, out;
  Gates gates;
  while (inf >> in1 >> op >> in2 >> line >> out) {
    gates[out] = {op, {in1, in2}};
  }

  std::cout << "Output: " << getOutput(wires, gates) << "\n";

  return 0;
}
