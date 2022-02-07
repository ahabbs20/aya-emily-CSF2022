#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  Fixedpoint newFixedPoint = (Fixedpoint) { .whole = whole, .frac = 0UL,
					    .sign = positive, .overflow = notOver,
					    .underflow = notUnder, .validity = valid};

  return newFixedPoint;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  Fixedpoint newFixedPoint = (Fixedpoint) { .whole = whole, .frac = frac,
					    .sign = positive, .overflow = notOver,
					    .underflow = notOver, .validity = valid};
    
  // set the value
  return newFixedPoint;
}


Fixedpoint fixedpoint_create_3(uint64_t whole, uint64_t frac, bool isNegative, int isValid) {
  int sign = isNegative;

  return (Fixedpoint) { .whole = whole, .frac = frac,
					    .sign = sign, .overflow = notOver,
					    .underflow = notUnder, .validity = isValid};

}


Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  bool isNegative = false;
  int start = 0;
  uint64_t whole = 0;
  uint64_t frac = 0;
  
  // check to see if negative
  if (*hex == '-') {
    isNegative = true;  
    start = 1;
  }

  char *end;
  whole = strtoul(hex + start, &end, 16);

  // check to see if whole is valid.
  if ((end - (hex + start)) > 16 || ((*end != '.') && (*end != '\0'))) {
    return fixedpoint_create_3(0UL, 0UL, isNegative, 1);
  }
  else if (*end == '.') {
    // create appropriate padding
    int length = strlen(end + 1);
    if (length < 16) {
      length = 16 - length;
    }

    start = (end - hex) + 1;

    frac = strtoul(hex + start, &end, 16);
    frac = frac << length * 4;

    if ((end - (hex + start)) > 16 || *end != '\0') {
      return fixedpoint_create_3(0UL, 0UL, isNegative, 1);
    }
  }

  return fixedpoint_create_3(whole, frac, isNegative, 0);

}

// emily = validate 
// It also includes uppercase, will edit later
// This should be fixed
bool validateChar(char toValidate) {
  bool isLowercase = toValidate >= 'a' && toValidate <= 'f';
  bool isUppercase = toValidate >= 'A' && toValidate <= 'F';
  bool isNumber = toValidate >= '0' && toValidate <= '9';

  if (isLowercase || isUppercase || isNumber) {
    return true;
  }

  return false;
}

// aya = update
uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
   return val.frac;
}

/* // basic outline for add/subtract
  1) check if signs are the same -> send to function that does magnitude addition
  2) check if signs are NOT the same -> send to function that does magnitude subtraction

  Let a = left, b = right
  let newWhole=0, newFrac

  // own function (all you need to know is their sign is the same, else it doesnt matter)
  // if the signs are both positive or both negative, it is magnitude addition
    // add fractions
    if (a.frac > a.frac + b.frac) { // carry over has occured
      newFrac = a.frac + b.frac;
      newWhole = 1;
    }
    // carry over

    // add whole
    if (a.whole > a.whole + b.whole) {
      // an overflow has occured, mark as overflows
    } else {
      newWhole += a.whole + b. whole
    }
    newFixedPoint (send to create_3)

  // this is itsown function
  if the signs are different, it is  magnitude subtraction
  let a be positive and b be negative
  if afrac < bfrac
    borrow from whole (+0x8000.....000UL)
    subtract as usual
  if a.whole < b.whole
    underflow
  else 
    subtract
    }

*/

// Emily
Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  
  if ((left.sign == negative && right.sign == negative) || (right.sign == positive && left.sign == positive)) {
    return fixedpoint_add_case_same_sign(left, right);
  } else if (left.sign == positive && right.sign == negative) {
    return fixedpoint_add_case_diff_sign(left, right);
  } else {
    return fixedpoint_add_case_diff_sign(right, left);
  }

}

Fixedpoint fixedpoint_add_case_diff_sign(Fixedpoint pos, Fixedpoint neg) {
  uint64_t newFrac = pos.frac - neg.frac;
  uint64_t newWhole = 0;
  uint64_t carry = 0;
  Fixedpoint toReturn;
  bool isNeg = false;

  if (pos.frac < neg.frac) {
    //Will become negative, magnitude of newWhole is switched 
    newFrac = neg.frac - pos.frac;
    carry = 1;
    //TODO: invert newFrac
  }
  
  if (pos.whole < neg.whole) {
    newWhole = neg.whole - pos.whole;
    isNeg = true;
  } else {
    newWhole = pos.whole - neg.whole;
  }

  if (newWhole > pos.whole) {
    //Overflow
    toReturn = fixedpoint_create_3(newWhole, newFrac, isNeg, 0);
    toReturn.overflow = over;
  } else {
    toReturn = fixedpoint_create_3(newWhole, newFrac, isNeg, 0);
  }
  return toReturn;
}

Fixedpoint fixedpoint_add_case_same_sign(Fixedpoint left, Fixedpoint right) {
  uint64_t newFrac = left.frac + right.frac;
  uint64_t newWhole = left.whole + right.whole;
  Fixedpoint toReturn;
  bool isNegative = right.sign == 1;

  if (newFrac < left.frac || newFrac < right.frac) {
    //TODO: fix frac part (overflow) 
    newWhole += 1;
  }

  if (newWhole < left.whole || newWhole < right.whole) {
    toReturn = fixedpoint_create_3(newWhole, newFrac, isNegative, 0);
    toReturn.overflow = over;
  } else {
    toReturn = fixedpoint_create_3(newWhole, newFrac, isNegative, 0);
  }
  return toReturn;
}

// Emily
Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  Fixedpoint negatedRight = fixedpoint_negate(right);
  return fixedpoint_add(left, negatedRight);
}

// aya
Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (fixedpoint_is_zero(val)) {
    return val;
  } else if (val.sign == positive) {
    val.sign = negative;
    return val;
  } else {
    val.sign = positive;
    return val;
  }
}

// Aya
Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // location of msb
  uint64_t mask = 1UL;
  uint64_t underflowing = 0;

  //printf("\nBefore: %lu.%lu    ", val.whole, val.frac);
  //uint64_t mostSignificantBit = 1 << val.frac;
  if ((mask & val.whole) == mask) {
    underflowing = 0x8000000000000000UL;
  }
  val.whole = val.whole >> 1;
  
  //mostSignificantBit = 1 << val.whole;

  if ((mask & val.frac) == mask) {
    val.underflow = under;
    return val;
  } else {
    val.frac = val.frac >> 1;
    val.frac += underflowing;
    //printf("Now: %lu.%lu\n", val.whole, val.frac);
    return val;
  }
}

// Aya
Fixedpoint fixedpoint_double(Fixedpoint val) {
  // location of msb
  uint64_t mask = 1UL << 63;
  uint64_t overflowing = 0;

  //uint64_t mostSignificantBit = 1 << val.frac;
  if ((mask & val.frac) == mask) {
    overflowing++;
  }
  val.frac = val.frac << 1;

  if ((mask & val.whole) == mask) {
    val.overflow = over;
    val.validity = invalid;
    return val;
  } else {
    val.whole = val.whole << 1;
    val.whole += overflowing;
    return val;
  }
}

// emily
int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  Fixedpoint subtracted = fixedpoint_sub(left, right);
  if (fixedpoint_is_zero(subtracted)) {
    return 0;
  }
  if (fixedpoint_is_neg(subtracted)) {
    return -1;
  }
  return 1;
}


int fixedpoint_is_zero(Fixedpoint val) {
  if ((val.validity == valid) && (val.whole == 0) && (val.frac == 0)) {
    return 1;
  } else {
    return 0;
  }
}

// aya
int fixedpoint_is_err(Fixedpoint val) {
  // TODO: implement
  return (val.overflow == over) || (val.underflow == under) || (val.validity == invalid);
}

// emily
int fixedpoint_is_neg(Fixedpoint val) {
  if ((val.validity == valid) && (val.sign == negative)) {
    return !fixedpoint_is_zero(val);
  }
  return 0;
}

// Aya
// Should this be val.overflow == over?
int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  return (val.overflow == over) && (val.sign == negative);;
}

// Emily
// Should this be val.overflow == over?
int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  return (val.overflow == over) && (val.sign == positive);
}

// Aya
int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  return (val.underflow == under) && (val.sign == negative);
}

// Emily
int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  return (val.underflow == under) && (val.sign == positive);
}

// Aya
int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  return val.validity == valid;
}



// Pair code together
char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}

