#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Note = std::vector<std::string>;

std::vector<Note> getNotes(std::ifstream &inf) {
  std::string line;
  std::vector<Note> notes;
  while (std::getline(inf, line)) {
    Note note;
    while (line.size() != 0) {
      note.push_back(line);
      std::getline(inf, line);
    }

    notes.push_back(note);
  }
  return notes;
}

Note getTransposedNote(Note note) {
  Note transposedNote(note[0].size());
  for (int i = 0; i < note[0].size(); ++i) {
    for (int j = 0; j < note.size(); ++j) {
      transposedNote[i] += note[j][i];
    }
  }

  return transposedNote;
}

int getReflection(const Note &note) {
  int maxReflection{0};
  for (int i = 0; i < note.size() - 1; ++i) {
    int j{0};
    int currReflection{0};
    while (i - j >= 0 && i + j + 1 < note.size() &&
           note[i - j] == note[i + j + 1]) {
      ++currReflection;
      ++j;
    }

    if (i - j + 1 == 0 || i + j + 1 == note.size()) {
      maxReflection = std::max(maxReflection, currReflection + i - j + 1);
    }
  }

  return maxReflection;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::vector<Note> notes{getNotes(inf)};
  int totalReflections{0};
  for (auto &note : notes) {
    totalReflections += getReflection(note) * 100;
    totalReflections += getReflection(getTransposedNote(note));
  }

  std::cout << "Total reflections: " << totalReflections << "\n";

  return 0;
}
