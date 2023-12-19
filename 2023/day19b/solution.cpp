#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Range = std::array<int, 2>;
using PartRange = std::array<Range, 4>;

struct Rule {
  char category;
  char condition;
  int boundary;
  std::string nextWorkflow;
};

using Workflow = std::vector<Rule>;

std::unordered_map<std::string, Workflow> getWorkflows(std::ifstream &inf) {
  std::unordered_map<std::string, Workflow> workflows;
  std::string line;

  while (std::getline(inf, line)) {
    if (line.empty()) {
      break;
    }

    std::string name{line.substr(0, line.find('{'))};
    line = line.substr(line.find('{') + 1, std::string::npos);

    Workflow workflow;
    while (line.find(',') != std::string::npos) {
      char category{line[0]};
      char condition{line[1]};
      int boundary{stoi(line.substr(2, line.find(':') - 2))};

      size_t colPos{line.find(':')};
      std::string nextWorkflow{
          line.substr(colPos + 1, line.find(',') - (colPos + 1))};
      line = line.substr(line.find(',') + 1, std::string::npos);

      workflow.push_back({category, condition, boundary, nextWorkflow});
    }

    std::string nextWorkflow{line.begin(), line.end() - 1};
    workflow.push_back({'x', '>', -1, nextWorkflow});
    workflows[name] = workflow;
  }

  return workflows;
}

void updateRangeMap(
    const std::string &workflow, PartRange partRange,
    const std::unordered_map<std::string, Workflow> &workflows,
    std::unordered_map<std::string, std::vector<PartRange>> &rangeMap) {
  if (workflow == "A" || workflow == "R") {
    rangeMap[workflow].push_back(partRange);
    return;
  }

  const std::unordered_map<char, int> categoryToIdx{
      {'x', 0}, {'m', 1}, {'a', 2}, {'s', 3}};

  PartRange passRange, failRange;
  passRange = failRange = partRange;
  for (int i = 0; i < workflows.at(workflow).size(); ++i) {
    Rule rule{workflows.at(workflow)[i]};
    int partRangeIdx{categoryToIdx.at(rule.category)};
    passRange = failRange;

    switch (rule.condition) {
      case '<':
        if (passRange[partRangeIdx][1] >= rule.boundary) {
          passRange[partRangeIdx][1] = rule.boundary - 1;
        }

        if (failRange[partRangeIdx][0] < rule.boundary) {
          failRange[partRangeIdx][0] = rule.boundary;
        }
        break;
      default:
        // >
        if (passRange[partRangeIdx][0] <= rule.boundary) {
          passRange[partRangeIdx][0] = rule.boundary + 1;
        }

        if (failRange[partRangeIdx][1] > rule.boundary) {
          failRange[partRangeIdx][1] = rule.boundary;
        }
    }

    if (passRange[partRangeIdx][0] > passRange[partRangeIdx][1]) {
      // impossible range
      passRange = {{{0, 0}, {0, 0}, {0, 0}, {0, 0}}};
    } else {
      updateRangeMap(rule.nextWorkflow, passRange, workflows, rangeMap);
    }

    rangeMap[workflow][i] = passRange;
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

  auto workflows{getWorkflows(inf)};

  // make a map from the workflow to the ranges
  std::unordered_map<std::string, std::vector<PartRange>> rangeMap;
  for (const auto &[k, v] : workflows) {
    rangeMap[k] = std::vector<PartRange>(v.size());
  }

  PartRange initPartRange{{{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}};
  updateRangeMap("in", initPartRange, workflows, rangeMap);

  long sum{0};
  for (auto &partRange : rangeMap["A"]) {
    if (partRange[0][0] != 0) {
      long subSum{1};
      for (auto &range : partRange) {
        subSum *= range[1] - range[0] + 1;
      }

      // by definition ranges can't overlap
      sum += subSum;
    }
  }

  std::cout << "Sum: " << sum << "\n";

  return 0;
}
