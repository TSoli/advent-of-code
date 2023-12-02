#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

int main() {
  std::string filename{"input.txt"};
  std::ifstream inf{filename};
  if (!inf) {
    std::cerr << "Couldn't open " + filename + "\n";
    return 1;
  }

  int sum = 0;
  while (inf) {
    std::string line;
    inf >> line;
    for (auto ch : line) {
      if (std::isdigit(ch)) {
        sum += 10 * (ch - '0');
        break;
      }
    }

    for (auto ch : std::ranges::views::reverse(line)) {
      if (std::isdigit(ch)) {
        sum += ch - '0';
        break;
      }
    }
  }

  std::cout << sum << std::endl;
  return 0;
}
