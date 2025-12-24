#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Machine {
  int target;
  set<int> buttons;
};

int getButton(string_view button) {
  int result{0};
  for (auto s : views::split(button, ',')) {
    int i{stoi(string(s.begin(), s.end()))};
    result += 1 << i;
  }
  return result;
}

int getTarget(const string_view target) {
  int result{0};
  for (int i = 0; i < target.length(); ++i) {
    if (target[i] == '#') {
      result += 1 << i;
    }
  }
  return result;
}

int getNumPresses(const Machine &machine, const set<int> &states) {
  if (states.contains(machine.target))
    return 0;
  set<int> newState;
  for (int state : states) {
    for (int button : machine.buttons) {
      newState.insert(state ^ button);
    }
  }

  return 1 + getNumPresses(machine, newState);
}

vector<Machine> getMachines(const vector<string> &lines) {
  vector<Machine> machines;
  for (const auto &line : lines) {
    istringstream iss{line};
    string data;
    iss >> data;
    string_view targetStr{string_view(data) | views::take(data.length() - 1) |
                          views::drop(1)};
    int target{getTarget(targetStr)};

    set<int> buttons;
    while (iss >> data) {
      if (data[0] == '{')
        break;
      string_view buttonStr{string_view(data) | views::take(data.length() - 1) |
                            views::drop(1)};
      buttons.insert(getButton(buttonStr));
    }
    machines.push_back({target, buttons});
  }
  return machines;
}

int main(int argc, char *argv[]) {
  ifstream inf;
  aoc::getInput(argc, argv, inf);
  vector<string> lines{aoc::readLines(inf)};
  vector<Machine> machines = getMachines(lines);

  int total{0};
  for (const auto &machine : machines) {
    int presses = getNumPresses(machine, {0});
    total += presses;
  }

  cout << "Total presses: " << total << endl;

  return 0;
}
