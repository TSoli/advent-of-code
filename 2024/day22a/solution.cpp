#include <fstream>
#include <iostream>
#include <vector>

const long kPrune{16777216};

long advanceSecret(long n) {
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

  for (int i = 0; i < secrets.size(); ++i) {
    for (int j = 0; j < 2000; ++j) {
      secrets[i] = advanceSecret(secrets[i]);
    }
  }

  long total{0};
  for (const long n : secrets) {
    total += n;
  }

  std::cout << "Total: " << total << "\n";

  return 0;
}
