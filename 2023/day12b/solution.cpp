#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void fold(std::vector<int> &nums, std::string &springs);
std::vector<int> getNums(std::string_view numList);
std::vector<std::pair<char, int>> getContiguousBlocks(std::string_view springs);
bool trimSprings(std::vector<int> &nums,
                 std::vector<std::pair<char, int>> &contiguousBlocks);
bool removeBlocks(std::vector<std::pair<char, int>> &blocks, int numToRemove);
long getArrangmenets(std::vector<int> nums, std::string_view springs);
std::string blockToString(const std::vector<std::pair<char, int>> &blocks);
std::string vecToString(const std::vector<int> &nums);

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

  std::string springs;
  std::string numList;

  long arrangements{0};
  while (inf >> springs) {
    inf >> numList;
    std::vector<int> nums{getNums(numList)};
    fold(nums, springs);

    arrangements += getArrangmenets(nums, springs);
  }

  std::cout << "Num arrangements: " << arrangements << "\n";

  return 0;
}

void fold(std::vector<int> &nums, std::string &springs) {
  auto oldSprings{springs};
  auto oldNums{nums};

  for (int i = 0; i < 4; ++i) {
    springs = springs + "?" + oldSprings;
    nums.insert(nums.end(), oldNums.begin(), oldNums.end());
  }
}

std::vector<int> getNums(std::string_view numList) {
  std::vector<int> nums;
  auto it{numList.begin()};

  while (it <= numList.end()) {
    auto nextIt{std::find(it, numList.end(), ',')};
    nums.push_back(std::stoi(std::string(it, nextIt)));
    it = nextIt + 1;
  }

  return nums;
}

std::vector<std::pair<char, int>>
getContiguousBlocks(std::string_view springs) {
  if (springs.empty()) {
    return {};
  }

  std::vector<std::pair<char, int>> contiguousBlocks{{springs.front(), 0}};
  for (char ch : springs) {
    if (ch != contiguousBlocks.back().first) {
      contiguousBlocks.push_back({ch, 1});
    } else {
      contiguousBlocks.back().second++;
    }
  }

  return contiguousBlocks;
}

bool trimSprings(std::vector<int> &nums,
                 std::vector<std::pair<char, int>> &contiguousBlocks) {
  auto numIt{nums.begin()};
  auto blockIt{contiguousBlocks.begin()};

  while (numIt != nums.end() || blockIt != contiguousBlocks.end()) {
    if (blockIt == contiguousBlocks.end()) {
      // there are still numbers left though!
      return false;
    }

    if (blockIt->first == '.') {
      blockIt = contiguousBlocks.erase(blockIt);
    } else if (blockIt->first == '#') {
      if (numIt == nums.end()) {
        // there are no more to remove
        return false;
      }

      if (!removeBlocks(contiguousBlocks, *numIt)) {
        return false;
      }

      numIt = nums.erase(numIt);
      blockIt = contiguousBlocks.begin();
    } else {
      // the '?' character
      if (numIt == nums.end()) {
        blockIt = contiguousBlocks.erase(blockIt);
        continue;
      }

      if (blockIt->second < *numIt && (blockIt + 1) != contiguousBlocks.end() &&
          (blockIt + 1)->first == '.') {
        blockIt = contiguousBlocks.erase(blockIt);
      } else {
        // can't trim anymore
        return true;
      }
    }
  }

  return true;
}

bool removeBlocks(std::vector<std::pair<char, int>> &blocks, int numToRemove) {
  auto blockIt{blocks.begin()};

  while (numToRemove) {
    if (blockIt == blocks.end()) {
      return false;
    }

    if (blockIt->second == 0) {
      blockIt = blocks.erase(blockIt);
      continue;
    }

    if (blockIt->first == '.') {
      return false;
    }

    if (blockIt->first == '#' && blockIt->second > numToRemove) {
      return false;
    }

    if (blockIt->first == '#') {
      numToRemove -= blockIt->second;
      blockIt = blocks.erase(blockIt);
      if (blockIt != blocks.end() && blockIt->first == '?' &&
          numToRemove == 0) {
        if (blockIt->second == 1) {
          blockIt = blocks.erase(blockIt);
        } else {
          blockIt->second--;
        }
      }
      continue;
    }
    // it's a '?'
    blockIt->second -= numToRemove;
    if (blockIt->second < 0) {
      numToRemove = -blockIt->second;
      blockIt = blocks.erase(blockIt);
    } else if (blockIt->second == 0 && (blockIt + 1) != blocks.end()) {
      if ((blockIt + 1)->first == '#') {
        return false;
      }
      // next is '.'
      blocks.erase(blockIt);
      break;
    } else if (blockIt->second == 1 || blockIt->second == 0) {
      blocks.erase(blockIt);
      break;
    } else {
      // you need a '.' to break the sequence of '#'
      blockIt->second--;
      break;
    }
  }

  return true;
}

long getArrangmenets(std::vector<int> nums, std::string_view springs) {
  static std::unordered_map<std::string, long> cache;
  std::string key{vecToString(nums) + std::string(springs)};
  if (cache.find(key) != cache.end()) {
    return cache[key];
  }

  std::vector<std::pair<char, int>> contiguousBlocks{
      getContiguousBlocks(springs)};
  bool isTrimmed{trimSprings(nums, contiguousBlocks)};

  if (!isTrimmed) {
    cache[key] = 0;
    return 0;
  }

  if (nums.size() == 0 && contiguousBlocks.size() == 0) {
    cache[key] = 1;
    return 1;
  }

  // must have '?' at the start
  // assume no fault at the start
  std::vector<std::pair<char, int>> blocksNoFault{contiguousBlocks};
  if (blocksNoFault[0].second == 1) {
    blocksNoFault.erase(blocksNoFault.begin());
  } else {
    blocksNoFault[0].second--;
  }

  auto numsNoFault{nums};

  // Try placing faulty spring at the start
  contiguousBlocks[0].second--;
  if (contiguousBlocks[0].second == 0) {
    // change it to a '#'
    contiguousBlocks.erase(contiguousBlocks.begin());
  }

  if (contiguousBlocks[0].first != '#') {
    // add '#' at the start
    contiguousBlocks.insert(contiguousBlocks.begin(), {'#', 1});
  } else {
    contiguousBlocks[0].second++;
  }

  long result{
      getArrangmenets(nums, blockToString(contiguousBlocks)) +
          getArrangmenets(numsNoFault, blockToString(blocksNoFault)),
  };
  cache[key] = result;
  return result;
}

std::string blockToString(const std::vector<std::pair<char, int>> &blocks) {
  std::string str;
  for (const auto &block : blocks) {
    str += std::string(block.second, block.first);
  }

  return str;
}

std::string vecToString(const std::vector<int> &nums) {
  std::string str;
  for (auto i : nums) {
    str += std::to_string(i) + ',';
  }
  return str;
}
