#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const long kPrune{16777216};

using Changes = std::array<int, 4>;
using ChangeTracker = std::unordered_map<std::array<int, 4>, long>;
using ChangeSet = std::unordered_set<std::array<int, 4>>;

template <> struct std::hash<std::array<int, 4>> {
  size_t operator()(const std::array<int, 4> &arr) const {
    size_t seed = 0;
    for (int i = 0; i < arr.size(); ++i) {
      seed ^= (std::hash<int>()(arr[i]) << i);
    }
    return seed;
  }
};

int advanceSecret(long n) {
  long m{n << 6};
  n ^= m;
  n %= kPrune;
  m = n >> 5;
  n ^= m;
  n %= kPrune;
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

  ChangeTracker changeTracker;
  for (int i = 0; i < secrets.size(); ++i) {
    std::vector<int> vals(2001, 0);
    std::deque<int> changes;
    ChangeSet changeSet;
    vals[0] = secrets[i];

    for (int j = 1; j < 2001; ++j) {
      secrets[i] = advanceSecret(secrets[i]);

      vals[j] = secrets[i] % 10;

      changes.push_back(vals[j] - vals[j - 1]);

      if (j < 4)
        continue;

      if (j > 4)
        changes.pop_front();

      std::array<int, 4> currChanges;
      for (size_t k = 0; k < currChanges.size(); ++k) {
        currChanges[k] = changes[k];
      }

      if (changeSet.contains(currChanges))
        continue;

      changeSet.insert(currChanges);
      changeTracker[currChanges] += vals[j];
    }
  }

  long maxBananas{0};
  for (const auto &[k, b] : changeTracker) {
    maxBananas = std::max(maxBananas, b);
  }

  std::cout << "Total bananas: " << maxBananas << "\n";

  return 0;
}
