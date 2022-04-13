#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include "elf_names.h"
#include "elf64.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Invalid number of parameters: please run with the form './magic filename'\n");
    return -1;
  }

  char* filename = argv[1];

  elf64 main = elf64(filename);
  int result = main.open_file();
  if (result != 0) {
    return result;
  }
  
  main.setup_for_print();
  main.print_system_info();
  main.print_section_headers();
  main.print_symbols();
  
  return 0;
}
