#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> getInitSequence(std::string_view input) {
  std::string str;
  std::vector<std::string> sequence;

  for (auto ch : input) {
    if (ch == ',') {
      sequence.push_back(str);
      str.clear();
    } else {
      str += ch;
    }
  }

  if (!str.empty()) {
    sequence.push_back(str);
  }

  return sequence;
}

int getHash(std::string_view input, int prod, int mod) {
  int hash{0};

  for (auto ch : input) {
    hash += ch;
    hash *= prod;
    hash %= mod;
  }

  return hash;
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
  std::vector<std::string> sequence{getInitSequence(input)};

  int sum{0};
  for (std::string_view str : sequence) {
    sum += getHash(str, 17, 256);
  }

  std::cout << "Result: " << sum << "\n";

  return 0;
}
