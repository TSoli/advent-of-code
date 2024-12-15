#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int kSeconds{10000};
const int xSize{101};
const int ySize{103};

void getCoords(std::string &str, int &x, int &y) {
  size_t start{str.find('=') + 1};
  size_t end{str.find(',', start)};
  x = std::stoi(str.substr(start, end - start));

  start = end + 1;
  end = str.find(' ', start);
  y = std::stoi(str.substr(start, end - start));
  str = str.substr(end + 1);
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

  std::vector<long> pxs, pys, vxs, vys;

  std::string line;
  while (std::getline(inf, line)) {
    int px, py, vx, vy;
    getCoords(line, px, py);
    getCoords(line, vx, vy);
    pxs.push_back(px);
    pys.push_back(py);
    vxs.push_back(vx);
    vys.push_back(vy);
  }

  std::vector<std::string> img(ySize, std::string(xSize, '.'));
  std::ofstream outf("output.txt");
  std::vector<std::string> fileImg;
  for (int i = 1; i < kSeconds + 1; ++i) {
    fileImg = img;

    for (int j = 0; j < pxs.size(); ++j) {
      long fx{(((pxs[j] + i * vxs[j]) % xSize) + xSize) % xSize};
      long fy{(((pys[j] + i * vys[j]) % ySize) + ySize) % ySize};

      fileImg[fy][fx] = '#';
    }

    bool isPossibleTree{false};
    for (const auto &line : fileImg) {
      if (line.find("##########") != std::string::npos) {
        isPossibleTree = true;
      }
    }

    if (!isPossibleTree)
      continue;

    outf << "Seconds: " << i << "\n\n";
    for (const auto &line : fileImg) {
      outf << line << "\n";
    }

    outf << "\n";
  }

  outf.close();

  return 0;
}
