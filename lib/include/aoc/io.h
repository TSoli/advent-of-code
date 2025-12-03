#pragma once
#include <fstream>
#include <string>
#include <vector>

namespace aoc {
void getInput(int argc, char *argv[], std::ifstream &inf);
std::vector<std::string> readLines(std::ifstream &inf, char delim = '\n');
} // namespace aoc
