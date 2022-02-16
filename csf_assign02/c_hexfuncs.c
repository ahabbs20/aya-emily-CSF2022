// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here
// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[]) {
    // read(1 (0 = std.in), data_buf, 16);
    return read(0, data_buf, 16);
}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]) {
    // write by character until null terminator
    int throw = write(1, s, 32);
    throw++; // to get rid of compiler issue
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[]) {
    // can just initialise no?
    long num = offset;
    unsigned mask = 0xf;
    int index = 7;
    
    for (int i = 0; i < 8; i++) {
        unsigned temp = num & mask;
        

        // if between 0 and 9, add 30 and convert to hex.
        if (temp >= 0 && temp <= 9) {
            sbuf[index--] = (char) temp + 48L;
        } else if (temp >= 10 && temp <= 15) {
            sbuf[index--] = (char) temp + 87L;
        }

        num = num >> 4;
    }

    sbuf[8] = '\0';

}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    // can just initialise no?
    long num = byteval;
    unsigned mask = 0xf;
    int index = 1;
    
    for (int i = 0; i < 2; i++) {
        unsigned temp = num & mask;

        // if between 0 and 9, add 30 and convert to hex.
        if (temp >= 0 && temp <= 9) {
            sbuf[index--] = (char) temp + 48L;
        } else if (temp >= 10 && temp <= 15) {
            sbuf[index--] = (char) temp + 87L;
        }

        num = num >> 4;
    }

    sbuf[2] = '\0';
}

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval) {
    if (byteval >= 0x20 && byteval <= 0x7e) {
        return byteval;
    } else {
        return '.';
    }
}