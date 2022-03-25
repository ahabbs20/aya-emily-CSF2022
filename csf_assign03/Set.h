#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include "Slot.h"
using namespace std;

class Set {
   public:
      Set() {}

      Set(int size);

      vector<Slot> set;
};

#endif