#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void getNums(const std::string &line, std::vector<int> &nums,
             std::vector<int> &winningNums);
int getScore(const std::vector<int> &nums, const std::vector<int> &winningNums);

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

  int totalScore = 0;
  std::vector<int> nums, winningNums;
  for (std::string line; std::getline(inf, line);) {
    getNums(line, nums, winningNums);
    totalScore += getScore(nums, winningNums);
    nums.clear();
    winningNums.clear();
  }

  std::cout << "Total score: " << totalScore << "\n";
  return 0;
}

void getNums(const std::string &line, std::vector<int> &nums,
             std::vector<int> &winningNums) {
  size_t split{line.find("|")};
  std::istringstream lineNums{line.substr(line.find(":") + 1, split - 1)};
  std::istringstream lineWinNums{line.substr(split + 1)};

  int num;
  while (lineNums >> num) {
    nums.push_back(num);
  }

  while (lineWinNums >> num) {
    winningNums.push_back(num);
  }
}

int getScore(const std::vector<int> &nums,
             const std::vector<int> &winningNums) {
  int numMatches = 0;
  // Since the number of nums are small don't bother sorting
  for (int num : nums) {
    for (int winningNum : winningNums) {
      if (num == winningNum) {
        numMatches++;
      }
    }
  }

  if (numMatches == 0) {
    return 0;
  }

  return 1 << (numMatches - 1);
}
