#include "lang/reader.h"

// Return the contents of a file as a string
char *read(char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Error: No such file %s", file_name);
    exit(EXIT_FAILURE);
  }

  // find file size
  fseek(file, 0, SEEK_END);
  uint64_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(file_size + 1);

  if (!buffer) {
    fprintf(stderr, "Error: Memory allocation failed!");
  }

  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  fclose(file);

  return buffer;
}