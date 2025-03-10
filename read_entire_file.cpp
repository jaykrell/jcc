#include "read_entire_file.h"
#include <stdio.h>
#include <vector>

void read_entire_file(const char *file_path, std::vector<char> &contents) {
  contents.reserve(0x10000);
  FILE *file = fopen(file_path, "rb");
  if (!file)
    return;
  while (1) {
    size_t bytes_to_read = (contents.capacity() - contents.size());
    size_t bytes_read =
        fread(&contents[contents.size()], 1, bytes_to_read, file);
    if (bytes_read)
      contents.resize(contents.size() + bytes_read);
    if (bytes_read < bytes_to_read) {
      fclose(file);
      return;
    }
    contents.reserve(contents.capacity() * 2);
  }
}
