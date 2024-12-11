#include <fstream>
#include <iostream>
#include <list>
#include <string>

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

  std::list<long> stones;
  long num;
  while (inf >> num) {
    stones.push_back(num);
  }

  for (int i = 0; i < 25; ++i) {
    auto it{stones.begin()};
    while (it != stones.end()) {
      std::string str{std::to_string(*it)};
      if (*it == 0) {
        *it = 1;
      } else if (str.length() % 2 == 0) {
        it = stones.erase(it);
        size_t pos{str.length() / 2};
        stones.insert(it, std::stol(str.substr(0, pos)));
        stones.insert(it, std::stol(str.substr(pos)));
        --it;
      } else {
        *it *= 2024;
      }
      ++it;
    }
  }

  std::cout << "Num stones: " << stones.size() << std::endl;

  return 0;
}
