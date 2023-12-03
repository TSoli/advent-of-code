#include <array>
#include <fstream>
#include <iostream>
#include <string>

char GEAR = '*';

int getGearRatio(std::string_view prevLine, std::string_view line,
                 std::string_view nextLine, int gearPos);
void updateGearRatio(std::string_view line, int pos, int &numGears,
                     int &gearRatio, int &left, int &right);

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
      if (line[i] == GEAR) {
        sum += getGearRatio(prevLine, line, nextLine, i);
      }
    }

    prevLine = line;
    line = nextLine;
  }

  std::cout << "Sum: " << sum << "\n";
  return 0;
}

int getGearRatio(std::string_view prevLine, std::string_view line,
                 std::string_view nextLine, int gearPos) {
  int numGears = 0;
  int gearRatio = -1;
  int left, right;
  left = right = 0;
  // middle and left
  std::array<bool, 2> top{false, false};
  std::array<bool, 2> bottom{top};

  // check left
  updateGearRatio(line, gearPos - 1, numGears, gearRatio, left, right);
  // check right
  updateGearRatio(line, gearPos + 1, numGears, gearRatio, left, right);
  // check top left
  updateGearRatio(prevLine, gearPos - 1, numGears, gearRatio, left, right);
  if (right > 0) {
    top[0] = true;
  }
  if (right > 1) {
    top[1] = true;
  }

  if (!top[0]) {
    // check above
    updateGearRatio(prevLine, gearPos, numGears, gearRatio, left, right);
    if (right > 0) {
      top[1] = true;
    }
  }

  if (!top[1]) {
    // check top right
    updateGearRatio(prevLine, gearPos + 1, numGears, gearRatio, left, right);
  }

  // check bottom left
  updateGearRatio(nextLine, gearPos - 1, numGears, gearRatio, left, right);
  if (right > 0) {
    bottom[0] = true;
  }
  if (right > 1) {
    bottom[1] = true;
  }

  if (!bottom[0]) {
    updateGearRatio(nextLine, gearPos, numGears, gearRatio, left, right);
    if (right > 0) {
      bottom[1] = true;
    }
  }

  if (!bottom[1]) {
    updateGearRatio(nextLine, gearPos + 1, numGears, gearRatio, left, right);
  }

  if (numGears == 2) {
    return gearRatio;
  }

  return 0;
}

void updateGearRatio(std::string_view line, int pos, int &numGears,
                     int &gearRatio, int &left, int &right) {
  left = right = 0;
  if (line.size() == 0 || !std::isdigit(line[pos])) {
    return;
  }

  int gearSize = 0;
  int multiplier = 1;
  int i = pos;
  while (i >= 0 && std::isdigit(line[i])) {
    gearSize += multiplier * (line[i] - '0');
    multiplier *= 10;
    ++left;
    --i;
  }

  ++pos;
  while (pos < line.size() && std::isdigit(line[pos])) {
    gearSize *= 10;
    gearSize += line[pos] - '0';
    ++pos;
    ++right;
  }

  numGears++;
  if (gearRatio == -1) {
    gearRatio = gearSize;
  } else {
    gearRatio *= gearSize;
  }
}
