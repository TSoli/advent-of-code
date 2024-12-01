#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>

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

  int l, r;
  std::unordered_map<int, int> ls, rs;

  while (inf >> l >> r) {
    ls[l]++;
    rs[r]++;
  }

  long total = 0;
  for (auto &[l, n] : ls) {
    if (rs.contains(l)) {
      total += n * l * rs[l];
    }
  }

  std::cout << "Similarity score: " << total << std::endl;

  return 0;
}
