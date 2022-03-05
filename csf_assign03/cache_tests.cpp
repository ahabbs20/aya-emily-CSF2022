// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"

// test fixture object
typedef struct {

} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {

  return NULL;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();


  TEST_FINI();

  return 0;
}
