#include "Set.h"

Set::Set(int size) : set(size) { 
  vector<Slot> temp(size);
  set = temp;
}