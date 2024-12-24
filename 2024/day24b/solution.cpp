#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Gate {
  std::string op;
  std::pair<std::string, std::string> inputs;
};

using Wires = std::unordered_map<std::string, bool>;
using Gates = std::unordered_map<std::string, Gate>;
using Outputs = std::vector<std::string>;

std::string getOutput(const std::string &in1, const std::string &in2,
                      const std::string &op, const Gates &gates) {
  for (const auto &[out, gate] : gates) {
    if (gate.op != op)
      continue;

    std::string in3{gate.inputs.first};
    std::string in4{gate.inputs.second};

    if ((in1 == in3 && in2 == in4) || (in1 == in4 && in2 == in3)) {
      return out;
    }
  }
  return "";
}

void swapGates(const std::string in1, const std::string in2, Gates &gates,
               Outputs &as, Outputs &bs, Outputs &cs, Outputs &ds) {
  Gate g1{gates[in1]};
  Gate g2{gates[in2]};

  gates[in1] = g2;
  gates[in2] = g1;

  for (int i = 0; i < as.size(); ++i) {
    if (in1 == as[i]) {
      as[i] = in2;
    } else if (in2 == as[i]) {
      as[i] = in1;
    }

    if (in1 == bs[i]) {
      bs[i] = in2;
    } else if (in2 == bs[i]) {
      bs[i] = in1;
    }

    if (in1 == cs[i]) {
      cs[i] = in2;
    } else if (in2 == cs[i]) {
      cs[i] = in1;
    }

    if (in1 == ds[i]) {
      ds[i] = in2;
    } else if (in2 == ds[i]) {
      ds[i] = in1;
    }
  }
}

Outputs getExpectedOutputs(Gates &gates, Outputs &as, Outputs &bs, Outputs &cs,
                           Outputs &ds) {
  Outputs wrong;

  // Check first gate
  Gate gate{gates.at("z00")};
  std::string in1{gate.inputs.first};
  std::string in2{gate.inputs.second};
  bool wrongInputs{
      !((in1 == "x00" && in2 == "y00") || (in1 == "y00" && in2 == "x00"))};
  if (gate.op != "XOR" || wrongInputs)
    wrong.push_back("z00");

  cs.resize(as.size());
  ds.resize(as.size());
  cs[1] = bs[0];
  int i{0};

  std::ostringstream oss;
  while (i < as.size() - 1) {
    oss.str("");
    oss.clear();
    oss.width(2);
    oss.fill('0');
    oss << ++i;

    gate = gates.at("z" + oss.str());
    std::string in1{gate.inputs.first};
    std::string in2{gate.inputs.second};

    if (gate.op != "XOR") {
      wrong.push_back("z" + oss.str());
      oss.str("");
      oss.clear();
      oss.width(2);
      oss.fill('0');
      oss << i;

      ds[i] = getOutput(as[i - 1], cs[i - 1], "AND", gates);
      cs[i] = getOutput(bs[i - 1], ds[i], "OR", gates);

      std::string labelSwap{getOutput(as[i], cs[i], "XOR", gates)};
      swapGates(labelSwap, "z" + oss.str(), gates, as, bs, cs, ds);

      wrong.push_back(labelSwap);
    } else if (in1 != as[i] && in2 != as[i]) {
      wrong.push_back(as[i]);

      Gate g1{gates.at(in1)};
      Gate g2{gates.at(in2)};
      if (g1.inputs.first == bs[i - 1] || g1.inputs.second == bs[i - 1]) {
        swapGates(as[i], in2, gates, as, bs, cs, ds);
      } else if (g2.inputs.first == bs[i - 1] ||
                 g2.inputs.second == bs[i - 1]) {
        swapGates(as[i], in1, gates, as, bs, cs, ds);
      }

      wrong.push_back(as[i]);
    }

    if (i > 1) {
      ds[i] = getOutput(as[i - 1], cs[i - 1], "AND", gates);
      cs[i] = getOutput(bs[i - 1], ds[i], "OR", gates);
    }
  }

  return wrong;
}

void getGates(std::ifstream &inf, Gates &gates, Outputs &as, Outputs &bs) {
  std::string in1, in2, op, out, line;
  while (inf >> in1 >> op >> in2 >> line >> out) {
    gates[out] = {op, {in1, in2}};
    if ((in1[0] == 'x' && in2[0] == 'y') || (in1[0] == 'y' && in2[0] == 'x')) {
      int n1{std::stoi(in1.substr(1))};
      int n2{std::stoi(in2.substr(1))};

      if (n1 != n2) {
        continue;
      }

      if (n1 + 1 > as.size()) {
        as.resize(n1 + 1);
        bs.resize(n1 + 1);
      }

      if (op == "XOR") {
        as[n1] = out;
      } else if (op == "AND") {
        bs[n1] = out;
      }
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

  std::string line;
  Wires wires;
  while (std::getline(inf, line), line != "") {
    size_t sep{line.find(':')};
    std::string name{line.substr(0, sep)};
    wires[name] = std::stoi(line.substr(sep + 2));
  }

  Outputs as, bs, cs, ds, wrong;
  Gates gates;
  getGates(inf, gates, as, bs);
  wrong = getExpectedOutputs(gates, as, bs, cs, ds);

  std::sort(wrong.begin(), wrong.end());

  std::cout << "Swapped outputs: ";
  for (const auto &s : wrong) {
    std::cout << s << ",";
  }

  std::cout << "\n";

  return 0;
}
