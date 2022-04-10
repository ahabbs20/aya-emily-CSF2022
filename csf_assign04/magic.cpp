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


bool verify_if_elf(Elf64_Ehdr* elf_header) {
  if (elf_header->e_ident[EI_MAG0] != 0x7F) { return false; }
  else if (elf_header->e_ident[EI_MAG1] != 0x45) { return false; }
  else if (elf_header->e_ident[EI_MAG2] != 0x4c) { return false; }
  else if (elf_header->e_ident[EI_MAG3] != 0x46) { return false; }
  return true;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Invalid number of parameters: please run with the form './magic filename'\n");
    return 0;
  }

  char* filename = argv[1];

  int fd = open(filename, O_RDONLY);

  if (fd < 0) {
    printf("File cannot be open: check filename to ensure it is correct\n");
    return -1;
  }

  struct stat statbuf;
  size_t file_size = 0;
  int rc = fstat(fd, &statbuf);
  uint8_t* data_main = NULL;

  if (rc != 0) {
    printf("Error: unable to figure out how many bytes the file has. Please restart.\n");
    return 0;
  } else {
    file_size = statbuf.st_size;
    void* data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
      printf("Error: bad memory region\n");
      return -2;
    }
    data_main = (uint8_t *)data;
    //Else the pointer value points to a region of mem in which the program can access file contents
  }


  Elf64_Ehdr *elf_header = (Elf64_Ehdr *)data_main; //get the header

  if (!verify_if_elf(elf_header)) {
    printf("Not an ELF file");
    return -3;
  }

  /*Print out file info*/
  printf("Object file type: %s\nInstruction set: %s\n", get_type_name(elf_header->e_type), get_machine_name(elf_header->e_machine));
  if (elf_header->e_ident[EI_DATA] == 1) {
    printf("Endianness: Little endian\n");
  } else {
    printf("Endianness: Big endian\n");
  }

  /*Section header code*/
  unsigned num_section_headers = elf_header->e_shnum; //get number of section headers
  unsigned size_header = elf_header->e_shentsize;
  unsigned offset = elf_header->e_shoff;

  Elf64_Shdr *section_header_table = (Elf64_Shdr *) (data_main + offset);
  char * name_table = (char*) (data_main + section_header_table[elf_header->e_shstrndx].sh_offset);

  Elf64_Shdr current = section_header_table[0];
  Elf64_Shdr symtab_info;
  unsigned strtab_offset;

  // For modularity, we would pass in those 3 parameters above as pointers, so that we can get them back later

  for (uint8_t i = 0; i < num_section_headers; i++) {
    current = section_header_table[i];

    if (current.sh_type == SHT_SYMTAB) {
      symtab_info = current;
    } else if (current.sh_type == SHT_STRTAB) {
      strtab_offset = current.sh_offset; // get offset to the string table
    }
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", i, name_table + current.sh_name, (long unsigned int) current.sh_type, current.sh_offset, current.sh_size);
  }
  
  /*Symbol Code*/
  Elf64_Sym *symtab = (Elf64_Sym *) (data_main + symtab_info.sh_offset);
  char * str_table = (char*) (data_main + strtab_offset); // find string table, something wrong with this...
  Elf64_Sym current_symbol;

  for (uint8_t i = 0; i < symtab_info.sh_size / symtab_info.sh_entsize; i++) {
    current_symbol = symtab[i];

    printf("Symbol %d: name=%s, size=%lx, info=%lx, other=%lx\n", i, str_table + current_symbol.st_name /*For some reason it's cutting into shstrtab*/, 
                                                                 current_symbol.st_size, 
                                                                 (long unsigned int) current_symbol.st_info, 
                                                                 (long unsigned int) current_symbol.st_other);
  }

  return 0;
}
