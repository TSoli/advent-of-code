#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  std::string filename{"input.txt"};
  std::ifstream inf{filename};

  int sum{0};
  for (std::string line; std::getline(inf, line);) {
    std::stringstream idLine{line.substr(line.find(":") + 1)};
    int rmax, gmax, bmax;
    rmax = gmax = bmax = 0;

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

      rmax = std::max(rmax, r);
      gmax = std::max(gmax, g);
      bmax = std::max(bmax, b);
    }

    sum += rmax * gmax * bmax;
  }

  std::cout << sum << std::endl;
  return 0;
}
