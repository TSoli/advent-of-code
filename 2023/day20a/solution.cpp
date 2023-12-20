#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct FlipFlop {
  int nextInputs;
  int currInputs;
  bool state;
  std::vector<std::string> outputs;
};

struct Conjunction {
  std::vector<std::pair<std::string, bool>> nextInputs;
  std::vector<std::pair<std::string, bool>> currInputs;
  std::unordered_map<std::string, int> state;
  std::vector<std::string> outputs;
};

std::vector<std::string> processOutputStr(const std::string &outputsStr) {
  std::istringstream outputStream{outputsStr};
  std::string output;
  std::vector<std::string> outputs;

  while (outputStream >> output) {
    outputs.push_back(output.substr(0, output.find(',')));
  }

  return outputs;
}

void processInputs(std::unordered_map<std::string, Conjunction> &conjunctions,
                   const std::unordered_map<std::string, FlipFlop> &flipFlops,
                   const std::vector<std::string> &broadcaster) {
  for (const auto &output : broadcaster) {
    if (conjunctions.contains(output)) {
      conjunctions[output].state["broadcaster"] = 0;
    }
  }

  for (const auto &[input, flipFlop] : flipFlops) {
    for (const auto &output : flipFlop.outputs) {
      if (conjunctions.contains(output)) {
        conjunctions[output].state[input] = 0;
      }
    }
  }

  for (const auto &[input, conjunction] : conjunctions) {
    for (const auto &output : conjunction.outputs) {
      if (conjunctions.contains(output)) {
        conjunctions[output].state[input] = 0;
      }
    }
  }
}

void getModules(std::ifstream &inf,
                std::unordered_map<std::string, FlipFlop> &flipFlops,
                std::unordered_map<std::string, Conjunction> &conjunctions,
                std::vector<std::string> &broadcaster) {
  std::string line;
  while (std::getline(inf, line)) {
    std::string name{line.substr(1, line.find('-') - 2)};
    std::string outputsStr{line.substr(line.find('>') + 2, std::string::npos)};
    std::vector<std::string> outputs{processOutputStr(outputsStr)};

    switch (line[0]) {
    case '%':
      // flip flop
      flipFlops[name] = {0, 0, false, outputs};
      break;
    case '&':
      // conjunction
      conjunctions[name] = {{}, {}, {}, outputs};
      break;
    default:
      // broadcaster
      broadcaster = outputs;
    }
  }

  processInputs(conjunctions, flipFlops, broadcaster);
}

void propogateOutputs(
    bool signal, const std::string &module,
    const std::vector<std::string> &outputs,
    std::unordered_map<std::string, FlipFlop> &flipFlops,
    std::unordered_map<std::string, Conjunction> &conjunctions,
    std::unordered_set<std::string> &toProcess, long &lows, long &highs) {
  if (signal) {
    highs += outputs.size();
  } else {
    lows += outputs.size();
  }

  for (const auto &output : outputs) {
    if (flipFlops.contains(output) && !signal) {
      toProcess.insert(output);
      flipFlops[output].nextInputs++;
    } else if (conjunctions.contains(output)) {
      conjunctions[output].nextInputs.push_back({module, signal});
      toProcess.insert(output);
    }
  }
}

void pushButton(const std::vector<std::string> &broadcaster,
                std::unordered_map<std::string, FlipFlop> &flipFlops,
                std::unordered_map<std::string, Conjunction> &conjunctions,
                long &lows, long &highs) {
  // from the button and broadcaster
  lows += 1 + broadcaster.size();
  std::unordered_set<std::string> toProcess{broadcaster.begin(),
                                            broadcaster.end()};
  for (const auto &op : broadcaster) {
    if (conjunctions.contains(op)) {
      conjunctions[op].state["broadcaster"] = 0;
    } else if (flipFlops.contains(op)) {
      flipFlops[op].currInputs++;
    }
  }

  while (!toProcess.empty()) {
    std::unordered_set<std::string> nextProcess;

    for (const auto &module : toProcess) {
      if (flipFlops.contains(module)) {
        while (flipFlops[module].currInputs) {
          flipFlops[module].state = !flipFlops[module].state;
          propogateOutputs(flipFlops[module].state, module,
                           flipFlops[module].outputs, flipFlops, conjunctions,
                           nextProcess, lows, highs);
          flipFlops[module].currInputs--;
        }
      } else {
        for (const auto &input : conjunctions[module].currInputs) {
          bool signal{false};
          conjunctions[module].state[input.first] = input.second;
          for (const auto &ip : conjunctions.at(module).state) {
            if (!ip.second) {
              signal = true;
              break;
            }
          }
          propogateOutputs(signal, module, conjunctions[module].outputs,
                           flipFlops, conjunctions, nextProcess, lows, highs);
        }
      }
    }

    for (const auto &module : nextProcess) {
      if (conjunctions.contains(module)) {
        conjunctions[module].currInputs = conjunctions[module].nextInputs;
        conjunctions[module].nextInputs = {};
      } else if (flipFlops.contains(module)) {
        flipFlops[module].currInputs = flipFlops[module].nextInputs;
        flipFlops[module].nextInputs = 0;
      }
    }
    toProcess = nextProcess;
  }
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

  std::unordered_map<std::string, FlipFlop> flipFlops;
  std::unordered_map<std::string, Conjunction> conjunctions;
  std::vector<std::string> broadcaster;

  getModules(inf, flipFlops, conjunctions, broadcaster);
  long lows, highs;
  lows = highs = 0;
  for (int i = 0; i < 1000; ++i) {
    pushButton(broadcaster, flipFlops, conjunctions, lows, highs);
  }

  std::cout << "Lows: " << lows << " Highs: " << highs << "\n";
  std::cout << "Result: " << lows * highs << "\n";
  return 0;
}
