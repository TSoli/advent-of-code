#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

const long kPrune{16777216};

using Changes = std::array<int, 4>;
using ChangeTracker = std::array<int, ((1 << 20) - 1)>;
using ChangeSet = std::array<bool, ((1 << 20) - 1)>;

int advanceSecret(long n) {
  long m{n << 6};
  n ^= m;
  n %= kPrune;
  m = n >> 5;
  n ^= m;
  m = n << 11;
  n ^= m;
  return n % kPrune;
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

  std::vector<long> secrets;
  long n;
  while (inf >> n) {
    secrets.push_back(n);
  }

  ChangeTracker changeTracker{};
  for (int i = 0; i < secrets.size(); ++i) {
    std::vector<int> vals(2001, 0);
    std::deque<int> changes;
    ChangeSet changeSet{};
    vals[0] = secrets[i];

    for (int j = 1; j < 2001; ++j) {
      secrets[i] = advanceSecret(secrets[i]);

      vals[j] = secrets[i] % 10;

      changes.push_back(vals[j] - vals[j - 1]);

      if (j < 4) continue;

      if (j > 4) changes.pop_front();

      int change{0};
      for (size_t k = 0; k < changes.size(); ++k) {
        // numbers in range [-9, 9] only require 5 bits so we can pack them all
        // into 20 bits
        change |= (changes[k] & ((1 << 5) - 1)) << (k * 5);
      }

      if (changeSet[change]) continue;

      changeSet[change] = true;
      changeTracker[change] += vals[j];
    }
  }

  int maxBananas{0};
  for (const auto b : changeTracker) {
    maxBananas = std::max(maxBananas, b);
  }

  std::cout << "Total bananas: " << maxBananas << "\n";

  return 0;
}
