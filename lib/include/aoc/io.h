#pragma once
#include <fstream>
#include <string>
#include <vector>

using namespace std;
namespace aoc {
void getInput(int argc, char *argv[], ifstream &inf);
vector<string> readLines(ifstream &inf, char delim = '\n');
vector<string> getListString(const string &str, const string &sep);
vector<int> getListInt(const string &str, const string &sep);
}  // namespace aoc
