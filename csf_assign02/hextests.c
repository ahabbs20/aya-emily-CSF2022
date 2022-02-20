// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  char test_data_2[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_2, "\n\n 123456789 \n");
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
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
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

}
