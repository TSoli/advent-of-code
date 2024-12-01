#include <algorithm>
#include <fstream>
#include <iostream>
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

  int l, r;
  std::vector<int> ls, rs;

  while (inf >> l >> r) {
    ls.push_back(l);
    rs.push_back(r);
  }

  std::sort(ls.begin(), ls.end());
  std::sort(rs.begin(), rs.end());

  long total = 0;
  for (int i = 0; i < ls.size(); ++i) {
    total += std::abs(ls[i] - rs[i]);
  }

  std::cout << "Total distance is: " << total << std::endl;

  return 0;
}
