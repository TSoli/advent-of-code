#include <fstream>
#include <iostream>
#include <string>

char SEP = '.';

bool isValid(std::string_view prevLine, std::string_view nextLine, int pos);

// This was a dumb way of doing it...
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

  std::string prevLine{""};
  std::string line, nextLine;
  inf >> line;
  nextLine = "";

  int sum = 0;
  while (inf) {
    inf >> nextLine;
    for (int i = 0; i < line.size(); ++i) {
      int num = 0;
      bool numIsValid = false;
      bool numFound = false;
      while (i < line.size() && std::isdigit(line[i])) {
        numFound = true;
        if (num == 0 && i > 0) {
          // Start of number so check before it
          numIsValid = !std::isdigit(line[i - 1]) && line[i - 1] != SEP;
          if (!numIsValid) {
            numIsValid = isValid(prevLine, nextLine, i - 1);
          }
        }

        if (!numIsValid) {
          // Check above/below
          numIsValid = isValid(prevLine, nextLine, i);
        }

        num *= 10;
        num += line[i++] - '0';
      }

      if (numFound && !numIsValid) {
        // Check characters after
        numIsValid = isValid(line, "", i) || isValid(prevLine, nextLine, i);
      }

      if (numIsValid) {
        sum += num;
      }
    }

    prevLine = line;
    line = nextLine;
  }

  std::cout << "Sum: " << sum << "\n";
  return 0;
}

bool isValid(std::string_view prevLine, std::string_view nextLine, int pos) {
  if (pos < 0) {
    return false;
  }

  if (prevLine.size() && pos < prevLine.size() &&
      !std::isdigit(prevLine[pos]) && prevLine[pos] != SEP) {
    return true;
  }
  if (nextLine.size() && pos < nextLine.size() &&
      !std::isdigit(nextLine[pos]) && nextLine[pos] != SEP) {
    return true;
  }

  return false;
}
