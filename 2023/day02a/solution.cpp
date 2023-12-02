#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// red, green, blue
std::array<int, 3> MAX_CUBES{12, 13, 14};

int main(int argc, char *argv[]) {
  std::string filename{"input.txt"};
  std::ifstream inf{filename};

  int sum{0};
  for (std::string line; std::getline(inf, line);) {
    // Game id is after the first space and up to the colon
    int id{std::stoi(
        line.substr(line.find(" ") + 1, line.find(":") - line.find(" ")),
        nullptr, 10)};

    // Assume game is valid and subtract back if not
    sum += id;

    std::stringstream idLine{line.substr(line.find(":") + 1)};
    for (std::string game; std::getline(idLine, game, ';');) {
      std::stringstream gameStream{game};
      int r, g, b;
      r = g = b = 0;

      while (gameStream) {
        int num;
        std::string colour;
        gameStream >> num >> colour;

        if (colour.back() == ',') {
          colour.pop_back();
        } else if (colour == "") {
          break;
        }

        if (colour == "red") {
          r += num;
        } else if (colour == "green") {
          g += num;
        } else if (colour == "blue") {
          b += num;
        } else {
          std::cerr << "Unexpected colour " << colour << std::endl;
          return 1;
        }
      }

      if (r > MAX_CUBES[0] || g > MAX_CUBES[1] || b > MAX_CUBES[2]) {
        sum -= id;
        break;
      }
    }
  }

  std::cout << sum << std::endl;
  return 0;
}
