#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;

  // TODO: add more objects to the test fixture
} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_double(TestObjs *objs);
void test_halve(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
void test_compare(TestObjs *objs);
// TODO: add more test functions

int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_compare);

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_is_overflow_pos);
  TEST(test_is_err);
  TEST(test_double);
  TEST(test_halve);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_compare(TestObjs *objs) {
  // Let a.b, c.d

  // a.b == c.d
  Fixedpoint equal1 = fixedpoint_create2(1UL, 0x8000000000000000UL); //1.5
  Fixedpoint equal2 = fixedpoint_create2(1UL, 0x8000000000000000UL); // 1.5
  ASSERT(0 == fixedpoint_compare(equal1, equal2));

  // a < c, b == d
  Fixedpoint compare1 = fixedpoint_create2(1UL, 0x8000000000000000UL); //1.5
  Fixedpoint compare2 = fixedpoint_create2(2UL, 0x8000000000000000UL); // 1.5
  ASSERT(-1  == fixedpoint_compare(compare1, compare2));

  // a < c, b != d
  Fixedpoint compare3 = fixedpoint_create2(1UL, 0x8000000000000000UL); //1.5
  Fixedpoint compare4 = fixedpoint_create2(2UL, 0x4000000000000000UL); // 1.5
  ASSERT(-1  == fixedpoint_compare(compare3, compare4));

  // a > c, b == d
  Fixedpoint compare5 = fixedpoint_create2(2UL, 0x8000000000000000UL); //1.5
  Fixedpoint compare6 = fixedpoint_create2(1UL, 0x8000000000000000UL); // 1.5
  ASSERT(1  == fixedpoint_compare(compare5, compare6));

  // a > c, b != d
  Fixedpoint compare7 = fixedpoint_create2(2UL, 0x8000000000000000UL); //1.5
  Fixedpoint compare8 = fixedpoint_create2(1UL, 0x4000000000000000UL); // 1.5
  ASSERT(1  == fixedpoint_compare(compare7, compare8));

  // a==c, b > d
  Fixedpoint compare9 = fixedpoint_create2(1UL, 0x8000000000000000UL); //1.5
  Fixedpoint compare10 = fixedpoint_create2(1UL, 0x4000000000000000UL); // 1.5
  ASSERT(1  == fixedpoint_compare(compare9, compare10));

  // a==c, b < d
  Fixedpoint compare11 = fixedpoint_create2(1UL, 0x4000000000000000UL); //1.25
  Fixedpoint compare12 = fixedpoint_create2(1UL, 0x8000000000000000UL); // 1.5
  ASSERT(-1  == fixedpoint_compare(compare11, compare12));

  // -a.b < +c.d
  Fixedpoint compare13 = fixedpoint_create_from_hex("-1.8");
  Fixedpoint compare14 = fixedpoint_create_from_hex("1.8");
  ASSERT(-1  == fixedpoint_compare(compare13, compare14));

  // a.b > -c.d
  Fixedpoint compare15 = fixedpoint_create_from_hex("1.8");
  Fixedpoint compare16 = fixedpoint_create_from_hex("-1.8");
  ASSERT(1  == fixedpoint_compare(compare15, compare16));

  // -a.b == -c.d
  Fixedpoint compare17 = fixedpoint_create_from_hex("1.8");
  Fixedpoint compare18 = fixedpoint_create_from_hex("-1.8");
  ASSERT(1  == fixedpoint_compare(compare17, compare18));
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  // Note, the number is 258627685.242
  (void) objs;

  //Might've found an error here with representation although i'm not sure
  Fixedpoint val0 = fixedpoint_create_from_hex("d73533cd5.ffb28a49");
  ASSERT(fixedpoint_is_valid(val0));
  ASSERT(0xd73533cd5UL == fixedpoint_whole_part(val0));
  ASSERT(0xffb28a4900000000UL == fixedpoint_frac_part(val0));

  // Format X.Y
  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));


  // Format -X.Y
  Fixedpoint val2 = fixedpoint_create_from_hex("-f6a5865.00f2");
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(fixedpoint_is_neg(val2));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val2));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val2));

  // Format X
  Fixedpoint val3 = fixedpoint_create_from_hex("f6a5865");
  ASSERT(fixedpoint_is_valid(val3));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val3));
  ASSERT(0x0UL == fixedpoint_frac_part(val3));

  // Format -X
  Fixedpoint val4 = fixedpoint_create_from_hex("-f6a5865");
  ASSERT(fixedpoint_is_valid(val4));
  ASSERT(fixedpoint_is_neg(val4));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val4));
  ASSERT(0x0UL == fixedpoint_frac_part(val4));

  // mix of upper and lower case
  Fixedpoint val5 = fixedpoint_create_from_hex("f6A5865.00F2");
  ASSERT(fixedpoint_is_valid(val5));
  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val5));
  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val5));
  
  // invalid letters in the whole portion
  Fixedpoint val6 = fixedpoint_create_from_hex("f6Z5865.002");
  ASSERT(!fixedpoint_is_valid(val6));
  ASSERT(0x0UL == fixedpoint_whole_part(val6));
  ASSERT(0x0UL == fixedpoint_frac_part(val6));

  // invalid letters in the frac portion
  Fixedpoint val7 = fixedpoint_create_from_hex("f6A5865.00Z2");
  ASSERT(!fixedpoint_is_valid(val7));
  ASSERT(0x0UL == fixedpoint_whole_part(val7));
  ASSERT(0x0UL == fixedpoint_frac_part(val7));
}

void test_format_as_hex(TestObjs *objs) {
  
  char *s;
  
  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
  
  // formatting negative numbers
  s = fixedpoint_format_as_hex(fixedpoint_create_from_hex("-1.8"));
  ASSERT(0 == strcmp(s, "-1.8"));
  free(s);
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;
  
  Fixedpoint lhs, rhs, sum;
  lhs = fixedpoint_create2(0UL, 0UL);
  rhs = fixedpoint_create2(0UL, 0UL);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 0UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create2(1UL, 0UL);
  rhs = fixedpoint_create2(1UL, 0UL);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 2UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create2(0UL, 0x4000000000000000UL);
  rhs = fixedpoint_create2(0UL, 0x4000000000000000UL);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 0UL);
  ASSERT(sum.frac == 0x8000000000000000UL);

  lhs = fixedpoint_create2(0UL, 0x8000000000000000UL);
  rhs = fixedpoint_create2(0UL, 0x8000000000000000UL);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 1UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create2(1UL, 0x8000000000000000UL);
  rhs = fixedpoint_create2(1UL, 0x8000000000000000UL);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 3UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create2(0x8000000000000000UL, 0UL);
  rhs = fixedpoint_create2(0x8000000000000000UL, 0UL);
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.overflow == over);
  ASSERT(fixedpoint_is_err(sum));
  ASSERT(sum.validity == invalid);
  ASSERT(sum.whole == 0UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create_from_hex("2");
  rhs = fixedpoint_create_from_hex("-1");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 1UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create_from_hex("0");
  rhs = fixedpoint_create_from_hex("-1");
  sum = fixedpoint_add(lhs, rhs);
  //ASSERT(sum.overflow == over);
  //ASSERT(sum.sign == negative);
  //ASSERT(fixedpoint_is_err(sum));
  //ASSERT(sum.validity == invalid);

  lhs = fixedpoint_create_from_hex("1.8");
  rhs = fixedpoint_create_from_hex("-0.8");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 1UL);
  ASSERT(sum.frac == 0UL);

  lhs = fixedpoint_create_from_hex("2.8"); // 2,5
  rhs = fixedpoint_create_from_hex("-1.C"); // 1.75
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(sum.whole == 0UL);
  ASSERT(sum.frac == 0xC000000000000000UL);

  lhs = fixedpoint_create_from_hex("8bd.0e34492025065");
  rhs = fixedpoint_create_from_hex("5d7b061d6.034f5d");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0x5d7b06a93UL == fixedpoint_whole_part(sum));
  ASSERT(0x1183a62025065000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-8a6a9f92d72.82a9b99ad4e76");
  rhs = fixedpoint_create_from_hex("-8a93a62c25996.e09875");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x8b1e10cbb8709UL == fixedpoint_whole_part(sum));
  ASSERT(0x63422e9ad4e76000UL == fixedpoint_frac_part(sum));
  
  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("d73533cd5.ffb28a49");
  rhs = fixedpoint_create_from_hex("3f7d.5f7f3e");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0xd73537c53UL == fixedpoint_whole_part(sum));
  ASSERT(0x5f31c84900000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("c2d77fa53.cf54ab3370");
  rhs = fixedpoint_create_from_hex("-4140b33626.7e6b14572c73");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x35133b3bd2UL == fixedpoint_whole_part(sum));
  ASSERT(0xaf166923bc730000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-4bd9aec72311.4f644e51c");
  rhs = fixedpoint_create_from_hex("-9391f.bc14cca1d00");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x4bd9aed05c31UL == fixedpoint_whole_part(sum));
  ASSERT(0x0b791af390000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("3c1ebf3e8.675db");
  rhs = fixedpoint_create_from_hex("-baa23c.cda121a90b4");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0x3c13151abUL == fixedpoint_whole_part(sum));
  ASSERT(0x99bc8e56f4c00000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-9a43d.a2b77");
  rhs = fixedpoint_create_from_hex("7fdd7fb5.aa727c4c21");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0x7fd3db78UL == fixedpoint_whole_part(sum));
  ASSERT(0x07bb0c4c21000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-6ebb455cf.b");
  rhs = fixedpoint_create_from_hex("583933917180fb.39");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(0x58392ca5bd2b2bUL == fixedpoint_whole_part(sum));
  ASSERT(0x8900000000000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-d5206e.8");
  rhs = fixedpoint_create_from_hex("01.3fdc8e4ed");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xd5206dUL == fixedpoint_whole_part(sum));
  ASSERT(0x402371b130000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create2(UINT64_MAX, 0UL);
  rhs = fixedpoint_create_from_hex("1.0");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_pos(sum) == 1);
  
  lhs = fixedpoint_create2(UINT64_MAX, 0UL);
  rhs = fixedpoint_create_from_hex("1.0");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_pos(sum) == 1);
}

void test_double(TestObjs *objs) {
  (void) objs;
  // doubling 1 makes 2
  Fixedpoint doubled_one = fixedpoint_double(objs->one);
  ASSERT(2UL == doubled_one.whole);
  ASSERT(0UL == doubled_one.frac);
  
  // doubling 0 makes 0
  Fixedpoint doubled_zero = fixedpoint_double(objs->zero);
  ASSERT(0UL == doubled_zero.whole);
  ASSERT(0UL == doubled_zero.frac);

  // doubling .25 makes .5
  Fixedpoint doubled_one_fourth = fixedpoint_double(objs->one_fourth);
  ASSERT(0UL == doubled_one_fourth.whole);
  ASSERT(0x8000000000000000UL == doubled_one_fourth.frac);

  // doubling negatives does not change sign
  doubled_one = objs->one;
  doubled_one = fixedpoint_negate(doubled_one);
  doubled_one = fixedpoint_double(doubled_one);
  ASSERT(2UL == doubled_one.whole);
  ASSERT(0UL == doubled_one.frac);
  ASSERT(fixedpoint_is_neg(doubled_one));
  
  // double carries over into whole
  // 0 + 2(.5)
  Fixedpoint doubled_one_half = fixedpoint_double(objs->one_half);
  ASSERT(1UL == doubled_one_half.whole);
  ASSERT(0UL == doubled_one_half.frac);
  
  // doubling with both whole and frac
  Fixedpoint doubled_real1 = fixedpoint_create2(1UL, 0x4000000000000000UL);
  doubled_real1 = fixedpoint_double(doubled_real1);
  ASSERT(2UL == doubled_real1.whole);
  ASSERT(0x8000000000000000UL == doubled_real1.frac);

  Fixedpoint doubled_real2 = fixedpoint_create2(1UL, 0x4000000000000000UL);
  doubled_real2 = fixedpoint_double(doubled_real2);
  ASSERT(2UL == doubled_real2.whole);
  ASSERT(0x8000000000000000UL == doubled_real2.frac);

  // double with carry over onto non-zero whole
  Fixedpoint doubled_real3 = fixedpoint_create2(1UL, 0x8000000000000000UL);
  doubled_real3 = fixedpoint_double(doubled_real3);
  ASSERT(3UL == doubled_real3.whole);
  ASSERT(0UL == doubled_real3.frac);

  // double cutting off whole
  Fixedpoint doubled_real4 = fixedpoint_create2(0x8000000000000000UL, 0);
  doubled_real4 = fixedpoint_double(doubled_real4);
  ASSERT(doubled_real4.overflow == over);
  ASSERT(doubled_real4.validity == invalid);
  // max * 2, note is cut off.

  
}

void test_halve(TestObjs *objs) {
  // 2 becomes 1
  Fixedpoint two = fixedpoint_create2(2UL, 0UL);
  Fixedpoint halved_two = fixedpoint_halve(two);
  ASSERT(halved_two.whole == 1UL);
  ASSERT(halved_two.frac == 0UL);

  // 0 becomes 0
  Fixedpoint halved_zero = objs->zero;
  halved_zero = fixedpoint_halve(halved_zero);
  ASSERT(halved_zero.whole == 0UL);
  ASSERT(halved_zero.frac == 0UL);

  // .5 becomes .25
  Fixedpoint halved_half = objs->one_half;
  halved_half = fixedpoint_halve(halved_half);
  ASSERT(halved_half.whole == 0UL);
  ASSERT(halved_half.frac == objs->one_fourth.frac);

  // does not change sign
  halved_half = objs->one_half;
  halved_half = fixedpoint_negate(halved_half);
  halved_half = fixedpoint_halve(halved_half);
  ASSERT(halved_half.whole == 0UL);
  ASSERT(halved_half.frac == objs->one_fourth.frac);
  ASSERT(fixedpoint_is_neg(halved_half));

  // 1 becomes .5
  Fixedpoint halved_one = objs->one;
  halved_one = fixedpoint_halve(halved_one);
  ASSERT(halved_one.whole == 0UL);
  ASSERT(halved_one.frac == objs->one_half.frac);
  
  // 3 becomes 1.5
  Fixedpoint halved_real3 = fixedpoint_create2(3UL, 0UL);
  halved_real3 = fixedpoint_halve(halved_real3);
  ASSERT(1UL == halved_real3.whole);
  ASSERT(0x8000000000000000UL == halved_real3.frac);

  // 1.5 becomes .75
  // lsb = 1 --> underflow
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, difference;
  
  // 0-0 = +0
  lhs = fixedpoint_create_from_hex("0");
  rhs = fixedpoint_create_from_hex("0");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(difference.whole == 0UL);
  ASSERT(difference.frac == 0UL);
  ASSERT(difference.sign == positive);

  lhs = fixedpoint_create_from_hex("-0");
  rhs = fixedpoint_create_from_hex("0");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(difference.whole == 0UL);
  ASSERT(difference.frac == 0UL);
  ASSERT(difference.sign == positive);

  lhs = fixedpoint_create_from_hex("8000000000000000");
  rhs = fixedpoint_create_from_hex("-8000000000000000");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_overflow_pos(difference));

  lhs = fixedpoint_create_from_hex("3af7b9110.3ebd3659");
  rhs = fixedpoint_create_from_hex("-75bc9.ff0cd2e23");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(strcmp(fixedpoint_format_as_hex(difference), "3af82ecda.3dca093b3") == 0);
  
  lhs = fixedpoint_create_from_hex("-8073dc.8b28fcb78f4397");
  rhs = fixedpoint_create_from_hex("7a2e1a6e.6f6c");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(strcmp(fixedpoint_format_as_hex(difference), "-7aae8e4a.fa94fcb78f4397") == 0);

  lhs = fixedpoint_create_from_hex("e680ce0.81f0c6d5300781");
  rhs = fixedpoint_create_from_hex("2b8c52586163693.9fb0fc");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(strcmp(fixedpoint_format_as_hex(difference), "-2b8c52577ae29b3.1dc0352acff87f") == 0);

  lhs = fixedpoint_create_from_hex("fe6d540ffc7.18b6f2c");
  rhs = fixedpoint_create_from_hex("-bb68ca1c9cb4d58.e21de8");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(strcmp(fixedpoint_format_as_hex(difference), "bb69c889f0c4d1f.fad4dac") == 0);

  lhs = fixedpoint_create_from_hex("39874e409c23.bec45ec7ab92");
  rhs = fixedpoint_create_from_hex("4045.e41");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(strcmp(fixedpoint_format_as_hex(difference), "39874e405bdd.dab45ec7ab92") == 0);

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(difference));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(difference));

  lhs = fixedpoint_create_from_hex("-d50.28");
  rhs = fixedpoint_create_from_hex("33.ff266b0133cd");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0xd84UL == fixedpoint_whole_part(difference));
  ASSERT(0x27266b0133cd0000UL == fixedpoint_frac_part(difference));
  
  lhs = fixedpoint_create_from_hex("2f397b3cbe1126c.bfba75");
  rhs = fixedpoint_create_from_hex("21df427cd.45b45938ad897");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(difference));
  ASSERT(0x2f397b1adecea9fUL == fixedpoint_whole_part(difference));
  ASSERT(0x7a061bc752769000UL == fixedpoint_frac_part(difference));
  
  lhs = fixedpoint_create_from_hex("-8f477ca57.57003bd70d3a3");
  rhs = fixedpoint_create_from_hex("f66d.dc9c56f5c0d1e");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0x8f478c0c5UL == fixedpoint_whole_part(difference));
  ASSERT(0x339c92ccce0c1000UL == fixedpoint_frac_part(difference));

  lhs = fixedpoint_create_from_hex("-281b.d2508833530");
  rhs = fixedpoint_create_from_hex("ff094684.eac");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0xff096ea0UL == fixedpoint_whole_part(difference));
  ASSERT(0xbd10883353000000UL == fixedpoint_frac_part(difference));
  
  lhs = fixedpoint_create_from_hex("45618b.9f00");
  rhs = fixedpoint_create_from_hex("6cc9aab1dfd751.270d79c77d0977");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0x6cc9aab19a75c5UL == fixedpoint_whole_part(difference));
  ASSERT(0x880d79c77d097700UL == fixedpoint_frac_part(difference));

  lhs = fixedpoint_create_from_hex("-6ce49aa876ce6eb.62514dc6a31b0f");
  rhs = fixedpoint_create_from_hex("82b2984c7.f837296ab");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0x6ce49b2b2966bb3UL == fixedpoint_whole_part(difference));
  ASSERT(0x5a887731531b0f00UL == fixedpoint_frac_part(difference));

  lhs = fixedpoint_create_from_hex("-899cfb.105");
  rhs = fixedpoint_create_from_hex("-0ae46500c3a74b.cbfd61d68ecb911");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(difference));
  ASSERT(0xae465003a0a50UL == fixedpoint_whole_part(difference));
  ASSERT(0xbbad61d68ecb9110UL == fixedpoint_frac_part(difference));

  lhs = fixedpoint_create_from_hex("-46a.f");
  rhs = fixedpoint_create_from_hex("09eed6418b2.c9c7b189");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0x9eed641d1dUL == fixedpoint_whole_part(difference));
  ASSERT(0xb9c7b18900000000UL == fixedpoint_frac_part(difference));

  lhs = fixedpoint_create_from_hex("-e45c3a00c0928.f842b");
  rhs = fixedpoint_create_from_hex("-058.12d4a2325a496eb");
  difference = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(difference));
  ASSERT(0xe45c3a00c08d0UL == fixedpoint_whole_part(difference));
  ASSERT(0xe56e0dcda5b69150UL == fixedpoint_frac_part(difference));
  
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;


  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));
  
  
  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  
  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  
  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  
  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  
  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
  
}

// TODO: implement more test functions
