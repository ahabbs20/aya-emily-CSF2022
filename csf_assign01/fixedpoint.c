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

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  bool isNegative = false;
  int start = 0;
  int endOfWhole = 0;
  uint64_t whole = 0;
  uint64_t frac = 0;
  bool isValid = true;

  if (*hex == '-') {
    isNegative = true;
    start = 1;
  }


  int i = 0;
  while ((*(hex + i) != '.') || (*(hex + i) != '/0')) {
    i++;
  }

  endOfWhole = i - 1;

  for (i = endOfWhole; i >= start; i--) {
    isValid = validateChar(*(hex + i));

    if (!isValid) {
      whole = 0;
      break;
    }

    //update

  }
  
  if (isValid && (*(hex + endOfWhole + 1) == '.')) {
    for (i = endOfWhole + 2; *(hex + i) != '/0'; i++) {
      isValid = validateChar(*(hex + i));

      if (!isValid) {
        frac = 0;
        whole = 0;
        break;
      }

      //update

    }
  }


  // GET RID OF LATER
  return create_fixedpoint_hex(isValid, whole, frac, isNegative);
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


Fixedpoint create_fixedpoint_hex(bool isValid, uint64_t whole, uint64_t frac, bool isNegative) {

  return DUMMY;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
   return val.frac;
}

// Emily
Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
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
  assert(0);
  return DUMMY;
}

// Aya
Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

// Aya
Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
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
    return 1;
  }
  return 0;
}

// Aya
int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  return (val.overflow == under) && (val.sign == negative);;
}

// Emily
int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  return (val.overflow == under) && (val.sign == positive);
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
