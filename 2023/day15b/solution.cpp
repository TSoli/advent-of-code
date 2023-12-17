#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

typedef struct Step {
  std::string id;
  bool isRemove;
  int lens;
} Step;

typedef struct Lens {
  int lens;
  int time;
} Lens;

int getHash(std::string_view input, int prod = 17, int mod = 256) {
  int hash{0};

  for (auto ch : input) {
    hash += ch;
    hash *= prod;
    hash %= mod;
  }

  return hash;
}

std::vector<Step> getSteps(std::string_view input) {
  std::string str;
  std::vector<Step> steps;

  bool isRemove{true};
  int lens{0};
  for (auto ch : input) {
    if (ch == ',') {
      steps.push_back({str, isRemove, lens});
      isRemove = true;
      str.clear();
    } else if (ch == '=') {
      isRemove = false;
    } else if (!isRemove) {
      lens = ch - '0';
    } else if (ch != '-') {
      str += ch;
    }
  }

  if (!str.empty()) {
    steps.push_back({str, isRemove, lens});
  }

  return steps;
}

void updateBoxes(std::vector<std::unordered_map<std::string, Lens>> &boxes,
                 std::vector<Step> &steps) {
  for (int time = 0; time < steps.size(); ++time) {
    Step step{steps[time]};
    int hash{getHash(step.id)};
    if (step.isRemove && boxes[hash].contains(step.id)) {
      boxes[hash].erase(boxes[hash].find(step.id));
    } else if (!step.isRemove) {
      if (boxes[hash].contains(step.id)) {
        boxes[hash][step.id].lens = step.lens;
      } else {
        boxes[hash][step.id] = {step.lens, time};
      }
    }
  }
}

int getFocusingPower(
    const std::vector<std::unordered_map<std::string, Lens>> &boxes) {
  int power{0};

  for (int i = 0; i < boxes.size(); ++i) {
    std::vector<Lens> contents;
    contents.reserve(boxes[i].size());
    for (auto &[k, v] : boxes[i]) {
      contents.push_back(v);
    }

    std::ranges::sort(contents,
                      [](Lens &a, Lens &b) { return a.time < b.time; });

    for (int j = 0; j < contents.size(); ++j) {
      power += (i + 1) * (j + 1) * contents[j].lens;
    }
  }

  return power;
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

  std::string input;
  inf >> input;
  std::vector<Step> steps{getSteps(input)};
  std::vector<std::unordered_map<std::string, Lens>> boxes(256);
  updateBoxes(boxes, steps);

  std::cout << "Result: " << getFocusingPower(boxes) << "\n";

  return 0;
}
