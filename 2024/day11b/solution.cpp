#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Cache = std::unordered_map<long long, std::array<long long, 75>>;

long long getNumStones(long long num, int blinks, Cache &cache) {
  if (blinks < 0) return 1;

  if (cache.contains(num) && cache[num][blinks] != 0) {
    return cache[num][blinks];
  }

  if (!cache.contains(num)) {
    cache[num] = std::array<long long, 75>{};
  }

  if (!num) {
    cache[num][blinks] = getNumStones(1, blinks - 1, cache);
    return cache[num][blinks];
  }

  std::string str{std::to_string(num)};
  if (str.length() % 2 == 0) {
    size_t pos{str.length() / 2};
    long long n1{std::stoll(str.substr(0, pos))};
    long long n2{std::stoll(str.substr(pos))};
    cache[num][blinks] = getNumStones(n1, blinks - 1, cache) +
                         getNumStones(n2, blinks - 1, cache);
    return cache[num][blinks];
  }

  cache[num][blinks] = getNumStones(num * 2024, blinks - 1, cache);
  return cache[num][blinks];
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

  std::vector<long long> stones;
  long long num;
  while (inf >> num) {
    stones.push_back(num);
  }

  long long total{0};
  Cache cache;
  for (const auto &stone : stones) {
    total += getNumStones(stone, 74, cache);
  }

  std::cout << "Num stones: " << total << std::endl;

  return 0;
}
