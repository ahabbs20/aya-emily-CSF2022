#include "Slot.h"

Slot::Slot() {
  tag = 0;
  load_ts = 0;
  access_ts = 0;
  valid = false;
  dirty = false;
}

Slot::Slot(uint32_t tag, uint32_t load_ts, uint32_t access_ts): tag(tag), load_ts(load_ts), access_ts(access_ts) {
  valid = true;
}
