#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace aoc {
void getInput(int argc, char *argv[], std::ifstream &inf) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <filename>\n";
    exit(1);
  }

  inf.open(argv[1]);

  if (!inf) {
    cerr << "Could not open file: " << argv[1] << "\n";
    exit(2);
  }
}

vector<string> readLines(std::ifstream &inf, char delim) {
  vector<string> lines;
  string line;
  while (getline(inf, line)) {
    lines.push_back(line);
  }
  return lines;
}

vector<string> getListString(const string &str, const string &sep) {
  vector<string> strings;
  size_t sepPos{0};
  size_t start{0};
  while (sepPos = str.find(sep, start), sepPos != string::npos) {
    strings.push_back(str.substr(start, sepPos - start));
    start = sepPos + sep.length();
  }

  strings.push_back(str.substr(start));
  return strings;
}

vector<int> getListInt(const string &str, const string &sep) {
  vector<int> ints;
  auto strings{getListString(str, sep)};
  for (const auto &s : strings) {
    ints.push_back(stoi(s));
  }

  return ints;
}

vector<long> getListLong(const string &str, const string &sep) {
  vector<long> longs;
  auto strings{getListString(str, sep)};
  for (const auto &s : strings) {
    longs.push_back(stol(s));
  }

  return longs;
}

} // namespace aoc
