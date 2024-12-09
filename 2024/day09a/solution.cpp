#include <fstream>
#include <iostream>
#include <vector>

struct File {
  int id, start, size;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: so2lution.out <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  int id{0};
  int pos{0};
  std::vector<File> files;

  char c;
  bool isFile{true};
  while (inf.get(c)) {
    int fileSize{c - '0'};
    files.push_back({isFile ? id : -1, pos, fileSize});
    id += isFile;
    pos += fileSize;
    isFile = !isFile;
  }

  size_t lPos{0};
  size_t rPos{files.size() - 1};
  File lFile{files[lPos]};
  File rFile{files[rPos]};

  if (rFile.id == -1) rFile = files[--rPos];

  long total{0};
  pos = 0;
  while (lPos < rPos) {
    total += lFile.id * ((2 * pos + (lFile.size - 1)) / 2.0) * lFile.size;
    pos += lFile.size;
    lFile = files[++lPos];

    int space{lFile.size};
    lFile = files[++lPos];
    while (space > 0) {
      if (rPos == lPos) break;
      if (rFile.size <= space) {
        total += rFile.id * ((2 * pos + (rFile.size - 1)) / 2.0) * rFile.size;
        space -= rFile.size;
        pos += rFile.size;
        rPos -= 2;
        rFile = files[rPos];
      } else {
        total += rFile.id * ((2 * pos + (space - 1)) / 2.0) * space;
        pos += space;
        rFile.size -= space;
        space = 0;
      }
    }
  }

  total += rFile.id * ((2 * pos + (rFile.size - 1)) / 2.0) * rFile.size;

  std::cout << "Checksum: " << total << std::endl;

  return 0;
}
