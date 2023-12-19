#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Rule {
  char category;
  char condition;
  int boundary;
  std::string nextWorkflow;
};

struct Part {
  int x;
  int m;
  int a;
  int s;
};

using Workflow = std::vector<Rule>;

std::vector<Part> getParts(std::ifstream &inf) {
  std::vector<Part> parts;
  std::string line;

  while (std::getline(inf, line)) {
    int x, m, a, s;
    size_t pos{line.find('x')};
    x = stoi(line.substr(pos + 2, line.find(pos, ',') - pos - 1));

    pos = line.find('m');
    m = stoi(line.substr(pos + 2, line.find(pos, ',') - pos - 1));

    pos = line.find('a');
    a = stoi(line.substr(pos + 2, line.find(pos, ',') - pos - 1));

    pos = line.find('s');
    s = stoi(line.substr(pos + 2, line.find(pos, ',') - pos - 1));

    parts.push_back({x, m, a, s});
  }

  return parts;
}

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

bool partIsAccepted(
    const Part &part,
    const std::unordered_map<std::string, Workflow> &workflows) {
  std::string currWorkflow{"in"};

  while (currWorkflow != "A") {
    if (currWorkflow == "R") {
      return false;
    }

    for (const auto &rule : workflows.at(currWorkflow)) {
      bool passedRule{false};
      int partNum;

      switch (rule.category) {
      case 'x':
        partNum = part.x;
        break;
      case 'm':
        partNum = part.m;
        break;
      case 'a':
        partNum = part.a;
        break;
      default:
        partNum = part.s;
      }

      switch (rule.condition) {
      case '<':
        passedRule = partNum < rule.boundary;
        break;
      default:
        passedRule = partNum > rule.boundary;
      }

      if (passedRule) {
        currWorkflow = rule.nextWorkflow;
        break;
      }
    }
  }

  return true;
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
  auto parts{getParts(inf)};

  long sum{0};
  for (const auto &part : parts) {
    if (partIsAccepted(part, workflows)) {
      sum += part.x + part.m + part.a + part.s;
    }
  }

  std::cout << "Sum: " << sum << "\n";

  return 0;
}
