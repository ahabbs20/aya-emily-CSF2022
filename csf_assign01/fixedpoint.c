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
  uint64_t left_frac = left.frac;
  uint64_t right_frac = right.frac;
  uint64_t frac_sum = left_frac + right_frac;

  uint64_t left_whole = left.whole;
  uint64_t right_whole = right.whole;
  uint64_t whole_sum = left_whole + right_whole;

  Fixedpoint sum;
  
  if (left.sign == negative && right.sign == negative) { // Note: The same as positive.
    bool isOverflow = false;
    if (frac_sum < right_frac || frac_sum < left_frac) {
      whole_sum += 1;
    }
    if (whole_sum < right_whole || whole_sum < left_whole) {
      isOverflow = true;
    }

    if (isOverflow) {
      sum = (Fixedpoint) { .whole = whole_sum, .frac = frac_sum,  // you can just use the sign of one
        .sign = negative, .overflow = over, .underflow = notUnder,
        .validity = valid};
    } else {
      sum = (Fixedpoint) { .whole = whole_sum, .frac = frac_sum,
        .sign = negative, .overflow = notOver, .underflow = notUnder,
        .validity = valid};
    }
    
  } else if (left.sign == positive && right.sign == positive) {
    bool isOverflow = false;
    if (frac_sum < right_frac || frac_sum < left_frac) {
      whole_sum += 1;
    } 

    if (whole_sum < right_whole || whole_sum < left_whole) {
      isOverflow = true;
    }

    if (isOverflow) {
      sum = (Fixedpoint) { .whole = whole_sum, .frac = frac_sum,
        .sign = positive, .overflow = over, .underflow = notUnder,
	.validity = valid};
    } else {
      sum = (Fixedpoint) { .whole = whole_sum, .frac = frac_sum, 
        .sign = positive, .overflow = notOver, .underflow = notUnder, 
        .validity = valid};
    }
  } else if (left.sign == positive && right.sign == negative) { // here you can just create a function with func(pos, neg) and thus no confusion
    bool isOverflow = false;	  
    frac_sum = left_frac - right_frac;
    whole_sum = left_whole - right_whole;
    
    if (whole_sum > 0 && frac_sum < 0) { 
      whole_sum -= 1;
      //TODO: figure out how to invert frac_sum 
    } else if (whole_sum < 0 && frac_sum > 0) {
      whole_sum *= -1;
      whole_sum -= 1;
      //TODO: figure out how to invert frac_sum
    } else if (whole_sum < 0 && frac_sum < 0) {
      whole_sum *= -1;
      frac_sum *= -1;
    }



  } else if (left.sign == negative && right.sign == positive) {
    bool isOverflow = false;
    frac_sum = right_frac - left_frac;
    whole_sum = right_whole - left_whole;
  } 


  return sum;
}

// Emily
Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

// aya
Fixedpoint fixedpoint_negate(Fixedpoint val) {
  // TODO: implement
  val.sign = negative;
  return val;
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

  //printf("\nBefore: %lu.%lu    ", val.whole, val.frac);
  //uint64_t mostSignificantBit = 1 << val.frac;
  if ((mask & val.frac) == mask) {
    overflowing++;
  }
  val.frac = val.frac << 1;
  
  //mostSignificantBit = 1 << val.whole;

  if ((mask & val.whole) == mask) {
    val.overflow = over;
    return val;
  } else {
    val.whole = val.whole << 1;
    val.whole += overflowing;
    //printf("Now: %lu.%lu\n", val.whole, val.frac);
    return val;
  }

  // TODO: implement
}

// emily
int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
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

