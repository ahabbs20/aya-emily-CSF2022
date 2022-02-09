#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

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

int compare_magnitudes(Fixedpoint left, Fixedpoint right) {
  if (left.whole > right.whole) {
    return 1;
  } else if (left.whole < right.whole) {
    return -1;
  } else {
    if (left.frac > right.frac) {
      return 1;
    } else if (left.frac < right.frac) {
      return -1;
    } else {
      return 0;
    }
  }
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

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
   return val.frac;
}

Fixedpoint magnitude_addition(Fixedpoint left, Fixedpoint right) {
  uint64_t fracSum = left.frac + right.frac;

  uint64_t wholeSum = left.whole + right.whole;

  if (wholeSum < left.whole) { // overflow with addition
    Fixedpoint newFP =  fixedpoint_create_3(wholeSum, fracSum, left.sign, invalid);
    newFP.overflow = over; 
    return newFP;
  }
  
  if (fracSum < left.frac) {
    if (wholeSum + 1 < wholeSum) { // overflow with addition
      Fixedpoint newFP =  fixedpoint_create_3(wholeSum, fracSum, left.sign, invalid);
      newFP.overflow = over; 
      return newFP;
    }

    wholeSum++;
  }

  return fixedpoint_create_3(wholeSum, fracSum, left.sign, valid);

}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  if (left.sign == negative && right.sign == positive) {
    return -1;
  } else if (left.sign == positive && right.sign == negative) {
    return 1;
  }

  // consider case where both are negative
  int indicator = 1;

  if (left.sign == negative) {
    indicator = -1;
  }

  return indicator * compare_magnitudes(left, right);
}

Fixedpoint magnitude_subtraction(Fixedpoint pos, Fixedpoint neg) {
  Fixedpoint left = pos;
  Fixedpoint right = neg;
  int isNegative = 0; // in the enum, this is positive
  if (compare_magnitudes(pos, neg) == -1) {
    left = neg;
    right = pos;
    isNegative = 1;
  }


  uint64_t newFrac = left.frac - right.frac;
  uint64_t newWhole = left.whole - right.whole;
  if (left.frac < right.frac) {
    newWhole--;
  }

  return (Fixedpoint) {.whole = newWhole, .frac = newFrac, 
                      .sign = isNegative, .overflow = notOver, 
                      .underflow = notUnder, .validity = valid};
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  if (left.sign == right.sign) {
    return magnitude_addition(left, right);
  } else if (left.sign == negative) {
    return magnitude_subtraction(right, left);
  } else {
    return magnitude_subtraction(left, right); 
  }
}

Fixedpoint correct_sign_of_zero(Fixedpoint zero) {
  if (zero.validity == valid && fixedpoint_is_zero(zero)) {
    zero.sign = positive;
    return zero;
  }

  return zero;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  Fixedpoint newFP;
  
  if (left.sign == positive && right.sign == positive) {
    newFP = magnitude_subtraction(left, right);
  } else if (left.sign == negative && right.sign == positive) {
    // (-) - (+) = (-) + (-)
    newFP = magnitude_addition(left, right);
  } else if (left.sign == positive && right.sign == negative) {
    // (+) - (-) = (+) + (+)
    newFP = magnitude_addition(left, right);
  } else if (left.sign == negative && right.sign == negative) {
    newFP = magnitude_subtraction(right, left);
  } 

  return correct_sign_of_zero(newFP);
} 

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (fixedpoint_is_zero(val)) {
    // just make sure it's zero
    val.sign = positive;
    return val;
  } else if (val.sign == positive) {
    val.sign = negative;
    return val;
  } else {
    val.sign = positive;
    return val;
  }
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // location of msb
  uint64_t mask = 1UL;
  uint64_t underflowing = 0;

  if ((mask & val.whole) == mask) {
    underflowing = 0x8000000000000000UL;
  }
  val.whole = val.whole >> 1;

  if ((mask & val.frac) == mask) {
    val.underflow = under;
    return val;
  } else {
    val.frac = val.frac >> 1;
    val.frac += underflowing;
    return val;
  }
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // location of msb
  uint64_t mask = 1UL << 63;
  uint64_t overflowing = 0;

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

int fixedpoint_is_zero(Fixedpoint val) {
  if ((val.validity == valid) && (val.whole == 0) && (val.frac == 0)) {
    return 1;
  } else {
    return 0;
  }
}

int fixedpoint_is_err(Fixedpoint val) {
  return (val.overflow == over) || (val.underflow == under) || (val.validity == invalid);
}

int fixedpoint_is_neg(Fixedpoint val) {
  if ((val.validity == valid) && (val.sign == negative)) {
    return !fixedpoint_is_zero(val);
  }
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  return (val.overflow == over) && (val.sign == negative);
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  return (val.overflow == over) && (val.sign == positive);
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  return (val.underflow == under) && (val.sign == negative);
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  return (val.underflow == under) && (val.sign == positive);
}

int fixedpoint_is_valid(Fixedpoint val) {
  return val.validity == valid;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  char *s = (char*) calloc(100, sizeof(char));
  if (val.sign == negative) {
    strcat(s, "-");
  }

  char final[20] = "";
  sprintf(final, "%lx", val.whole);
  strcat(s, final);

  if (val.frac != 0UL) {
    strcat(s, ".");

    sprintf(final, "%-.016lx", val.frac);

    for (int i = 15; i > 0; i--) {
      if (final[i] != '0') {
        break;
      } else {
        final[i] = '\0';
      } 
    }

    strcat(s, final);
  }
  return s;

}

