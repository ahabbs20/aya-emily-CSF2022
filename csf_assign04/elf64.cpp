#include "elf64.h"

elf64::elf64(char * filename) : filename(filename) { }

bool elf64::verify_if_elf() {
  if (elf_header->e_ident[EI_MAG0] != 0x7F) { return false; }
  else if (elf_header->e_ident[EI_MAG1] != 0x45) { return false; }
  else if (elf_header->e_ident[EI_MAG2] != 0x4c) { return false; }
  else if (elf_header->e_ident[EI_MAG3] != 0x46) { return false; }
  return true;
}

int elf64::validating_file(int fd) {
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  void * data = NULL;
  if (rc != 0) {
    printf("Error: unable to figure out how many bytes the file has. Please restart.\n");
    return -3;
  } else {
    size_t file_size = statbuf.st_size;
    data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
      printf("Error: bad memory region\n");
      return -4;
    }
    data_main = (uint8_t *)data;
    //Else the pointer value points to a region of mem in which the program can access file contents
  }
  return 0;
}

int elf64::open_file() {
  fd = open(filename, O_RDONLY);

  if (fd < 0) {
    printf("File cannot be open: check filename to ensure it is correct\n");
    return -2;
  }

  int result = validating_file(fd); // validate file is correct
  if (result < 0) {                 // error was encountered
    return result;
  }
  
  elf_header = (Elf64_Ehdr *)data_main; //get the header

  if (!verify_if_elf()) {
    printf("Not an ELF file\n");
    return -5;
  }
  
  return 0;
}

void elf64::print_system_info() {
  printf("Object file type: %s\nInstruction set: %s\n", get_type_name(elf_header->e_type), get_machine_name(elf_header->e_machine));
  if (elf_header->e_ident[EI_DATA] == 1) {
    printf("Endianness: Little endian\n");
  } else {
    printf("Endianness: Big endian\n");
  }
}

void elf64::setup_for_print() {
  num_section_headers = elf_header->e_shnum; //get number of section headers
  size_header = elf_header->e_shentsize;
  offset = elf_header->e_shoff;
  section_header_table = (Elf64_Shdr *) (data_main + offset);
  name_table = (char*) (data_main + section_header_table[elf_header->e_shstrndx].sh_offset);
}

void elf64::print_section_headers() {
  Elf64_Shdr current;

  for (uint8_t i = 0; i < num_section_headers; i++) {
    current = section_header_table[i];

    if (current.sh_type == SHT_SYMTAB) {
      symtab_info = current;
    } 

    if (strcmp(name_table + current.sh_name, ".strtab") == 0) {
      strtab_info = current;
    } 
   
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", i, name_table + current.sh_name, 
                                  (long unsigned int) current.sh_type, current.sh_offset, current.sh_size);
  }
}

void elf64::print_symbols() {
  Elf64_Sym *symtab = (Elf64_Sym *) (data_main + symtab_info.sh_offset);
  char * str_table = (char  *) (data_main + strtab_info.sh_offset);
  Elf64_Sym current_symbol;

  for (uint8_t i = 0; i < symtab_info.sh_size / symtab_info.sh_entsize; i++) {
    current_symbol = symtab[i];

    printf("Symbol %d: name=%s, size=%lx, info=%lx, other=%lx\n", i, str_table + current_symbol.st_name, 
                                                                 current_symbol.st_size, 
                                                                 (long unsigned int) current_symbol.st_info, 
                                                                 (long unsigned int) current_symbol.st_other);
  }

  end_file(); // close file
}

void elf64::end_file() {
    close(fd);
}
