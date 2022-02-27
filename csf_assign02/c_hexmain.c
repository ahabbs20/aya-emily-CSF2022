// C implementation of hexdump main function
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

#include "hexfuncs.h"  // this is the only header file which may be included!
#include <unistd.h> 

int main(void) {
  // TODO: implement the main function

  char newLine[5] = "\n";
  char offset_colon[5] = ":";
  char single_space[5] = " ";
  char byte_as_hex[5];
  char input[20];
  
  char offset_string[10];
  unsigned int offset = 0;
  int num_read_charas = hex_read(input);
  
  while (num_read_charas != 0) {
    input[num_read_charas] = '\0';
    // format/print offset
    hex_format_offset(offset, offset_string);
    hex_write_string(offset_string);
    hex_write_string(offset_colon);
    hex_write_string(single_space);
    offset += 16;

    int counter = 0;
    // format/print hexdump
    for (int i = 0; i < 16; i++) {
      if (counter < num_read_charas) {
        hex_format_byte_as_hex(input[counter], byte_as_hex);
        input[counter] = hex_to_printable(input[counter]);
      } else {
        byte_as_hex[0] = ' ';
        byte_as_hex[1] = ' ';
        byte_as_hex[2] = '\0';
      }

      hex_write_string(byte_as_hex);
      hex_write_string(single_space);
      counter++;
    }

    hex_write_string(single_space);
    hex_write_string(input);
    hex_write_string(newLine);

    num_read_charas = hex_read(input);
  }

}
