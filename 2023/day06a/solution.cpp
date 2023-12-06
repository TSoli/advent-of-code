#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  std::getline(inf, line);
  std::stringstream timesStream{line.substr(line.find(":") + 1)};

  std::vector<int> times;
  int val;
  while (timesStream >> val) {
    times.push_back(val);
  }

  std::getline(inf, line);
  std::stringstream distanceStream{line.substr(line.find(":") + 1)};
  std::vector<int> distances;

  while (distanceStream >> val) {
    distances.push_back(val);
  }

  int prod{1};
  for (int i = 0; i < times.size(); ++i) {
    // d = t(n - t)
    // => t^2 -nt + d = 0
    int t{static_cast<int>(
        std::ceil(
            (times[i] + std::sqrt((times[i] * times[i]) - (4 * distances[i]))) /
            2) -
        1)};
    prod *= 1 + 2 * (t - (times[i] / 2.0));
  }

  std::cout << prod << "\n";

  return 0;
}
