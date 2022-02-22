// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  char test_data_2[16];
  char test_data_3[16];
  char test_data_4[16];
  char test_data_5[3];
  char test_data_6[16];
  char test_data_7[3];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_2, "\n\n 123456789 \n");
  strcpy(objs->test_data_3, "~!@#$%^&*()_+=}");
  strcpy(objs->test_data_4, "\x1\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe"); //1-14 non printable
  strcpy(objs->test_data_5, "\x1f\x20"); //31 and 32 (range of printable is 32 and up inclusive)
  strcpy(objs->test_data_6, "\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c"); //126 and up (range of printable is up to and includes 126
  strcpy(objs->test_data_7, "\xfe\xff"); //Upper range of non-printable (254 and 255)
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  hex_format_offset(16L, buf);
  ASSERT(0 == strcmp(buf, "00000010"));

  hex_format_offset(8L, buf);
  ASSERT(0 == strcmp(buf, "00000008"));

  hex_format_offset(10L, buf);
  ASSERT(0 == strcmp(buf, "0000000a"));

  hex_format_offset(256L, buf);
  ASSERT(0 == strcmp(buf, "00000100"));

  hex_format_offset(0x11111111L, buf);
  ASSERT(0 == strcmp(buf, "11111111"));

  hex_format_offset(0xffffffffL, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  //Hello, world!\n
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48")); //H
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65")); //e
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c")); //l
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c")); //l
  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f")); //o
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c")); //,
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20")); // 
  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77")); //w
  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f")); //o
  hex_format_byte_as_hex(objs->test_data_1[9], buf);
  ASSERT(0 == strcmp(buf, "72")); //r
  hex_format_byte_as_hex(objs->test_data_1[10], buf);
  ASSERT(0 == strcmp(buf, "6c")); //l
  hex_format_byte_as_hex(objs->test_data_1[11], buf);
  ASSERT(0 == strcmp(buf, "64")); //d
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21")); //!
  hex_format_byte_as_hex(objs->test_data_1[13], buf);
  ASSERT(0 == strcmp(buf, "0a")); //\n

  //Non-printable chars < 32
  //0x01-0x0e
  hex_format_byte_as_hex(objs->test_data_4[0], buf);
  ASSERT(0 == strcmp(buf, "01"));
  hex_format_byte_as_hex(objs->test_data_4[1], buf);
  ASSERT(0 == strcmp(buf, "01"));
  hex_format_byte_as_hex(objs->test_data_4[2], buf);
  ASSERT(0 == strcmp(buf, "02"));
  hex_format_byte_as_hex(objs->test_data_4[3], buf);
  ASSERT(0 == strcmp(buf, "03"));
  hex_format_byte_as_hex(objs->test_data_4[4], buf);
  ASSERT(0 == strcmp(buf, "04"));
  hex_format_byte_as_hex(objs->test_data_4[5], buf);
  ASSERT(0 == strcmp(buf, "05"));
  hex_format_byte_as_hex(objs->test_data_4[6], buf);
  ASSERT(0 == strcmp(buf, "06"));
  hex_format_byte_as_hex(objs->test_data_4[7], buf);
  ASSERT(0 == strcmp(buf, "07"));
  hex_format_byte_as_hex(objs->test_data_4[8], buf);
  ASSERT(0 == strcmp(buf, "08"));
  hex_format_byte_as_hex(objs->test_data_4[9], buf);
  ASSERT(0 == strcmp(buf, "09"));
  //skip index- already tested 0a
  hex_format_byte_as_hex(objs->test_data_4[11], buf);
  ASSERT(0 == strcmp(buf, "0b"));
  hex_format_byte_as_hex(objs->test_data_4[12], buf);
  ASSERT(0 == strcmp(buf, "0c"));
  hex_format_byte_as_hex(objs->test_data_4[13], buf);
  ASSERT(0 == strcmp(buf, "0d"));
  hex_format_byte_as_hex(objs->test_data_4[14], buf);
  ASSERT(0 == strcmp(buf, "0e"));

  //Non-printable chars <= 32
  hex_format_byte_as_hex(objs->test_data_5[0], buf);
  ASSERT(0 == strcmp(buf, "1f"));
  hex_format_byte_as_hex(objs->test_data_5[1], buf);
  ASSERT(0 == strcmp(buf, "20"));

  //Non-printable chars >= 127
  // 7e to 8c
  hex_format_byte_as_hex(objs->test_data_6[0], buf);
  ASSERT(0 == strcmp(buf, "7e"));
  hex_format_byte_as_hex(objs->test_data_6[1], buf);
  ASSERT(0 == strcmp(buf, "7f"));
  hex_format_byte_as_hex(objs->test_data_6[2], buf);
  ASSERT(0 == strcmp(buf, "80"));
  hex_format_byte_as_hex(objs->test_data_6[3], buf);
  ASSERT(0 == strcmp(buf, "81"));
  hex_format_byte_as_hex(objs->test_data_6[4], buf);
  ASSERT(0 == strcmp(buf, "82"));
  hex_format_byte_as_hex(objs->test_data_6[5], buf);
  ASSERT(0 == strcmp(buf, "83"));
  hex_format_byte_as_hex(objs->test_data_6[6], buf);
  ASSERT(0 == strcmp(buf, "84"));
  hex_format_byte_as_hex(objs->test_data_6[7], buf);
  ASSERT(0 == strcmp(buf, "85"));
  hex_format_byte_as_hex(objs->test_data_6[8], buf);
  ASSERT(0 == strcmp(buf, "86"));
  hex_format_byte_as_hex(objs->test_data_6[9], buf);
  ASSERT(0 == strcmp(buf, "87"));
  hex_format_byte_as_hex(objs->test_data_6[10], buf);
  ASSERT(0 == strcmp(buf, "88"));
  hex_format_byte_as_hex(objs->test_data_6[11], buf);
  ASSERT(0 == strcmp(buf, "89"));
  hex_format_byte_as_hex(objs->test_data_6[12], buf);
  ASSERT(0 == strcmp(buf, "8a"));
  hex_format_byte_as_hex(objs->test_data_6[13], buf);
  ASSERT(0 == strcmp(buf, "8b"));
  hex_format_byte_as_hex(objs->test_data_6[14], buf);
  ASSERT(0 == strcmp(buf, "8c"));
}

void testHexToPrintable(TestObjs *objs) {
  //Hello, world!\n
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[2]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[3]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[4]));
  ASSERT(',' == hex_to_printable(objs->test_data_1[5]));
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('w' == hex_to_printable(objs->test_data_1[7]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[8]));
  ASSERT('r' == hex_to_printable(objs->test_data_1[9]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[10]));
  ASSERT('d' == hex_to_printable(objs->test_data_1[11]));
  ASSERT('!' == hex_to_printable(objs->test_data_1[12]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));

  //\n\n 123456789 \n
  ASSERT('.' == hex_to_printable(objs->test_data_2[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_2[1]));
  ASSERT(' ' == hex_to_printable(objs->test_data_2[2]));
  ASSERT('1' == hex_to_printable(objs->test_data_2[3]));
  ASSERT('2' == hex_to_printable(objs->test_data_2[4]));
  ASSERT('3' == hex_to_printable(objs->test_data_2[5]));
  ASSERT('4' == hex_to_printable(objs->test_data_2[6]));
  ASSERT('5' == hex_to_printable(objs->test_data_2[7]));
  ASSERT('6' == hex_to_printable(objs->test_data_2[8]));
  ASSERT('7' == hex_to_printable(objs->test_data_2[9]));
  ASSERT('8' == hex_to_printable(objs->test_data_2[10]));
  ASSERT('9' == hex_to_printable(objs->test_data_2[11]));
  ASSERT(' ' == hex_to_printable(objs->test_data_2[12]));
  ASSERT('.' == hex_to_printable(objs->test_data_2[13]));
  
  //~!@#$%^&*()_+=}{
  ASSERT('~' == hex_to_printable(objs->test_data_3[0]));
  ASSERT('!' == hex_to_printable(objs->test_data_3[1]));
  ASSERT('@' == hex_to_printable(objs->test_data_3[2]));
  ASSERT('#' == hex_to_printable(objs->test_data_3[3]));
  ASSERT('$' == hex_to_printable(objs->test_data_3[4]));
  ASSERT('%' == hex_to_printable(objs->test_data_3[5]));
  ASSERT('^' == hex_to_printable(objs->test_data_3[6]));
  ASSERT('&' == hex_to_printable(objs->test_data_3[7]));
  ASSERT('*' == hex_to_printable(objs->test_data_3[8]));
  ASSERT('(' == hex_to_printable(objs->test_data_3[9]));
  ASSERT(')' == hex_to_printable(objs->test_data_3[10]));
  ASSERT('_' == hex_to_printable(objs->test_data_3[11]));
  ASSERT('+' == hex_to_printable(objs->test_data_3[12]));
  ASSERT('=' == hex_to_printable(objs->test_data_3[13]));
  ASSERT('}' == hex_to_printable(objs->test_data_3[14]));

  //Lower range of non-printable
  for (int i = 0; i < 16; i++) {
    ASSERT('.' == hex_to_printable(objs->test_data_4[i]));
  }
  
  //31 is non printable and 32 is (lower edge case printable)
  ASSERT('.' == hex_to_printable(objs->test_data_5[0]));
  ASSERT(' ' == hex_to_printable(objs->test_data_5[1]));
  
  //126 is printable and >=127 is not (upper edge cases printable)
  ASSERT('~' == hex_to_printable(objs->test_data_6[0]));
  for (int i = 1; i < 16; i++) {
    ASSERT('.' == hex_to_printable(objs->test_data_6[i]));
  }
  
  //Upper range of non-printable
  ASSERT('.' == hex_to_printable(objs->test_data_7[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_7[1]));

}
