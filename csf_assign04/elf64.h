#ifndef ELF64_H
#define ELF64_H

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

class elf64 {
  public:
    elf64(char *filename);
    bool verify_if_elf();
    int open_file();
    void print_system_info();
    void print_section_headers();
    void print_symbols();
    void setup_for_print();

  private:
    char* filename;
    uint8_t * data_main = NULL;
    Elf64_Ehdr *elf_header = NULL;
    unsigned num_section_headers; //get number of section headers
    unsigned size_header;
    unsigned offset;
    Elf64_Shdr *section_header_table;
    char * name_table;
};

#endif