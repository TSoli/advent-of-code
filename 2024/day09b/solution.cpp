#include <fstream>
#include <iostream>
#include <ranges>
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
  std::vector<File> spaces;

  char c;
  bool isFile{true};
  while (inf.get(c)) {
    int fileSize{c - '0'};
    if (isFile) {
      files.push_back({id, pos, fileSize});
    } else {
      spaces.push_back({-1, pos, fileSize});
    }

    id += isFile;
    pos += fileSize;
    isFile = !isFile;
  }

  long total{0};
  for (const auto &file : std::ranges::reverse_view(files)) {
    int i{0};
    int pos{file.start};
    while (spaces[i].start < pos) {
      if (spaces[i].size >= file.size) {
        pos = spaces[i].start;
        spaces[i].start += file.size;
        spaces[i].size -= file.size;
        break;
      }
      ++i;
    }

    total += file.id * ((2 * pos + (file.size - 1)) / 2.0) * file.size;
  }
  std::cout << "Checksum: " << total << std::endl;

  return 0;
}
