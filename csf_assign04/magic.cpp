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

int main(int argc, char **argv) {
  // TODO: implement

  /*Ok, basically no penality for x64 only, i think we should do that and then generalise later*/
  /* like, once we figure out x64, it's trivial to just change everything to x86 */


  /*
  Overview:
    ELF header: Find obj file type, instruction set, endianess
    Section Header: print out name, type, offset, and size of each section
    
  
  
  */


  // try to open file
  // if cannot open, not an elf
  // get file information using fstat syscall
  // create mmapping
  // -----------------------------------------------------------------------------------------
  // decode header
    // collect obj file type, instruction set, endianness
    // for obj file type and instruction set:
      // translate e_type + e_machine  to strings with get_type_name and get_machine_name
    // for endianness -> access e_indent[ei_data] (offset 0x05), 1 = little, 2 = big
  // -----------------------------------------------------------------------------------------
  // Section headers
    // Note, if the section header is for SHT_SYMTAB, note the offset/address

    // find e_shoff (offset x28 from start)
    // note number of section headers in e_shnum (offset x3c)
    // go to e_shstrndx to find index of table entry with section names (offset x3e)
    // create object of type ELF64_Shdr
    // for loop
      // Section header
      // print N (verify in range 0 to e_shnum-1)
      // Name
        // offset pointer by sh_name
        // go to offset of string in .shstrtab, print name
        // return back to location
      // everything else (size, info, other) print with %lx -> sh_size, sh_type, sh_offset
  // ----------------------------------------------------------------------------------------------
  // Symbol information
  // find sh_offset of sht_symtab(maybe when we're iterating)
  // for loop
    // create obj of elf
    // Name
        // offset pointer by sh_name
        // go to offset of string in .shstrtab, print name
        // return back to location
    // print out size, info, other
}
