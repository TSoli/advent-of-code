#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

bool isSafe(const std::vector<int> &diffs) {
  int numBad = 0;
  int i = 0;
  if ((std::abs(diffs[i]) > 3) && std::abs(diffs[i + 1]) <= 3) {
    ++numBad;
    ++i;
  }

  int sign = 0;

  while (i < diffs.size() - 1) {
    int diff = diffs[i];
    int nextDiff = diffs[i + 1];
    if (diff == 0) {
      ++numBad;
      ++i;
    } else if (std::abs(diff) > 3 || sgn(diff) != sgn(nextDiff)) {
      ++numBad;
      int resDiff = diff + nextDiff;
      int resSgn = sgn(resDiff);
      if (sign == 0) {
        sign = resSgn;
      }

      if (resSgn != sign || std::abs(resDiff) > 3) {
        // unfixable error (but just remove the first record if it is at the
        // start)
        if (i != 0) return false;
      }

      i += 2;
    } else {
      if (sign == 0) {
        sign = sgn(diff);
      }
      ++i;
    }

    if (numBad > 1) {
      return false;
    }
  }

  if (i < diffs.size() && (sign != sgn(diffs[i]) || std::abs(diffs[i]) > 3)) {
    ++numBad;
  }
  return numBad <= 1;
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
  int total = 0;
  while (std::getline(inf, line)) {
    std::istringstream linestream(line);
    int prev, curr;
    linestream >> prev;
    std::vector<int> diffs;
    while (linestream >> curr) {
      diffs.push_back(curr - prev);
      prev = curr;
    }

    total += isSafe(diffs);
  }

  std::cout << "Number safe reports: " << total << std::endl;

  return 0;
}
